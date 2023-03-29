#include "qmkmotioneditvlayout.h"
#include "qmkcreatemotiondlg.h"
#include "qmkmotionparamlayout.h"
#include "qmkpushbutton.h"
#include "mainwindow.h"
#include "mkGlobalData.h"
#include "../sharedfiles/mkRobotKin.h"
#include "../sharedfiles/mkZoeRobotics_define.h"
#include "qmksetupvlayout.h"

#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QComboBox>
#include <QScrollBar>
#include <QGroupBox>
#include <QDebug>


extern MKZoeRobotKin robotKin;
extern QByteArray qss_data;
extern MainWindow *g_MainWindow;
extern globalSharedData gSharedData;
extern MapObjActor   *selectedTargetObj;
QmkMotionEditVLayout::QmkMotionEditVLayout(QWidget *parent) : QVBoxLayout(parent)
{
     cout<<"created QmkMotionEditVLayout"<<endl;

     this->setObjectName(QString::fromUtf8("motionPlan_widget"));
    createMotionLists();
     //return;
    // ++++++ Test ++++++
    FoldArmParam *param = new FoldArmParam;
    strcpy(param->name, "Order Americano");
    strcpy(param->desc, "Process #1");
    param->type = PLACE_TARGET;
    param->rot[0]=0.0;
    param->rot[1]=151.0;

    vecFoldArmParams.push_back(param);
    addMotionList(param);

    // ++++++ Test ++++++
    FoldArmParam *param1 = new FoldArmParam;
    strcpy(param1->name, "Order Coffee Wipped Cream");
    strcpy(param1->desc, "Process #2");
    param1->type = PLACE_TARGET;
    param1->rot[0]=0.0;
    param1->rot[1]=151.0;

    vecFoldArmParams.push_back(param1);
    addMotionList(param1);
    //---------------------
}

QmkMotionEditVLayout::~QmkMotionEditVLayout()
{
    clearAllItemsTableWidget();
}

void QmkMotionEditVLayout::createMotionLists()
{
    QVBoxLayout *listShowVLayout = new QVBoxLayout;
    QHBoxLayout *buttonHLayoutCmdButton1 = new QHBoxLayout;
    QHBoxLayout *buttonHLayoutCmdButton2 = new QHBoxLayout;
    QHBoxLayout *buttonHLayoutButton3 = new QHBoxLayout;


    QmkSetupVLayout *setupVLayout = new QmkSetupVLayout;
//    setupVLayout->setMargin(0);
    setupVLayout->setContentsMargins(0,0,0,0);

    QLabel *title_label =  new QLabel("MOTION EDITOR");
    title_label->setAlignment(Qt::AlignHCenter);

//    this->setSpacing(0);
//    this->addWidget(title_label);

    for(int i=0; i<4; i++) {
        buttonsCmd[i] = new QmkPushButton(bNameCommand[i]);
        buttonHLayoutCmdButton1->addWidget(buttonsCmd[i]);

        buttonsCmd[i+4] = new QmkPushButton(bNameCommand[i+4]);
        buttonHLayoutCmdButton2->addWidget(buttonsCmd[i+4]);
    }
    buttonsCmd[8] = new QmkPushButton(bNameCommand[8]);
    buttonHLayoutCmdButton2->addWidget(buttonsCmd[8]);

//    buttonMoveInMotion = new QmkPushButton("Move In Motion");
    buttonMoveInMotion = new QmkPushButton("Order Coffee #2");
    buttonSave         = new QmkPushButton("Save To File");
    buttonHLayoutButton3->addWidget(buttonSave);
    buttonHLayoutButton3->addWidget(buttonMoveInMotion);

    QLabel *labelCreateMotion = new QLabel(" Create Motion By Command");
    labelCreateMotion->setAlignment(Qt::AlignCenter);

    QVBoxLayout *groupBoxMotionCmdVLayout = new QVBoxLayout;
    groupBoxMotionCmdVLayout->addWidget(labelCreateMotion);
    groupBoxMotionCmdVLayout->addSpacing(10);
    groupBoxMotionCmdVLayout->addLayout(buttonHLayoutCmdButton1);
    groupBoxMotionCmdVLayout->addLayout(buttonHLayoutCmdButton2);
    groupBoxMotionCmdVLayout->setSpacing(0);



    QGroupBox * groupBoxMotionCmd = new QGroupBox;//("         Create Motion By Command");
    groupBoxMotionCmd->setLayout(groupBoxMotionCmdVLayout);
    groupBoxMotionCmd->setContentsMargins(10,0,10,0);
    ///////////////////////////////////////////////////////////////////
    QLabel *labelMotionName = new QLabel("User Defined Motion Lists");
    labelMotionName->setAlignment(Qt::AlignCenter);

    tableWidgetMotionLists = new QTableWidget(0,4);
    tableWidgetMotionLists->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetMotionLists->setHorizontalHeaderLabels( QStringList() <<tr("Motion Name") << tr("Category")<<tr("Description")<<tr("cat ID"));
    tableWidgetMotionLists->horizontalHeader()->resizeSections(QHeaderView::Stretch);//->setResizeMode(QHeaderView::Stretch);
    tableWidgetMotionLists->setColumnWidth(0, 180);//tableWidgetParmas->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignRight);
    tableWidgetMotionLists->setColumnWidth(1, 180);
    tableWidgetMotionLists->setColumnWidth(2, -1);
    tableWidgetMotionLists->setColumnHidden(3,true);
    tableWidgetMotionLists->setMinimumHeight(50);

    ///////////////////////////////////////////////////////////////////
    paramLayout = new QmkMotionParamLayout;

   //////////////////////////////////////////////////////////////////

    listShowVLayout->addLayout(setupVLayout);
    listShowVLayout->addSpacing(0);
    listShowVLayout->addWidget(title_label);
    listShowVLayout->addWidget(groupBoxMotionCmd);
    listShowVLayout->addSpacing(0);
    listShowVLayout->addWidget(labelMotionName);
    listShowVLayout->addSpacing(0);
    listShowVLayout->addLayout(buttonHLayoutButton3);
    listShowVLayout->addSpacing(0);
    listShowVLayout->addWidget(tableWidgetMotionLists);
    listShowVLayout->addSpacing(0);
    listShowVLayout->addLayout(paramLayout);

    connect(tableWidgetMotionLists, &QTableWidget::itemSelectionChanged,
            this, &QmkMotionEditVLayout::slot_selectionChangedTableWidgetMotionList);
    //////////////////////////////////////////////////////////////
    this->addSpacing(0);
    this->addLayout(listShowVLayout);
    this->addStretch();

    connect(buttonsCmd[0], &QmkPushButton::mkClicked, this, &QmkMotionEditVLayout::clickedButton_dropCup);
    connect(buttonsCmd[1], &QmkPushButton::mkClicked, this, &QmkMotionEditVLayout::clickedButton_createRotateEE);
    connect(buttonsCmd[2], &QmkPushButton::mkClicked, this, &QmkMotionEditVLayout::clickedButton_createLinearFlyTo);
    connect(buttonsCmd[3], &QmkPushButton::mkClicked, this, &QmkMotionEditVLayout::clickedButton_createFoldingArm);
    connect(buttonsCmd[4], &QmkPushButton::mkClicked, this, &QmkMotionEditVLayout::clickedButton_createPlaceTarget);
    connect(buttonsCmd[5], &QmkPushButton::mkClicked, this, &QmkMotionEditVLayout::clickedButton_createGrabTarget);
    connect(buttonsCmd[6], &QmkPushButton::mkClicked, this, &QmkMotionEditVLayout::clickedButton_createCircularMotion);
    connect(buttonsCmd[7], &QmkPushButton::mkClicked, this, &QmkMotionEditVLayout::clickedButton_createSpiralMotion);
    connect(buttonsCmd[8], &QmkPushButton::mkClicked, this, &QmkMotionEditVLayout::clickedButton_testMotion);

    connect(buttonMoveInMotion, &QmkPushButton::mkClicked,this, &QmkMotionEditVLayout::clickedButton_MoveInMotion);

    cout<<"created QmkMotionEditVLayout  done"<<endl;
}

void QmkMotionEditVLayout::clearAllItemsTableWidget()
{
   // tableWidgetParmas->setRowCount(0);
    if(createMotionDlg) {delete createMotionDlg; createMotionDlg=nullptr;}
    if(editRepeat1) { delete editRepeat1; editRepeat1=nullptr;}
    if(editSpeed1)  { delete editSpeed1; editSpeed1=nullptr;}
}

void *QmkMotionEditVLayout::searchMotion(string name)
{
    cout<<"searchMotion: name-"<<name<<endl;
    //////////////////////////////////////////////////////////
    // Search in vecFoldArmParams
    auto itrFA = std::find_if(vecFoldArmParams.begin(), vecFoldArmParams.end(),
                            [&](FoldArmParam *i){
                              return (i && (string(i->name)== name));
                });
    if(itrFA!=vecFoldArmParams.end())
    {
        return (*itrFA);
    }

    //////////////////////////////////////////////////////////
    // Search in vecFoldArmParams
    auto itrPT = std::find_if(vecPlaceTargetParams.begin(), vecPlaceTargetParams.end(),
                            [&](PlaceTargetParam *i){
                              return (i && (string(i->name)== name));
                });
    if(itrPT!=vecPlaceTargetParams.end())
    {
        return (*itrPT);
    }

    return nullptr;
}

void QmkMotionEditVLayout::addMotionList(void * param)
{
    const int constRrow=4;
    int currentRow = tableWidgetMotionLists->rowCount();
    tableWidgetMotionLists->setRowCount(currentRow+1);
    QTableWidgetItem *colItems[constRrow]={nullptr};
    for(int i=0; i<constRrow; i++){
        colItems[i] = new QTableWidgetItem;
        tableWidgetMotionLists->setItem(currentRow,i, colItems[i]);
    }
    MotionParam *mParam = static_cast<MotionParam *>(param);
    int type = mParam->type;
    colItems[0]->setText(mParam->name);
    colItems[1]->setText(bNameCommand[type]);// Category
    colItems[2]->setText(mParam->desc);
    colItems[3]->setText(QString::number(type));
    tableWidgetMotionLists->selectRow(currentRow);

}

void QmkMotionEditVLayout::slot_selectionChangedTableWidgetMotionList()
{

    if(tableWidgetMotionLists->rowCount()<1) return;

    if(tableWidgetMotionLists->selectedItems().size()<1){
        paramLayout->clearLayout();
        return;
    }
    string name = tableWidgetMotionLists->selectedItems().at(0)->text().toStdString();

    // Make column 4th unhidden to grab a data...
    tableWidgetMotionLists->setColumnHidden(3,false);
    int    type = tableWidgetMotionLists->selectedItems().at(3)->text().toInt();
    tableWidgetMotionLists->setColumnHidden(3,true);

   // cout<<"name:"<<Name<<"Type: "<< type<<endl;


    selectedParam = searchMotion(name);
    cout<<"Selected Param Name:"<<name<<", param:"<<selectedParam<<", selected row number:"<<tableWidgetMotionLists->selectedItems().at(0)->row()<<endl;
    selectMotionList(selectedParam,type);
}

void QmkMotionEditVLayout::clickedButton_MoveInMotion()
{
    g_MainWindow->action_orderSequence(tableWidgetMotionLists->selectedItems().at(0)->row()+1);
    /*
    if(!selectedParam)return;
    MotionParam *mParam = static_cast<MotionParam*>(selectedParam);
    if(mParam->type==FOLD_ARM)
    {
        FoldArmParam *faParam = static_cast<FoldArmParam*>(selectedParam);
        cout<<"clickedButton_MoveInMotion:Name="<<faParam->name<<endl;
    }
    else if(mParam->type==PLACE_TARGET)
    {
        PlaceTargetParam *ptParam = static_cast<PlaceTargetParam*>(selectedParam);
        cout<<"clickedButton_MoveInMotion:Name = "<<ptParam->name<<endl;
    }
    */
}

void QmkMotionEditVLayout::selectMotionList(void *param, int type)
{
    switch(type)
    {
    case MOVE_X:break;
    case MOVE_Z:break;
    case FLY_TO:  break;
    case FOLD_ARM:     paramLayout->selectFoldArm(static_cast<FoldArmParam*>(param));     break;
    case PLACE_TARGET: paramLayout->selectPlaceTarget(static_cast<PlaceTargetParam*>(param)); break;
    case GRAB_TARGET:break;
    case CIRCULAR_MOTION: break;
    case SPIRAL_MOTION: break;
    }
}

void QmkMotionEditVLayout::clickedButton_dropCup()
{
//    double maxPos=0.0;
//    int miliSecond = 20;
    cout<<"clickedButton_dropCup()"<<endl;
//    gSharedData.trTime.transitT = 0.0;
//    gSharedData.trTime.deltaT = double(miliSecond)/1000.0;
//    for(int i=0; i<NUMJNT; i++){
//        double deltaPos = targetJntPos[i] -  gSharedData.curJointPos[i];

//        if(i==2) deltaPos *=robotKin.Link1;
//        else if(i==3) deltaPos *=robotKin.Link2;

//        if(maxPos<=abs(deltaPos))
//        {
//            maxPos=abs(deltaPos);
//        }
//    }
//    double angTf=maxPos/linVel;
//    if(angTf<=1.0) angTf=1.0;
//    gSharedData.trTime.finalT = angTf;
//    qDebug()<<"executeMotion: time(maxAng, angTf)"<<maxPos<<angTf;
//    int rev = robotKin.setCubicalPolynominalJointInter(gSharedData.trTime.finalT, gSharedData.curJointPos, targetJntPos);
//    if(rev<1) return;
    g_MainWindow->action_dropCup();
}

void QmkMotionEditVLayout::clickedButton_createRotateEE()
{
    //g_MainWindow->linearMoveTimer->stop();
    g_MainWindow->action_moveEERotate();
}

void QmkMotionEditVLayout::clickedButton_createLinearFlyTo()
{
    if(!selectedTargetObj) return;
    g_MainWindow->action_moveLinear();

}

void QmkMotionEditVLayout::clickedButton_createFoldingArm()
{
    clearAllItemsTableWidget();
    g_MainWindow->stopAllTimers();
    createMotionDlg = new QmkCreateMotionDlg(FOLD_ARM);
    connect(createMotionDlg->buttonAccept, &QmkPushButton::mkClicked,
        [&](){// Lambda Function...

        if(createMotionDlg->paramVLayout->lineEditInput[0]->text().isEmpty()) {
            createMotionDlg->note->setText("Note: Name is empty, please fill the name");
            return;
        }
        double r1=createMotionDlg->paramVLayout->lineEditInput[2]->text().toDouble();
        double r2=createMotionDlg->paramVLayout->lineEditInput[3]->text().toDouble();
        if(r1< robotKin.minJntLimit[2] || r1>robotKin.maxJntLimit[2])
        {
             createMotionDlg->note->setText("Note: Joint R1 is out of range. <"+
                                               QString::number(robotKin.minJntLimit[2]) + ", "
                                                + QString::number(robotKin.maxJntLimit[2]) + ">");
             return;
        }
        if(r2< robotKin.minJntLimit[3] || r1>robotKin.maxJntLimit[3])
        {
             createMotionDlg->note->setText("Note: Joint R2 is out of range. <"+
                                               QString::number(robotKin.minJntLimit[3]) + ", "
                                                + QString::number(robotKin.maxJntLimit[3]) + ">");
             return;
        }
        FoldArmParam *param = new FoldArmParam;
        param->type=FOLD_ARM;
        strcpy(param->name, createMotionDlg->paramVLayout->lineEditInput[0]->text().toStdString().c_str());
        strcpy(param->desc, createMotionDlg->paramVLayout->lineEditInput[1]->text().toStdString().c_str());
        param->rot[0]=r1;
        param->rot[1]=r2;

        vecFoldArmParams.push_back(param);
        addMotionList(param);
        createMotionDlg->close();
    });
    createMotionDlg->exec();
    delete createMotionDlg;
    createMotionDlg=nullptr;
}

void QmkMotionEditVLayout::clickedButton_createPlaceTarget()
{
    clearAllItemsTableWidget();
    g_MainWindow->stopAllTimers();
    createMotionDlg = new QmkCreateMotionDlg(PLACE_TARGET);
    connect(createMotionDlg->buttonAccept, &QmkPushButton::mkClicked,
            [&]() {// Lambda Function implementation
        if(createMotionDlg->paramVLayout->lineEditInput[0]->text().isEmpty()) {
            createMotionDlg->note->setText("Note: Name is empty, please fill the name");
            return;
        }
        if(createMotionDlg->paramVLayout->lineEditInput[2]->text().isEmpty()) {
            createMotionDlg->note->setText("Note: Name is empty, please fill the target name");
            return;
        }
        PlaceTargetParam *param = new PlaceTargetParam;
        param->type=PLACE_TARGET;
        strcpy(param->name,       createMotionDlg->paramVLayout->lineEditInput[0]->text().toStdString().c_str());
        strcpy(param->desc,       createMotionDlg->paramVLayout->lineEditInput[1]->text().toStdString().c_str());
        strcpy(param->targetName, createMotionDlg->paramVLayout->lineEditInput[2]->text().toStdString().c_str());
        for(int i=0; i<4; i++) {
            double val = createMotionDlg->paramVLayout->lineEditInput[3+i]->text().toDouble();
            param->metric[i]=val;
        }
        vecPlaceTargetParams.push_back(param);
        addMotionList(param);
        createMotionDlg->close();
    });
    createMotionDlg->exec();
    delete createMotionDlg;
    createMotionDlg=nullptr;
}

void QmkMotionEditVLayout::clickedButton_createGrabTarget()
{

}

void QmkMotionEditVLayout::clickedButton_createCircularMotion()
{
    CircleProfile circleProfile;
    circleProfile.speed=360.0/2.5;
    circleProfile.radius=50/1.5;
    circleProfile.cenPosX=robotKin.curEEx;//-circleProfile.radius;//1000;
    circleProfile.cenPosY=robotKin.curEEy;///-250;
//    if(robotKin.curEEth>=0)
//        circleProfile.EETheta=90;
//    else
//        circleProfile.EETheta=-90;
    circleProfile.EETheta = robotKin.curEEth*RAD2DEG;
    circleProfile.arcAng=360*1;
    g_MainWindow->action_moveCircle(circleProfile);
}

void QmkMotionEditVLayout::clickedButton_createSpiralMotion()
{
    SpiralProfile sprialProfile;
    sprialProfile.speed=360.0/2.5;
    sprialProfile.radius=50/2;
    sprialProfile.cenPosX=robotKin.curEEx;//-sprialProfile.radius;//1000;
    sprialProfile.cenPosY=robotKin.curEEy;///-250;
//    if(robotKin.curEEth>=0)
//        sprialProfile.EETheta=90;
//    else
//        sprialProfile.EETheta=-90;
    sprialProfile.EETheta = robotKin.curEEth*RAD2DEG;
    sprialProfile.arcAng=360*3;
    sprialProfile.heightZ = -50;
    g_MainWindow->action_moveSpiral(sprialProfile);
}

void QmkMotionEditVLayout::clickedButton_testMotion()
{
    g_MainWindow->action_testMotion();
}
