#ifndef QMKPUSHBUTTON_H
#define QMKPUSHBUTTON_H

#include <QPushButton>
#include <QWidget>

class QmkPushButton : public QPushButton
{
     Q_OBJECT
public:
//     QmkPushButton(QWidget *parent = nullptr);
     QmkPushButton(QString name, QWidget *parent = nullptr);

signals:
     void mkClicked();

//    // QObject interface
protected:
    virtual void mousePressEvent(QMouseEvent *e) override;


//    // QPaintDevice interface
public:
    virtual void mouseReleaseEvent(QMouseEvent *e)   override;



     // QAbstractButton interface
protected:
     virtual bool hitButton(const QPoint &pos) const override;
};

#endif // QMKPUSHBUTTON_H
