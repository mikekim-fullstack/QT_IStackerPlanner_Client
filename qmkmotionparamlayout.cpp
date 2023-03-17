#include "qmkmotionparamlayout.h"
#include "mkGlobalData.h"
#include "../sharedfiles/mkRobotKin.h"
#include "mainwindow.h"
#include "qmkpushbutton.h"
#include <iostream>


#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleValidator>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QScrollBar>
#include <QGroupBox>

extern MKZoeRobotKin robotKin;
extern QByteArray qss_data;
extern MainWindow *g_MainWindow;

QmkMotionParamLayout::QmkMotionParamLayout()
{
    labelMain = new QLabel("No Selection");
    labelMain->setAlignment(Qt::AlignCenter);
    tableWidget = new QTableWidget(0,3);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Parameter Name")<<tr("Input")<<tr("Note"));
    tableWidget->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    tableWidget->setColumnWidth(0, 150);
    tableWidget->setColumnWidth(1, 180);
    tableWidget->setColumnWidth(2, -1);
    tableWidget->setMinimumWidth(580);

    addWidget(labelMain);
    //addSpacing(10);
    addWidget(tableWidget);
}

QmkMotionParamLayout::~QmkMotionParamLayout()
{
    clearLayout();
}

void QmkMotionParamLayout::clearLayout()
{
    for(int i=0; i<10; i++) {
        if(lineEditInput[i]) delete lineEditInput[i];
        lineEditInput[i]=nullptr;
    }
    if(labelMain) labelMain->setText("No Selection");
    if(tableWidget){
        tableWidget->setRowCount(0);
    }
//    removeWidget(labelMain);
//    removeWidget(tableWidget);
//    addWidget(labelMain);
//    addWidget(tableWidget);
//    addStretch();

}

void QmkMotionParamLayout::createFoldArm()
{
    clearLayout();
    labelMain->setText("Fold Arm Motion");
    const int N=4;
    const char* paramName[N]={"Name","Desc","Arm#1 Rot","Arm#2 Rot"};
    const char* paramNote[N]={"Should be filled out.", "", "[deg]", "[deg]"};
    int currentRow = tableWidget->rowCount();
    for(int i=0; i<N; i++){
        tableWidget->setRowCount(currentRow+1);

        QTableWidgetItem *itemCol1 = new  QTableWidgetItem(paramName[i]);
        itemCol1->setFlags(itemCol1->flags() & ~Qt::ItemIsEditable);
        lineEditInput[i] = new QLineEdit;
        QTableWidgetItem *itemCol3 = new QTableWidgetItem(paramNote[i]);
        itemCol3->setFlags(itemCol3->flags() & ~Qt::ItemIsEditable);

        tableWidget->setItem(currentRow, 0, itemCol1);
        tableWidget->setCellWidget(currentRow, 1, lineEditInput[i]);
        tableWidget->setItem(currentRow, 2, itemCol3);
        currentRow++;
    }
    for(int i=0; i<2; i++) {
        lineEditInput[2+i]->setValidator(
                    new QDoubleValidator(robotKin.minJntLimit[2+i],robotKin.maxJntLimit[2+i],2,this));
        lineEditInput[2+i]->setText(QString::number(0,'f',2));
    }
    //edit->setText(QString::number(0,'f',2));
    verticalResizeTableViewToContents();
}

void QmkMotionParamLayout::createPlaceTarget()
{
    clearLayout();
    labelMain->setText("Place Target Motion");
    const int N=7;
    const char* paramName[N]={"Name","Desc","Target","Above Height", "Below Height", "Withrawn Distance", "Speed"};
    const char* paramNote[N]={"Should be filled out.", "", "Select Target Name",
                              "Above from Target[mm]","Below from Target[mm]", "Extracted from Target[mm]", "[mm/sec]" };
    int currentRow = tableWidget->rowCount();
    for(int i=0; i<N; i++){
        tableWidget->setRowCount(currentRow+1);

        QTableWidgetItem *itemCol1 = new  QTableWidgetItem(paramName[i]);
        itemCol1->setFlags(itemCol1->flags() & ~Qt::ItemIsEditable);
        lineEditInput[i] = new QLineEdit;
        QTableWidgetItem *itemCol3 = new QTableWidgetItem(paramNote[i]);
        itemCol3->setFlags(itemCol3->flags() & ~Qt::ItemIsEditable);

        tableWidget->setItem(currentRow, 0, itemCol1);
        tableWidget->setCellWidget(currentRow, 1, lineEditInput[i]);
        tableWidget->setItem(currentRow, 2, itemCol3);
        currentRow++;
    }
    //tableWidget->setMaximumHeight(100);

    for(int i=3; i<N; i++) {
        lineEditInput[i]->setValidator( new QDoubleValidator(-1000,1000,2,this));
        lineEditInput[i]->setText(QString::number(0,'f',2));
    }
    //tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalResizeTableViewToContents();
}

void QmkMotionParamLayout::selectFoldArm(FoldArmParam *param)
{
    if(!param) return;
    createFoldArm();
    lineEditInput[0]->setText(param->name);
    lineEditInput[1]->setText(param->desc);
    lineEditInput[2]->setText(QString::number(param->rot[0],'f',2));
    lineEditInput[3]->setText(QString::number(param->rot[1],'f',2));
}

void QmkMotionParamLayout::selectPlaceTarget(PlaceTargetParam *param)
{
    if(!param) return;
    createPlaceTarget();
    lineEditInput[0]->setText(param->name);
    lineEditInput[1]->setText(param->desc);
    lineEditInput[2]->setText(param->targetName);
    for(int i=0; i<4; i++)  lineEditInput[3+i]->setText(QString::number(param->metric[i],'f',2));

}
void QmkMotionParamLayout::verticalResizeTableViewToContents()
{
    int rowTotalHeight=0;

    // Rows height
    int count=tableWidget->verticalHeader()->count();
    for (int i = 0; i < count; ++i) {
        // 2018-03 edit: only account for row if it is visible
        if (!tableWidget->verticalHeader()->isSectionHidden(i)) {
            rowTotalHeight+=tableWidget->verticalHeader()->sectionSize(i);
        }
    }

    // Check for scrollbar visibility
    if (!tableWidget->horizontalScrollBar()->isHidden())
    {
         rowTotalHeight+=tableWidget->horizontalScrollBar()->height();
    }

    // Check for header visibility
    if (!tableWidget->horizontalHeader()->isHidden())
    {
         rowTotalHeight+=tableWidget->horizontalHeader()->height();
    }
    tableWidget->setMinimumHeight(rowTotalHeight/2);
}

