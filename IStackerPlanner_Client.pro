QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 app_bundle
TARGET = IStackerRobotPlannerClient
TEMPLATE = app
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DEPRECATED_WARNINGS _CRT_SECURE_NO_WARNINGS
QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mkTopCamera.cpp \
    mkfilemanager.cpp \
    ../sharedfiles/mkmatrix.cpp \
    ../sharedfiles/mkZoeRobotics_velProfile.cpp \
    ../sharedfiles/mkVariables.cpp \
    qmkcreatemotiondlg.cpp \
    qmkinputtargetnamedlg.cpp \
    qmkjointeditvlayout.cpp \
    qmkjointsliderlayout.cpp \
    qmkmotioneditvlayout.cpp \
    qmkmotionparamlayout.cpp \
    qmkpushbutton.cpp \
    qmksetupvlayout.cpp \
    qmktabwidget.cpp \
    qmktargeteditvlayout.cpp \
    qmktargetsliderhlayout.cpp \
    qmkvtkwidget.cpp \
    mkFrontCamera.cpp \
    mksocketclient.cpp \
    renderrobotarm.cpp

HEADERS += \
    UIMainWindow.h \
    mainwindow.h \
    mkGlobalData.h \
    ../sharedfiles/mkRobotKin.h \
    ../sharedfiles/mkglobalclass.h \
    ../sharedfiles/mkZoeRobotics_define.h \
    ../sharedfiles/mkglobalclass.h \
#    ../sharedfiles/mkZoeRobotics_globalDataStruct.h \
    ../sharedfiles/mkZoeRobotics_velProfile.h \
    mkTopCamera.h \
    mkFrontCamera.h \
    mkfilemanager.h \
    mklinkedlist.h \
    ../sharedfiles/mkmatrix.h \
    qmkcreatemotiondlg.h \
    qmkinputtargetnamedlg.h \
    qmkjointeditvlayout.h \
    qmkjointsliderlayout.h \
    qmkmotioneditvlayout.h \
    qmkmotionparamlayout.h \
    qmkpushbutton.h \
    qmksetupvlayout.h \
    qmktabwidget.h \
    qmktargeteditvlayout.h \
    qmktargetsliderhlayout.h \
    qmkvtkwidget.h \
    renderrobotarm.h \
    ../sharedfiles/mkringbuffer.h \
    mksocketclient.h \
    vtk_headers.h

#FORMS += \
#    mainwindow.ui

TRANSLATIONS += \
    ZoeRoboticsPlanner_en_CA.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
##################################################################
unix {
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Interaction/Widgets
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Common/Color
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Color
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Filters/General
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Utilities/KWIML
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Utilities/KWIML
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Infovis/Core
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Infovis/Core
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Views/Qt
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Views/Qt
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Rendering/OpenGL2
     INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/OpenGL2
     INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Views/Core
     INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Views/Core
     INCLUDEPATH +=  /Users/mikekim/Developer/VTK-8.2.0/source/Views/Infovis
    INCLUDEPATH +=  /Users/mikekim/Developer/VTK-8.2.0/built/Views/Infovis
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/Context2D
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/IO/Export
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/IO/Export
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/GL2PSOpenGL2
    INCLUDEPATH +=/Users/mikekim/Developer/VTK-8.2.0/source/Filters/Modeling

    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/Label
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Rendering/Core
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Rendering/Label
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Interaction/Style
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Filters/Sources
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Common/Color
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Color
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Filters/General
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Filters/Geometry
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Imaging/Sources
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Rendering/OpenGL
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Filters/Hybrid
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Filters/Geometry
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Imaging/Sources
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Rendering/OpenGL2
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/OpenGL2
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Filters/Sources
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Rendering/Annotation
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Common/Math
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Common/Transforms
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/IO/Import
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/IO/Import
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/GUISupport/Qt
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/OpenGL
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/GUISupport/Qt
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/OpenGL
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Rendering/FreeType
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Common/Core
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Interaction/Image/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Interaction/Style
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/Core
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Interaction/Image/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Interaction/Widgets/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Interaction/Style/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/Core/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/IO/Image/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/IO/Core/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Filters/Core/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Filters/Sources/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Filters/General/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Filters/Hybrid
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Filters/Modeling
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Imaging
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Imaging/Hybrid
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/Annotation
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/FreeType
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Rendering/Volume
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Common/ExecutionModel
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Common/DataModel
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Imaging/Color
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Imaging/Core
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/Imaging/General
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/built/IO/Geometry

    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Core/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Transforms/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/DataModel/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Color/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/ExecutionModel/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Math/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/System/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Misc/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/ComputationalGeometry/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Imaging/Core
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Imaging/Color
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Imaging/General

    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Core/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Transforms/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/DataModel/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Color/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/ExecutionModel/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Math/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/System/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/Misc/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/ComputationalGeometry/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Imaging/Color

    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Rendering/Core/

    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/IO/Image/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/IO/Core/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/IO/Geometry

    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Interaction/Image/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Interaction/Widgets/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Interaction/Style/

    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Filters/Core/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Filters/Sources/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Filters/General/
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Imaging
    INCLUDEPATH += /Users/mikekim/Developer/VTK-8.2.0/source/Common/ExecutionModel

#For windows OS, copy all dll from vtk to debug or release directory
#    CONFIG(debug, debug|release) {
#    message("This is OSX debug mode")
    ##########################
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkChartsCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkCommonColor-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkCommonComputationalGeometry-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkCommonCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkCommonDataModel-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkCommonExecutionModel-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkCommonMath-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkCommonMisc-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkCommonSystem-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkCommonTransforms-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkDICOMParser-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkDomainsChemistry-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkDomainsChemistryOpenGL2-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersAMR-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersExtraction-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersFlowPaths-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersGeneral-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersGeneric-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersGeometry-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersHybrid-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersHyperTree-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersImaging-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersModeling-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersParallel-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersParallelImaging-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersPoints-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersProgrammable-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersSMP-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersSelection-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersSources-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersStatistics-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersTexture-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersTopology-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkFiltersVerdict-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkGUISupportQt-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkGUISupportQtOpenGL-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkGUISupportQtSQL-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkGeovisCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOAMR-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOAsynchronous-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOCityGML-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOEnSight-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOExodus-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOExport-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOExportOpenGL2-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOExportPDF-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOGeometry-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOImage-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOImport-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOInfovis-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOLSDyna-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOLegacy-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOMINC-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOMovie-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIONetCDF-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOPLY-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOParallel-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOParallelXML-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOSQL-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOSegY-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOTecplotTable-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOVeraOut-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOVideo-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOXML-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkIOXMLParser-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingColor-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingFourier-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingGeneral-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingHybrid-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingMath-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingMorphological-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingSources-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingStatistics-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkImagingStencil-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkInfovisCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkInfovisLayout-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkInteractionImage-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkInteractionStyle-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkInteractionWidgets-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkLocalExample-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkNetCDF-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkParallelCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingAnnotation-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingContext2D-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingContextOpenGL2-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingExternal-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingFreeType-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingFreeTypeFontConfig-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingGL2PSOpenGL2-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingImage-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingLOD-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingLabel-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingOpenGL2-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingQt-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingVolume-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkRenderingVolumeOpenGL2-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkViewsContext2D-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkViewsCore-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkViewsInfovis-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkViewsQt-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkdoubleconversion-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkexodusII-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkexpat-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkfreetype-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkgl2ps-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkglew-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkhdf5-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkhdf5_hl-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkjpeg-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkjsoncpp-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtklibharu-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtklibxml2-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtklz4-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtklzma-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkmetaio-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkogg-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkpng-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkproj-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkpugixml-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtksqlite-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtksys-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtktheora-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtktiff-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkverdict-8.2.1
    LIBS += -L/Users/mikekim/Developer/VTK-8.2.0/built/lib/Debug/ -lvtkzlib-8.2.1
}

DISTFILES += \
    stylesheet_darkgray.qss

RESOURCES += \
    resources.qrc

