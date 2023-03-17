#include "qmkcreatemotiondlg.h"
#include "qmkmotionparamlayout.h"
#include "mkGlobalData.h"
#include "../sharedfiles/mkRobotKin.h"
#include "mainwindow.h"
#include <iostream>


#include <QLabel>
#include "qmkpushbutton.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleValidator>
#include <QTableWidget>
#include <QHeaderView>


extern MKZoeRobotKin robotKin;
extern QByteArray qss_data;
extern MainWindow *g_MainWindow;

QmkCreateMotionDlg::QmkCreateMotionDlg(int type)
{
    setModal(true);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    setWindowFlags(Qt::Dialog);
    buttonAccept = new QmkPushButton("OK");
    buttonCancel = new QmkPushButton("Cancel");
    connect(buttonCancel, &QmkPushButton::mkClicked, this, &QmkCreateMotionDlg::clickedButtonCancel);
    switch(type)
    {
    case MOVE_X:break;
    case MOVE_Z:break;
    case FLY_TO:  break;
    case FOLD_ARM:createFoldArmLayout(); break;
    case PLACE_TARGET:createPlaceTargetLayout(); break;
    case GRAB_TARGET:break;
    case CIRCULAR_MOTION: break;
    case SPIRAL_MOTION: break;
    }
}

QmkCreateMotionDlg::~QmkCreateMotionDlg()
{
//    for(int i=0; i<6; i++) {
//        if(lineEditInput[i]) delete lineEditInput[i];
//    }

}

void QmkCreateMotionDlg::createFoldArmLayout()
{
    setWindowTitle("Create Fold Arm Motion");

    paramVLayout = new QmkMotionParamLayout;
    paramVLayout->createFoldArm();

    QHBoxLayout *hBoxLayoutButton = new QHBoxLayout;
    note = new QLabel("Note:");
    hBoxLayoutButton->addWidget(buttonAccept);
    hBoxLayoutButton->addWidget(buttonCancel);
    paramVLayout->addLayout(hBoxLayoutButton);
    paramVLayout->addWidget(note);

    setLayout(paramVLayout);

}

void QmkCreateMotionDlg::createPlaceTargetLayout()
{
    setWindowTitle("Create Place Target Motion");

    paramVLayout = new QmkMotionParamLayout;
    paramVLayout->createPlaceTarget();

    QHBoxLayout *hBoxLayoutButton = new QHBoxLayout;
    note = new QLabel("Note:");
    hBoxLayoutButton->addWidget(buttonAccept);
    hBoxLayoutButton->addWidget(buttonCancel);
    paramVLayout->addLayout(hBoxLayoutButton);
    paramVLayout->addWidget(note);

    setLayout(paramVLayout);
}

void QmkCreateMotionDlg::clickedButtonOK()
{
    this->close();
}

void QmkCreateMotionDlg::clickedButtonCancel()
{
    this->close();
}
