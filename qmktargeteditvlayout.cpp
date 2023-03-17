#include "qmktargeteditvlayout.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QModelIndexList>
#include <QTableWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <../sharedfiles/mkRobotKin.h>
#include "mainwindow.h"
#include "renderrobotarm.h"
#include "qmkinputtargetnamedlg.h"
#include "qmkpushbutton.h"
#include "qmktargetsliderhlayout.h"
#include "qmkjointeditvlayout.h"
#include "jntctrldialog.h"
#include "mkGlobalData.h"
#include "mkfilemanager.h"
#include "../sharedfiles/mkZoeRobotics_velProfile.h"
#define myqDebug() qDebug() << fixed << qSetRealNumberPrecision(3)

extern  MKZoeRobotKin robotKin;
extern MainWindow *g_MainWindow;
extern globalSharedData gSharedData;
MapObjActor   *selectedTargetObj=nullptr;
static void convertW2LEE(double *posEE)
{
    if(!posEE) return;
//        double tmpPos[4];
//        memcpy(tmpPos, posEE, sizeof(double)*4);
    mkMatrix L_EE = rotZrad(-posEE[3])*traXYZ(posEE[0], posEE[1],0);
    posEE[0]=L_EE.m_ppData[0][3];
    posEE[1]=L_EE.m_ppData[1][3];
}
QmkTargetEditVLayout::QmkTargetEditVLayout(QWidget *parent) : QVBoxLayout(parent)
{
    cout<<"created QmkTargetEditVLayout"<<endl;
    createPORTargetLists();
}

QmkTargetEditVLayout::~QmkTargetEditVLayout()
{
            if(tableWidget) delete tableWidget;
            if(selectedLabel) delete selectedLabel;
            for(int i=0; i<4; i++) if(buttonsTop[i]) delete buttonsTop[i];
            for(int i=0; i<2; i++) if(buttonsBottom[i]) delete buttonsBottom[i];
         //   for(int i=0; i<4; i++) if(POREdit[i]) delete POREdit[i];

}

void QmkTargetEditVLayout::createPORTargetLists()
{
    const int numButton = 6;


    QHBoxLayout *buttonHLayoutTop = new QHBoxLayout;
    QHBoxLayout *buttonHLayoutBottom = new QHBoxLayout;


    QVBoxLayout *listShowVLayout = new QVBoxLayout;
    QVBoxLayout *descShowVLayout = new QVBoxLayout;


    tableWidget = new QTableWidget(0,4);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode( QAbstractItemView::SingleSelection );
    tableWidget->setHorizontalHeaderLabels( QStringList() <<tr("Name") << tr("Target Type")<<tr("Move Type")<<tr("Description"));
    tableWidget->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    tableWidget->setColumnWidth(0, -1);//tableWidgetParmas->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignRight);
    tableWidget->setColumnWidth(1, 100);
    tableWidget->setColumnWidth(2, 100);
    tableWidget->setColumnWidth(3, 100);
    tableWidget->setMinimumHeight(200);
    //tableWidget->setFixedWidth(300);

   // listWidget->setMinimumHeight(200);
    const char *bNameTop[numButton]={"Add","Modify","Delete","Move2Target", "MoveRobot", "MoveAllRobot"};
    //const char *bNameBottom[3]={"SaveToFile", "Update Target with Current Configure", "Go Configure"};
    const char *bNameBottom[3]={"SaveToFile", "GetCurrentConfig", "Go Configure"};

//    QButtonGroup *radioButtonGroup = new QButtonGroup;

    QRadioButton *jointMove = new QRadioButton("Jonit");
    QRadioButton *linearMove = new QRadioButton("Linear");
//    radioButtonGroup->addButton(jointMove);
//    radioButtonGroup->addButton(linearMove);
    jointMove->setChecked(true);

    for(int i=0; i<numButton; i++) {
        buttonsTop[i] = new QmkPushButton(bNameTop[i]);
        //buttonsTop[i]->setFixedWidth(100);
        // ... add redio buttons ...
        if(i==3){
            buttonHLayoutTop->addWidget(jointMove);
            buttonHLayoutTop->addWidget(linearMove);
        }
        buttonHLayoutTop->addWidget(buttonsTop[i]);
    }

    for(int i=0; i<3; i++) {
        buttonsBottom[i] = new QmkPushButton(bNameBottom[i]);
        buttonHLayoutBottom->addWidget(buttonsBottom[i]);
    }
    QLabel *listTitle = new QLabel("Target Name Lists");
    listTitle->setAlignment(Qt::AlignCenter);
    listShowVLayout->addWidget(listTitle);
    listShowVLayout->addWidget(tableWidget);

    ////////////////////////////////////////////
    //selectedLabel = new QLabel("");
    //descShowVLayout->addSpacing(30);
    //descShowVLayout->addWidget(new QLabel("Description of Selected Target: ") );
   // descShowVLayout->addWidget(selectedLabel );
    //descShowVLayout->addSpacing(30);
    const char *rangeName[4]={"Tx[mm]","Ty[mm]","Tz[mm]","TrotZ[deg]"};
    for(int i=0; i<4; i++)
    {
        targetSliderLayout[i] = new QmkTargetSliderHLayout(this, i, QString(rangeName[i]), robotKin.minTargetLimit[i],robotKin.maxTargetLimit[i]);
        descShowVLayout->addLayout(targetSliderLayout[i]);
        //descShowVLayout->addLayout(fillEditRange(const_cast<char *>(rangeName[i]), i));
    }
    //descShowVLayout->addStretch();//addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding));

    QLabel *title_label =  new QLabel("TARGET EDITOR");
    title_label->setAlignment(Qt::AlignHCenter);
    buttonHLayoutTop->setSpacing(2);
    this->addSpacing(10);
    this->addWidget(title_label);
    this->addLayout(buttonHLayoutTop);
    this->addLayout(listShowVLayout);
    this->addLayout(descShowVLayout);
    this->addSpacing(10);
    this->addLayout(buttonHLayoutBottom);
    this->addSpacing(10);
   // this->addStretch();
    connect(buttonsTop[0], &QmkPushButton::mkClicked, this, &QmkTargetEditVLayout::clickedButton_Add);
    connect(buttonsTop[1], &QmkPushButton::mkClicked, this, &QmkTargetEditVLayout::clickedButton_ModifyTarget);
    connect(buttonsTop[2], &QmkPushButton::mkClicked, this, &QmkTargetEditVLayout::clickedButton_Delete);
    connect(buttonsTop[3], &QmkPushButton::mkClicked, this, &QmkTargetEditVLayout::clickedButton_MoveToTargetAnimation);
    connect(buttonsTop[4], &QmkPushButton::mkClicked, this, &QmkTargetEditVLayout::clickedButton_MoveToTargetRobot);
    connect(buttonsTop[5], &QmkPushButton::mkClicked, this, &QmkTargetEditVLayout::clickedButton_MoveToAllTargetRobot);

    connect(buttonsBottom[0], &QmkPushButton::mkClicked, this, &QmkTargetEditVLayout::clickedButton_SaveToFile);
    connect(buttonsBottom[1], &QmkPushButton::mkClicked, this, &QmkTargetEditVLayout::clickedButton_UpdateWithCurrentConfigure);
    connect(buttonsBottom[2], &QmkPushButton::mkClicked, this, &QmkTargetEditVLayout::clickedButton_GoConfigure);

    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, &QmkTargetEditVLayout::slot_selectedListWidgetItem);
    connect(tableWidget, &QTableWidget::doubleClicked, this, &QmkTargetEditVLayout::clickedButton_MoveToTarget);


    connect(jointMove, &QRadioButton::toggled, this, &QmkTargetEditVLayout::toggledJointMove);
}
void QmkTargetEditVLayout::toggledJointMove(bool checked)
{
    qDebug()<<"toggledJointMove"<<checked;
    isJointMove=checked;
}
//void QmkTargetEditVLayout::toggledLinearMove(bool checked)
//{
//    qDebug()<<"toggledJointMove"<<checked;
//}
void QmkTargetEditVLayout::loadTargetXMLFile()
{
    mkFileManager myFile(this);
    myFile.readTargetListXML(TARGET_SAVE_PATH);
    bool hasIt=false;
    for( MapObjActor * objActor:g_MainWindow->renderRobotFront->objTarget) {
        int currentRow = tableWidget->rowCount();
        tableWidget->setRowCount(currentRow+1);

        QTableWidgetItem *colItems[4]={nullptr};
        for(int i=0; i<4; i++){
            colItems[i] = new QTableWidgetItem;
            colItems[i]->setFlags(colItems[i]->flags() & ~Qt::ItemIsEditable);// Disable Editable
            tableWidget->setItem(currentRow,i, colItems[i]);
        }
        colItems[0]->setText(objActor->param.name);
        colItems[1]->setText(objActor->param.getTargetType().c_str());// Category
        colItems[2]->setText(objActor->param.getMoveType().c_str());
        colItems[3]->setText(objActor->param.desc);

        hasIt=true;
    }

    if(hasIt){
        tableWidget->selectRow(0);
    }
}
void QmkTargetEditVLayout::slot_selectedListWidgetItem()
{
    qDebug()<<"<< slot_selectedListWidgetItem()";
    g_MainWindow->stopAllTimers();
    if(tableWidget==nullptr) return;
    if(tableWidget->selectedItems().size()<1){
       // paramLayout->clearLayout();
        return;
    }

    QItemSelectionModel *select = tableWidget->selectionModel();

    if(select->hasSelection()) //check if has selection
    {
        cout<<"selected row number: "<<select->selectedRows().at(0).row()<<endl; // return selected row(s)
        //select->selectedColumns(); // return selected column(s)
    }
    QList<QTableWidgetItem*> items = tableWidget->selectedItems();
    if(items.size()==0) return;
   // cout<<"items:"<<items.size()<<endl;
    QString itName =  tableWidget->selectedItems().at(0)->text();
    queueHistoryName.push(itName);

    /////////////////////////////////////////////////
    // Handling Previous Target Properties...
    if(queueHistoryName.size()>1) {
        QString name = queueHistoryName.front();
        MapObjActor *prevObj = g_MainWindow->renderRobotFront->searchTarget(name.toStdString());
        if(prevObj){

            cout<<"Previous selected actor: "<<prevObj->actor->GetProperty()<<endl;

            if(prevObj->param.getTargetType()=="APPROACH")
            {
                prevObj->actor->GetProperty()->SetColor(0.,0.,0.3);
                prevObj->actor->SetScale(0.5,1,0.5);
            }
            else {
                prevObj->actor->GetProperty()->SetColor(1,1,1);
                prevObj->actor->SetScale(1,1,1);
            }

            prevObj->actor->GetProperty()->SetOpacity(0.5);
            prevSelectedName=name;

        }
        queueHistoryName.pop();
    }
    //////////////////////////////////////////////////
    // Handling Selected Target Properties
    selectedTargetObj = g_MainWindow->renderRobotFront->searchTarget(itName.toStdString());
    if(selectedTargetObj) {
        cout<<"After selected actor: "<<selectedTargetObj->actor->GetProperty()<<endl;
        //selectedTargetObj->setColorObj();

        if(selectedTargetObj->param.getTargetType()=="APPROACH")
        {
            selectedTargetObj->actor->GetProperty()->SetColor(0.,0.,0.3);
            selectedTargetObj->actor->SetScale(0.5,1,0.5);
        }
        else {
            selectedTargetObj->actor->GetProperty()->SetColor(1,1,1);
            selectedTargetObj->actor->SetScale(1,1,1);
        }

        selectedTargetObj->actor->GetProperty()->SetColor(1,0,0);
        selectedTargetObj->actor->GetProperty()->SetOpacity(1);

        g_MainWindow->renderRobotFront->_renderWindow->Render();
        updateTargetInfo();
    }
}
/*
void QmkTargetEditVLayout::slot_selectedListWidgetItem()
{
    qDebug()<<"<< slot_selectedListWidgetItem()";
    g_MainWindow->stopAllTimers();
    if(tableWidget==nullptr) return;
    if(tableWidget->selectedItems().size()<1){
       // paramLayout->clearLayout();
        return;
    }

    QItemSelectionModel *select = tableWidget->selectionModel();

    if(select->hasSelection()) //check if has selection
    {
        cout<<"selected row number: "<<select->selectedRows().at(0).row()<<endl;; // return selected row(s)
        //select->selectedColumns(); // return selected column(s)
    }
    QList<QTableWidgetItem*> items = tableWidget->selectedItems();
    if(items.size()==0) return;
   // cout<<"items:"<<items.size()<<endl;
    QString itName =  tableWidget->selectedItems().at(0)->text();
    queueHistoryName.push(itName);
    if(LastPickedTargetActor) {
        cout<<"Previous selected actor: "<<LastPickedTargetActor->GetProperty()<<endl;

        if(LastPickedTargetType=="APPROACH")
        {
            LastPickedTargetActor->GetProperty()->SetColor(0.,0.,0.3);
            LastPickedTargetActor->SetScale(0.5,1,0.5);
        }
        else {
            LastPickedTargetActor->GetProperty()->SetColor(1,1,1);
            LastPickedTargetActor->SetScale(1,1,1);
        }

        LastPickedTargetActor->GetProperty()->SetOpacity(0.5);

    }

    selectedTargetObj = g_MainWindow->renderRobotFront->searchTarget(itName.toStdString());
    if(selectedTargetObj) {
        cout<<"After selected actor: "<<selectedTargetObj->actor->GetProperty()<<endl;
        //selectedTargetObj->setColorObj();

        if(selectedTargetObj->param.getTargetType()=="APPROACH")
        {
            selectedTargetObj->actor->GetProperty()->SetColor(0.,0.,0.3);
            selectedTargetObj->actor->SetScale(0.5,1,0.5);
        }
        else {
            selectedTargetObj->actor->GetProperty()->SetColor(1,1,1);
            selectedTargetObj->actor->SetScale(1,1,1);
        }

        selectedTargetObj->actor->GetProperty()->SetColor(1,0,0);
        selectedTargetObj->actor->GetProperty()->SetOpacity(1);

        LastPickedTargetActor = selectedTargetObj->actor;
        LastPickedTargetType = selectedTargetObj->param.getTargetType();


        g_MainWindow->renderRobotFront->_renderWindow->Render();
        updateTargetInfo();
    }
}
*/
void QmkTargetEditVLayout::clickedButton_GoConfigure()
{
    g_MainWindow->stopAllTimers();
    if(!selectedTargetObj) return;
    if(tableWidget==nullptr) return;

    for(int i=0; i<4; i++)
    {
        double val = targetSliderLayout[i]->edit->text().toDouble();
        if(i==3) val *= DTOR;
        selectedTargetObj->param.posTargetEE[i]=val;
    }

    mkMatrix Ttarget;
    vtkSmartPointer<vtkTransform> transformTarget = vtkSmartPointer<vtkTransform>::New();
    Ttarget = traXYZ(selectedTargetObj->param.posTargetEE[0]+robotKin.offDisX,
                     selectedTargetObj->param.posTargetEE[1]+robotKin.offDisY,
                     selectedTargetObj->param.posTargetEE[2]+robotKin.offDisZ)*rotZrad(selectedTargetObj->param.posTargetEE[3]+M_PI_2);
    transformTarget->SetMatrix(Ttarget.m_pData);

    selectedTargetObj->actor->SetUserTransform(transformTarget);
    g_MainWindow->renderRobotFront->_renderWindow->Render();
    updateTargetInfo();
}

void QmkTargetEditVLayout::updateTargetInfo()
{
   // selectedLabel->setText(selectedTargetObj->desc);
    for(int i=0; i<4; i++) targetSliderLayout[i]->setValue(selectedTargetObj->param.posTargetEE[i]);
    g_MainWindow->renderRobotTop->moveSelectedTarget(selectedTargetObj->param.posTargetEE);


}

void QmkTargetEditVLayout::setSelectionTarget(MapObjActor *objTarget)
{

    QList<QTableWidgetItem*> matched = tableWidget->findItems(objTarget->param.name, Qt::MatchExactly);
    if(matched.size()>0)
    {
        tableWidget->setCurrentItem(matched.at(0));
        //slot_selectedListWidgetItem();
         qDebug()<<"---I found Item:"<<matched.at(0)->text();

    }

}
void QmkTargetEditVLayout::clickedButton_Add()
{
    g_MainWindow->stopAllTimers();
    inputDialog = new QmkInputTargetNameDlg("Create Target Name");
    targetName="";
    connect(inputDialog->buttonAccept, &QPushButton::clicked,
        [this](){// Lambda Function...
             targetName = inputDialog->targetNameLineEdit->text().toStdString();
             string desc = inputDialog->targetDescLineEdit->text().toStdString();
             string targetType = inputDialog->targetTypeCombo->currentText().toStdString();
             string moveType = inputDialog->moveTypeCombo->currentText().toStdString();
             string value[4]={targetName, desc, targetType, moveType};
             selectedTargetObj = g_MainWindow->renderRobotFront->searchTarget(targetName);
             if(selectedTargetObj==nullptr)// No duplicated
             {

                 selectedTargetObj = g_MainWindow->renderRobotFront->addTarget(value);
                 int currentRow = tableWidget->rowCount();
                 tableWidget->setRowCount(currentRow+1);

                 QTableWidgetItem *colItems[4]={nullptr};
                 for(int i=0; i<4; i++){
                     colItems[i] = new QTableWidgetItem;
                     tableWidget->setItem(currentRow,i, colItems[i]);
                 }
                 colItems[0]->setText(selectedTargetObj->param.name);
                 colItems[1]->setText(selectedTargetObj->param.getTargetType().c_str());// Category
                 colItems[2]->setText(selectedTargetObj->param.getMoveType().c_str());
                 colItems[3]->setText(selectedTargetObj->param.desc);

                 tableWidget->selectRow(currentRow);
                 inputDialog->close();
             }
             else// Already Target Name exists...
             {
                QString str="Note: Target Name ["+QString(targetName.c_str())+"] is already used.\n Please choose different name.";
                inputDialog->note->setText(str);
                inputDialog->note->show();
               // cout<<"found"<<endl;
             }
           // cout<<"++++++++"<<inputDialog->targetNameLineEdit->text().toStdString()<<" --------";
        }// -------------- end of Lambda Function ---------------
    );
    inputDialog->exec();
    delete inputDialog;
    tableWidget->setFocus();
}
void QmkTargetEditVLayout::clickedButton_Delete()
{
    g_MainWindow->stopAllTimers();
    if(!selectedTargetObj) {
        g_MainWindow->renderRobotTop->targetActor->VisibilityOff();
        g_MainWindow->renderRobotTop->_renderWindow->Render();
        return;
    }

     QList<QTableWidgetItem*> items = tableWidget->selectedItems();
     if(items.size()==0){
         selectedTargetObj=nullptr;
         return;
     }

     QItemSelectionModel *select = tableWidget->selectionModel();
     int selectedRow=-1;
     if(select->hasSelection()) //check if has selection
     {
         selectedRow = select->selectedRows().at(0).row(); // return selected row(s)
         cout<<"selectedRow: "<<selectedRow<<endl;
     }
    else return;

     std::vector<MapObjActor*>  *objVector = &g_MainWindow->renderRobotFront->objTarget;

    /////////////////////////////////////
    //1. Remove MapObjActor from std::vector<MapObjActor*>...
     auto itr = std::find_if(objVector->begin(),
                        objVector->end(),
                        [&](MapObjActor* i)  {
                            cout<<"clickedButton_Delete()::"<<i->param.name<<endl;
                            return (i&&(i==selectedTargetObj));
                        });

     if (itr != objVector->end()) {
         // Once find the MapObjActor then remove actor from _renderer
         // and remove MapObjActor object from std::vector...
         (*itr)->removeActor();
         objVector->erase(itr);
         selectedTargetObj=nullptr;
     }
     //////////////////////////////
     //2. Remove string item from QListWidget

     foreach(QTableWidgetItem * item, items)
     {
         delete item;
     }
     tableWidget->removeRow(selectedRow);

     g_MainWindow->renderRobotFront->_renderWindow->Render();
}
void QmkTargetEditVLayout::clickedButton_ModifyTarget()
{
     if(!selectedTargetObj) return;
     g_MainWindow->stopAllTimers();
    if(tableWidget->selectedItems().at(0)==nullptr) return;

    inputDialog = new QmkInputTargetNameDlg("Modify Targe Info");
    currentName = tableWidget->selectedItems().at(0)->text().toStdString();
    inputDialog->targetNameLineEdit->setText(currentName.c_str());
    inputDialog->targetDescLineEdit->setText(selectedTargetObj->param.desc);
    inputDialog->targetTypeCombo->setCurrentText(selectedTargetObj->param.getTargetType().c_str());
    inputDialog->moveTypeCombo->setCurrentText(selectedTargetObj->param.getMoveType().c_str());
    connect(inputDialog->buttonAccept, &QPushButton::clicked,
        [this](){// Lambda Function...
             targetName = inputDialog->targetNameLineEdit->text().toStdString();
             string desc = inputDialog->targetDescLineEdit->text().toStdString();
             string targetType = inputDialog->targetTypeCombo->currentText().toStdString();
             string moveType = inputDialog->moveTypeCombo->currentText().toStdString();
             string value[4]={targetName, desc, targetType, moveType};
             if(targetName == currentName )
             {
                 selectedTargetObj->setData(value);

                 tableWidget->selectedItems().at(0)->setText(targetName.c_str());
                 tableWidget->selectedItems().at(1)->setText(targetType.c_str());
                 tableWidget->selectedItems().at(2)->setText(moveType.c_str());
                 tableWidget->selectedItems().at(3)->setText(desc.c_str());
                 slot_selectedListWidgetItem();
                 updateTargetInfo();
                 g_MainWindow->renderRobotFront->_renderWindow->Render();
                 inputDialog->close();
             }
             else
             {
                 MapObjActor * currentMapObjActor = g_MainWindow->renderRobotFront->searchTarget(targetName);
                 if(currentMapObjActor==nullptr)// No duplicated
                 {
                     selectedTargetObj->setData(value);

                     tableWidget->selectedItems().at(0)->setText(targetName.c_str());
                     tableWidget->selectedItems().at(1)->setText(targetType.c_str());
                     tableWidget->selectedItems().at(2)->setText(moveType.c_str());
                     tableWidget->selectedItems().at(3)->setText(desc.c_str());
                     slot_selectedListWidgetItem();
                     updateTargetInfo();
                     g_MainWindow->renderRobotFront->_renderWindow->Render();
                     inputDialog->close();
                 }
                 else// Already Target Name exists...
                 {
                    QString str="Note: Target Name ["+QString(targetName.c_str())+"] is already used.\n Please choose different name.";
                    inputDialog->note->setText(str);
                    inputDialog->note->show();
                   // cout<<"found"<<endl;
                 }
             }
           // cout<<"++++++++"<<inputDialog->targetNameLineEdit->text().toStdString()<<" --------";
        }// -------------- end of Lambda Function ---------------
    );
    inputDialog->exec();
    delete inputDialog;
    tableWidget->setFocus();
}



void QmkTargetEditVLayout::clickedButton_UpdateWithCurrentConfigure()
{
    if(!selectedTargetObj) return;
    g_MainWindow->stopAllTimers();
    mkMatrix Ttarget;
    vtkSmartPointer<vtkTransform> transformTarget = vtkSmartPointer<vtkTransform>::New();
    Ttarget = traXYZ(robotKin.param.EEx+robotKin.offDisX, robotKin.param.EEy+robotKin.offDisY, robotKin.param.EEz+robotKin.offDisZ)*rotZrad(robotKin.param.EEth+M_PI_2);
    transformTarget->SetMatrix(Ttarget.m_pData);
    selectedTargetObj->actor->SetUserTransform(transformTarget);
    selectedTargetObj->param.posTargetEE[0] = robotKin.param.EEx;
    selectedTargetObj->param.posTargetEE[1] = robotKin.param.EEy;
    selectedTargetObj->param.posTargetEE[2] = robotKin.param.EEz;
    selectedTargetObj->param.posTargetEE[3] = robotKin.param.EEth;
    g_MainWindow->renderRobotFront->_renderWindow->Render();
    g_MainWindow->renderRobotTop->moveSelectedTarget(selectedTargetObj->param.posTargetEE);
}



void QmkTargetEditVLayout::clickedButton_MoveToTarget()
{
    if(!selectedTargetObj) return;
    g_MainWindow->stopAllTimers();
    MKZoeRobotKin testRobotKin(robotKin);
    bool rec = testRobotKin.invKinEE_W_XYR(selectedTargetObj->param.posTargetEE[0],selectedTargetObj->param.posTargetEE[1], selectedTargetObj->param.posTargetEE[3]);
    if(rec)
    {
         testRobotKin.param.Pz = selectedTargetObj->param.posTargetEE[2];
         robotKin = testRobotKin;
         robotKin.updateCurrentJoint();
         //g_MainWindow->jointCtrlDialogBox->updateSilder();
         g_MainWindow->JointEditVLayout->updateSilder();
         g_MainWindow->renderRobotFront->updateZoeRobotConfig();
         g_MainWindow->renderRobotFront->_renderWindow->Render();
         g_MainWindow->renderRobotTop->updateZoeRobotConfig();
         g_MainWindow->renderRobotTop->_renderWindow->Render();
    }
    else {
        cout<<"No solution: Robot can't reach to the target."<<endl;
        QMessageBox msgError;
        msgError.setText("No solution!\nRobot can't reach to the target!");
        msgError.setIcon(QMessageBox::Critical);
        msgError.setWindowTitle("Error");
        msgError.exec();
    }
}
int QmkTargetEditVLayout::TEST_gen_linear_profile()
{
    qDebug()<<"--------- TEST_gen_linear_profile()---------";
    #define F_CPU 84000000UL
//    #define TICK_PRESCALE 128
    #define tick_freq (F_CPU / 128)
    #define MAX_MOTIONDATA_SIZE 1500
    #ifndef SIGN
      #define SIGN(a) ((a < 0) ? -1 : 1)
    #endif

    typedef struct _MOTIONDATA_
    {
      unsigned long Cn=0; // pluse frequency (one counter time)
      uint16_t steps=0; // how many pulse should be counted
      int16_t dir=0; // stepper motor direction(1: CCW, -1:CW)
    } MOTIONDATA;

    typedef struct _KINEMATICS_DATA_
    {

      MOTIONDATA motionData[MAX_MOTIONDATA_SIZE];
      volatile uint16_t indexMotionData=0;
      volatile bool activated=false;
      volatile bool pulseTick=false;
      volatile bool pulseDown=false;
      volatile uint16_t step_count=0;

      uint16_t dataSize=0;
      unsigned long totalSteps=0;
//      unsigned long getMotionCn()
//      {
//        return motionData[indexMotionData].Cn;
//      }
//      uint16_t getMotionSteps()
//      {
//        return motionData[indexMotionData].steps;
//      }
//      int16_t getMotionDir() {
//        return motionData[indexMotionData].dir;
//      }
//      void nextMotionData()
//      {
//        ++indexMotionData;
//      }
//      bool isMotionDone()
//      {
//        if (indexMotionData >= dataSize) return true;
//        return false;
//      }
//      void motionDone()
//      {
//        indexMotionData=0;
//        activated=false;
//        pulseTick=false;
//        pulseDown=false;
//        step_count=0;
//      }
      void reset()
      {
        indexMotionData=0;
        activated=false;
        pulseTick=false;
        pulseDown=false;
        step_count=0;
        memset(motionData,0,sizeof(MOTIONDATA)*MAX_MOTIONDATA_SIZE);
        dataSize=0;
        totalSteps=0;

      }
    } KIN_DATA;
    KIN_DATA kinData[3];
    kinData[0].reset();
    kinData[1].reset();
    kinData[2].reset();

    unsigned long Cn[3]={0}; // pluse frequency (one counter time)
    uint16_t Steps[3]={0}; // how many pulse should be counted
    int16_t Dir[3]={0};
  //  unsigned long totalSteps[3]={0};
  ///////////////////////////////////
  double aCoEE[2][4], curEEPos[2];
  double EEx[2]={1193, 1193};
  double EEy[2]={-137, -357};
  double EETheta = -1.5708;
  double Vel = 150;
  bool isSameTrajectory=true;
  double maxPos=0.0;
  double transitT = 0.0;
  double initialEEPos[2]={EEx[0],EEy[0]};
  double finalEEPos[2]  ={EEx[1],EEy[1]};

  for(int i=0; i<2; i++){
            double deltaPos = finalEEPos[i] -  initialEEPos[i];

            // if(i==2) deltaPos *=LINK_2;

            if(maxPos<=abs(deltaPos))
            {
                maxPos=abs(deltaPos);
            }
        }
  double Tf=maxPos/Vel;
  if(Tf<=1.0) Tf=1.0;
  for(int i=0; i<2; i++) {
            aCoEE[i][0] = initialEEPos[i];
            aCoEE[i][1] = 0.0;
            aCoEE[i][2] = 3.0*(finalEEPos[i]-initialEEPos[i])/(Tf*Tf);
            aCoEE[i][3] = -2.0*(finalEEPos[i]-initialEEPos[i])/(Tf*Tf*Tf);
            if(abs(aCoEE[i][2])>1e-10) {
              isSameTrajectory=false;
            }
        }
  double deltaT=30/1000.0;// 4 milisec
  int maxN=int(Tf/deltaT+0.5);
  double currT=0.0;
  double currEEPos[3];
  ///////////////////////////////////
  double dx=(EEx[1]-EEx[0]);
  double dy=(EEy[1]-EEy[0]);

//  double sx = (EEx[0]<EEx[1]) ? 1 : -1;
//  double sy = (EEy[0]<EEy[1]) ? 1 : -1;
//  double err = dx+dy;
//  int x = (EEx[0]);
//  int y = (EEy[0]);
  double dist = sqrt(dx*dx + dy*dy);
////////////////////////////////////////////////////////
  // ++ deltaT_Lx is duration when EE move 1mm ++
//  double DIST2STEP[4]={16.0, 2291.83*2, 763.944*4.0,80.0}; //[X, R1, R2, Z]
  double DIST2STEP[4] = {16.0, 2291.831, 3055.775, 16.0};
  double STEP2DIST[4]={1.0/DIST2STEP[0], 1.0/DIST2STEP[1], 1.0/DIST2STEP[2], 1.0/DIST2STEP[3]}; //[X, R1, R2, Z]


  double deltaPos[3]={0}, residue[3]={0}, prevResidue[3]={0};
  double deltaPos_residue[3]={0};
  double prevPos[3]={0}, steps[3]={0};
  double abs_sum_steps[3]={0};
  double totalDistance[3]={0};
  //double dist[nAng+1] ;
  int cnt_zeros[3]={0};
  int index_step[3]={0};

  // double deltaT_per_1deg = 1.0/circleProfile.speed;
  double abs_step[3]={0};
  double abs_step_1[3]={0},abs_step_residue [3]={0};
//  int acc_area = round(dist*0.1); // accelleration area from start (x%)
//  int acc_dec_area = acc_area;
  // double deltaT = 1.0/Vel;
  // int acc_area[3] = {10};
  bool bCheckSum=true;
  // bool bDebug=0;
  double sum_deltaPos[3]={0}, sum_stepPos[3]={0};
  double diffSteps=0;
 ////////////////////////////////////////////////////////
MKZoeRobotKin kinematics(robotKin);
/////////////////////////////////////////////////////////////////////////
// Linear for index 0
  int n=0;
  if(!kinematics.invKinEE_W_XYR(EEx[0], EEy[0], EETheta))
  {
    //if(bDebug) printf("!!!! NO IK SOLUTION !!!!\n");
    return ERROR+1;
  }
   prevPos[0] = kinematics.param.Px;// Calculated by arcMotionIK (initial deltaPosue)
   prevPos[1] = kinematics.param.th1;
   prevPos[2] = kinematics.param.th2;
  //////////////////////////////////////////////////////////////////////
  // -- Start to calcute the number of pulses, delay time counts (Cn) and direction of motors

  while(1)
  {
    currT +=deltaT;

    //cout<<"count="<<n<<", ";
    for(int i=0; i<2; i++){
        curEEPos[i] =aCoEE[i][3]*pow(currT,3) +aCoEE[i][2]*pow(currT,2)  + aCoEE[i][1]*    currT  + aCoEE[i][0];
        //cout<<curEEPos[i]<<", ";
    }
   // cout<<currT<<"\n-------------------------------\n";
    //if (x == round(EEx[1]) && y == round(EEy[1])) break;
    //-------------------------------------------------------//
    //////////////////////////////////////////////////////////
    //  the I.K.
    if(!kinematics.invKinEE_W_XYR(curEEPos[0], curEEPos[1], EETheta))
    {
      // if(bDebug) printf("!!!! NO IK SOLUTION !!!!\n");
      return 200+n;//ERROR+2+;
    }
    // double delX = mkVelProfile.kinParam.x  - EEx[0];
    // double delY = mkVelProfile.kinParam.y  - EEy[0];
    // if(sqrt(delX*delX + delY*delY)>=dist) break;
    ////////////////////////////////////////////////
    // Handing 3 motors...
    for(int CH=0; CH<3; CH++)
    {
      //////////////////////////////////////////////////
    // Handling Acceleration and Deceleration by adding more time
    // if(i<acc_dec_area)                              deltaT = exp(double(acc_area)*1.5/double(acc_dec_area))/Vel;
    // else if(i> (dist-acc_dec_area)) deltaT = exp(double(acc_area)*1.5/double(acc_dec_area))/Vel;
    // else                                            deltaT = 1.0/Vel;
      /////////////////////////////////////////
      // Calculate Delta Position
      // 0: X - Linear Motion (mm)
      if(CH==0) {
        deltaPos[CH] = (kinematics.param.Px - prevPos[CH]); // [mm]
        totalDistance[CH] += abs(deltaPos[CH]);
        prevPos[CH] = kinematics.param.Px;
      }
      /////////////////////////////////////////
      // 1: R1 - Rotation Motion (rad)
      else if(CH==1) {
        deltaPos[CH] = (kinematics.param.th1 - prevPos[CH]); // [mm]
        totalDistance[CH] += abs(deltaPos[CH]);
        prevPos[CH] = kinematics.param.th1;

      }
      /////////////////////////////////////////
      // 2: R2 - Rotation Motion (rad)
      else if(CH==2) {
        deltaPos[CH] = (kinematics.param.th2 - prevPos[CH]); // [mm]
        totalDistance[CH] += abs(deltaPos[CH]);
        prevPos[CH] = kinematics.param.th2;
      }
      // "sum_deltaPos[CH]"" is for checking if there is difference between real step count and calculate count...
      sum_deltaPos[CH] += deltaPos[CH];
    //%---------------------------------------------------------------------//
    //% Only accept more than one step if not, calculate the residue and pass
    //% it to next step...
      if(n==0) {
          deltaPos_residue[CH] =  deltaPos[CH];
      } else {
          deltaPos_residue[CH] =  deltaPos[CH] + prevResidue[CH]; // Add Residue
      }

      abs_step[CH] = abs(DIST2STEP[CH]*deltaPos_residue[CH]);
      if(abs_step[CH]>=1.0) {
        abs_step_1[CH] = round(abs_step[CH]);
        abs_step_residue[CH] = abs_step[CH] - abs_step_1[CH];
        steps[CH] = SIGN(deltaPos[CH])*abs_step_1[CH];
        residue[CH] = SIGN(deltaPos[CH]) * abs_step_residue[CH] * STEP2DIST[CH];
      }
      else {
        steps[CH]=0;
        residue[CH] =deltaPos_residue[CH];
      }
      prevResidue[CH] = residue[CH];
      abs_sum_steps[CH] += abs(steps[CH]);

      // prevResidue = SIGN(deltaPos) * (incPos - abs_sum_steps[CH]*STEP2DIST[CH]);

      ///////////////////////////////////////////////

      // if(bDebug) printf("---- step[%d] --- %f, abs_step=%f, val=%f\n",i,steps[CH],abs_step[CH], deltaPos_residue[CH]);

      //--------------------------------------------------------------------//
      // Based on number of steps, calcuate Cn(count for delay) and direction
      // of motor and number of steps for this duration...

      if ((steps[CH]) == 0) {
        cnt_zeros[CH]++;
        index_step[CH]++;
      }
      else
      {
        if(cnt_zeros[CH]>0){
          kinData[CH].motionData[index_step[CH]].Cn = int(tick_freq * deltaT * cnt_zeros[CH] / abs(steps[CH])+0.5);
          cnt_zeros[CH]=0;
        } else {
            kinData[CH].motionData[index_step[CH]].Cn = int(tick_freq * deltaT / abs(steps[CH])+0.5);

        }
        kinData[CH].motionData[index_step[CH]].steps = uint16_t(abs(steps[CH])+0.5);
        kinData[CH].motionData[index_step[CH]].dir = SIGN(deltaPos[CH]); // stepper motor direction...
        // "sum_stepPos[CH]"" is for checking if there is difference between real step count and calculate count...
        sum_stepPos[CH] += double(kinData[CH].motionData[index_step[CH]].steps)*double(kinData[CH].motionData[index_step[CH]].dir);

        index_step[CH]++;

      }

      //%-----------------------------------------------------------------------------
      //% Lastly check if there is unfinised pulse, if so add one more pulse
      //% with previous direction...
      /*
      if(n>=(maxN-1)) {
        kinData[CH].totalSteps = unsigned long(totalDistance[CH] * DIST2STEP[CH]+0.5);
        if(cnt_zeros[CH]>0)
        {
          kinData[CH].motionData[index_step[CH]].Cn = unsigned long(tick_freq * deltaT * (cnt_zeros[CH])+0.5) ;
          kinData[CH].motionData[index_step[CH]].steps=1;
          kinData[CH].motionData[index_step[CH]].dir= kinData[CH].motionData[index_step[CH]-1].dir; // stepper motor direction...

          abs_sum_steps[CH] += 1;
          steps[CH] = kinData[CH].totalSteps - (abs_sum_steps[CH]);
          if(steps[CH]>0){
            kinData[CH].motionData[index_step[CH]].Cn = unsigned long(tick_freq * deltaT * cnt_zeros[CH] / abs(steps[CH])+0.5) ;
            kinData[CH].motionData[index_step[CH]].steps=uint16_t(abs(steps[CH])+0.5);
            kinData[CH].motionData[index_step[CH]].dir = kinData[CH].motionData[index_step[CH]-1].dir; // stepper motor direction...

          }
          abs_sum_steps[CH] += abs(steps[CH]);
          index_step[CH]++;
        }
        else {
          steps[CH] = kinData[CH].totalSteps - abs_sum_steps[CH];
          if(steps[CH]>0){
            kinData[CH].motionData[index_step[CH]].Cn= unsigned long(tick_freq * deltaT/ abs(steps[CH])+0.5) ;
            kinData[CH].motionData[index_step[CH]].steps=uint16_t(abs(steps[CH])+0.5);
            kinData[CH].motionData[index_step[CH]].dir = kinData[CH].motionData[index_step[CH]-1].dir; // stepper motor direction...


              abs_sum_steps[CH] += abs(steps[CH]);
          }
          index_step[CH]++;
        }// ----------else

         kinData[CH].dataSize = index_step[CH];
        if(kinData[CH].totalSteps!=abs_sum_steps[CH]) bCheckSum=false;
      }//
      */

    }// --------- for(CH)

 cout<<"cnt="<<n<<": ";
  if(currT>=Tf) {
      for(int CH=0; CH<3; CH++) {
          //%-----------------------------------------------------------------------------
          //% Lastly check if there is unfinised pulse, if so add one more pulse
          //% with previous direction...
          //if(n>=(maxN-1)) {
            kinData[CH].totalSteps = int(totalDistance[CH] * DIST2STEP[CH]+0.5);
            if(cnt_zeros[CH]>0)
            {
              kinData[CH].motionData[index_step[CH]].Cn = int(tick_freq * deltaT * (cnt_zeros[CH])+0.5) ;
              kinData[CH].motionData[index_step[CH]].steps=1;
              kinData[CH].motionData[index_step[CH]].dir= kinData[CH].motionData[index_step[CH]-1].dir; // stepper motor direction...

              abs_sum_steps[CH] += 1;
              steps[CH] = kinData[CH].totalSteps - (abs_sum_steps[CH]);
              if(steps[CH]>0){
                kinData[CH].motionData[index_step[CH]].Cn = int(tick_freq * deltaT * cnt_zeros[CH] / abs(steps[CH])+0.5) ;
                kinData[CH].motionData[index_step[CH]].steps=uint16_t(abs(steps[CH])+0.5);
                kinData[CH].motionData[index_step[CH]].dir = kinData[CH].motionData[index_step[CH]-1].dir; // stepper motor direction...

              }
              abs_sum_steps[CH] += abs(steps[CH]);
              index_step[CH]++;
            }
            else {
              steps[CH] = kinData[CH].totalSteps - abs_sum_steps[CH];
              if(steps[CH]>0){
                kinData[CH].motionData[index_step[CH]].Cn= int(tick_freq * deltaT/ abs(steps[CH])+0.5) ;
                kinData[CH].motionData[index_step[CH]].steps=uint16_t(abs(steps[CH])+0.5);
                kinData[CH].motionData[index_step[CH]].dir = kinData[CH].motionData[index_step[CH]-1].dir; // stepper motor direction...


                  abs_sum_steps[CH] += abs(steps[CH]);
              }
              index_step[CH]++;
            }// ----------else

             kinData[CH].dataSize = index_step[CH];
            if(kinData[CH].totalSteps!=abs_sum_steps[CH]) bCheckSum=false;


//            cout<<" CH="<<CH<<
//                 ", index="<< index_step[CH]<<
//               "=> Cn:"<<
//            kinData[CH].motionData[n].Cn <<", steps:"<<
//            kinData[CH].motionData[n].steps<<", dir:"<<
//            kinData[CH].motionData[n].dir<<
//               ", delta="<<
//            deltaPos[CH]<< ", totoalStepDiff="<<kinData[CH].totalSteps<<", "<<abs_sum_steps[CH]<<endl;
      }
      break;
  }
  else {

//      for (int j=0; j<3; j++) {
//          cout<<" CH="<<j<<
//               ", index="<< index_step[j]<<
//             "=> Cn:"<<
//          kinData[j].motionData[n].Cn <<", steps:"<<
//          kinData[j].motionData[n].steps<<", dir:"<<
//          kinData[j].motionData[n].dir<<
//             ", delta="<<
//          deltaPos[j];//<<", Cn:"<<Cn[j]<<", steps:"<< Steps[j]<<", Dir:"<<Dir[j]<<", ";
//      }

  }
//  cout<<endl<<flush;
   n++;
  }
  qDebug()<<"bCheckSum: "<<bCheckSum<<", totalSteps1="<<  kinData[0].totalSteps<<", abs_sum_steps[j]:"<<abs_sum_steps[0]<<", kinData[0].totalSteps="<<kinData[0].totalSteps<<", DataSize2="<< index_step[1]<<", DataSize3="<< index_step[2];
  if(bCheckSum){
    return 1;
  }// Calculation is right
  else {
     return 1;//ERROR+3;
  }// total steps and sum of step is different (Calculation is wrong)...
}

int QmkTargetEditVLayout::TEST_gen_linear2_profile(double EE1x, double EE2x, double EE1y, double EE2y, double EE1t)
{
    qDebug()<<"--------- TEST_gen_linear2_profile()---------";
    #if 1
    #define F_CPU 84000000UL
    //    #define TICK_PRESCALE 128
    #define tick_freq (F_CPU / 128)
    #define MAX_MOTIONDATA_SIZE 1500
    #ifndef SIGN
      #define SIGN(a) ((a < 0) ? -1 : 1)
    #endif

    typedef struct _MOTIONDATA_
    {
      int Cn=0; // pluse frequency (one counter time)
      int steps=0; // how many pulse should be counted
      int16_t dir=0; // stepper motor direction(1: CCW, -1:CW)
    } MOTIONDATA;

    typedef struct _KINEMATICS_DATA_
    {

      MOTIONDATA motionData[MAX_MOTIONDATA_SIZE];
      volatile uint16_t indexMotionData=0;
      volatile bool activated=false;
      volatile bool pulseTick=false;
      volatile bool pulseDown=false;
      volatile uint16_t step_count=0;

      uint16_t dataSize=0;
      unsigned long totalSteps=0;

      void reset()
      {
        indexMotionData=0;
        activated=false;
        pulseTick=false;
        pulseDown=false;
        step_count=0;
        memset(motionData,0,sizeof(MOTIONDATA)*MAX_MOTIONDATA_SIZE);
        dataSize=0;
        totalSteps=0;

      }
    } KIN_DATA;
    KIN_DATA kinData[3];
    kinData[0].reset();
    kinData[1].reset();
    kinData[2].reset();

    ///////////////////////////////////
    double  curEEPos[2];
    double EEx[2]={EE1x, EE2x};//{1193, 1193};
    double EEy[2]={EE1y, EE2y};//{-137, -357};
    double EETheta = EE1t;//-1.5708;

    //double EEx[2]={1193.031, 1193};
    //double EEy[2]={-148.031, -401};
    //double EETheta = -1.571;

    double initialEEPos[2]={EEx[0],EEy[0]};

    ///////////////////////////////////
    double dx=(EEx[1]-EEx[0]);
    double dy=(EEy[1]-EEy[0]);


    double Vel = 70;
    double deltaT_orignal = 1.0/Vel;
    double deltaT = deltaT_orignal;
    double deltaDist=1.0;//[mm]
    double totalDist = sqrt(dx*dx + dy*dy);

    if(totalDist<=10)
       deltaDist=0.2; // 0.2mm

    double dirUnitVecor[2]={dx/totalDist, dy/totalDist};
    double startRegidual = totalDist-floor(totalDist);
    int maxN = int(totalDist/deltaDist+0.5);
    ////////////////////////////////////////////////////////
    // ++ deltaT_Lx is duration when EE move 1mm ++
//    double DIST2STEP[4]={16.0, 2291.83*2, 763.944*4.0,80.0}; //[X, R1, R2, Z] // old
    double DIST2STEP[4] = {16.0, 2291.831, 3055.775, 16.0};
    double STEP2DIST[4]={1.0/DIST2STEP[0], 1.0/DIST2STEP[1], 1.0/DIST2STEP[2], 1.0/DIST2STEP[3]}; //[X, R1, R2, Z]


    double deltaPos[3]={0};
    double deltaPosPrev[3]={0};
    double deltaPos_residue[3]={0};
    double prevPos[3]={0}, steps[3]={0};
    double abs_sum_steps[3]={0};
    double totalDistance[3]={0};

    int index_step[3]={0};

    // double deltaT_per_1deg = 1.0/circleProfile.speed;
    double abs_step[3]={0};

    int acc_area = int(totalDist*0.1+0.5); // accelleration area from start (x%)
    int acc_dec_area = acc_area;

    bool bCheckSum=true;
    // bool bDebug=0;
    double sum_deltaPos[3]={0}, sum_stepPos[3]={0};
    double sumdL[3]={0}, sumDist[3]={0};
    ////////////////////////////////////////////////////////
    MKZoeRobotKin kinematics(robotKin);
    /////////////////////////////////////////////////////////////////////////

    myqDebug()<<"EE1x="<<EEx[0]<<", EE1y="<<EEy[0]<<", EE2x="<<EEx[1]<<", EE2y="<<EEy[1]<<", EE1t="<<EETheta<<
                ", acc_dec_area="<<acc_dec_area<< ", totalDist"<<totalDist<<", maxN: "<<maxN<<", startRegidual="<<startRegidual;

    curEEPos[0]= initialEEPos[0]+dirUnitVecor[0]*(startRegidual);
    curEEPos[1]= initialEEPos[1]+dirUnitVecor[1]*(startRegidual);
    if(!kinematics.invKinEE_W_XYR(curEEPos[0], curEEPos[1], EETheta))
    {
        //if(bDebug) printf("!!!! NO IK SOLUTION !!!!\n");
        return ERROR+1;
    }
    prevPos[0] = kinematics.param.Px;// Calculated by arcMotionIK (initial deltaPosue)
    prevPos[1] = kinematics.param.th1;
    prevPos[2] = kinematics.param.th2;

    for(int n=0; n<maxN; n++)
    {

        curEEPos[0]= initialEEPos[0]+dirUnitVecor[0]*(startRegidual+(n+1)*deltaDist);
        curEEPos[1]= initialEEPos[1]+dirUnitVecor[1]*(startRegidual+(n+1)*deltaDist);
        //-------------------------------------------------------//
        //////////////////////////////////////////////////////////
        //  the I.K.
        if(!kinematics.invKinEE_W_XYR(curEEPos[0], curEEPos[1], EETheta))
        {
          // if(bDebug) printf("!!!! NO IK SOLUTION !!!!\n");
          return 200+n;//ERROR+2+;
        }

        ////////////////////////////////////////////////
        // Handing 3 motors...
        for(int CH=0; CH<3; CH++)
        {
          //////////////////////////////////////////////////
        // Handling Acceleration and Deceleration by adding more time
    //     if(n<acc_dec_area)                   deltaT = deltaT_orignal*exp(double(acc_area)*1.5/double(acc_dec_area));
    //     else if(n> (totalDist-acc_dec_area)) deltaT = deltaT_orignal*exp(double(acc_area)*1.5/double(acc_dec_area));
    //     else                                 deltaT = deltaT_orignal;


          /////////////////////////////////////////
          // Calculate Delta Position
          // 0: X - Linear Motion (mm)
          if(CH==0) {
            deltaPos[CH] = (kinematics.param.Px - prevPos[CH]); // [mm]
            totalDistance[CH] += abs(deltaPos[CH]);
            prevPos[CH] = kinematics.param.Px;
          }
          /////////////////////////////////////////
          // 1: R1 - Rotation Motion (rad)
          else if(CH==1) {
            deltaPos[CH] = (kinematics.param.th1 - prevPos[CH]); // [mm]
            totalDistance[CH] += abs(deltaPos[CH]);
            prevPos[CH] = kinematics.param.th1;

          }
          /////////////////////////////////////////
          // 2: R2 - Rotation Motion (rad)
          else if(CH==2) {
            deltaPos[CH] = (kinematics.param.th2 - prevPos[CH]); // [mm]
            totalDistance[CH] += abs(deltaPos[CH]);
            prevPos[CH] = kinematics.param.th2;
          }
          // "sum_deltaPos[CH]"" is for checking if there is difference between real step count and calculate count...
          sum_deltaPos[CH] += deltaPos[CH];
        //%---------------------------------------------------------------------//
        //% Only accept more than one step if not, calculate the residue and pass
        //% it to next step...
          if(n==0) {
              deltaPos_residue[CH] =  deltaPos[CH];
          } else {
              sumdL[CH] = sumdL[CH] + abs(deltaPosPrev[CH]);
              deltaPos_residue[CH] =  deltaPos[CH]+SIGN(deltaPos[CH])*(sumdL[CH] - sumDist[CH]);
          }
           deltaPosPrev[CH]= deltaPos[CH];
          abs_step[CH] = abs(DIST2STEP[CH]*deltaPos_residue[CH]);
          if(abs_step[CH]>=1.0) {
              steps[CH] = SIGN(deltaPos[CH])*int(abs_step[CH]+0.5);
          }
          else {
            steps[CH]=0;
          }
          abs_sum_steps[CH] += abs(steps[CH]);
          //--------------------------------------------------------------------//
          // Based on number of steps, calcuate Cn(count for delay) and direction
          // of motor and number of steps for this duration...

          if ((steps[CH]) == 0) {
            kinData[CH].motionData[index_step[CH]].Cn = kinData[CH].motionData[index_step[CH]-1].Cn;
            kinData[CH].motionData[index_step[CH]].dir = kinData[CH].motionData[index_step[CH]-1].dir;

          }
          else
          {
            kinData[CH].motionData[index_step[CH]].Cn = int(tick_freq * deltaT / abs(steps[CH])+0.5);
            kinData[CH].motionData[index_step[CH]].steps = int(abs(steps[CH])+0.5);
            kinData[CH].motionData[index_step[CH]].dir = SIGN(deltaPos[CH]); // stepper motor direction...
            // "sum_stepPos[CH]"" is for checking if there is difference between real step count and calculate count...
            sum_stepPos[CH] += double(kinData[CH].motionData[index_step[CH]].steps)*double(kinData[CH].motionData[index_step[CH]].dir);

          }
            sumDist[CH] = sumDist[CH] + kinData[CH].motionData[index_step[CH]].steps*STEP2DIST[CH];
            index_step[CH]++;
            if(index_step[CH]>=MAX_MOTIONDATA_SIZE){
                qDebug()<<"----- MAX_MOTIONDATA_SIZE over: "<<index_step[CH];
                return -1;
            }
        }// --------- for(CH)

          for (int j=0; j<3; j++) {
              cout<<" CH="<<j<< ", index="<< index_step[j]<< "=> Cn:"<<
              kinData[j].motionData[n].Cn <<", steps:"<<
              kinData[j].motionData[n].steps<<", dir:"<<
              kinData[j].motionData[n].dir<<", delta="<< deltaPos[j];
          }

        cout<<endl<<flush;
        }

    for(int CH=0; CH<3; CH++) {
          kinData[CH].totalSteps = int(totalDistance[CH] * DIST2STEP[CH]+0.5);
          kinData[CH].dataSize = index_step[CH];
          if(kinData[CH].totalSteps!=abs_sum_steps[CH]) bCheckSum=false;
          cout<<"CH="<<CH<<": totalSteps="<<kinData[CH].totalSteps<< ", totoalStepDiff="<<kinData[CH].totalSteps<<", "<<abs_sum_steps[CH]<<endl;
    }

    qDebug()<<"bCheckSum: "<<bCheckSum;
    if(bCheckSum){
    return 1;
    }// Calculation is right
    else {
     return 1;//ERROR+3;
        }// total steps and sum of step is different (Calculation is wrong)...
    #endif
}
void QmkTargetEditVLayout::clickedButton_MoveToTargetAnimation()
{
    //TEST_gen_linear_profile();// testing

    if(!selectedTargetObj) return;
    g_MainWindow->stopAllTimers();
    double simLFactor=1.0, simAFactor=60.0;
    double linVel = 150*simLFactor; // [mm/sec]

    /////////////////////////////////////////////////////////
    // Check if tharget point is reachable or not by testing inverse kinematics...
    robotKin.getJointPos(gSharedData.curJointPos);
    robotKin.getWEEPos(gSharedData.curEEPos);
    MKZoeRobotKin testSelectedTargetRobotKin(robotKin);
   // qDebug()<<"LastPickedMoveType: "<<LastPickedMoveType.c_str()<<", selectedTargetObj->param.getMoveType():"<<selectedTargetObj->param.getMoveType().c_str();
   std::string prevMoveType="";
    MapObjActor *prevObj = g_MainWindow->renderRobotFront->searchTarget(prevSelectedName.toStdString());
   if(prevObj)
   {
       prevMoveType = prevObj->param.getMoveType();
   }

   qDebug()<<"prevName:"<<prevSelectedName<<", prevMoveType: "<<prevMoveType.c_str()<<", selectedTargetObj->param.getMoveType():"<<selectedTargetObj->param.getMoveType().c_str();
//    if(selectedTargetObj->param.getMoveType()=="JOINT" && prevMoveType!="CARTESIAN") {
   if(isJointMove)
   {

        bool rec = testSelectedTargetRobotKin.invKinEE_W_XYR(
                    selectedTargetObj->param.posTargetEE[0],
                    selectedTargetObj->param.posTargetEE[1],
                    selectedTargetObj->param.posTargetEE[3]);
        if(!rec)
        {
                cout<<"No solution: Robot can't reach to the target."<<endl;
                QMessageBox msgError;
                msgError.setText("No solution!\nRobot can't reach to the target!");
                msgError.setIcon(QMessageBox::Critical);
                msgError.setWindowTitle("Error");
                msgError.exec();
                return;
        }

        double  targetJntPos[NUMJNT];
        testSelectedTargetRobotKin.param.Pz = selectedTargetObj->param.posTargetEE[2];
        testSelectedTargetRobotKin.getJointPos(targetJntPos);
        //////////////////////////////////////////////////////////////////////////
        // Calculate the final time...
        // 1. Find the max angle from current joints to tartget joints(jointconfigure)
        double maxPos=0.0;
        int miliSecond = 20;
        gSharedData.trTime.transitT = 0.0;
        gSharedData.trTime.deltaT = double(miliSecond)/1000.0;
        for(int i=0; i<NUMJNT; i++){
            double deltaPos = targetJntPos[i] -  gSharedData.curJointPos[i];

            if(i==2) deltaPos *=robotKin.Link1;
            else if(i==3) deltaPos *=robotKin.Link2;

            if(maxPos<=abs(deltaPos))
            {
                maxPos=abs(deltaPos);
            }
        }
        double angTf=maxPos/linVel;
        if(angTf<=1.0) angTf=1.0;
        gSharedData.trTime.finalT = angTf;
        qDebug()<<"Joint: executeMotion: time(maxAng, angTf)"<<maxPos<<angTf;
        int rev = robotKin.setCubicalPolynominalJointInter(gSharedData.trTime.finalT, gSharedData.curJointPos, targetJntPos);
        if(rev<1) return;
        g_MainWindow->jointInterpolationMoveTimer->start(miliSecond);
    }
    else // if(selectedTargetObj->param.getMoveType()=="CARTESIAN")
    {
//        TEST_gen_linear2_profile(gSharedData.curEEPos[EEX], selectedTargetObj->param.posTargetEE[EEX],
//                                 gSharedData.curEEPos[EEY], selectedTargetObj->param.posTargetEE[EEY],
//                                 gSharedData.curEEPos[EET]
//                                 );// testing
        int rev=0;
       // ... TESTING  OF MICROCONTROLLER ...
       if(0){
            MKVelProfile velProfile;
            LINEARProfile linearProfile;
            linearProfile.EEx[0]=gSharedData.curEEPos[EEX];
            linearProfile.EEy[0]=gSharedData.curEEPos[EEY];

            linearProfile.EEx[1]=selectedTargetObj->param.posTargetEE[EEX];
            linearProfile.EEy[1]=selectedTargetObj->param.posTargetEE[EEY];
            linearProfile.EETheta = gSharedData.curEEPos[EET];
            linearProfile.posZ=gSharedData.curEEPos[EEZ];
            linearProfile.heightZ=selectedTargetObj->param.posTargetEE[EEZ]-gSharedData.curEEPos[EEZ];
            linearProfile.Vel=70;
            rev = velProfile.gen_linear_profile(linearProfile);
            if(rev!=1){
                cout<<"Erro: "<<rev<<endl;
                QMessageBox msgError;
                msgError.setText("Error!");
                msgError.setIcon(QMessageBox::Critical);
                msgError.setWindowTitle("Error");
                msgError.exec();
                return;
            }
       }
        // .......................................
        bool rec = testSelectedTargetRobotKin.invKinEE_W_XYRZ(
                    selectedTargetObj->param.posTargetEE[EEX],//X
                    selectedTargetObj->param.posTargetEE[EEY],//Y
                    selectedTargetObj->param.posTargetEE[EET],//THETA
                    selectedTargetObj->param.posTargetEE[EEZ]);//Z
        if(!rec)
        {
                cout<<"No solution: Robot can't reach to the target."<<endl;
                QMessageBox msgError;
                msgError.setText("No solution!\nRobot can't reach to the target!");
                msgError.setIcon(QMessageBox::Critical);
                msgError.setWindowTitle("Error");
                msgError.exec();
                return;
        }

        double diffTargetEE[4];
        for(int i=0; i<4; i++) diffTargetEE[i] = selectedTargetObj->param.posTargetEE[i]-gSharedData.curEEPos[i];


        /*
        // Transform World EE coordinate to Local frame
        mkMatrix L_deltaEE = rotZrad(-gSharedData.curEEPos[EET])*traXYZ(diffTargetEE[EEX], diffTargetEE[EEY],0);
//        cout<<"current in World: "<<gSharedData.curEEPos[0]<<", "<<gSharedData.curEEPos[1]<<", "<<gSharedData.curEEPos[2]<<", "<<gSharedData.curEEPos[3]<<endl;
//        cout<<"next Target in World: "<<selectedTargetObj->param.posTargetEE[0]<<", "<<selectedTargetObj->param.posTargetEE[1]<<", "<<selectedTargetObj->param.posTargetEE[2]<<", "<<selectedTargetObj->param.posTargetEE[3]<<endl;
//        cout<<"delta Distance in World: "<<diffTargetEE[0]<<", "<<diffTargetEE[1]<<", "<<diffTargetEE[2]<<", "<<diffTargetEE[3]<<endl;
//        cout<<"delta Distance in Local: "<<L_deltaEE.m_ppData[0][3]<<", "<<L_deltaEE.m_ppData[1][3]<<endl;

        double deltaLTarget[4]={L_deltaEE.m_ppData[0][3], L_deltaEE.m_ppData[1][3], diffTargetEE[EEZ], diffTargetEE[EET]};
        double  targetEEPos[NUMJNT];// [EEX, EEY, EEZ, EET]
        testSelectedTargetRobotKin.getLEEPos(targetEEPos);
        //////////////////////////////////////////////////////////////////////////
        // Calculate the final time...
        // 1. Find the max angle from current EE to tartget EE
        double maxPos=0.0;
        int miliSecond = 20;
        gSharedData.trTime.transitT = 0.0;
        gSharedData.trTime.deltaT = double(miliSecond)/1000.0;
        for(int i=0; i<NUMJNT; i++){
            double deltaPos = deltaLTarget[i];//targetEEPos[i] -  gSharedData.curEEPos[i];

            if(i==EET) deltaPos *=robotKin.Link2;// Rotation

            if(maxPos<=abs(deltaPos))
            {
                maxPos=abs(deltaPos);
            }
        }
        // linVel=20;
        double angTf=maxPos/linVel;
        if(angTf<=1.0) angTf=1.0;
        gSharedData.trTime.finalT = angTf;
        qDebug()<<"Linear: executeMotion: time(distance, maxTf, deltaTime)"<<maxPos<<angTf<<gSharedData.trTime.deltaT ;
        /////////////////////////////////////////////
        // In the Local Frame on EE
        convertW2LEE(gSharedData.curEEPos);
        int rev = robotKin.setCubicalPolynominalEEInter(gSharedData.trTime.finalT, gSharedData.curEEPos, targetEEPos);
        if(rev<1) return;
         g_MainWindow->linearMoveTimer->start(miliSecond);
         */
        /////////////////////////////////////////////
         double deltaLTarget[4]={ diffTargetEE[EEX],  diffTargetEE[EEY], diffTargetEE[EEZ], diffTargetEE[EET]};// test
         double  targetEEPos[NUMJNT];// [EEX, EEY, EEZ, EET]
         testSelectedTargetRobotKin.getWEEPos(targetEEPos);
         //////////////////////////////////////////////////////////////////////////
         // Calculate the final time...
         // 1. Find the max angle from current EE to tartget EE
         double maxPos=0.0;
         int miliSecond = 70;//20;
         gSharedData.trTime.transitT = 0.0;
         gSharedData.trTime.deltaT = double(miliSecond)/1000.0;
         for(int i=0; i<NUMJNT; i++){
             double deltaPos = deltaLTarget[i];//targetEEPos[i] -  gSharedData.curEEPos[i];
//             double deltaPos = targetEEPos[i] -  gSharedData.curEEPos[i];

             if(i==EET) deltaPos *=robotKin.Link2;// Rotation

             if(maxPos<=abs(deltaPos))
             {
                 maxPos=abs(deltaPos);
             }
         }
         // linVel=20;
         maxPos = sqrt(diffTargetEE[0]*diffTargetEE[0]+
                       diffTargetEE[1]*diffTargetEE[1]+
                       diffTargetEE[2]*diffTargetEE[2]+
                       diffTargetEE[3]*diffTargetEE[3]*180.0*180.0/M_PI/M_PI*10);
         double angTf=maxPos/linVel;
         if(angTf<=1.0) angTf=1.0;
         gSharedData.trTime.finalT = angTf;
         qDebug()<<"Linear: executeMotion: time(distance, maxTf, deltaTime)"<<maxPos<<angTf<<gSharedData.trTime.deltaT ;
         /////////////////////////////////////////////
         // In the Local Frame on EE
         //convertW2LEE(gSharedData.curEEPos);
         rev = robotKin.setCubicalPolynominalEEInter(gSharedData.trTime.finalT, gSharedData.curEEPos, targetEEPos);
         if(rev<1) return;
          g_MainWindow->linearMoveTimer->start(miliSecond);
    }

}

void QmkTargetEditVLayout::clickedButton_MoveToTargetRobot()
{
    if(!selectedTargetObj) return;
    if(isJointMove) {


    MKZoeRobotKin testRobotKin(robotKin);
    bool rec = testRobotKin.invKinEE_W_XYRZ(selectedTargetObj->param.posTargetEE[0],//EEx
                                           selectedTargetObj->param.posTargetEE[1], //EEy
                                           selectedTargetObj->param.posTargetEE[3], //EETh
                                           selectedTargetObj->param.posTargetEE[2]);//EEZ
    int ret = testRobotKin.checkJointLimit();
    if(ret>0) {
        cout<<"Exceed joint limit: "<<ret<<endl;
        return;
    }
    if(rec)
        g_MainWindow->action_moveMultiJointRobot(testRobotKin.param.Px,
                                                 testRobotKin.param.th1,
                                                 testRobotKin.param.th2,
                                                 testRobotKin.param.Pz);
    else cout<<"No solution: Robot can't reach to the target."<<endl;

    }
    else {
     g_MainWindow->action_moveLinear();
    }
 //   g_MainWindow->stopAllTimers();
//    MKZoeRobotKin testRobotKin(robotKin);
//    bool rec = testRobotKin.invKinEE_W_XYR(selectedTargetObj->param.posTargetEE[0],selectedTargetObj->param.posTargetEE[1], selectedTargetObj->param.posTargetEE[3]);
//    if(rec)
//    {
//         testRobotKin.param.Pz = selectedTargetObj->param.posTargetEE[2];
//         robotKin = testRobotKin;
//         g_MainWindow->action_moveMultiJointRobot();
//    }
//    else {
//        cout<<"No solution: Robot can't reach to the target."<<endl;
//        QMessageBox msgError;
//        msgError.setText("No solution!\nRobot can't reach to the target!");
//        msgError.setIcon(QMessageBox::Critical);
//        msgError.setWindowTitle("Error");
//        msgError.exec();
//    }

}

void QmkTargetEditVLayout::clickedButton_MoveToAllTargetRobot()
{
    g_MainWindow->action_moveAllTargetRobot();
}

void QmkTargetEditVLayout::clickedButton_SaveToFile()
{
    mkFileManager myFile(this);
    myFile.saveTargetListXML(TARGET_SAVE_PATH);
}

