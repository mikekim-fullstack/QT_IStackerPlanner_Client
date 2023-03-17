#ifndef QMKTARGETEDITVLAYOUT_H
#define QMKTARGETEDITVLAYOUT_H
#include <queue>
#include <QVBoxLayout>
#include <QPushButton>
#include "mkGlobalData.h"
class QHBoxLayout;
class QLineEdit;
class QLabel;
class QTableWidget;
class QmkPushButton;
class QmkInputTargetNameDlg;
//class MapObjActor;
class QListWidgetItem;
class QmkTargetSliderHLayout;

class QmkTargetEditVLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit QmkTargetEditVLayout(QWidget *parent = nullptr);
    ~QmkTargetEditVLayout() override;
signals:

public slots:
    void clickedButton_Add();
    void clickedButton_ModifyTarget();
    void clickedButton_Delete();
    void clickedButton_UpdateWithCurrentConfigure();
    void clickedButton_GoConfigure();
    void clickedButton_MoveToTarget();
    void clickedButton_MoveToTargetAnimation();
    void clickedButton_MoveToTargetRobot();
    void clickedButton_MoveToAllTargetRobot();
    void clickedButton_SaveToFile();
    void slot_selectedListWidgetItem();//(QListWidgetItem *item);
    void toggledJointMove(bool checked);
public:
    QTableWidget *tableWidget=nullptr;
    std::string targetName, currentName;
    QmkInputTargetNameDlg *inputDialog=nullptr;
    QmkPushButton *buttonsTop[6];
    QmkPushButton *buttonsBottom[3];
    //QLineEdit * POREdit[4];
    QmkTargetSliderHLayout *targetSliderLayout[4];
    int selectedTarget=-1;
    QLabel *selectedLabel=nullptr;
    //MapObjActor   *selectedTargetObj=nullptr;
    MapObjActor* LastPickedTargetObj=nullptr;
    vtkActor* LastPickedTargetActor=nullptr;
    std::queue<QString> queueHistoryName;
    QString prevSelectedName;
    bool isJointMove=true;

//    std::string LastPickedTargetType;
//    std::string LastPickedMoveType;
   // vtkProperty* LastPickedProperty;

public:
    void loadTargetXMLFile();
    void createPORTargetLists();
    void updateTargetInfo();
    void setSelectionTarget(MapObjActor *objTarget);

    int TEST_gen_linear_profile();
    int TEST_gen_linear2_profile(double EE1x, double EE1y, double EE2x, double EE2y, double EE1t);

};



#endif // QMKTARGETEDITVLAYOUT_H
