#ifndef JNTCTRLDIALOG_H
#define JNTCTRLDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#define MAX_JOINT_NUM 4
class QmkJointSliderLayout;
class JntCtrlDialog : public QDialog
{
    Q_OBJECT
public:
    JntCtrlDialog(QWidget* parent=nullptr);
public slots:
    void changeAlwaysOnTop(bool state);
    void clicked_resetZeroButton(bool checked);
    void clicked_goConfigButton(bool checked);
private:
    QLabel *label;
    QGridLayout *grid_layout ;
    QLineEdit *edit_EEInfo[3];
public:

    QmkJointSliderLayout *jointSlider[MAX_JOINT_NUM];
public:
    QGroupBox *createJointGroup();
    void updateSilder();
    void updateSilder(int jntID);
    void updateEEinfo();
};

#endif // JNTCTRLDIALOG_H
