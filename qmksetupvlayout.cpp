#include "qmksetupvlayout.h"
#include "mainwindow.h"
#include "qmkpushbutton.h"
#include "../sharedfiles/mkRobotKin.h"

#include <QLabel>
#include <QSpacerItem>
#include <QGroupBox>

extern MKZoeRobotKin robotKin;
extern QByteArray qss_data;
extern MainWindow *g_MainWindow;

QmkSetupVLayout *g_SetupLayout=nullptr;
QmkSetupVLayout::QmkSetupVLayout(QWidget *parent) : QVBoxLayout(parent)
{
    g_SetupLayout=this;
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QGroupBox * groupBox = new QGroupBox;
    QVBoxLayout *groupBoxVLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxVLayout);
    groupBox->setContentsMargins(10,0,10,0);
    groupBoxVLayout->setSpacing(0);

    QLabel *title_label =  new QLabel("SETTINGS FOR ROBOT");
    title_label->setAlignment(Qt::AlignHCenter);
    this->setSpacing(0);
    this->addWidget(title_label);

    QLabel *label[2];
    label[0]= new QLabel("Robot Operation");
    label[1]= new QLabel("Robot Homing");
    QHBoxLayout *buttonHLayout[2] = {0};
    for(int i=0; i<2; i++) {
        buttonHLayout[i] = new QHBoxLayout;
        groupBoxVLayout->addLayout(buttonHLayout[i]);
    }
    buttonHLayout[0]->addWidget(label[0]);
    for(int i=0; i<6; i++) {
        buttonsRobotOperation[i] = new QmkPushButton(buttonNameRobotOperation[i]);
        buttonHLayout[0]->addWidget(buttonsRobotOperation[i]);
    }

    buttonHLayout[1]->addWidget(label[1]);
    for(int i=0; i<4; i++) {
        buttonsHoming[i] = new QmkPushButton(buttonNameHoming[i]);
        buttonHLayout[1]->addWidget(buttonsHoming[i]);
    }

    mainVLayout->addWidget(groupBox);
    mainVLayout->addSpacing(10);
    //////////////////////////////////////////////////////////////////////////////////////////
    this->addSpacing(20);
    this->addLayout(mainVLayout);
    //this->addStretch();
    ///////////////////////////////////////////////////////////////////////////////////////////
    /// \brief connect
    connect(buttonsRobotOperation[0], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_connectToServer);
    connect(buttonsRobotOperation[1], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_reboot);
    connect(buttonsRobotOperation[2], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_turnPowerOnOff);
    connect(buttonsRobotOperation[3], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_pause);
    connect(buttonsRobotOperation[4], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_stop);
    connect(buttonsRobotOperation[5], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_getPos);

    connect(buttonsHoming[0], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_homingR2);
    connect(buttonsHoming[1], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_homingR1);
    connect(buttonsHoming[2], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_homingX);
    connect(buttonsHoming[3], &QmkPushButton::mkClicked, this, &QmkSetupVLayout::clickedButton_homingZ);


}

void QmkSetupVLayout::clickedButton_connectToServer()
{
    g_MainWindow->connectToServer();
}

void QmkSetupVLayout::clickedButton_reboot()
{
    g_MainWindow->action_rebootRobot();
}

void QmkSetupVLayout::clickedButton_turnPowerOnOff()
{
    bPowerOn = !bPowerOn;
    if(bPowerOn) {
        g_MainWindow->action_controlPower(true);
        buttonsRobotOperation[1]->setText("TurnOff Power");
    }
    else {
        g_MainWindow->action_controlPower(false);
        buttonsRobotOperation[1]->setText("TurnOn Power");
    }
}

void QmkSetupVLayout::clickedButton_pause()
{
//    bPause =!bPause;
//    if(bPause){
//        buttonsRobotOperation[3]->setText(" Pause ");

//    }
//    else{
//        buttonsRobotOperation[3]->setText("Resume");
//    }
    cout<<"clickedButton_pause()"<<endl;
    g_MainWindow->action_pause(bPause);

//    g_MainWindow->action_stop();
}

void QmkSetupVLayout::clickedButton_stop()
{
    g_MainWindow->action_stop();
}

void QmkSetupVLayout::clickedButton_savePos()
{
    g_MainWindow->action_savePos();
}

void QmkSetupVLayout::clickedButton_getPos()
{
    g_MainWindow->action_getAllPosition();
}

void QmkSetupVLayout::clickedButton_homingR2()
{
//     g_MainWindow->action_homingRobot(2);
    g_MainWindow->action_setZeroEncoder(2);
}

void QmkSetupVLayout::clickedButton_homingR1()
{
//     g_MainWindow->action_homingRobot(1);
    g_MainWindow->action_setZeroEncoder(1);
}

void QmkSetupVLayout::clickedButton_homingX()
{
     g_MainWindow->action_homingRobot(0);
}

void QmkSetupVLayout::clickedButton_homingZ()
{
     g_MainWindow->action_homingRobot(3);
}
