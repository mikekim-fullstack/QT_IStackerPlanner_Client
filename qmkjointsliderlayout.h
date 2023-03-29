#ifndef QMKJOINTSLIDERLAYOUT_H
#define QMKJOINTSLIDERLAYOUT_H

#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
class JntCtrlDialog;
class QmkJointEditVLayout;
class QmkJointSliderLayout : public QHBoxLayout
{
public:
    QmkJointSliderLayout(QmkJointEditVLayout *parent, QString name, int jointID);
    QmkJointSliderLayout(JntCtrlDialog *parent, QString name, int jointID);
    ~QmkJointSliderLayout() override;
    bool update_robot;
    //JntCtrlDialog *parentDlg=nullptr;
    QmkJointEditVLayout *parentVLayout=nullptr;
    int jntID;
    QLabel *name;
    QSlider *slider;
    QLineEdit *edit;
    QPushButton *buttonDecrease, *buttonIncrease;
    int preValue;
    bool isUpdatedJoint;
    double ang;
    double residueAngle;
    void setQss(QByteArray &qss);
    void setUpdateRobot(bool set);
    void cmdMoveJoint(int dir, int jntID);
public slots:
    void sliderMoved(int value);
    void sliderReleased();
    void setValue(double val);

    void btnDecreasePressed();
    void btnDecreaseReleased();
    void btnIncreasePressed();
    void btnIncreaseReleased();
    //void setValue(int axis, double value);

};

#endif // QMKJOINTSLIDERLAYOUT_H
