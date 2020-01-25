#ifndef BASE_H
#define BASE_H

void moveStraight(double dist, bool stopEarly, int time, double maxVal = 100);
void turn(double angle, bool stopEarly, int time, double maxVal = 100);
void curveBase(double leftPow, double rightPow, double fastSideDist);
void curveBasePID(double leftSetPoint, double rightSetPoint, int time, double maxVal = 100);
void curveBaseCombo(double dist, double angle, int time, double maxVal = 100);
void curveBaseVel(double leftSetPoint, double rightSetPoint, int time, double maxVal = 100);

#endif