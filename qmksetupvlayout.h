#ifndef QMKSETUPVLAYOUT_H
#define QMKSETUPVLAYOUT_H

#include "mkGlobalData.h"
#include <QVBoxLayout>

class QmkPushButton;
class QTableWidget;
class QLabel;
class QLineEdit;
class QmkCreateMotionDlg;
class QmkMotionParamLayout;

class QmkSetupVLayout : public QVBoxLayout
{
    Q_OBJECT
private:
    char const *buttonNameRobotOperation[6]={"Connect","Reboot", "TurnOn Power"," Pause ", "Stop", "Get Pos"};
    char const *buttonNameHoming[4]={"Arm Joint2", "Arm Joint1", "X-axis", "Z-axis"};

    QmkPushButton *buttonsHoming[4]={nullptr};
    bool bPowerOn=false;
    bool bPause=false;
public:
     QmkPushButton *buttonsRobotOperation[6]={nullptr};
public:

    QmkSetupVLayout(QWidget *parent=nullptr);

public slots:
    void clickedButton_connectToServer();
    void clickedButton_reboot();
    void clickedButton_turnPowerOnOff();
    void clickedButton_pause();
    void clickedButton_stop();
    void clickedButton_savePos();
    void clickedButton_getPos();
    void clickedButton_homingR2();
    void clickedButton_homingR1();
    void clickedButton_homingX();
    void clickedButton_homingZ();

};

#endif // QMKSETUPVLAYOUT_H
