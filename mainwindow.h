#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vtkSmartPointer.h"    // Required for smart pointer internal ivars.
#include <QTimer>
#include <queue>
#include "mksocketclient.h"
#include "../sharedfiles/mkglobalclass.h"
#include "mkGlobalData.h"
#include "../sharedfiles/mkRobotKin.h"

//class mkSocketServer;
class JntCtrlDialog;
class QmkJointEditVLayout;
class QmkTargetEditVLayout;
class QmkMotionEditVLayout;
class QmkTabWidget;
//class QmkSetupVLayout;

QT_BEGIN_NAMESPACE
class QVTKOpenGLWidget;
class QSplitter;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QPushButton;
class QLabel;
class QSpinBox;
class QStackedWidget;
class QVBoxLayout;
class QHBoxLayout;
class QmkVTKWidget;
class QmkJointSliderLayout;
class QFile;
class CmkLeftWidget;
class CmkLeftOperationWidget;
class CmkLeftCalibWidget;
class CmkLeftSetupWidget;
class vtkCaptionWidget;
class vtkCaptionRepresentation;
class vtkCaptionActor2D;
class QThread;
class QKeyEvent;
//class QTimer;
class CmkNavDisplayWidget;
QT_END_NAMESPACE

//class UIMainWindow;


class CRenderRobotArm;
class MainWindow : public QWidget//QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public:
    double jointSpeedPercent=100;
    double jointAccPercent=100;
    MKZoeRobotKin tmpRobotKin;
    int statusPauseMode=0;
public:

   QHBoxLayout *mainHLayout ;
   QHBoxLayout *bottomHLayout;
   QTabWidget *tab;
   QSplitter *split_left_right, *split_top_bottom ;
   QGroupBox *groupBoxJoint, *groupBoxTarget, *groupBoxMotion;


   QWidget        *bottomWidget;

    QVTKOpenGLWidget *qvtkOpenGLWidgetFront, *qvtkOpenGLWidgetTop;
    CRenderRobotArm * renderRobotFront=nullptr, *renderRobotTop=nullptr;
    JntCtrlDialog *jointCtrlDialogBox=nullptr;
    QmkJointEditVLayout *JointEditVLayout=nullptr;
    QmkTargetEditVLayout *TargetEditVLayout=nullptr;
    QmkMotionEditVLayout *MotionEditVLayout=nullptr;
    QmkTargetEditVLayout *POREditVLayout_tmp2=nullptr;
   // QmkSetupVLayout *setupVLayout=nullptr;
    QmkTabWidget *tabWidgetCtrl=nullptr;

    QTimer *jointInterpolationMoveTimer=nullptr;
    QTimer *linearMoveTimer=nullptr;
    QTimer socketResponseTimer;
    QTimer socketJobSchedulerTimer;
    mkSocketClient *socketHandler=nullptr;

    ///////////////////////////////////////
    /// \brief jobSchedulerStatus
    ///// Handling queueing jobs...
    volatile int statusJobScheduler=-1;
    volatile int numberCurrentJob=1;
    int testCurrentJob=-1;
    std::queue<PacketJobs*> queuePacketJobs;
    // -------------------------------------
    // QWidget interface
public:
    void createLayoutMain();
    void createLayoutJoint();
    void createLayoutTarget();
    void createLayoutMotion();
    void updateSingleJointMove(int axis,  double value);
    void updateAllJointMove(double *jntPos);
    void updateJointValue();
    void stopAllTimers();
    void connectToServer();
    MapObjActor * findTarget(const char *name);


    void action_rebootRobot();
    void action_controlPower(bool bPowerOn);
    void action_setZeroEncoder(int axisID);
    void action_homingRobot(int axisID);
    void action_getEncoderValue(int axisID);
    void action_moveMultiJointRobot();
    void action_moveMultiJointRobot(double X, double R1, double R2, double Z);
    void action_moveAllTargetRobot();
    void action_moveSingleJointRobot(int axisID, double vel=100.0);
    void action_moveSingleJointRobot(int axisID, double pos[], double vel=100.0);
    void action_moveCircle(CircleProfile & circleProfile);
    void action_moveSpiral(SpiralProfile & spiralProfile);
    void action_orderSequence(int orderID);
    void action_dropCup();
//    void action_moveLinear(double EEx_start, double EEx_end, double EEy_start, double EEy_end, double EETheta_start, double Speed);
//    void action_moveRotateEE(double EEx_start, double EEy_start, double EETheta_start, double EETheta_end, double Speed);
    void action_moveEERotate();
    void action_moveLinear();
    void action_getAllPosition();
    void action_setPosition();
    void action_stop();
    void action_pause(int bPause);
    void action_getPauseStatus();
    void action_savePos();
   // void action_getPos();
    void action_testMotion();
public:
    bool checkPauseState();
 signals:
    void widgetVisibilityChanged(bool state);
public slots:
    void initState();
    void processSocketResponseTimer();
    void processSocketJobSchedulerTimer();


protected:
    void resizeEvent(QResizeEvent *event) override;



    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;


    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *event) override
    {
        // 'true' means visible..
        emit widgetVisibilityChanged(true);
    }
    virtual void hideEvent(QHideEvent *event) override
    {
        // 'false' means hidden..
        emit widgetVisibilityChanged(false);
    }
};
#endif // MAINWINDOW_H
