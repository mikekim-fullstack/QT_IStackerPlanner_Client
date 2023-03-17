#ifndef QMKTARGETSLIDERHLAYOUT_H
#define QMKTARGETSLIDERHLAYOUT_H

#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>

class QmkTargetEditVLayout;
class QmkTargetSliderHLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    QmkTargetSliderHLayout(QmkTargetEditVLayout *parent, int id, QString name, double min, double max);
    ~QmkTargetSliderHLayout() override;
    void setQss(QByteArray &qss);
public:
    int ID=0;
    QLabel *nameLabel, *minLabel, *maxLabel;
    QSlider *slider;
    QLineEdit *edit;
    QmkTargetEditVLayout *parent;
public slots:
    void sliderMoved(int value);
    void sliderReleased();
    void setValue(double val);
};

#endif // QMKTARGETSLIDERHLAYOUT_H
