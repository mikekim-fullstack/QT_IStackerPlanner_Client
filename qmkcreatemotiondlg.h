#ifndef QMKCREATEMOTIONDLG_H
#define QMKCREATEMOTIONDLG_H

#include <QDialog>

class QListWidgetItem;
class QmkPushButton;
class QLineEdit;
class QLabel;
class QTableWidget;
class QmkMotionParamLayout;

class QmkCreateMotionDlg : public QDialog
{
    Q_OBJECT
public:
    QmkCreateMotionDlg(int type);
    ~QmkCreateMotionDlg() override;
    void createFoldArmLayout();
    void createPlaceTargetLayout();
public:
    QmkMotionParamLayout * paramVLayout=nullptr;
    QmkPushButton * buttonAccept, *buttonCancel;
    QLabel *note=nullptr;
public slots:
    void clickedButtonOK();
    void clickedButtonCancel();

};

#endif // QMKCREATEMOTIONDLG_H
