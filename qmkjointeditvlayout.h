#ifndef QMKJOINTEDITVLAYOUT_H
#define QMKJOINTEDITVLAYOUT_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include "mkGlobalData.h"
#ifndef MAX_JOINT_NUM
    #define MAX_JOINT_NUM 4
#endif
class QHBoxLayout;
class QLineEdit;
class QLabel;
class QListWidget;
class QmkPushButton;
class QListWidgetItem;
class QmkJointSliderLayout;
class QmkPushButton;
class QSpinBox;
class QmkJointEditVLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit QmkJointEditVLayout();

public slots:
    void clicked_resetZeroButton();
    void clicked_goConfigButton();
    void clicked_selectedAxis(int index);
    void clicked_setJointSpeed(int speed);

private:
    QLabel *label;
    QGridLayout *grid_layout ;
    QLineEdit *edit_EEInfo[3];
    QComboBox *comboBox_selectAxis;
    QSpinBox *speedSpinBox;
    int currentSelectedAxis=0;

public:
    QmkPushButton *resetZeroButton =nullptr;
    QmkPushButton *goConfigButton=nullptr;
    QmkJointSliderLayout *jointSlider[MAX_JOINT_NUM];
public:
    QGroupBox *createJointGroup();
    void updateSilder();
    void updateSilder(int jntID);
    void updateEEinfo();
};

#endif // QMKJOINTEDITVLAYOUT_H
