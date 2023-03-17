#ifndef QMKINPUTTARGETNAMEDLG_H
#define QMKINPUTTARGETNAMEDLG_H

#include <QDialog>
class QListWidgetItem;
class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;
class QmkInputTargetNameDlg : public QDialog
{
    Q_OBJECT
public:
    char name[128];
    QLineEdit *targetNameLineEdit, *targetDescLineEdit;
    QComboBox *targetTypeCombo, *moveTypeCombo;
    QPushButton * buttonAccept, *buttonCancel;
    QLabel *note=nullptr;
public:
    QmkInputTargetNameDlg(QString title, QWidget *parent=nullptr);
    ~QmkInputTargetNameDlg() override;
public slots:
    void clickedButtonOK();
    void clickedButtonCancel();


    // QWidget interface
//protected:
//    virtual void closeEvent(QCloseEvent *event);
};



#endif // QMKINPUTTARGETNAMEDLG_H
