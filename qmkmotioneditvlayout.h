#ifndef QMKMOTIONEDITVLAYOUT_H
#define QMKMOTIONEDITVLAYOUT_H
#include "mkGlobalData.h"
#include <QVBoxLayout>

class QmkPushButton;
class QTableWidget;
class QLabel;
class QLineEdit;
class QmkCreateMotionDlg;
class QmkMotionParamLayout;

class QmkMotionEditVLayout : public QVBoxLayout
{
private:
    const char *bNameCommand[9]={"DropCup","RotateEE", "LinearFly To", "Folding Arm",
                                 "Place Target", "Grab Target","Circular Motion", "Spiral Motion","Test Order"};
public:
    QmkMotionEditVLayout(QWidget *parent=nullptr);
    ~QmkMotionEditVLayout() override ;
    void createMotionLists();
    void clearAllItemsTableWidget();
    void* searchMotion(std::string name);
    void addMotionList(void * param);
    void selectMotionList(void *param, int type);

public:
    std::vector<FoldArmParam*> vecFoldArmParams;
    std::vector<PlaceTargetParam*> vecPlaceTargetParams;
   // QTableWidget *tableWidgetParmas=nullptr;
    QTableWidget *tableWidgetMotionLists=nullptr;
    QmkPushButton *buttonsCmd[9]={nullptr};
    QmkPushButton *buttonSave=nullptr;
    QmkPushButton *buttonMoveInMotion=nullptr;
    QLabel *labelCmdName=nullptr;
    QLineEdit *editRepeat1=nullptr;
    QLineEdit *editSpeed1=nullptr;

    QmkCreateMotionDlg *createMotionDlg=nullptr;
    QmkMotionParamLayout *paramLayout=nullptr;
    void *selectedParam=nullptr;


public slots:
    void slot_selectionChangedTableWidgetMotionList();
    void clickedButton_MoveInMotion();
    void clickedButton_dropCup();
    void clickedButton_createRotateEE();
    void clickedButton_createLinearFlyTo();
    void clickedButton_createFoldingArm();
    void clickedButton_createPlaceTarget();
    void clickedButton_createGrabTarget();
    void clickedButton_createCircularMotion();
    void clickedButton_createSpiralMotion();
    void clickedButton_testMotion();


};

#endif // QMKMOTIONEDITVLAYOUT_H
