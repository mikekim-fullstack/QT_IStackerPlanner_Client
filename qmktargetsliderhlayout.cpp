#include "qmktargetsliderhlayout.h"
#include "qmktargeteditvlayout.h"
#include "mainwindow.h"
#include "jntctrldialog.h"
#include "../sharedfiles/mkRobotKin.h"
#include "../sharedfiles/mkmatrix.h"
#include "renderrobotarm.h"
//#include "mkGlobalData.h"

extern MainWindow *g_MainWindow;
extern MKZoeRobotKin robotKin;
extern MapObjActor   *selectedTargetObj;
QmkTargetSliderHLayout::QmkTargetSliderHLayout(QmkTargetEditVLayout *parent, int id, QString name, double min, double max)
{
    this->parent = parent;
    ID = id;
    nameLabel = new QLabel(name+":");
    nameLabel->setMinimumWidth(70);
    minLabel = new QLabel(QString::number(min,'f',1));
    minLabel->setFixedWidth(40);
    maxLabel = new QLabel(QString::number(max,'f',1));
    maxLabel->setFixedWidth(40);
    slider = new QSlider(Qt::Orientation::Horizontal);
    slider->setRange(min,max);
    edit = new QLineEdit;

    edit->setMaximumWidth(50);
    slider->setValue(0);
    edit->setText(QString::number(0.0));
    edit->setValidator(new QDoubleValidator(-1000,1000,2,this));
    edit->setAlignment(Qt::AlignRight);
    //slider->setMinimumWidth(150);
    addWidget(nameLabel);
    addWidget(minLabel);
    addWidget(slider);
    addWidget(maxLabel);
    addWidget(edit);

    nameLabel->setObjectName(QStringLiteral("joint_label"));
    maxLabel->setObjectName(QStringLiteral("joint_label"));
    edit->setObjectName(QStringLiteral("joint_ang_edit"));
    slider->setObjectName(QStringLiteral("joint_slider"));

     connect(slider, &QSlider::sliderMoved, this,  &QmkTargetSliderHLayout::sliderMoved);
}

QmkTargetSliderHLayout::~QmkTargetSliderHLayout()
{

}

void QmkTargetSliderHLayout::setQss(QByteArray &qss)
{
    nameLabel->setStyleSheet( QString::fromLatin1(  qss) );
    maxLabel->setStyleSheet( QString::fromLatin1(  qss) );
    slider->setStyleSheet( QString::fromLatin1(  qss) );
    edit->setStyleSheet( QString::fromLatin1(  qss) );
}

void QmkTargetSliderHLayout::sliderMoved(int value)
{    if(!parent) return;
     if(!selectedTargetObj) return;
    double value2, diff, currentValue=0;
    currentValue = selectedTargetObj->param.posTargetEE[ID];
    if(ID==3) currentValue *=RTOD;


    diff = (value -currentValue);


    edit->setText(QString::number(value,'f',2));
    if(ID==3) diff *= DTOR;
    mkMatrix Ttarget;
    vtkSmartPointer<vtkTransform> transformTarget = vtkSmartPointer<vtkTransform>::New();
//    Ttarget = traXYZ(robotKin.param.EEx+robotKin.offDisX, robotKin.param.EEy+robotKin.offDisY, robotKin.param.EEz+robotKin.offDisZ);
//    transformTarget->SetMatrix(Ttarget.m_pData);
//    MapObjActor *newActor = new MapObjActor;
//    newActor->param.posEE[0] = robotKin.param.EEx;
//    newActor->param.posEE[1] = robotKin.param.EEy;
//    newActor->param.posEE[2] = robotKin.param.EEz;
//    newActor->param.posEE[3] = robotKin.param.EEth;
   // strcpy(newActor->name, name.c_str());


    selectedTargetObj->param.posTargetEE[ID] +=diff;
    Ttarget = traXYZ(selectedTargetObj->param.posTargetEE[0]+robotKin.offDisX,
                     selectedTargetObj->param.posTargetEE[1]+robotKin.offDisY,
                     selectedTargetObj->param.posTargetEE[2]+robotKin.offDisZ)*rotZrad(selectedTargetObj->param.posTargetEE[3]+M_PI_2);
    transformTarget->SetMatrix(Ttarget.m_pData);

    selectedTargetObj->actor->SetUserTransform(transformTarget);
    g_MainWindow->renderRobotTop->moveSelectedTarget(selectedTargetObj->param.posTargetEE);
    g_MainWindow->updateJointValue();


//    switch( ID)
//    {
//        case 0:
//            g_MainWindow->updateTargetMove(ID, diff);
//            break;
//        case 1:
//            g_MainWindow->updateTargetMove(ID, diff);
//            break;
//        case 2:
//            g_MainWindow->updateTargetMove(ID, diff*DTOR);
//         //   parentDlg->updateSilder(1);// update Y value
//            break;
//        case 3:
//            g_MainWindow->updateTargetMove(ID, diff*DTOR);
//           // parentDlg->updateSilder(1);// update Y value
//            break;
//    }
//    //parentDlg->updateEEinfo();
}

void QmkTargetSliderHLayout::sliderReleased()
{

}

void QmkTargetSliderHLayout::setValue(double val)
{
    double currentValue=0;
    switch( ID)
    {
        case 0:// Tx
            currentValue = val;
            break;
        case 1:// Ty
            currentValue = val;
        break;
        case 2://Tz
            currentValue = val;
            break;
        case 3:// Tang
            currentValue = val*RTOD;
            break;

    }

    slider->setValue(int(currentValue));
    edit->setText(QString::number(currentValue,'f',2));
}
