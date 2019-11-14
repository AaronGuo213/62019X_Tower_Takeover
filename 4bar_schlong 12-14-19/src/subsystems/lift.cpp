#include "main.h"

const int atCube[] = {0, 0, 0, 0, 0};
const int onTower[] = {0, 0, 0};

void runLift(double percent) {

    lift.move_voltage(percent * 120); //runs the lift motor out of 12000mV

}

void liftToGnd(int threshhold) {

    liftStat = LiftStatus::manual;
    runLift(-100);
    while(getLiftHeight() > threshhold)
        delay(10);
    runLift(0);

}

void liftSafetyNet() { //prevents the motors from overheating and breaking

    if(lift.is_over_temp())
        lift.set_voltage_limit(0);
    else
        lift.set_voltage_limit(12000);

}

int getLiftHeight() {

    return liftPot.get_value() - 1220;

}
enum class LiftStatus;
LiftStatus liftStat = LiftStatus::idle;

int liftSetPoint;
bool resetIntegral = false;

void liftCtrl(void* param) {

    std::uint32_t now = millis();
    PID hold = initPID(1, 1, 0, 0.3, 0.0002, 0); //kP = 0.3, kI = 0.0001
    PID slow = initPID(0, 0, 1, 0, 0, 0.15); //kD = 0.15
    PID move = initPID(1, 1, 1, 0.45, 0.00012, 1.5);
    double holdVal = 0, slowVal = 0, moveVal = 0;
    int slowTimer = 300;

    while(true) {

        //std::cout << cubeSensor.get_value() << std::endl;
        liftSetPoint = liftSetPoint > 2600 ? 2600 : liftSetPoint; //lift cannot be higher than 1800
        liftSetPoint = liftSetPoint < 0 ? 0 : liftSetPoint; //lift cannot be lower than 0

        if(resetIntegral) {
            hold.integral = 0;
            move.integral = 0;
            resetIntegral = false;
        }

        if(liftStat != LiftStatus::manual) {

            if(liftStat == LiftStatus::idle)
                runLift(0);

            else if(liftStat == LiftStatus::slow) {
        
                if(slowTimer == 0) { //once the lift has been slowed to prevent bouncing
                    slowTimer = 300;
                    setLiftHold();
                    liftSetPoint = getLiftHeight(); //makes the lift hold at the current spot
                    std::cout << "switch" << std::endl;
                }
                else
                    slowTimer -= 10;

                slow.error = -getLiftHeight() * 5 / 7; //updates error for slowPID
                slowVal = runPID(&slow); //updates slowVal, refernce misc.cpp
                runLift(slowVal);

                //std::cout << "liftPos: " << getLiftHeight() << " | slow.derivative: " << slow.derivative << " | slowVal: " << slowVal << std::endl;

            }

            else if(liftStat == LiftStatus::hold) {

                hold.error = (liftSetPoint - getLiftHeight()) * 5 / 7; //updates error for holdPID
                holdVal = runPID(&hold); //updates the holdVal, reference misc.cpp
                runLift(holdVal);

                //std::cout << "liftSetPoint: " << liftSetPoint << " | liftPos: " << getLiftHeight() << " | hold.error: " << hold.error << " | holdVal: " << holdVal << std::endl;

            }

            else if(liftStat == LiftStatus::move) {

                move.error = (liftSetPoint - getLiftHeight()) * 5 / 7;
                moveVal = runPID(&move);
                runLift(moveVal);

                /*if(abs(move.error) < 10)
                    liftStat = LiftStatus::hold;*/
                //std::cout << "liftSetPoint: " << liftSetPoint << " | liftPos: " << getLiftHeight() << " | move.error: " << move.error << " | moveVal: " << moveVal << std::endl;

            }

        }

        else
            slowTimer = 300;

        liftSafetyNet();
        Task::delay_until(&now, 10);

    }

}

void setLiftHold(bool updateSetPoint) {

    liftStat = LiftStatus::hold;
    if(updateSetPoint)
        liftSetPoint = getLiftHeight();
    resetIntegral = true;

}

void setLiftIdle() {

    liftStat = LiftStatus::idle;

}

void updateLift() {

    if(l1() && !r1()) {
        liftStat = LiftStatus::manual;
        runLift(100);
    }

    else if(!l1() && r1()) {
        liftStat = LiftStatus::manual;
        runLift(-100);
    }

    else if(master.get_digital(E_CONTROLLER_DIGITAL_DOWN))
        liftStat = LiftStatus::idle;

    else if(liftStat == LiftStatus::manual || liftStat == LiftStatus::idle)
        liftStat = LiftStatus::slow;

}

void moveLift(int setPoint) {

    liftSetPoint = setPoint;
    resetIntegral = true;
    liftStat = LiftStatus::move;

}
