#include "qmktabwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QLineEdit>
#include <../sharedfiles/mkRobotKin.h>


extern  MKZoeRobotKin robotKin;
QmkTabWidget::QmkTabWidget(QWidget *parent) : QTabWidget(parent)
{


   // createPORTargetLists();
    createMotionLists();
    cteateOrderLists();

   // this->addTab(contentWidget[0], ("POR Target Listes"));
    this->addTab(contentWidget[0], ("Motion Lists"));
    this->addTab(contentWidget[1], ("Order Lists"));
}

QmkTabWidget::~QmkTabWidget()
{
    for(int i=0; i<NUM_TAB; i++) if(contentWidget[i]) delete contentWidget[i];
   // if(tabWidget) delete tabWidget;
}

//void QmkTabWidget::createPORTargetLists()
//{

//    const int numButton = 5;
//    contentWidget[0] = new QWidget(this);
//    QVBoxLayout *mainVLayout = new QVBoxLayout;
//    QHBoxLayout *buttonHLayout = new QHBoxLayout;
//    QHBoxLayout *contentHLayout = new QHBoxLayout;

//    QVBoxLayout *listShowVLayout = new QVBoxLayout;
//    QVBoxLayout *descShowVLayout = new QVBoxLayout;

//    QPushButton *buttons[numButton];
//    QListWidget *listWidget = new QListWidget;
//    for(int i=0; i<10; i++) listWidget->addItem("Target "+QString::number(i));
//    const char *bName[numButton]={"Add","Duplicate","Delete","Update","MoveToTarget"};
//    for(int i=0; i<numButton; i++) {
//        buttons[i] = new QPushButton(bName[i]);
//        buttonHLayout->addWidget(buttons[i]);
//    }
//    contentHLayout->addLayout(listShowVLayout);
//    contentHLayout->addLayout(descShowVLayout);

//    listShowVLayout->addWidget(new QLabel("POR Target Lists"));
//    listShowVLayout->addWidget(listWidget);


//    mainVLayout->addLayout(buttonHLayout);
//    mainVLayout->addLayout(contentHLayout);
//    contentWidget[0]->setLayout(mainVLayout);
//    ////////////////////////////////////////////
//    selectedLabel = new QLabel("Selected Target: ");
//    descShowVLayout->addSpacing(50);
//    descShowVLayout->addWidget(selectedLabel );
//    descShowVLayout->addSpacing(50);
//    char *rangeName[4]={"X[mm]    ","Y[mm]","Z[mm]    ","RotZ[deg]"};
//    for(int i=0; i<4; i++)
//    {
//        descShowVLayout->addLayout(fillEditRange(rangeName[i], i));
//    }
//    descShowVLayout->addStretch();//addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding));
//}

void QmkTabWidget::createMotionLists()
{
    int i=0;
    contentWidget[i] = new QWidget(this);
}

void QmkTabWidget::cteateOrderLists()
{
    int i=1;
    contentWidget[i] = new QWidget(this);
}

//QHBoxLayout *QmkTabWidget::fillEditRange(char *name, int jntID)
//{
//    QHBoxLayout * hLayout = new QHBoxLayout;
//    hLayout->addWidget(new QLabel(name));
//    hLayout->addWidget(new QLabel(QString::number(robotKin.minJntLimit[jntID])));
//    POREdit[jntID] = new QLineEdit;
//    hLayout->addWidget(POREdit[jntID]);
//    hLayout->addWidget(new QLabel(QString::number(robotKin.maxJntLimit[jntID])));
//    return hLayout;
//}
