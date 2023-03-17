#include "jntctrldialog.h"
#include <iostream>
#include "qmkjointsliderlayout.h"
#include <QPushButton>
#include "mkRobotKin.h"
#include "mainwindow.h"
extern MKZoeRobotKin robotKin;
extern QByteArray qss_data;
extern MainWindow *g_MainWindow;
using namespace std;
JntCtrlDialog::JntCtrlDialog(QWidget* parent): QDialog(parent)
{

    cout<<"construction: JntCtrlDialog"<<endl;
    label = new QLabel("Hello!!");
    setWindowTitle(tr("Joint Control Dialog"));
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowOpacity(0.8);
    setWindowFlags(Qt::Dialog|Qt::Popup|Qt::WindowStaysOnTopHint| Qt::CustomizeWindowHint | Qt::WindowTitleHint );
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    grid_layout = new QGridLayout;


     grid_layout->setVerticalSpacing(10);

    grid_layout->addWidget(createJointGroup(), 0, 0);

    setLayout(grid_layout);
    updateEEinfo();
}
void JntCtrlDialog::changeAlwaysOnTop(bool state)
{
 cout<<"changeAlwaysOnTop:"<<state<<endl;

    if (state == false)// hidden
            setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else{// shown
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
        show();
    }
}

QGroupBox *JntCtrlDialog::createJointGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Joint Control"));
    groupBox->setObjectName(QStringLiteral("joint_left_groupbox"));
    QVBoxLayout *vbox = new QVBoxLayout;
//    auto verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Maximum, QSizePolicy::Expanding);
//    vbox->addItem(verticalSpacer);
    const char *name[MAX_JOINT_NUM]={"Joint   X[mm]", "Joint   Z[mm]","Joint R1[deg]","Joint R2[deg]"};
    for(int i=0; i<MAX_JOINT_NUM; i++)
    {
        jointSlider[i] = new QmkJointSliderLayout(this, name[i], i);
        jointSlider[i]->setQss( qss_data );
        jointSlider[i]->slider->setRange(robotKin.minJntLimit[i], robotKin.maxJntLimit[i]);
        vbox->addLayout(jointSlider[i]);
    }
    /////////////////////////////////////////////////////////////
    /// \brief EndEffectorInfoLayout
    ///
    const char *nameEE[3]={"EEx:", "EEy:", "EEang:"};
    QHBoxLayout *EndEffectorInfoLayout = new QHBoxLayout;
    QLabel *labelEE[3];
    for(int i=0; i<3; i++) {
        labelEE[i] = new QLabel(nameEE[i]);
        labelEE[i]->setMaximumWidth(40);
        edit_EEInfo[i] = new QLineEdit;
        edit_EEInfo[i]->setMinimumWidth(80);
        edit_EEInfo[i]->setMaximumWidth(80);
        edit_EEInfo[i]->setDisabled(true);
        edit_EEInfo[i]->setAlignment(Qt::AlignRight);
        EndEffectorInfoLayout->addWidget(labelEE[i]);
        EndEffectorInfoLayout->addWidget(edit_EEInfo[i]);
    }
    vbox->addLayout(EndEffectorInfoLayout);
    ////////////////////////////////////////////////////////////
    /// \brief buttonsLayout
    ///
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    QPushButton *resetZeroButton = new QPushButton(tr("Reset to Zero "));
    QPushButton *goConfigButton = new QPushButton(tr("Go Configure"));
    resetZeroButton->setObjectName(QStringLiteral("joint_button"));
    goConfigButton->setObjectName(QStringLiteral("joint_button"));

    resetZeroButton->setDefault(false);
    resetZeroButton->setAutoDefault(false);
    goConfigButton->setDefault(false);
    goConfigButton->setAutoDefault(false);

    buttonsLayout->addWidget(resetZeroButton);
    buttonsLayout->addWidget(goConfigButton);
    vbox->addLayout(buttonsLayout);

    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    //jointSlider[0]->name->setMargin(10);
    //////////////////////////////////
    connect(resetZeroButton, SIGNAL(clicked(bool)), this, SLOT(clicked_resetZeroButton(bool)) );
    connect(goConfigButton, SIGNAL(clicked(bool)), this, SLOT(clicked_goConfigButton(bool)) );



    return groupBox;
}

void JntCtrlDialog::updateSilder()
{
   // cout<<"================================================"<<std::endl;
    for(int i=0; i<MAX_JOINT_NUM; i++)
    {
        jointSlider[i]->setValue(robotKin.curAxisValue[i]);
       // std::cout<<i<<"="<<robotKin.curAxisValue[i]<<std::endl;
    }
    updateEEinfo();

}

void JntCtrlDialog::updateSilder(int jntID)
{
    jointSlider[jntID]->setValue(robotKin.curAxisValue[jntID]);

}

void JntCtrlDialog::updateEEinfo()
{
    for(int i=MAX_JOINT_NUM; i<MAX_JOINT_NUM+3; i++)
    {
       // cout<<"__edit_EEInfo__: "<<edit_EEInfo<<", i="<<i<<", value:"<<robotKin.curAxisValue[i]<<endl;
        double val = robotKin.curAxisValue[i];
        QString unit="mm";
        if(i==(MAX_JOINT_NUM+2)) {
            unit="deg";
            val*=RTOD;
        }
       edit_EEInfo[i-MAX_JOINT_NUM]->setText(QString::number(val,'f',2)+unit);
    }
}

void JntCtrlDialog::clicked_resetZeroButton(bool checked)
{
    for(int i=0; i<MAX_JOINT_NUM; i++)
    {
        jointSlider[i]->setValue(0) ;
    }
    robotKin.forKin(0,0,0,0);
    g_MainWindow->updateJointValue();
    updateEEinfo();

}
void JntCtrlDialog::clicked_goConfigButton(bool checked)
{
    double ang[MAX_JOINT_NUM]={0};
    for(int i=0; i<MAX_JOINT_NUM; i++)
    {
        ang[i] = jointSlider[i]->edit->text().toDouble();
         if(i>=2) ang[i]*=DTOR;
        jointSlider[i]->setValue(ang[i]) ;

    }

    robotKin.forKin(ang);
    g_MainWindow->updateJointValue();
    updateEEinfo();
}
