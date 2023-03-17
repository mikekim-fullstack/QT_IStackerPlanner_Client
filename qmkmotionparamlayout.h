#ifndef QMKMOTIONPARAMLAYOUT_H
#define QMKMOTIONPARAMLAYOUT_H

#include "mkGlobalData.h"
#include <QVBoxLayout>

class QListWidgetItem;
class QmkPushButton;
class QLineEdit;
class QLabel;
class QTableWidget;

class QmkMotionParamLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    QmkMotionParamLayout();
    ~QmkMotionParamLayout() override;
public:
    QTableWidget *tableWidget=nullptr;
    QLineEdit *lineEditInput[10]={nullptr};
    QmkPushButton * buttonAccept=nullptr, *buttonCancel=nullptr;
    QLabel *labelMain=nullptr;
    QLabel *labelNote=nullptr;

public :
    void clearLayout();
    void createFoldArm();
    void createPlaceTarget();

    void selectFoldArm(FoldArmParam * param);
    void selectPlaceTarget(PlaceTargetParam *param);
    void verticalResizeTableViewToContents();



};

#endif // QMKMOTIONPARAMLAYOUT_H
