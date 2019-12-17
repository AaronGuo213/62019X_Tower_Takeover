#ifndef TRAY_H
#define TRAY_H

void runTray(double percent);
int getTrayPos();
double calcTrayPow(bool isOuttaking);
void updateTray();
void traySafetyNet();

extern double traySetPoint;
extern bool resetTrayIntegral;
enum class TrayStatus {idle, hold, manual};
extern TrayStatus trayStat;
void ctrlTray(void* param);
void moveTray(int setPoint);

#endif