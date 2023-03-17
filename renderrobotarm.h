#ifndef RENDERROBOTARM_H
#define RENDERROBOTARM_H

#include <QObject>
#include "vtk_headers.h"
#include "../sharedfiles/mkmatrix.h"
#include <vtkTextWidget.h>
#include <vtkTextRepresentation.h>
#include "mkGlobalData.h"
#include "../sharedfiles/mkRobotKin.h"

#define nActorGroup 8
#define nHG 5
#define nST 65
#define nXM 10
#define nZM 16
#define nL1 10
#define nL2 4
#define nCE 3
#define nAR 4
#define nTarget 100
class CRenderRobotArm : public QObject
{
    Q_OBJECT
public:
    explicit CRenderRobotArm(vtkRenderWindow *renderWindow, vtkRenderer *renderer, QObject *parent = nullptr);
    ~CRenderRobotArm();

    CRenderRobotArm(const CRenderRobotArm & other);
    CRenderRobotArm & operator=(const CRenderRobotArm & other);
    void visibilityFrontViewer();
    void visibilityTopViewer();
signals:

public slots:

/////////////////////////////////////
public:
    vtkRenderWindow *_renderWindow ;
    vtkRenderer *_renderer;
public:
    //TX40RobotKin myRobot;
    double toolOffsetXlen;
public:

    int numActors[nActorGroup];

    mkMatrix T1, T2, T3, T4, T5;
    vtkSmartPointer<vtkAxesActor> axes[2];


    vtkSmartPointer<vtkTransform> transformPointerHinge[10];
    vtkSmartPointer<vtkTransform> transformPointerStations;
    vtkSmartPointer<vtkTransform> transformPointerXmotion;
    vtkSmartPointer<vtkTransform> transformPointerZmotion;
    vtkSmartPointer<vtkTransform> transformPointerLink1[30];
    vtkSmartPointer<vtkTransform> transformPointerLink2[30];
    vtkSmartPointer<vtkTransform> transformPointerEE[10];
    vtkSmartPointer<vtkTransform> transformPointerCtrlArrow[10];

    vtkSmartPointer<vtkTransform> transformPointerEEHandler[10];

    vtkSmartPointer<vtkPolyDataMapper> lineMapper[3];
    vtkSmartPointer<vtkLineSource> lineSource[3];
    vtkSmartPointer<vtkActor> lineActor[3];

//    vtkSmartPointer<vtkActor> targetLocRight[2];
//    vtkSmartPointer<vtkActor> targetLocLeft[2];
    vtkActor* targetActor;// Will be populated by creating new target position...

    MapObjActor objHinge[nHG];
    MapObjActor objStations[nST];
    MapObjActor objXmotion[nXM];
    MapObjActor objZmotion[nZM];
    MapObjActor objLink1[nL1];
    MapObjActor objLink2[nL2];
    MapObjActor objCupEE[nCE];
    MapObjActor objControlArrow[nAR];

    std::vector<MapObjActor*> objTarget;
    MapObjActor* selectedTarget;// for Topview
    //MapObjActor objTarget[nTarget];
    vtkNew<vtkCoordinate> coordinate;
    //vtkNew<vtkPoints> pointCircle2;
    mkMatrix matXmotion[nXM], matZmotion[nZM], matLink1[nL1], matLink2[nL2], matCupEE[nCE], matCtrlArrow[nAR];
    //mkMatrix matJnt[5];
    //MKZoeRobotKin robotKin;
    ///////////////////////////////////////////////////

public:
    void setRenderer(vtkRenderWindow *renderWindow, vtkRenderer *renderer);
    bool loadVRMLFileForZoeRobot(  const char *filePath);
    bool loadVRMLFileForZoeRobot_bk(  const char *filePath);
    void resetZoeRobotPosition();
    void updateZoeRobotConfig();//double x, double y, double rot1, double rot2);
    void updateZoeRobotConfig_bk();
    MapObjActor * addTargetFromeFile(string name, string desc, string *type , double *pos);
    //void createSelectedTarget();
    void moveSelectedTarget(double *pos);
    MapObjActor * addTarget(string value[]);
    MapObjActor * searchTarget(string name);



    void updateTargetDistanceText(double dist);
    void updateCartLocationText(char * loc);
    void addOriginAxis();
    void moveRobotStation(double x, double y, double z);
};

#endif // RENDERROBOTARM_H
