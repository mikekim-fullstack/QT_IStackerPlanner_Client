#include "qmkjointeditvlayout.h"
#include <iostream>
#include "qmkjointsliderlayout.h"
#include <QPushButton>
#include "../sharedfiles/mkRobotKin.h"
#include "mainwindow.h"
#include "qmkpushbutton.h"
#include <QSpinBox>
extern MKZoeRobotKin robotKin;
extern QByteArray qss_data;
extern MainWindow *g_MainWindow;

QmkJointEditVLayout::QmkJointEditVLayout()
{
    cout<<"construction: Joint Editor"<<endl;

//    grid_layout = new QGridLayout;


//     grid_layout->setVerticalSpacing(10);

//    grid_layout->addWidget(createJointGroup(), 0, 0);

//    addLayout(grid_layout);
    createJointGroup();
    updateEEinfo();
}

QGroupBox *QmkJointEditVLayout::createJointGroup()
{
//    QGroupBox *groupBox = new QGroupBox;
//    groupBox->setObjectName(QStringLiteral("joint_left_groupbox"));
//    groupBox->setContentsMargins(10,0,10,0);
    //QVBoxLayout *vbox = new QVBoxLayout;

    QLabel *title_label =  new QLabel("JOINT EDITOR");
    title_label->setAlignment(Qt::AlignHCenter);
    this->addSpacing(5);
    this->addWidget(title_label);

    const char *name[MAX_JOINT_NUM]={"Joint   X[mm]", "Joint   Z[mm]","Joint R1[deg]","Joint R2[deg]"};

    for(int i=0; i<MAX_JOINT_NUM; i++)
    {
        jointSlider[i] = new QmkJointSliderLayout(this, name[i], i);
        jointSlider[i]->setQss( qss_data );
        jointSlider[i]->slider->setRange(robotKin.minJntLimit[i], robotKin.maxJntLimit[i]);
        jointSlider[i]->setMargin(0);
        this->addLayout(jointSlider[i]);
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
        //edit_EEInfo[i]->setTextMargins(0,0,0,0);
        EndEffectorInfoLayout->addWidget(labelEE[i]);
        EndEffectorInfoLayout->addWidget(edit_EEInfo[i]);
    }
    this->addLayout(EndEffectorInfoLayout);
    ////////////////////////////////////////////////////////////
    /// \brief buttonsLayout
    QLabel *speedLabel = new QLabel("JointSpeed");
    speedSpinBox = new QSpinBox;
    speedSpinBox->setRange(10, 100);
    speedSpinBox->setSingleStep(10);
    speedSpinBox->setValue(100);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    resetZeroButton = new QmkPushButton(tr("Reset to Zero "));
    goConfigButton = new QmkPushButton(tr("Go Configure"));
    comboBox_selectAxis = new QComboBox;
    resetZeroButton->setObjectName(QStringLiteral("joint_button"));
    goConfigButton->setObjectName(QStringLiteral("joint_button"));
    comboBox_selectAxis->addItem("X-Axis");
    comboBox_selectAxis->addItem("R1-Axis");
    comboBox_selectAxis->addItem("R2-Axis");
    comboBox_selectAxis->addItem("Z-Axis");
    comboBox_selectAxis->addItem("All-Axis");



    resetZeroButton->setDefault(false);
    resetZeroButton->setAutoDefault(false);
    goConfigButton->setDefault(false);
    goConfigButton->setAutoDefault(false);

    buttonsLayout->addWidget(speedLabel);
    buttonsLayout->addWidget(speedSpinBox);
    buttonsLayout->addWidget(resetZeroButton);
    buttonsLayout->addWidget(comboBox_selectAxis);
    buttonsLayout->addWidget(goConfigButton);
    buttonsLayout->setMargin(0);
    this->addLayout(buttonsLayout);

    //this->addStretch(1);
    //groupBox->setLayout(vbox);

    //jointSlider[0]->name->setMargin(10);
    //////////////////////////////////
//    connect(resetZeroButton, SIGNAL(clicked(bool)), this, SLOT(clicked_resetZeroButton(bool)) );
//    connect(goConfigButton, SIGNAL(clicked(bool)), this, SLOT(clicked_goConfigButton(bool)) );

    connect(resetZeroButton, &QmkPushButton::mkClicked, this, &QmkJointEditVLayout::clicked_resetZeroButton);
    connect(goConfigButton, &QmkPushButton::mkClicked, this, &QmkJointEditVLayout::clicked_goConfigButton );
    connect(comboBox_selectAxis, SIGNAL(currentIndexChanged(int)), this ,SLOT(clicked_selectedAxis(int)));
    connect(speedSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QmkJointEditVLayout::clicked_setJointSpeed);
    comboBox_selectAxis->setCurrentIndex(4);
    return nullptr;
}

void QmkJointEditVLayout::clicked_resetZeroButton()
{
    for(int i=0; i<MAX_JOINT_NUM; i++)
    {
        jointSlider[i]->setValue(0) ;
    }
    robotKin.forKin(0,0,0,0);
    g_MainWindow->updateJointValue();
    updateEEinfo();
    //resetZeroButton->setFocus();
}

void QmkJointEditVLayout::clicked_goConfigButton()
{
    if(currentSelectedAxis==4){
        // Select all joint motions...
        double pos[MAX_JOINT_NUM]={0};
        for(int i=0; i<MAX_JOINT_NUM; i++)
        {
            pos[i] = jointSlider[i]->edit->text().toDouble();
            if(i>=2) pos[i]*=DTOR;
            jointSlider[i]->setValue(pos[i]) ;

        }
        robotKin.forKin(pos);
        robotKin.printForKin();
        g_MainWindow->updateJointValue();
        g_MainWindow->action_moveMultiJointRobot();
        updateEEinfo();
        robotKin.printForKin();
        cout<<"clicked_goConfigButton: AllJoints-"<<currentSelectedAxis<<endl;
    }
    else {
    // Select only one joint motion...
        double pos[MAX_JOINT_NUM]={0};
        for(int i=0; i<MAX_JOINT_NUM; i++)
        {
            pos[i] = jointSlider[i]->edit->text().toDouble();
            if(i>=2) pos[i]*=DTOR;
            jointSlider[i]->setValue(pos[i]) ;

        }
        robotKin.forKin(pos);
        robotKin.printForKin();
        g_MainWindow->updateJointValue();
        g_MainWindow->action_moveSingleJointRobot(currentSelectedAxis);
        robotKin.printForKin();
        updateEEinfo();
        cout<<"clicked_goConfigButton: singleJoint-"<<currentSelectedAxis<<endl;

    }

}

void QmkJointEditVLayout::clicked_selectedAxis(int index)
{
    currentSelectedAxis=index;
    cout<<"currentSelectedAxis:"<<index<<endl<<flush;
}

void QmkJointEditVLayout::clicked_setJointSpeed(int speed)
{
    cout<<"speed: "<<speed<<endl<<flush;
    g_MainWindow->jointSpeedPercent = speed;
    g_MainWindow->jointAccPercent = speed;
}



void QmkJointEditVLayout::updateSilder()
{
    for(int i=0; i<MAX_JOINT_NUM; i++)
    {
        jointSlider[i]->setValue(robotKin.curAxisValue[i]);
       // std::cout<<i<<"="<<robotKin.curAxisValue[i]<<std::endl;
    }
    updateEEinfo();
}

void QmkJointEditVLayout::updateSilder(int jntID)
{
    jointSlider[jntID]->setValue(robotKin.curAxisValue[jntID]);
}

void QmkJointEditVLayout::updateEEinfo()
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
