#include "qmkinputtargetnamedlg.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
QmkInputTargetNameDlg::QmkInputTargetNameDlg(QString title,QWidget *parent): QDialog(parent)
{
    setModal(true);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    setWindowTitle(title);
    setWindowFlags(Qt::Dialog);
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout * hLayout1 = new QHBoxLayout;
    QHBoxLayout * hLayout2 = new QHBoxLayout;
    QHBoxLayout * hLayout3 = new QHBoxLayout;
    QHBoxLayout * hLayout4 = new QHBoxLayout;

    QStringList dest;
    dest<<"FINAL"<<"APPROACH";
    targetTypeCombo = new QComboBox;
    targetTypeCombo->addItems(dest);

    QStringList motion;
    motion<<"JOINT"<<"CARTESIAN";
    moveTypeCombo = new QComboBox;
    moveTypeCombo->addItems(motion);

    buttonAccept = new QPushButton("OK");
    buttonCancel = new QPushButton("Cancel");
    targetNameLineEdit = new QLineEdit("T_");
    targetDescLineEdit = new QLineEdit("");
   // targetDescLineEdit->setFixedWidth(150);
    note = new QLabel("Note:");
    hLayout1->addWidget(new QLabel("Target Name:"));
    hLayout1->addWidget(targetNameLineEdit);

    hLayout2->addWidget(new QLabel("Description:"));
    hLayout2->addWidget(targetDescLineEdit);

    hLayout3->addWidget(new QLabel("Target Type:"));
    hLayout3->addWidget(targetTypeCombo);

    hLayout3->addWidget(new QLabel("Motion Type:"));
    hLayout3->addWidget(moveTypeCombo);

    hLayout4->addWidget(buttonAccept);
    hLayout4->addWidget(buttonCancel);
    mainVLayout->addLayout(hLayout1);
    mainVLayout->addLayout(hLayout2);
    mainVLayout->addLayout(hLayout3);
    mainVLayout->addWidget(note);
    mainVLayout->addLayout(hLayout4);
    note->hide();


    setLayout(mainVLayout);
    connect(buttonCancel, &QPushButton::clicked, this, &QmkInputTargetNameDlg::clickedButtonCancel);
    targetNameLineEdit->setFocus();
}

QmkInputTargetNameDlg::~QmkInputTargetNameDlg()
{
    if(buttonAccept) delete buttonAccept;
    if(buttonCancel) delete buttonCancel;
    if(targetNameLineEdit) delete targetNameLineEdit;
    if(targetDescLineEdit) delete targetDescLineEdit;
    if(note) delete note;
}

void QmkInputTargetNameDlg::clickedButtonOK()
{
    this->close();
}

void QmkInputTargetNameDlg::clickedButtonCancel()
{
    targetNameLineEdit->setText("");
    this->close();
}

//void QmkInputTargetNameDialog::closeEvent(QCloseEvent *event)
//{
//     //targetNameLineEdit->setText("");
//}
