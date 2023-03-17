#include "qmkpushbutton.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
//QmkPushButton::QmkPushButton(QWidget *parent):QPushButton(parent)
//{

//}

QmkPushButton::QmkPushButton(QString name, QWidget *parent):QPushButton(name, parent)
{
    //connect(this, &QAbstractButton::clicked, this, &QmkPushButton::clicked);
}

void QmkPushButton::mousePressEvent(QMouseEvent *e)
{
  // qDebug() << "MouseButtonPress";
     if(e->buttons()==Qt::LeftButton){

         emit mkClicked();
         setFocus();
         //qDebug() << "---- left button";
         // emit QmkPushButton::click();
     }
     QPushButton::mousePressEvent(e);
}

void QmkPushButton::mouseReleaseEvent(QMouseEvent *e)
{
   // qDebug() << "mouseReleaseEvent";

   // emit QmkPushButton::click();
    QPushButton::mouseReleaseEvent(e);
}

bool QmkPushButton::hitButton(const QPoint &pos) const
{
  //  qDebug() << "hit button";
    return false;
    //return QPushButton::hitButton(pos);
}

