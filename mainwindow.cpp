#include "mainwindow.h"
#include <QDateTime>
//#include "UIMainWindow.h"
//#include "ui_mainwindow.h"
#include "renderrobotarm.h"
#include "mkFrontCamera.h"
#include "mkTopCamera.h"
#include "vtk_headers.h"

#include "jntctrldialog.h"
#include "qmktabwidget.h"
#include "qmkjointeditvlayout.h"
#include "qmktargeteditvlayout.h"
#include "qmkmotioneditvlayout.h"
#include "qmkpushbutton.h"
#include "mkfilemanager.h"
#include "mklinkedlist.h"
//#include "qmksetupvlayout.h"


#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include "vtkGenericOpenGLRenderWindow.h"
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkSmartPointer.h"
#include <vtkVectorText.h>
#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPolyDataSilhouette.h>
#include <qsplitter.h>
#include <QKeyEvent>
#include "QVTKOpenGLWidget.h"


#include <QDebug>
#include <QLabel>
#include <QSpacerItem>
#include <QScrollArea>
#include "qmksetupvlayout.h"
#define SEL_NONE 0
#define SEL_PAUSE 1
#define SEL_RESUME 2
//#define SERVER_ADDR "10.88.111.19"// UBUNTU
//#define SERVER_ADDR "10.88.111.6"//"10.88.111.3" // IMAC
#define SERVER_ADDR "127.0.0.1"// Local host

MKZoeRobotKin robotKin;
globalSharedData gSharedData;
int winSize[2];
int viewMode=0;
CamViewer camView[10];
QByteArray qss_data;
MainWindow *g_MainWindow=nullptr;
extern MapObjActor   *selectedTargetObj;
static void runJointInterMove_timer();
static void runLinearMove_timer();

extern QmkSetupVLayout *g_SetupLayout;
MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
    const bool bTEST=false;
    // +++++++++++++++++ test ++++++++++++++++
    if(bTEST){
        FoldArmParam a, b, c;
        a.setTargetType("APPROACH");
        a.setMoveType("CARTESIAN");

        cout<<"static obj value: "<<a.getTargetType()<< ", "<<  a.getMoveType()<<endl;
        main_LinkTest();
        main_LListTest();
        return;
    }

    ////////////////////////////////////////////
    // Read skin style sheet...
    QFile styleFile( ":/stylesheet_darkgray.qss" );
    //QFile styleFile( "stylesheet_darkgray.qss" );
    styleFile.open( QFile::ReadOnly );
    qss_data = styleFile.readAll();
    //qDebug()<<qss_data<<"\n---------- end of file qss -----------";
    styleFile.close();

    linearMoveTimer = new QTimer(this);
    jointInterpolationMoveTimer = new QTimer(this);

    createLayoutMain();
    createLayoutJoint();
    createLayoutTarget();
    createLayoutMotion();

    ////////////////////////////////////////////////////////////////////////////////
    // 1. For Front View
    // VTK Renderer

    vtkSmartPointer<vtkRenderer> rendererFront = vtkSmartPointer<vtkRenderer>::New();
    vtkRenderWindow* renderWindowFront= qvtkOpenGLWidgetFront->GetRenderWindow();

    //    renderWindowFront->SetMultiSamples(4);
    //    rendererFront->UseFXAAOn();
    //    rendererFront->GetUseFXAA();


    qvtkOpenGLWidgetFront->SetRenderWindow(renderWindowFront);
    qvtkOpenGLWidgetFront->GetRenderWindow()->AddRenderer(rendererFront);

    qvtkOpenGLWidgetFront->setMinimumWidth(200);
    // vtkRenderWindowInteractor *renderWindowInteractor = qvtkOpenGLWidgetFront->GetInteractor();

    renderRobotFront = new CRenderRobotArm(renderWindowFront, rendererFront);

    renderRobotFront->loadVRMLFileForZoeRobot(RESOURCE_3DFILE_PATH);

    renderRobotFront->visibilityFrontViewer();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractorFront =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractorFront->SetRenderWindow ( renderWindowFront );

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); //like paraview
    vtkSmartPointer<mkFrontCamera> styleFront =vtkSmartPointer<mkFrontCamera>::New();
    styleFront->addRenderer(rendererFront);

    renderWindowInteractorFront->SetInteractorStyle( styleFront );

    // Set up the lighting.
    //
    vtkSmartPointer<vtkLight> lightFront = vtkSmartPointer<vtkLight>::New();

    lightFront->SetFocalPoint(1500.0, 0.0,1500.0);
    lightFront->SetPosition(1500.0, -5000.0, 1500.0);
    rendererFront->AddLight(lightFront);

    vtkSmartPointer<vtkLight> lightLeft = vtkSmartPointer<vtkLight>::New();
    lightLeft->SetFocalPoint(1000.0, 0, 1000);
    lightLeft->SetPosition(-2500.0, -1000.0, 1000.0);
    rendererFront->AddLight(lightLeft);

    vtkSmartPointer<vtkLight> lightRight = vtkSmartPointer<vtkLight>::New();
    lightRight->SetFocalPoint(000.0, 0, 000);
    lightRight->SetPosition(25000.0, -10000.0, 20000.0);
    // rendererFront->AddLight(lightRight);


    vtkSmartPointer<vtkLight> lightTop = vtkSmartPointer<vtkLight>::New();
    lightTop->SetFocalPoint(1500.0, 1500, 0);
    lightTop->SetPosition(1500.0, 1500.0, 10000.0);
    rendererFront->AddLight(lightTop);

    // Setup the background gradient
    rendererFront->GradientBackgroundOn();
    rendererFront->SetBackground2(0.2,0.202,0.202);
    rendererFront->SetBackground(0.2,0.20,0.20);

    vtkCamera *cameraFront = rendererFront->GetActiveCamera();
    int n=0;
    //cameraFront->Print(std::cout);
    {// viewMode == 0
        // For Parallel Projection View
        n = 0;
        double cp[2]={2, 3000};
        double fp[3]={1000.0, .0, 1000.0};
        double vp[3]={1000.0, -500.0, 1000.0};
        double up[3] = {.0, .0, 1.0};
        memcpy(camView[n].clipping,cp, sizeof(double)*2);
        memcpy(camView[n].focalPoint,fp, sizeof(double)*3);
        memcpy(camView[n].pos,vp, sizeof(double)*3);
        memcpy(camView[n].viewUP,up, sizeof(double)*3);
    }
    {// viewMode == 1
        // For Left to Right Perspective View...
        n=1;//4;
        double cp[2]={10688.5, 16119.6};//2295.88, 9430.33};
        double fp[3]={966.44, 167.852, 630.25};//971.024, 344.171, 798.637};
        double vp[3]={961.273, 33.5629, 14007.4};//-3153.1, -2781.63, 2914.07};
        double up[3] = {0,1,0};

        memcpy(camView[n].clipping,cp, sizeof(double)*2);
        memcpy(camView[n].focalPoint,fp, sizeof(double)*3);
        memcpy(camView[n].pos,vp, sizeof(double)*3);
        memcpy(camView[n].viewUP,up, sizeof(double)*3);
        camView[n].viewAng =  10;
    }
    {
        // For Right to Left  Perspective View...
        n=2;
        double cp[2]={4263.15, 11757};
        double fp[3]={945.896, 236.317, 936.511};
        double vp[3]={4681.05, -4671.97, 5424.03};
        double up[3] = {-0.282699, 0.521131, 0.805297};

        memcpy(camView[n].clipping,cp, sizeof(double)*2);
        memcpy(camView[n].focalPoint,fp, sizeof(double)*3);
        memcpy(camView[n].pos,vp, sizeof(double)*3);
        memcpy(camView[n].viewUP,up, sizeof(double)*3);
        camView[n].viewAng =  15;
    }

    {
        n=4;
        // For Right to Left  Perspective View...
        double cp[2]={3133.4, 10990.2};//1696.72, 9139.61};
        double fp[3]={924.137, 166.02, 712.725};//511.019, 305.114, 644.475};
        double vp[3]={5182.88, -1923.67, 5546.47};//3388.98, -2223.04, 4485.84};
        double up[3] = {-0.590508, 0.407661, 0.696501};//-0.48206, 0.521249, 0.704214};

        memcpy(camView[n].clipping,cp, sizeof(double)*2);
        memcpy(camView[n].focalPoint,fp, sizeof(double)*3);
        memcpy(camView[n].pos,vp, sizeof(double)*3);
        memcpy(camView[n].viewUP,up, sizeof(double)*3);
        camView[n].viewAng =  15;
    }
    {
        n=5;
        double cp[2]={5567.27, 12377.9};
        double fp[3]={972.655, -53.9113, 854.039};
        double vp[3]={-6630.89, 3924.93, 2335.9};
        double up[3] = {.155346, -0.0701209, 0.985368};
        memcpy(camView[n].clipping,cp, sizeof(double)*2);
        memcpy(camView[n].focalPoint,fp, sizeof(double)*3);
        memcpy(camView[n].pos,vp, sizeof(double)*3);
        memcpy(camView[n].viewUP,up, sizeof(double)*3);
        camView[n].viewAng =  15;
    }
    {
        n=3;
        double cp[2]={4440.73, 11510.7};
        double fp[3]={945.896, 236.317, 936.511};
        double vp[3]={-5390.34, -3579.58, 2799.7};
        double up[3] = {0.207557, 0.128833, 0.969702};

        memcpy(camView[n].clipping,cp, sizeof(double)*2);
        memcpy(camView[n].focalPoint,fp, sizeof(double)*3);
        memcpy(camView[n].pos,vp, sizeof(double)*3);
        memcpy(camView[n].viewUP,up, sizeof(double)*3);
        camView[n].viewAng =  15;
    }

    if(0)
    {
        viewMode=0;
        cameraFront->ParallelProjectionOn();
        cameraFront->SetParallelScale(1250.0); // tweak as needed
        cameraFront->SetClippingRange(camView[viewMode].clipping);
        cameraFront->SetFocalPoint(camView[viewMode].focalPoint);
        cameraFront->SetPosition(camView[viewMode].pos);
        cameraFront->SetViewUp(camView[viewMode].viewUP);
    }
    else {
        viewMode=2;
        cameraFront->SetClippingRange(camView[viewMode].clipping);
        cameraFront->SetFocalPoint(camView[viewMode].focalPoint);
        cameraFront->SetPosition(camView[viewMode].pos);
        cameraFront->SetViewUp(camView[viewMode].viewUP);
        cameraFront->SetViewAngle(camView[viewMode].viewAng);
    }
    renderWindowInteractorFront->Initialize();
    //   styleFront->setRobotRenderer(renderRobotFront, renderRobotTop);

    ////////////////////////////////////////////////////////////////////////////////
    // 2. For Top View
    // VTK Renderer

    vtkSmartPointer<vtkRenderer> rendererTop = vtkSmartPointer<vtkRenderer>::New();
    vtkRenderWindow* renderWindowTop= qvtkOpenGLWidgetTop->GetRenderWindow();
    qvtkOpenGLWidgetTop->SetRenderWindow(renderWindowTop);
    qvtkOpenGLWidgetTop->GetRenderWindow()->AddRenderer(rendererTop);

    qvtkOpenGLWidgetTop->setMinimumWidth(200);
    // vtkRenderWindowInteractor *renderWindowInteractor = qvtkOpenGLWidgetTop->GetInteractor();

    renderRobotTop = new CRenderRobotArm(renderWindowTop, rendererTop);
    *renderRobotTop =  *renderRobotFront;
    renderRobotTop->visibilityTopViewer();
    // When Target Selected from "renderRobotFront", show it in the "renderRobotTop" too.
    //renderRobotTop->createSelectedTarget();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractorTop =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractorTop->SetRenderWindow ( renderWindowTop );

    // vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); //like paraview
    vtkSmartPointer<mkTopCamera> styleTop =vtkSmartPointer<mkTopCamera>::New();

    styleTop->addRenderer(rendererTop);
    renderWindowInteractorTop->SetInteractorStyle( styleTop );

    // Set up the lighting.
    rendererTop->AddLight(lightTop);
    // Setup the background gradient
    //     rendererTop->GradientBackgroundOn();
    //     rendererTop->SetBackground2(0.1,0.1,0.1);
    rendererTop->SetBackground(0.2,0.2,0.2);

    vtkCamera *cameraTop = ( vtkCamera *)rendererTop->GetActiveCamera();
    cameraTop->ParallelProjectionOn();
    cameraTop->SetParallelScale(503.591);//(600.0); // tweak as needed
    cameraTop->SetClippingRange(1, 3000);
    //Set the camera pose in the usual way:

    cameraTop->SetFocalPoint(979.465, -10.5841, 0);//1000, 00.0, 000.0);
    cameraTop->SetPosition(979.465, -10.5841, 2500);//(1000.0, 0.0, 2500.0);
    cameraTop->SetViewUp(0.0, 1.0, 0.0);
    renderWindowInteractorTop->Initialize();


    styleFront->setRobotRenderer(renderRobotFront, renderRobotTop);
    styleTop->setRobotRenderer(renderRobotTop, renderRobotFront);

    setLayout(mainHLayout);

    g_MainWindow=this;

    // +++ Joint Controll on DialogBox +++
    //     jointCtrlDialogBox = new JntCtrlDialog();
    //     connect(this, &MainWindow::widgetVisibilityChanged, jointCtrlDialogBox, &JntCtrlDialog::changeAlwaysOnTop);
    //     jointCtrlDialogBox->show();
    //     jointCtrlDialogBox->setModal(false);
    // ----------------------------------------

    TargetEditVLayout->loadTargetXMLFile();

    //    connect(refreshRenderWinowTimer, &QTimer::timeout, this, runRefreshRenderWindow_timer);
    connect(linearMoveTimer, &QTimer::timeout, this, runLinearMove_timer);
    connect(jointInterpolationMoveTimer, &QTimer::timeout, this, runJointInterMove_timer);
    ////////////////////////////////////////////
    socketHandler = new mkSocketClient;
//    socketHandler->connect2Server("10.88.111.3");// IMac
    socketHandler->connect2Server(SERVER_ADDR);// Ubuntu

    connect(&socketResponseTimer, &QTimer::timeout, this, &MainWindow::processSocketResponseTimer);
    connect(&socketJobSchedulerTimer, &QTimer::timeout, this, &MainWindow::processSocketJobSchedulerTimer);
    connect(socketHandler, &mkSocketClient::connectedSignal, this, &MainWindow::initState);
    socketResponseTimer.start(1);
    socketResponseTimer.start(20);
}

MainWindow::~MainWindow()
{
    if(linearMoveTimer) delete linearMoveTimer;
    if(jointInterpolationMoveTimer) delete jointInterpolationMoveTimer;
    if(renderRobotFront) {delete renderRobotFront; renderRobotFront=nullptr;}
    if(renderRobotTop) {delete renderRobotTop; renderRobotTop=nullptr;}
    if(socketHandler) delete socketHandler;
    //if(jointCtrlDialogBox) {cout<<"dialog"<<endl;jointCtrlDialogBox->close(); delete jointCtrlDialogBox; jointCtrlDialogBox=nullptr;}

    //    delete renderRobot;
    //    delete ui;
}

void MainWindow::createLayoutMain()
{
    mainHLayout = new QHBoxLayout;
    mainHLayout->setMargin(0);
    mainHLayout->setContentsMargins(0, 0, 0, 0);

    split_left_right = new QSplitter;
    split_top_bottom = new QSplitter;
    split_top_bottom->setContentsMargins(0,0,0,0);

    qvtkOpenGLWidgetFront = new QVTKOpenGLWidget;
    qvtkOpenGLWidgetFront->setContentsMargins(0, 0, 0, 0);
    qvtkOpenGLWidgetTop = new QVTKOpenGLWidget;
    qvtkOpenGLWidgetTop->setContentsMargins(0, 0, 0, 0);

    ///////////////////////////////////





    //////////////////////////////////

    bottomHLayout = new QHBoxLayout ;
    bottomWidget = new QWidget;

    QScrollArea *bottomScrollArea = new QScrollArea;
    bottomScrollArea->setWidgetResizable(true);


    mainHLayout->addWidget(split_left_right);
    bottomWidget->setLayout(bottomHLayout);
    bottomScrollArea->setWidget(bottomWidget);


    split_left_right->setOrientation(Qt::Horizontal);
    split_left_right->addWidget(qvtkOpenGLWidgetFront);
    split_left_right->addWidget(split_top_bottom);
    split_left_right->setSizes(QList<int>({INT_MAX, INT_MAX}));

    split_top_bottom->setOrientation(Qt::Vertical);
    split_top_bottom->addWidget(qvtkOpenGLWidgetTop);

    split_top_bottom->addWidget(bottomScrollArea);
    split_top_bottom->setSizes(QList<int>({INT_MAX, INT_MAX}));


}

void MainWindow::createLayoutJoint()
{
    JointEditVLayout = new QmkJointEditVLayout;
    JointEditVLayout->setMargin(0);
    JointEditVLayout->setContentsMargins(0,0,0,0);

}

void MainWindow::createLayoutTarget()
{
    QVBoxLayout * joint_targetVLayout = new QVBoxLayout;




    TargetEditVLayout = new QmkTargetEditVLayout;
    TargetEditVLayout->setMargin(0);
    TargetEditVLayout->setContentsMargins(0,0,0,0);

    groupBoxTarget = new QGroupBox();
    groupBoxTarget->setLayout(TargetEditVLayout);
    groupBoxTarget->setContentsMargins(10,0,10,0);

    joint_targetVLayout->addLayout(JointEditVLayout);
    joint_targetVLayout->addWidget(groupBoxTarget);

    bottomHLayout->addLayout(joint_targetVLayout);

}

//void MainWindow::createLayoutSetup()
//{
//    setupVLayout = new QmkSetupVLayout;
//    setupVLayout->setMargin(0);
//    setupVLayout->setContentsMargins(0,0,0,0);
//    bottomHLayout->addLayout(setupVLayout);
//}

void MainWindow::createLayoutMotion()
{
    // --- MKIM test ---
    MotionEditVLayout = new QmkMotionEditVLayout;
    MotionEditVLayout->setMargin(0);
    MotionEditVLayout->setContentsMargins(0,0,0,0);
    bottomHLayout->addLayout(MotionEditVLayout);
}

void MainWindow::updateSingleJointMove(int axis, double value)
{

    switch( axis)
    {
    case 0:
        robotKin.forKin(robotKin.curX+value, robotKin.curZ, robotKin.curR1, robotKin.curR2);
        break;
    case 1:
        robotKin.forKin(robotKin.curX, robotKin.curZ+value, robotKin.curR1, robotKin.curR2);
        break;
    case 2:
        robotKin.forKin(robotKin.curX, robotKin.curZ, robotKin.curR1+value, robotKin.curR2);
        break;
    case 3:
        robotKin.forKin(robotKin.curX, robotKin.curZ, robotKin.curR1, robotKin.curR2+value);
        break;
    }
    robotKin.updateCurrentJoint();
    renderRobotFront->updateZoeRobotConfig();

    renderRobotTop->updateZoeRobotConfig();

    renderRobotFront->_renderWindow->Render();
    renderRobotTop->_renderWindow->Render();
}

void MainWindow::updateAllJointMove(double *jntPos)
{
    robotKin.forKin(jntPos[0], jntPos[1], jntPos[2], jntPos[3]);
    robotKin.updateCurrentJoint();
    renderRobotFront->updateZoeRobotConfig();

    renderRobotTop->updateZoeRobotConfig();

    renderRobotFront->_renderWindow->Render();
    renderRobotTop->_renderWindow->Render();
}

void MainWindow::updateJointValue()
{
    robotKin.updateCurrentJoint();
    renderRobotFront->updateZoeRobotConfig();

    renderRobotTop->updateZoeRobotConfig();

    renderRobotFront->_renderWindow->Render();
    renderRobotTop->_renderWindow->Render();
}

void MainWindow::stopAllTimers()
{
    jointInterpolationMoveTimer->stop();
    linearMoveTimer->stop();
}

void MainWindow::connectToServer()
{
    socketHandler->connect2Server(SERVER_ADDR);
}
///////////////////////////////////////////////////////////////////////////////
///// Begining of Action for robot functions...
void MainWindow::action_rebootRobot()
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    PacketJobs newJob[2];
    numberCurrentJob++;
    newJob[0].packf("J%d N%d G%d M%d\n",numberCurrentJob, 1, SC_REBOOT, 1);
//    newJob[1].packf("J%d N%d G%d M%d\n",numberCurrentJob, 2, SC_SETPOS, 1);
    newJob[1].packf("J%d N%d G%d M%d\n",numberCurrentJob,   0, SC_STATUS_ALL_POS, 1);
    for(int i=0; i<2; i++) {
        socketHandler->write(newJob[i].get());
    }
}

void MainWindow::action_controlPower(bool bPowerOn)
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    PacketJobs newJob;
    numberCurrentJob++;
    if(bPowerOn)
        newJob.packf("J%d N%d G%d M%d\n",numberCurrentJob, 0, SC_POWER, 1);
    else
        newJob.packf("J%d N%d G%d M%d\n",numberCurrentJob, 0, SC_POWER, 0);
    socketHandler->write(newJob.get());
}

void MainWindow::action_setZeroEncoder(int axisID)
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    PacketJobs newJob;
    numberCurrentJob++;
    newJob.packf("J%d N%d G%d M%d\n",numberCurrentJob, 0, SC_SET_ZERO_ENCODER, axisID);
    socketHandler->write(newJob.get());
}

void MainWindow::action_homingRobot(int axisID)
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    int jN=5;
    PacketJobs newJob[8];
    numberCurrentJob++;
    // Predefined Params for 3 different stages of execution
    // Axis ID: 4 = [0: X, 1: R1, 2: R2, 3:Z]
    // Stage:   3 = [0: Case-approaching to home S/W, 1: Case-move away from homeS/W, 2: Case-giving a little distance]
    double pos[4][3]  ={{-1700,100,5}, {-360,10,2}, {-360,10,2}, {-1700,100,5}};
//    double vel[4][3]  ={{50,50,50},    {25,15,15},  {50,25,25},  {50,50,50}};
//    double accel[4][3]={{50,25,25},    {25,15,15},  {50,25,25},  {50,25,25}};
    double vel[4][3]  ={{40,10,10},    {16.5, 10.0, 10.0},  {33.3,16.5, 16.5},  {40, 10, 10}};// percent of max velocity
    double accel[4][3]={{40,10,10},    {25.0 ,15.0 ,15.0 },  {50.0, 25.0, 25.0},  {40, 10, 10}};// percent of max acceleration

    newJob[0].packf("J%d N%d G%d M%d\n",
                    numberCurrentJob, 1, SC_STATUS, axisID);
    // Case1: approaching to home S/W
    newJob[1].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                    numberCurrentJob, 2, SC_HOMING, axisID, pos[axisID][0], pos[axisID][0], pos[axisID][0], pos[axisID][0], vel[axisID][0], accel[axisID][0]);
    // Case2: moving away from homeS/W after hit the homeS/W...
    newJob[2].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                    numberCurrentJob, 3, SC_HOMING, axisID, pos[axisID][1], pos[axisID][1], pos[axisID][1], pos[axisID][1], vel[axisID][1], accel[axisID][1]);
    // Case3: giving a little distance from homeS/W contact point...
    newJob[3].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                    numberCurrentJob, 4, SC_HOMING, axisID, pos[axisID][2], pos[axisID][2], pos[axisID][2], pos[axisID][2], vel[axisID][2], accel[axisID][2] );

    // If Joint type is the Rotation, After homing reposition to the center (0 ang)...
    if(axisID==1 || axisID==2) {
        jN=8;
        newJob[4].packf("J%d N%d G%d M%d\n",
                        numberCurrentJob, 5, SC_STATUS_ALL_POS, 1);
        newJob[5].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                        numberCurrentJob, 6, SC_SET_SPEED, axisID, 0.0, 0.0, 0.0, 0.0, vel[axisID][0], accel[axisID][0] );
        newJob[6].packf("J%d N%d G%d M%d\n",
                        numberCurrentJob, 7, SC_MOVE, 1);
        newJob[7].packf("J%d N%d G%d M%d\n",
                        numberCurrentJob,   0, SC_STATUS_ALL_POS, 1);

    }
    else {

        newJob[4].packf("J%d N%d G%d M%d\n",
                        SC_HOMING,   0, SC_STATUS_ALL_POS, 1);
    }
    //////////////////////////
//    newJob[4].packf("J%d N%d G%d M%d\n",
//                    SC_HOMING,   0, SC_STATUS_ALL_POS, 1);//test
    // ------------------------------------------------------------------------
    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }
}

void MainWindow::action_getEncoderValue(int axisID)
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    PacketJobs newJob;
    numberCurrentJob++;
    newJob.packf("J%d N%d G%d M%d\n",numberCurrentJob, 0, SC_GET_ENCODER, axisID);
    socketHandler->write(newJob.get());
}
void MainWindow::action_moveSingleJointRobot(int axisID, double vel)
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen){
        return;
    }
    const int jN=2;
    PacketJobs newJob[jN];
    int axisMap[4]={0,2,3,1};
    numberCurrentJob++;
    newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                    numberCurrentJob, 1, SC_SET_SPEED, axisMap[axisID],
                    robotKin.param.Px, robotKin.param.th1*RTOD, robotKin.param.th2*RTOD, robotKin.param.Pz, vel, vel);
    newJob[1].packf("J%d N%d G%d M%d\n",
                    numberCurrentJob, 0, SC_MOVE, axisID);

    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }
    newJob[0].print();
}
void MainWindow::action_moveSingleJointRobot(int axisID, double pos[], double vel)
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen){
        return;
    }
    const int jN=2;
    PacketJobs newJob[jN];
    int axisMap[4]={0,3,1,2};
    numberCurrentJob++;
    newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                    numberCurrentJob, 1, SC_SET_SPEED, axisMap[axisID],
                    pos[0], pos[2]*RTOD, pos[3]*RTOD, pos[1], vel, vel);
    newJob[1].packf("J%d N%d G%d M%d\n",
                    numberCurrentJob, 0, SC_MOVE, axisID);

    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }
    newJob[0].print();
}

void MainWindow::action_moveCircle(CircleProfile &circleProfile)
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    MKZoeRobotKin testRobotKin(robotKin);
    bool ret = testRobotKin.invKinEE_W_XYR(circleProfile.cenPosX,//+circleProfile.radius,//EEx
                                           circleProfile.cenPosY, //EEy
                                           circleProfile.EETheta*DTOR //EETh
                                           );//EEZ

    // J: Job ID
    // N: Sequence Number
    // G: SEND_CMD Command (defined mkglobalclass.h)
    // M: ACTION_MODE Action Mode (defined mkglobalclass.h)
    // W: traget EEth [rad]   - data[0]
    // X: EE speed [deg/sec]  - data[1]
    // Y: Arc angle[deg]      - data[2]
    if(ret==false) {
        cout<<"IK Error "<<endl;
        return;
    }

        const int jN=2;
        PacketJobs newJob[jN];
        numberCurrentJob++;

        newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f\n",
                        numberCurrentJob, 1, SC_GEN_CIRCLE, CIRCLE_MODE,
                        circleProfile.speed,
                        circleProfile.radius,
                        circleProfile.arcAng);
        newJob[1].packf("J%d N%d G%d M%d\n",
                        numberCurrentJob, 0, SC_MOVE, CIRCLE_MODE);



        for(int i=0; i<jN; i++) {
            socketHandler->write(newJob[i].get());
        }
}

void MainWindow::action_moveSpiral(SpiralProfile &spiralProfile)
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    MKZoeRobotKin testRobotKin(robotKin);
    double originalPz = testRobotKin.param.Pz;
    bool rec = testRobotKin.invKinEE_W_XYRZ(spiralProfile.cenPosX,//+spiralProfile.radius,//EEx
                                           spiralProfile.cenPosY, //EEy
                                           spiralProfile.EETheta*DTOR, //EETh
                                           spiralProfile.heightZ+testRobotKin.param.Pz
                                           );//EEZ
    int ret = testRobotKin.checkJointLimit();
    if(ret>0) {
        cout<<"Exceed joint limit: "<<ret<<endl;
        return;
    }

    // J: Job ID
    // N: Sequence Number
    // G: SEND_CMD Command (defined mkglobalclass.h)
    // M: ACTION_MODE Action Mode (defined mkglobalclass.h)
    // W: traget EEth [rad]   - data[0]
    // X: EE speed [deg/sec]  - data[1]
    // Y: Arc angle[deg]      - data[2]
    // Z: heightZ[mm]         - data[3]
    if(rec)
    {
        const int jN=2;
        PacketJobs newJob[jN];
        numberCurrentJob++;
        newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f\n",
                        numberCurrentJob, 1, SC_GEN_SPIRAL, CIRCLE_MODE,
                        spiralProfile.speed,
                        spiralProfile.radius,
                        spiralProfile.arcAng,
                        spiralProfile.heightZ);
        newJob[1].packf("J%d N%d G%d M%d\n",
                        numberCurrentJob, 0, SC_MOVE, CIRCLE_MODE);



        for(int i=0; i<jN; i++) {
            socketHandler->write(newJob[i].get());
        }

    }
    else cout<<"No solution: Robot can't reach to the target."<<endl;
}

void MainWindow::action_orderSequence(int orderID)
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    const int jN=1;
    PacketJobs newJob[jN];
    numberCurrentJob++;
        newJob[0].packf("J%d N%d G%d M%d\n",
                    numberCurrentJob,   0, SC_ORDER, orderID);
    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }
}

void MainWindow::action_dropCup()
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    const int jN=1;
    PacketJobs newJob[jN];
    numberCurrentJob++;
        newJob[0].packf("J%d N%d G%d M%d\n",
                    numberCurrentJob,   0, SC_DROP_CUP, 130);
    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }
}

void MainWindow::action_moveEERotate()
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    if(abs(selectedTargetObj->param.posTargetEE[3]-robotKin.curEEth)<0.01){
        cout<<"Too short distance: action_moveRoteEE: "<<abs(selectedTargetObj->param.posTargetEE[3]-robotKin.curEEth)<<endl;
        return;
    }
    MKZoeRobotKin testRobotKin(robotKin);
    bool rec = testRobotKin.invKinEE_W_XYRZ(robotKin.curEEx,//EEx
                                           robotKin.curEEy, //EEy
                                           selectedTargetObj->param.posTargetEE[3], //target EETh
                                           robotKin.curZ);//EEZ
    // J: Job ID
    // N: Sequence Number
    // G: SEND_CMD Command (defined mkglobalclass.h)
    // M: ACTION_MODE Action Mode (defined mkglobalclass.h)
    // W: traget EEth [rad]   - data[0]
    // X: EE speed [deg/sec]   - data[1]

    if(rec) {

        const int jN=2;
        PacketJobs newJob[jN];
        numberCurrentJob++;
        newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f\n",
                        numberCurrentJob, 1, SC_GEN_EEROTATION, CARTESIAN_MODE,
                        selectedTargetObj->param.posTargetEE[3],// EEth[start, end]//[rad]
                        40*DTOR);//  speed[rad/sec]
        newJob[0].print();
        newJob[1].packf("J%d N%d G%d M%d\n",
                        numberCurrentJob, 0, SC_MOVE, CARTESIAN_MODE);

        for(int i=0; i<jN; i++) {
            socketHandler->write(newJob[i].get());
        }
    }
    else cout<<"No solution:(action_moveRoteEE) Robot can't reach to the target."<<endl;
}

void MainWindow::action_moveLinear()
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    MKZoeRobotKin testRobotKin(robotKin);
    bool rec = testRobotKin.invKinEE_W_XYRZ(selectedTargetObj->param.posTargetEE[0],//EEx
                                           selectedTargetObj->param.posTargetEE[1], //EEy
                                           robotKin.curEEth, // Using current EE angle not target's angle...
                                           //selectedTargetObj->param.posTargetEE[3], //EETh
                                           selectedTargetObj->param.posTargetEE[2]);//EEZ
    testRobotKin.printForKin();
    cout<<"target: "<<selectedTargetObj->param.posTargetEE[0]<<", EEy:"<<selectedTargetObj->param.posTargetEE[1]<<", EEz:"<<selectedTargetObj->param.posTargetEE[2]<<", EEt"<<robotKin.curEEth<<endl;
    cout<<"currentEEx: "<<robotKin.curEEx<<", EEy:"<<robotKin.curEEy<<", EEz:"<<robotKin.curZ<<", EEt"<<robotKin.curEEth<<endl;
    if(rec) {

        const int jN=2;
        PacketJobs newJob[jN];
        numberCurrentJob++;
        // J: Job ID
        // N: Sequence Number
        // G: SEND_CMD Command (defined mkglobalclass.h)
        // M: ACTION_MODE Action Mode (defined mkglobalclass.h)
        // W: traget EEx position[mm]   - data[0]
        // X: traget EEy position[mm]   - data[1]
        // Y: target EEz position[mm]   - data[2]
        // Z: EE speed [mm/sec]         - data[3]


        double diffX = robotKin.curEEx - selectedTargetObj->param.posTargetEE[0];
        double diffY = robotKin.curEEy - selectedTargetObj->param.posTargetEE[1];
        double diffZ = robotKin.curZ - selectedTargetObj->param.posTargetEE[2];

        double diffEE = sqrt(diffX*diffX + diffY*diffY + diffZ*diffZ);

        // ... Too small distance between EEx EEy and EE target (x,y) ...
//        if(abs(diffX)<1 && abs(diffY)<1 && abs(diffZ)>2.00){
//            newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
//                            numberCurrentJob, 1, SC_SET_SPEED, 3,
//                            robotKin.param.Px, robotKin.param.th1*RTOD, robotKin.param.th2*RTOD, selectedTargetObj->param.posTargetEE[2],
//                    150.0, 150.0);
//            newJob[1].packf("J%d N%d G%d M%d\n",
//                                    numberCurrentJob, 2, SC_MOVE, 3);
//            newJob[2].packf("J%d N%d G%d M%d\n",
//                            numberCurrentJob, 0, SC_STATUS_ALL_POS, MULTI_ALL_JNT_MODE);
//            for(int i=0; i<3; i++) {
//                socketHandler->write(newJob[i].get());
//            }
//        }
        if(diffEE>=.1 ){


            newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f\n",
                            numberCurrentJob, 1, SC_GEN_EELINEAR, CARTESIAN_MODE,
                            selectedTargetObj->param.posTargetEE[0],// EEx[start, end]
                            selectedTargetObj->param.posTargetEE[1],// EEy[start, end]
                            selectedTargetObj->param.posTargetEE[2],// EEz[start, end]
                            150.0);// EETheta, speed
    //        newJob[0].print();


            newJob[1].packf("J%d N%d G%d M%d\n",
                            numberCurrentJob, 0, SC_MOVE, CARTESIAN_MODE);


            for(int i=0; i<jN; i++) {
                socketHandler->write(newJob[i].get());
            }
        }


    }
    else cout<<"Too small to move(less than 2mm):(action_moveLinear) Robot can't reach to the target."<<endl;
}
void MainWindow::action_moveMultiJointRobot()
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    const int jN=2;
    PacketJobs newJob[jN];
    numberCurrentJob++;
    newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                    numberCurrentJob, 1, SC_SET_SPEED, MULTI_ALL_JNT_MODE,
                    robotKin.param.Px, robotKin.param.th1*RTOD, robotKin.param.th2*RTOD, robotKin.param.Pz,  jointSpeedPercent, jointAccPercent);
    newJob[1].packf("J%d N%d G%d M%d\n",
                    numberCurrentJob, 0, SC_MOVE, MULTI_ALL_JNT_MODE);

    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }
}

void MainWindow::action_moveMultiJointRobot(double X, double R1, double R2, double Z)
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    const int jN=2;
    PacketJobs newJob[jN];
    numberCurrentJob++;
    newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                    numberCurrentJob, 1, SC_SET_SPEED, MULTI_ALL_JNT_MODE,
                    X, R1*RTOD, R2*RTOD, Z, jointSpeedPercent, jointAccPercent);
    newJob[1].packf("J%d N%d G%d M%d\n",
                    numberCurrentJob, 0, SC_MOVE, MULTI_ALL_JNT_MODE);

    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }
}

void MainWindow::action_moveAllTargetRobot()
{
    if(checkPauseState()) return;


    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    int numSequence=1;
    numberCurrentJob++;
    for( MapObjActor * objActor:renderRobotFront->objTarget) {
        //objActor->param.posTargetEE;//EEx,EEy,EEz,EEth;
//        objActor->param.getTargetType().c_str();
//        objActor->param.getMoveType().c_str();
        MKZoeRobotKin testRobotKin(robotKin);
        bool rec = testRobotKin.invKinEE_W_XYRZ(objActor->param.posTargetEE[0],//EEx
                                               objActor->param.posTargetEE[1], //EEy
                                               objActor->param.posTargetEE[3], //EETh
                                               objActor->param.posTargetEE[2]);//EEZ
        if(!rec) continue;
        const int jN=2;
        PacketJobs newJob[jN];
        newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                        numberCurrentJob, numSequence, SC_SET_SPEED, MULTI_ALL_JNT_MODE,
                        testRobotKin.param.Px, testRobotKin.param.th1*RTOD,
                        testRobotKin.param.th2*RTOD, testRobotKin.param.Pz,  jointSpeedPercent, jointAccPercent);
        numSequence++;
        newJob[1].packf("J%d N%d G%d M%d\n",
                        numberCurrentJob, numSequence, SC_MOVE, 1);
        numSequence++;

        for(int i=0; i<jN; i++) {
            socketHandler->write(newJob[i].get());
        }
    }
    PacketJobs newJob;
    newJob.packf("J%d N%d G%d M%d\n",
                    numberCurrentJob,   0, SC_STATUS_ALL_POS, 1);
    socketHandler->write(newJob.get());
}


void MainWindow::action_getAllPosition()
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    PacketJobs newJob;
    newJob.packf("J%d N%d G%d M%d\n",  ++numberCurrentJob,   0, SC_STATUS_ALL_POS, 1);
    socketHandler->write(newJob.get());
}

void MainWindow::action_setPosition()
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    PacketJobs newJob;
    numberCurrentJob++;
    newJob.packf("J%d N%d G%d\n", numberCurrentJob,   0, SC_SETPOS);
    socketHandler->write(newJob.get());
}

void MainWindow::action_stop()
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    const int jN=1;
    PacketJobs newJob[jN];

    newJob[0].packf("J%d N%d G%d\n", ++numberCurrentJob,   0, SC_STOP);

    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }

}

void MainWindow::action_pause(int bPause)
{
    cout<<"action_pause:"<<statusPauseMode<<endl;
    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    int status = SEL_NONE;

    if(statusPauseMode==SEL_NONE){
        status=SEL_PAUSE;
    }
    else if(statusPauseMode==SEL_PAUSE){
        status=SEL_RESUME;
    }
//    statusPauseMode = bPause?SEL_PAUSE:SEL_RESUME;// 1: pause, 2: resume

    const int jN=1;
    PacketJobs newJob[jN];

    newJob[0].packf("J%d N%d G%d M%d\n", ++numberCurrentJob,   0, SC_PAUSE, status);

//    if(statusPauseMode==SEL_RESUME) statusPauseMode=SEL_NONE;
    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }
}

void MainWindow::action_getPauseStatus()
{
    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }
    const int jN=1;
    PacketJobs newJob[jN];

    newJob[0].packf("J%d N%d G%d\n", ++numberCurrentJob,   0, SC_PAUSE_STATUS);

    for(int i=0; i<jN; i++) {
        socketHandler->write(newJob[i].get());
    }
}

void MainWindow::action_savePos()
{
    if(checkPauseState()) return;

    if(!socketHandler->bSocketOpen) {
        cout<<"Socket is not open!\n"<<flush;
        return;
    }

    PacketJobs newJob;
    numberCurrentJob++;
    newJob.packf("J%d N%d G%d\n", numberCurrentJob,   0, SC_SAVE_POS);
    socketHandler->write(newJob.get());
}
 MapObjActor * MainWindow::findTarget(const char *name)
 {
     int nSize = renderRobotFront->objTarget.size();
     for(int i=0; i<nSize; i++){
         MapObjActor *objActor = renderRobotFront->objTarget[i];
         if(strcmp(objActor->param.name,name)==0){
             return objActor;
         }
     }
     return nullptr;
 }
void MainWindow::action_testMotion()
{
    if(checkPauseState()) return;

    int sel=2;
    if(sel==1)
    {
        if(!socketHandler->bSocketOpen) {
            cout<<"Socket is not open!\n"<<flush;
            return;
        }
        int numSequence=0;
        numberCurrentJob++;
        testCurrentJob=numberCurrentJob;
        int count=0;
        int n=4;
        MapObjActor *objActorFirst = renderRobotFront->objTarget[0];
        for( MapObjActor * objActor:renderRobotFront->objTarget) {
            //objActor->param.posTargetEE;//EEx,EEy,EEz,EEth;
            //        objActor->param.getTargetType().c_str();
            //        objActor->param.getMoveType().c_str();

            MKZoeRobotKin testRobotKin(robotKin);
            bool rec = testRobotKin.invKinEE_W_XYRZ(objActor->param.posTargetEE[0],//EEx
                    objActor->param.posTargetEE[1], //EEy
                    objActor->param.posTargetEE[3], //EETh
                    objActor->param.posTargetEE[2]);//EEZ
            if(!rec) continue;

            qDebug()<<"targetName: "<<objActor->param.name<<", count="<<++count;
            if(count==n) {
                objActor=objActorFirst;
            }

            const int jN=2;
            PacketJobs newJob[jN];
            newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                            numberCurrentJob, ++numSequence, SC_SET_SPEED, MULTI_ALL_JNT_MODE,
                            testRobotKin.param.Px, testRobotKin.param.th1*RTOD,
                            testRobotKin.param.th2*RTOD, testRobotKin.param.Pz,  jointSpeedPercent, jointAccPercent);


            if(count==n) // sequence==0: indicating this is the last sequence.
                newJob[1].packf("J%d N%d G%d M%d\n",
                                numberCurrentJob, 0, SC_MOVE, MULTI_ALL_JNT_MODE);

            else
                newJob[1].packf("J%d N%d G%d M%d\n",
                                numberCurrentJob, ++numSequence, SC_MOVE, MULTI_ALL_JNT_MODE);

            // ...Sending packets to the controller server. ...
            for(int i=0; i<jN; i++) {
                // Wait for 10 sec before running the next motion...
                //            QTime dieTime= QTime::currentTime().addMSecs(50);
                //            while (QTime::currentTime() < dieTime) QCoreApplication::processEvents(QEventLoop::AllEvents, 20);

                socketHandler->write(newJob[i].get());
                // delay 20ms
                QTime dieTime= QTime::currentTime().addMSecs(20);
                while (QTime::currentTime() < dieTime)
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
            }
            if(count==n) break;
        }
        qDebug()<<"targetName: "<<"total sequence: "<<numSequence;
    }
    else if(sel==2)
    {
        const char *targetNames[]={"T_CupPlace_1_Entrance", "T_Place1_1",
                                   "T_CupPlace_2_Entrance", "T_CupPlace_2",
                                   "T_Cup_Drop_Entry",      "T_Cup_Drop_Place",
                                   "T_TAG67_CENTER_Y_600",  "T_CoffeePlace",
                                   "T_CupPlace_1_Entrance" // return to first place...
                                  };


        int numSequence=0;

        for(int i=0; i<8+1; i+=2){
            MapObjActor * objActor0=nullptr;
            objActor0 = findTarget(targetNames[i]);
            // ... Joint Motion before the linear motion ...
            if(objActor0){
                objActor0->param.print();
                MKZoeRobotKin testRobotKin(robotKin);
                bool rec = testRobotKin.invKinEE_W_XYRZ(objActor0->param.posTargetEE[0],//EEx
                        objActor0->param.posTargetEE[1], //EEy
                        objActor0->param.posTargetEE[3], //EETh
                        objActor0->param.posTargetEE[2]);//EEZ
                //            if(!rec) continue;
                const int jN=2;
                PacketJobs newJob[jN];
                newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f V%5.3f A%5.3f\n",
                                numberCurrentJob, ++numSequence, SC_SET_SPEED, MULTI_ALL_JNT_MODE,
                                testRobotKin.param.Px, testRobotKin.param.th1*RTOD,
                                testRobotKin.param.th2*RTOD, testRobotKin.param.Pz,  jointSpeedPercent, jointAccPercent);
                ++numSequence;
                if(i==8) numSequence=0; // ending of this job...
                newJob[1].packf("J%d N%d G%d M%d\n",
                                numberCurrentJob, numSequence, SC_MOVE, MULTI_ALL_JNT_MODE);

                // ...Sending packets to the controller server. ...
                for(int j=0; j<jN; j++) {
                    socketHandler->write(newJob[j].get());
                    // delay 20ms
                    QTime dieTime= QTime::currentTime().addMSecs(20);
                    while (QTime::currentTime() < dieTime)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
                }
                if(i==8) return;

            }

            // ... Linear foreward motion get started ...
            MapObjActor * objActor1=nullptr;
            objActor1 = findTarget(targetNames[i+1]);

            if(objActor1){
                objActor1->param.print();

                const int jN=2;
                PacketJobs newJob[jN];
                newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f\n",
                                numberCurrentJob, ++numSequence, SC_GEN_EELINEAR, CARTESIAN_MODE,
                                objActor1->param.posTargetEE[0],// EEx
                        objActor1->param.posTargetEE[1],// EEy
                        objActor1->param.posTargetEE[2],// EEz
                        350.0);// EETheta, speed

                newJob[1].packf("J%d N%d G%d M%d\n",
                                numberCurrentJob, ++numSequence, SC_MOVE, CARTESIAN_MODE);
                // ...Sending packets to the controller server. ...
                for(int j=0; j<jN; j++) {
                    socketHandler->write(newJob[j].get());
                    // delay 20ms
                    QTime dieTime= QTime::currentTime().addMSecs(20);
                    while (QTime::currentTime() < dieTime)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
                }

            }
            // ... Linear backword motion get started ...
            if(objActor0){
                objActor0->param.print();

                const int jN=2;
                PacketJobs newJob[jN];
                newJob[0].packf("J%d N%d G%d M%d W%5.3f X%5.3f Y%5.3f Z%5.3f\n",
                                numberCurrentJob, ++numSequence, SC_GEN_EELINEAR, CARTESIAN_MODE,
                                objActor0->param.posTargetEE[0],// EEx
                        objActor0->param.posTargetEE[1],// EEy
                        objActor0->param.posTargetEE[2],// EEz
                        350.0);// EETheta, speed

                newJob[1].packf("J%d N%d G%d M%d\n",
                                numberCurrentJob, ++numSequence, SC_MOVE, CARTESIAN_MODE);
                // ...Sending packets to the controller server. ...
                for(int j=0; j<jN; j++) {
                    socketHandler->write(newJob[j].get());
                    // delay 20ms
                    QTime dieTime= QTime::currentTime().addMSecs(20);
                    while (QTime::currentTime() < dieTime)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
                }
            }
        }

        // 0. ... Joint Motion ...
        // T_CupPlace_1_Entrance

        // 1. ... Linear Motion ...
        // T_Place1_1
        // T_CupPlace_1_Entrance

        // 2. Joint Motion
        // T_CupPlace_2_Entrance

        // 3. Linear Motion ...
        // T_CupPlace_2
        // T_CupPlace_2_Entrance

        // 4. Joint Motion
        // T_Cup_Drop_Entry

        // 5. Linear Motion
        // T_Cup_Drop_Place
        // T_Cup_Drop_Entry

        // 6. Joint Motion
        // T_TAG67_CENTER_Y_600

        // 7. Linear Motion
        // T_CoffeePlace
        // T_TAG67_CENTER_Y_600

    }
}

bool MainWindow::checkPauseState()
{
    cout<<"statusPauseMode: "<<statusPauseMode<<endl<<flush;
    if(statusPauseMode!=SEL_NONE) return true;// if pause is selected, return;
    return false;
}

void MainWindow::initState()
{
//    action_getAllPosition();
//    action_getPauseStatus();
}
// End of Action functions
///////////////////////////////////////////////////////////////////////////////////
void MainWindow::processSocketResponseTimer()
{
    if(socketHandler->ringBuffer.isEmpty()) return;
    int codeValue =-1;
    if(socketHandler->ringBuffer.codeSeen('R')){

        codeValue = socketHandler->ringBuffer.codeValue();
        if(codeValue!=RC_UPDATE_MOTION) qDebug()<<"ResponseTimer: "<<QDateTime::currentDateTime()<<":"<<socketHandler->ringBuffer.curBuffer;
        switch (codeValue) {
        case RC_UPDATE_MOTION:
        case RC_ACK_STOP:
        case RC_STATUS_ALL_POS:
            {
                PacketResSocketAxisPos packetAxisPos;
                packetAxisPos.unpack(socketHandler->ringBuffer.getCmd());
//                qDebug()<<"client-received: "<<socketHandler->ringBuffer.getCmd()<<", "<<packetAxisPos.pos[0]<<", "<<packetAxisPos.pos[1]<<", "<<packetAxisPos.pos[2]<<", "<<packetAxisPos.pos[3]<<Qt::endl;
//                packetAxisPos.print();
                robotKin.forKin(packetAxisPos.pos[0], packetAxisPos.pos[3],
                        packetAxisPos.pos[1]*DTOR, packetAxisPos.pos[2]*DTOR);
                JointEditVLayout->updateSilder();
                g_MainWindow->updateJointValue();
            }
            break;
        case RC_STATUS_JOB_DONE:
            if( socketHandler->ringBuffer.codeSeen('J') ) {
                statusJobScheduler = (int)socketHandler->ringBuffer.codeValue();
                cout<<"Response: RC_STATUS_JOB_DONE"<<endl<<flush;
            }

            cout<<"I got the job done response..."<<endl<<flush;
            break;
        case RC_PAUSE_STATUS:{
            if( socketHandler->ringBuffer.codeSeen('S') ) {
                statusPauseMode = (int)socketHandler->ringBuffer.codeValue();
                if(g_SetupLayout){

                            if(statusPauseMode==SEL_NONE){
                                g_SetupLayout->buttonsRobotOperation[3]->setText(" Pause ");

                            }
                            else if(statusPauseMode==SEL_PAUSE || statusPauseMode==SEL_RESUME){
                                g_SetupLayout->buttonsRobotOperation[3]->setText("Resume");
                            }
                }
                cout<<"Response: RC_PAUSE_STATUS:"<<statusPauseMode<<endl<<flush;
            }
            break;
        }
        case RC_ORDER_DONE:
            {
                cout<<"Received data from Server:["<<QDateTime::currentSecsSinceEpoch()<<"]:" <<socketHandler->ringBuffer.getCmd()<<endl<<flush;

                int J=-1,N=-1;
                if(socketHandler->ringBuffer.codeSeen('J')){
                    J = socketHandler->ringBuffer.codeValue();
                }
                if(socketHandler->ringBuffer.codeSeen('N')){
                    N = socketHandler->ringBuffer.codeValue();
                }
//                qDebug()<<"J="<<J<<", N="<<N<<"\n";
                if(J==testCurrentJob && N==0){

                    // Wait for 1 sec before running the next motion...
                    QTime dieTime= QTime::currentTime().addMSecs(1000);
                    while (QTime::currentTime() < dieTime) QCoreApplication::processEvents(QEventLoop::AllEvents, 20);

//                    action_testMotion();
                    qDebug()<<"ORDER Done!!\n";

                 }
                break;
              }

        }
    }
    else if(strcmp(socketHandler->ringBuffer.getCmd(),"Welcome to IStacker Server!")==0) {
        // After connected to the server at the begining, ask current all position and update the view...
        action_rebootRobot();
        action_setPosition();
        action_getAllPosition();
        action_getPauseStatus();
    }
//    if(codeValue==RC_ORDER_DONE)


    /////////////////////////////////////////////////
        socketHandler->ringBuffer.readDone();
}

void MainWindow::processSocketJobSchedulerTimer()
{

}
////////////////////////////////////////////////////////////////////////////////////
void runJointInterMove_timer()
{
    if(gSharedData.trTime.transitT>=gSharedData.trTime.finalT) {
        g_MainWindow->stopAllTimers();
        return;
    }
    // ++ Make sure that we are at the final position ++
    if((gSharedData.trTime.transitT+gSharedData.trTime.deltaT)>=gSharedData.trTime.finalT) {
        gSharedData.trTime.transitT=gSharedData.trTime.finalT;
    }
    for(int i=0; i<NUMJNT; i++){
        gSharedData.curJointPos[i] =
                robotKin.aCo[i][3]*pow(gSharedData.trTime.transitT,3) +
                robotKin.aCo[i][2]*pow(gSharedData.trTime.transitT,2)  +
                robotKin.aCo[i][1]*    gSharedData.trTime.transitT  +
                robotKin.aCo[i][0];
    }
    gSharedData.trTime.transitT +=gSharedData.trTime.deltaT;
    g_MainWindow->updateAllJointMove(gSharedData.curJointPos);
    g_MainWindow->JointEditVLayout->updateSilder();

}
void runLinearMove_timer()
{
/*
    if(gSharedData.trTime.transitT>=gSharedData.trTime.finalT) {
        cout<<" stoped: runLinearMove_timer()"<<endl;
        g_MainWindow->stopAllTimers();
        return;
    }
    // ++ Make sure that we are at the final position ++
    if((gSharedData.trTime.transitT+gSharedData.trTime.deltaT)>=gSharedData.trTime.finalT) {
        gSharedData.trTime.transitT=gSharedData.trTime.finalT;
    }
    //cout<<"curEEPos: ";
    for(int i=0; i<NUMJNT; i++){
        gSharedData.curEEPos[i] =
                robotKin.aCoEE[i][3]*pow(gSharedData.trTime.transitT,3) +
                robotKin.aCoEE[i][2]*pow(gSharedData.trTime.transitT,2)  +
                robotKin.aCoEE[i][1]*    gSharedData.trTime.transitT  +
                robotKin.aCoEE[i][0];
        //cout<<gSharedData.curEEPos[i]<<", ";
    }
    //cout<<gSharedData.trTime.transitT<<"\n-------------------------------\n";
    bool rev=robotKin.invKinEE_L2W_XYRZ(gSharedData.curEEPos);
    //   bool rev=robotKin.invKinEE_L_XYRZ_Delta(gSharedData.curEEPos);
    if(!rev) g_MainWindow->linearMoveTimer->stop();


    gSharedData.trTime.transitT +=gSharedData.trTime.deltaT;
    robotKin.updateCurrentJoint();
    g_MainWindow->renderRobotFront->updateZoeRobotConfig();

    g_MainWindow->renderRobotTop->updateZoeRobotConfig();
    g_MainWindow->renderRobotFront->_renderWindow->Render();
    g_MainWindow->renderRobotTop->_renderWindow->Render();
    */
    static long count=0;
    if(gSharedData.trTime.transitT>=gSharedData.trTime.finalT) {
        cout<<" stoped: runLinearMove_timer()"<<endl;
        g_MainWindow->stopAllTimers();
        count=0;
        return;
    }
    // ++ Make sure that we are at the final position ++
    if((gSharedData.trTime.transitT+gSharedData.trTime.deltaT)>=gSharedData.trTime.finalT) {
        gSharedData.trTime.transitT=gSharedData.trTime.finalT;
    }

    cout<<"curEEPos["<<count++<<"] "<<gSharedData.trTime.transitT<<": ";
    for(int i=0; i<NUMJNT; i++){
        gSharedData.curEEPos[i] =
                robotKin.aCoEE[i][3]*pow(gSharedData.trTime.transitT,3) +
                robotKin.aCoEE[i][2]*pow(gSharedData.trTime.transitT,2)  +
                robotKin.aCoEE[i][1]*    gSharedData.trTime.transitT  +
                robotKin.aCoEE[i][0];
//        cout<<gSharedData.curEEPos[i]<<", ";
    }
//    cout<<gSharedData.trTime.transitT<<"\n";
    double px =robotKin.param.Px, pz=robotKin.param.Pz, th1=robotKin.param.th1, th2=robotKin.param.th2;
    bool rev=robotKin.invKinEE_W_XYRZ(gSharedData.curEEPos[0],gSharedData.curEEPos[1],gSharedData.curEEPos[3],gSharedData.curEEPos[2]);
    cout<<"X,Z,R1,R2: "<<robotKin.param.Px<<", "<<robotKin.param.Pz<<", "<<(robotKin.param.th1)*180.0/M_PI<<", "<<(robotKin.param.th2)*180.0/M_PI;
    cout<<" :delta:"<<robotKin.param.Px-px<<", "<<robotKin.param.Pz-pz<<", "<<(robotKin.param.th1-th1)*180.0/M_PI<<", "<<(robotKin.param.th2-th2)*180.0/M_PI<<"\n";


    if(!rev) g_MainWindow->linearMoveTimer->stop();


    gSharedData.trTime.transitT +=gSharedData.trTime.deltaT;
    robotKin.updateCurrentJoint();



    g_MainWindow->renderRobotFront->updateZoeRobotConfig();
    g_MainWindow->renderRobotTop->updateZoeRobotConfig();

    g_MainWindow->renderRobotFront->_renderWindow->Render();
    g_MainWindow->renderRobotTop->_renderWindow->Render();

    g_MainWindow->JointEditVLayout->updateSilder();

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(g_MainWindow==nullptr) return;
    winSize[0]=width(); winSize[1]=height();
    qDebug()<<winSize[0]<<winSize[1];
//    robotKin.forKin(0,0,0,0);
    split_top_bottom->setSizes(QList<int>({int(winSize[1]*0.40), int(winSize[1]*0.60)}));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    vtkCamera *cameraFront = renderRobotFront->_renderer->GetActiveCamera();
    int key = event->key();
//    cout<< "++++++ "<<event->key()<<", "<<cameraFront<<endl;
//    cameraFront->Print(std::cout);
    if(key==90)// 'Z'
    {
        viewMode=0;
        // cameraFront->Print(std::cout);
        cameraFront->ParallelProjectionOn();
        cameraFront->SetParallelScale(1250.0); // tweak as needed
        cameraFront->SetClippingRange(camView[viewMode].clipping);
        cameraFront->SetFocalPoint(camView[viewMode].focalPoint);
        cameraFront->SetPosition(camView[viewMode].pos);
        cameraFront->SetViewUp(camView[viewMode].viewUP);
        renderRobotFront->_renderWindow->Render();
        return;
    }
    else if(key==88)// 'X'
    {
        viewMode=1;
        // cameraFront->Print(std::cout);

        cameraFront->ParallelProjectionOff();
        cameraFront->SetClippingRange(camView[viewMode].clipping);
        cameraFront->SetFocalPoint(camView[viewMode].focalPoint);
        cameraFront->SetPosition(camView[viewMode].pos);
        cameraFront->SetViewUp(camView[viewMode].viewUP);
        cameraFront->SetViewAngle(camView[viewMode].viewAng);
        renderRobotFront->_renderWindow->Render();
        return;

    }
    else if(key==67)// 'C'
    {
        viewMode=2;
        cameraFront->ParallelProjectionOff();
        cameraFront->SetClippingRange(camView[viewMode].clipping);
        cameraFront->SetFocalPoint(camView[viewMode].focalPoint);
        cameraFront->SetPosition(camView[viewMode].pos);
        cameraFront->SetViewUp(camView[viewMode].viewUP);
        cameraFront->SetViewAngle(camView[viewMode].viewAng);
        renderRobotFront->_renderWindow->Render();
        return;
    }
    else if(key==86)// 'V'
    {
        viewMode=3;
        cameraFront->ParallelProjectionOff();
        cameraFront->SetClippingRange(camView[viewMode].clipping);
        cameraFront->SetFocalPoint(camView[viewMode].focalPoint);
        cameraFront->SetPosition(camView[viewMode].pos);
        cameraFront->SetViewUp(camView[viewMode].viewUP);
        cameraFront->SetViewAngle(camView[viewMode].viewAng);
        renderRobotFront->_renderWindow->Render();
        return;
    }
    else if(key==66)// 'B'
    {
        viewMode=4;
        cameraFront->ParallelProjectionOff();
        cameraFront->SetClippingRange(camView[viewMode].clipping);
        cameraFront->SetFocalPoint(camView[viewMode].focalPoint);
        cameraFront->SetPosition(camView[viewMode].pos);
        cameraFront->SetViewUp(camView[viewMode].viewUP);
        cameraFront->SetViewAngle(camView[viewMode].viewAng);
        renderRobotFront->_renderWindow->Render();
        return;
    }
    else if(key==78)// 'N'
    {
        viewMode=5;
        cameraFront->ParallelProjectionOff();
        cameraFront->SetClippingRange(camView[viewMode].clipping);
        cameraFront->SetFocalPoint(camView[viewMode].focalPoint);
        cameraFront->SetPosition(camView[viewMode].pos);
        cameraFront->SetViewUp(camView[viewMode].viewUP);
        cameraFront->SetViewAngle(camView[viewMode].viewAng);
        renderRobotFront->_renderWindow->Render();
        return;
    }
    else
    {
        QWidget::keyPressEvent(event);
    }

}

