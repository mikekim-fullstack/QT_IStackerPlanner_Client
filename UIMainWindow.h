#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKOpenGLWidget.h"
#include <QDebug>
QT_BEGIN_NAMESPACE

class CRenderRobotArm;
class UIMainWindow
{
public:
    QAction *actionOpenFile;
    QAction *actionExit;
    QAction *actionPrint;
    QAction *actionHelp;
    QAction *actionSave;
    QWidget *centralwidget;
    QGroupBox *groupBoxGraph;
    QVBoxLayout *vboxLayout;
    QSplitter *splitter;
    QFrame *tableFrame;
    QVBoxLayout *vboxLayout1;
    QVTKOpenGLWidget *qvtkWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1002, 666);
        vboxLayout = new QVBoxLayout(groupBoxGraph);
        vboxLayout->setSpacing(0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);

        groupBoxGraph = new QGroupBox(centralwidget);
        groupBoxGraph->setObjectName(QString::fromUtf8("groupBoxGraph"));
        groupBoxGraph->setGeometry(QRect(9, 9, 984, 569));
        /*
        const QIcon icon = QIcon(QString::fromUtf8(":/Icons/help.png"));
        MainWindow->setWindowIcon(icon);
        MainWindow->setIconSize(QSize(22, 22));
        actionOpenFile = new QAction(MainWindow);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        actionOpenFile->setEnabled(true);
        const QIcon icon1 = QIcon(QString::fromUtf8(":/Icons/fileopen.png"));
        actionOpenFile->setIcon(icon1);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        const QIcon icon2 = QIcon(QString::fromUtf8(":/Icons/print.png"));
        actionPrint->setIcon(icon2);
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionHelp->setIcon(icon);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        const QIcon icon3 = QIcon(QString::fromUtf8(":/Icons/filesave.png"));
        actionSave->setIcon(icon3);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupBoxGraph = new QGroupBox(centralwidget);
        groupBoxGraph->setObjectName(QString::fromUtf8("groupBoxGraph"));
        groupBoxGraph->setGeometry(QRect(9, 9, 984, 569));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBoxGraph->sizePolicy().hasHeightForWidth());
        groupBoxGraph->setSizePolicy(sizePolicy);


        splitter = new QSplitter(groupBoxGraph);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        tableFrame = new QFrame(splitter);
        tableFrame->setObjectName(QString::fromUtf8("tableFrame"));
        vboxLayout1 = new QVBoxLayout(tableFrame);
        vboxLayout1->setSpacing(0);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        vboxLayout1->setContentsMargins(0, 0, 0, 0);
        */
//        splitter->addWidget(tableFrame);
  //      qvtkWidget = new QVTKOpenGLWidget(splitter);
        qvtkWidget = new QVTKOpenGLWidget(groupBoxGraph);
        qvtkWidget->setObjectName(QString::fromUtf8("qvtkWidget"));
//        sizePolicy.setHeightForWidth(qvtkWidget->sizePolicy().hasHeightForWidth());
//        qvtkWidget->setSizePolicy(sizePolicy);
        qvtkWidget->setMinimumSize(QSize(300, 300));
        //splitter->addWidget(qvtkWidget);

        vboxLayout->addWidget(qvtkWidget);
       //  vboxLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        /*
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1002, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        int sSize[4];
        statusbar->getContentsMargins(&sSize[0],&sSize[1],&sSize[2],&sSize[3]);
        qDebug()<<sSize[0]<<sSize[1]<<sSize[2]<<sSize[3];
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setOrientation(Qt::Horizontal);
        toolBar->setIconSize(QSize(22, 22));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpenFile);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuFile->addAction(actionHelp);
        menuFile->addAction(actionExit);
        toolBar->addAction(actionOpenFile);
        toolBar->addAction(actionSave);
        toolBar->addSeparator();
        toolBar->addAction(actionPrint);
        toolBar->addAction(actionHelp);
*/
        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
//        actionOpenFile->setText(QCoreApplication::translate("MainWindow", "Open File...", nullptr));
//        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
//        actionPrint->setText(QCoreApplication::translate("MainWindow", "Print", nullptr));
//        actionHelp->setText(QCoreApplication::translate("MainWindow", "Help", nullptr));
//        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
//        groupBoxGraph->setTitle(QCoreApplication::translate("MainWindow", "Views", nullptr));
//        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public UIMainWindow {};
} // namespace Ui

QT_END_NAMESPACE
#endif // UIMAINWINDOW_H
