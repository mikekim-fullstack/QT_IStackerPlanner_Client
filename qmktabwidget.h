#ifndef QmkTabWidget_H
#define QmkTabWidget_H

#include <QWidget>
#include <QTabWidget>
class QHBoxLayout;
class QLineEdit;
class QLabel;
#define NUM_TAB 2
class QmkTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit QmkTabWidget(QWidget *parent = nullptr);
    ~QmkTabWidget() override;
public:
   // QTabWidget *tabWidget=nullptr;
    QWidget *contentWidget[NUM_TAB]={nullptr};
public:

    int selectedTarget=-1;
    QLabel *selectedLabel=nullptr;
public:

    void createMotionLists();
    void cteateOrderLists();
//    QHBoxLayout *fillEditRange(char * name, int jntID);

public:


signals:

};

#endif // QmkTabWidget_H
