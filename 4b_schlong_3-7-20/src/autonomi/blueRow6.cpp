#include "main.h"

void blueRow6() {

    //deploys antitip
    runIntk(100);
    moveTray(0);
    double distToCube = getSonarInches(10);
    if(distToCube > 19 || distToCube < 14)
        distToCube = 16;
    moveStraight(distToCube - 6.5, 40);
    //moveStraight(10, 40);
    moveLift(150);
    delay(500);
    moveLift(0);
    delay(500);

    //gets the row of 4
    moveStraight(10.5, 50);
    delay(1500);
    moveStraight(16, 50);
    setLiftIdle();

    //gets the cube under tower
    turnRelative(-12, 100);
    moveStraight(14, 60);

    //goes to outtake
    moveStraight(-32, 100);
    runIntk(0);
    runIntkDist(-140, 150);
    delay(100);
    moveTray(400);
    delay(1000);
    runIntk(0);
    turnAbsolute(135, 70);
    runBase(60);
    delay(600);
    runBase(0);
    outtake();
    moveTray(0);
    moveStraight(-40, 60);

}