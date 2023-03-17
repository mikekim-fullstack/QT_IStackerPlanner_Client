#include "mainwindow.h"
//#define _CRT_SECURE_NO_WARNINGS

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

#include "QVTKOpenGLWidget.h"

#include <QApplication>
#include <QtGui>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
//#ifdef Q_OS_OSX
//#include "osxHelper.h"
//#endif
//void disableGLHiDPI( long a_id );
// Library needed for processing XML documents
#include <QtXml>
// Library needed for processing files
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
    QApplication a(argc, argv);
    qDebug() << "App path : " << qApp->applicationDirPath();
    MainWindow w;
    w.showMaximized();
    w.show();
    return a.exec();
}
