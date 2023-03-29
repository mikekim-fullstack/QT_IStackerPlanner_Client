#include "mainwindow.h"
#include "qmkjointsliderlayout.h"
#include "../sharedfiles/mkmatrix.h"
#include "../sharedfiles/mkRobotKin.h"
#include "../sharedfiles/mkZoeRobotics_define.h"
#include "jntctrldialog.h"
#include "qmkjointeditvlayout.h"
#include <QValidator>
#include <QDebug>
extern MainWindow *g_MainWindow;
extern MKZoeRobotKin robotKin;
QmkJointSliderLayout::QmkJointSliderLayout(QmkJointEditVLayout *parent, QString jntname, int jointID)
{
    update_robot=true;
    this->parentVLayout = parent;
    jntID = jointID;// start from index 1
    preValue = 0;
    residueAngle = 0.0;
    isUpdatedJoint=false;
    name = new QLabel(jntname);
    slider = new QSlider(Qt::Orientation::Horizontal);
    edit = new QLineEdit;
    buttonDecrease = new QPushButton("-<<");
    buttonIncrease = new QPushButton(">>+");

    //setContentsMargins(10, 0, 10, 0);

    slider->setValue(0);
    slider->setMinimumWidth(150);

    edit->setValidator(new QDoubleValidator(-3000,3000,2,this));
    edit->setAlignment(Qt::AlignRight);
    edit->setText(QString::number(0,'f',2));
    edit->setMaximumWidth(50);

    buttonDecrease->setMaximumWidth(50);
    buttonIncrease->setMaximumWidth(50);

    addWidget(name);
    addWidget(slider);
    addWidget(buttonDecrease);
    addWidget(edit);
    addWidget(buttonIncrease);
    setMargin(0);
    addSpacing(0);

//    if(jointID==1){
//        name->setObjectName(QStringLiteral("joint_label"));
//        edit->setObjectName(QStringLiteral("joint_ang_edit"));
//        slider->setObjectName(QStringLiteral("joint_slider"));
//    }
//    else {
//        name->setObjectName(QStringLiteral("joint_label"));
//        edit->setObjectName(QStringLiteral("joint_ang_edit"));
//        slider->setObjectName(QStringLiteral("joint_slider"));
//    }

    name->setObjectName(QStringLiteral("joint_label"));
    edit->setObjectName(QStringLiteral("joint_ang_edit"));
    slider->setObjectName(QStringLiteral("joint_slider"));
    buttonDecrease->setObjectName(QStringLiteral("joint_btn_press"));
    buttonIncrease->setObjectName(QStringLiteral("joint_btn_press"));

    edit->setAlignment(Qt::AlignCenter);

    connect(slider, &QSlider::sliderMoved, this,  &QmkJointSliderLayout::sliderMoved);
    connect(slider, &QSlider::sliderReleased, this,  &QmkJointSliderLayout::sliderReleased);

    connect(buttonDecrease, &QPushButton::pressed, this,  &QmkJointSliderLayout::btnDecreasePressed);
    connect(buttonDecrease, &QPushButton::released, this,  &QmkJointSliderLayout::btnDecreaseReleased);

    connect(buttonIncrease, &QPushButton::pressed, this,  &QmkJointSliderLayout::btnIncreasePressed);
    connect(buttonIncrease, &QPushButton::released, this,  &QmkJointSliderLayout::btnIncreaseReleased);

}

QmkJointSliderLayout::QmkJointSliderLayout(JntCtrlDialog *parent,QString jntname, int jointID)
{
    update_robot=true;
   // this->parentDlg = parent;
    jntID = jointID;// start from index 1
    preValue = 0;
    residueAngle = 0.0;
    isUpdatedJoint=false;
    name = new QLabel(jntname);
    slider = new QSlider(Qt::Orientation::Horizontal);
    edit = new QLineEdit;

    //setContentsMargins(10, 0, 10, 0);

    slider->setValue(0);
    slider->setMinimumWidth(150);

    edit->setValidator(new QDoubleValidator(-3000,3000,2,this));
    edit->setAlignment(Qt::AlignRight);
    edit->setText(QString::number(0,'f',2));
    edit->setMaximumWidth(100);
    addWidget(name);
    addWidget(slider);
    addWidget(edit);
    if(jointID==1){
        name->setObjectName(QStringLiteral("joint1_label"));
        edit->setObjectName(QStringLiteral("joint1_ang_edit"));
        slider->setObjectName(QStringLiteral("joint1_slider"));
    }
    else {
        name->setObjectName(QStringLiteral("joint_label"));
        edit->setObjectName(QStringLiteral("joint_ang_edit"));
        slider->setObjectName(QStringLiteral("joint_slider"));
    }
    connect(slider, &QSlider::sliderMoved, this,  &QmkJointSliderLayout::sliderMoved);
    connect(slider, &QSlider::sliderReleased, this,  &QmkJointSliderLayout::sliderReleased);

}


QmkJointSliderLayout::~QmkJointSliderLayout()
{

}

void QmkJointSliderLayout::setQss(QByteArray &qss)
{
    name->setStyleSheet( QString::fromLatin1(  qss) );
    slider->setStyleSheet( QString::fromLatin1(  qss) );
    edit->setStyleSheet( QString::fromLatin1(  qss) );
}

void QmkJointSliderLayout::setUpdateRobot(bool set)
{
    update_robot = set;
}



void QmkJointSliderLayout::sliderMoved(int value)
{
   // qDebug()<< "slider moveing, "<< jntID;

    double value2, diff, currentValue=0;

    switch( jntID)
    {
        case 0:
            currentValue = robotKin.curX;
            break;
        case 1:
            currentValue = robotKin.curZ;
            break;
        case 2:
            currentValue = robotKin.curR1*RTOD;
            break;
        case 3:
            currentValue = robotKin.curR2*RTOD;
            break;
    }

    diff = (value -currentValue);
    if(abs(diff)<1.0) residueAngle = diff;

   // value2 = value + residueAngle;
//    value2 = value + diff;
//    if(isUpdatedJoint){
//        value2=ang;
//        isUpdatedJoint = false;
//    }
    edit->setText(QString::number(value,'f',2));

    switch( jntID)
    {
        case 0:
            g_MainWindow->updateSingleJointMove(jntID, diff);
            break;
        case 1:
            g_MainWindow->updateSingleJointMove(jntID, diff);
            break;
        case 2:
            g_MainWindow->updateSingleJointMove(jntID, diff*DTOR);
         //   parentDlg->updateSilder(1);// update Y value
            break;
        case 3:
            g_MainWindow->updateSingleJointMove(jntID, diff*DTOR);
           // parentDlg->updateSilder(1);// update Y value
            break;
    }
   // if(parentDlg) parentDlg->updateEEinfo();
    if(parentVLayout) parentVLayout->updateEEinfo();

}

void QmkJointSliderLayout::sliderReleased()
{
   // qDebug()<< "slider mouse released";
    /*
    if( g_MainWindow->isRobotConnected && g_MainWindow->cs9Client->m_statusPower==1){
        for(int i=0; i<6; i++) {
            g_MainWindow->cs9Client->m_curJointAng[i] = gSharedData.Jdeg[i+1];
        }
        g_MainWindow->cs9Client->moveJoint(20);

    }
    */
}

void QmkJointSliderLayout::setValue(double val)
{
    double currentValue=0;
    switch( jntID)
    {
        case 0:// X
            currentValue = val;
            break;
        case 1:// Z
            currentValue = val;
        break;
        case 2://R1
            currentValue = val*RTOD;
            break;
        case 3:// R2
            currentValue = val*RTOD;
            break;

    }

    slider->setValue(int(currentValue));
    edit->setText(QString::number(currentValue,'f',2));
//    if(update_robot==true){
//        myRobotBrain.curRadAng[jntID-1] = Ang*DTOR;//DEG2RAD(Ang);
//        gSharedData.Jdeg[jntID] = Ang;
//        g_MainWindow->renderRobot->renderForwardKin(gSharedData.Jdeg);
//        g_MainWindow->motionPlan_widget->update();
//    }

}

void QmkJointSliderLayout::cmdMoveJoint(int dir, int jntID)
{
    double pos[4];

    robotKin.getJointPos(pos);
    robotKin.printForKin();
    if(dir<0){
        pos[jntID]=robotKin.minJntLimit[jntID];
        if(jntID>=2){
            pos[jntID] *=DEG2RAD;
            pos[jntID] +=0.0175;// 1deg off
        }
        else {
            pos[jntID]+=1.0;// 1mm off
        }


    }
    else if(dir>0){
        pos[jntID]=robotKin.maxJntLimit[jntID];
        if(jntID>=2){
            pos[jntID] *=DEG2RAD;
            pos[jntID] -=0.0175;// 1deg off
        }
        else {
            pos[jntID]-=1.0;// 1mm off
        }
    }

    g_MainWindow->action_moveSingleJointRobot(jntID,pos,15.0);
}

void QmkJointSliderLayout::btnDecreasePressed()
{
    qDebug()<<"btnDecreasePressed()"<<jntID<<"\n";
   cmdMoveJoint(-1, jntID);

}

void QmkJointSliderLayout::btnDecreaseReleased()
{
    qDebug()<<"btnDecreaseReleased()"<<jntID<<"\n";
    g_MainWindow->action_stop();
}

void QmkJointSliderLayout::btnIncreasePressed()
{
    qDebug()<<"btnIncreasePressed()"<<jntID<<"\n";
    cmdMoveJoint(1, jntID);
}
void QmkJointSliderLayout::btnIncreaseReleased()
{
    qDebug()<<"btnIncreaseReleased()"<<jntID<<"\n";
    g_MainWindow->action_stop();
}

