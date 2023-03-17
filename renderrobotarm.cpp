#include <string.h>
#include "renderrobotarm.h"
#include "../sharedfiles/mkmatrix.h"
//#include "mkstaublikin.h"
//#include "cmktargetspine.h"
#include "mainwindow.h"
#include <vtkLineSource.h>
#include <vtkTransform.h>
#include <vtkPolyDataSilhouette.h>
#include <vtkImageData.h>
#include <vtkImageViewer2.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkLookupTable.h>
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkCullerCollection.h>
#include <vtkCuller.h>
#include <vtkFrustumCoverageCuller.h>
#include <vtkWindowToImageFilter.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty2D.h>
#include <vtkNamedColors.h>
#include <vtkRendererCollection.h>
#include <vtkCellArray.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolygon.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSet.h>
// To setup the ContourWidget and its representation:
#include <vtkContourWidget.h>
#include <vtkProperty.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkArrowSource.h>
#include <vtkTransformPolyDataFilter.h>

#include <qdebug.h>
#define J0 0
#define J1 1
#define J2 2
#define J3 3
#define J4 4
#define J5 5
#define J6 6
#define J7 7
#define J8 8

#define nL 2
enum ObjName {HINGE, STATION, XMOTION, ZMOTION, LINK1, LINK2, CUPEE, CTRLARROW};
extern MKZoeRobotKin robotKin;
extern MainWindow *g_MainWindow;
//CRenderRobotArm *g_RenderRobotArm;
CRenderRobotArm::CRenderRobotArm(vtkRenderWindow *renderWindow, vtkRenderer *renderer, QObject *parent) : QObject(parent)
{
    _renderWindow = renderWindow;
    _renderer = renderer;

    toolOffsetXlen=0;
    _renderWindow->SetMultiSamples(4);
    _renderer->UseFXAAOn();
    _renderer->GetUseFXAA();
}
CRenderRobotArm::~CRenderRobotArm()
{
    cout<<"CRenderRobotArm Destructor"<<endl;
    objTarget.size();
    for( MapObjActor * obj: objTarget)
    {
        delete obj;
    }
    objTarget.clear();
//    for(int i=0; i<numActors[HINGE]; i++)
//    {
//        objHinge[i].actor->Delete();
//    }
//    for(int i=0; i<numActors[STATION]; i++)
//    {
//        objStations[i].actor->Delete();
//    }
//    for(int i=0; i<numActors[XMOTION]; i++)
//    {
//        objXmotion[i].actor->Delete();
//    }
//    for(int i=0; i<numActors[ZMOTION]; i++)
//    {
//        objZmotion[i].actor->Delete();
//    }
//    for(int i=0; i<numActors[LINK1]; i++)
//    {
//        objLink1[i].actor->Delete();
//    }
//    for(int i=0; i<numActors[LINK2]; i++)
//    {
//        objLink2[i].actor->Delete();
//    }
//    for(int i=0; i<numActors[CUPEE]; i++)
//    {
//        objCupEE[i].actor->Delete();
//    }
//    for(int i=0; i<numActors[CTRLARROW]; i++)
//    {
//        objControlArrow[i].actor->Delete();
//    }
}

CRenderRobotArm::CRenderRobotArm(const CRenderRobotArm &other)
{
    cout<<"copy Constructor"<<endl;
    if(&other==this) return;
    for(int i=0; i<nActorGroup; i++)
    {
        numActors[i] = other.numActors[i];
    }
    axes[0] = vtkSmartPointer<vtkAxesActor>::New();
    axes[0]->SetTotalLength(200,200,200);
    axes[0]->SetAxisLabels(false);
    _renderer->AddActor(axes[0]);

    //////////////////////////////////////////////////////////////////////////////////
    /// 1. Import Hinge 3D Objects...
    memcpy(this->objHinge, other.objHinge, sizeof(nHG));
    for(int i=0; i<numActors[HINGE]; i++)
    {
        objHinge[i].actor  = vtkSmartPointer<vtkActor>::New();// middle of pointer
        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objHinge[i].actor->GetMapper()->GetInput());
        vtkSmartPointer<vtkPolyData> new_pd = vtkSmartPointer<vtkPolyData>::New();
        new_pd->DeepCopy(pd);
        vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputData(new_pd);
        objHinge[i].actor->SetMapper(mapper);
        objHinge[i].actor->SetProperty(other.objHinge[i].actor->GetProperty());

    }
    for(int i=0; i<numActors[HINGE]; i++)
    {
        _renderer->AddActor(objHinge[i].actor);
    }
    //////////////////////////////////////////////////////////////////////////////////
    /// 2. Import Stationary 3D Objects...
    /// transformPointerStations = vtkSmartPointer<vtkTransform>::New();
    for(int i=0; i<numActors[STATION]; i++)
    {
        objStations[i].actor  = vtkSmartPointer<vtkActor>::New();// middle of pointer
        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objStations[i].actor->GetMapper()->GetInput());
        vtkSmartPointer<vtkPolyData> new_pd = vtkSmartPointer<vtkPolyData>::New();
        new_pd->DeepCopy(pd);
        vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputData(new_pd);
        objStations[i].actor->SetMapper(mapper);
        objStations[i].actor->SetProperty(other.objStations[i].actor->GetProperty());
    }
    for(int i=0; i<numActors[STATION]; i++)
    {
        _renderer->AddActor(objStations[i].actor);
    }

}

CRenderRobotArm &CRenderRobotArm::operator=(const CRenderRobotArm &other)
{
   // cout<<"CRenderRobotArm &CRenderRobotArm::operator=(const CRenderRobotArm &other)"<<endl;

    if(&other==this) return *this;
    for(int i=0; i<nActorGroup; i++)
    {
        numActors[i] = other.numActors[i];
    }
   // for(int i=0; i<nActorGroup; i++) cout<<"2.---numActors["<<i<<"]="<<numActors[i]<<endl;
    axes[0] = vtkSmartPointer<vtkAxesActor>::New();
    axes[0]->SetTotalLength(200,200,200);
    axes[0]->SetAxisLabels(false);
    _renderer->AddActor(axes[0]);

    ///////////////////////////////////////////////////////////////////////////////////
    // 0. Copy Target 3D Object
    {
        targetActor  = vtkActor::New();// middle of pointer
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();

        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.targetActor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        targetActor->SetMapper(mapper);
        targetActor->RotateZ(180);
        targetActor->SetProperty(other.targetActor->GetProperty());
        targetActor->SetVisibility(false);
        _renderer->AddActor(targetActor);
    }
//    //////////////////////////////////////////////////////////////////////////////////
//    /// 1. Import Hinge 3D Objects...
   // memcpy(this->objHinge, other.objHinge, sizeof(nHG));
    for(int i=0; i<numActors[HINGE]; i++)
    {
        objHinge[i].actor  = vtkActor::New();// middle of pointer
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();

        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objHinge[i].actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        objHinge[i].actor->SetMapper(mapper);
        objHinge[i].actor->RotateZ(180);
        objHinge[i].actor->SetProperty(other.objHinge[i].actor->GetProperty());
        objHinge[i].actor->SetVisibility(false);
        memcpy(objHinge[i].param.posTargetEE, objHinge[i].actor->GetCenter(), sizeof(double)*3);
       // cout<<"Hinge Center["<<i<<"]="<<objHinge[i].param.posEE[0]<<", "<<objHinge[i].param.posEE[1]<<", "<<objHinge[i].param.posEE[2]<<endl;

    }
    for(int i=0; i<numActors[HINGE]; i++)
    {
        _renderer->AddActor(objHinge[i].actor);
    }

    //////////////////////////////////////////////////////////////////////////////////
    /// 2. Import Stationary 3D Objects...
    /// transformPointerStations = vtkSmartPointer<vtkTransform>::New();
  //  memcpy(this->objStations, other.objStations, sizeof(nST));
    for(int i=0; i<numActors[STATION]; i++)
    {
        objStations[i].actor  = vtkActor::New();// middle of pointer
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();

        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objStations[i].actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        objStations[i].actor->SetMapper(mapper);
        objStations[i].actor->RotateZ(180);
        objStations[i].actor->SetProperty(other.objStations[i].actor->GetProperty());
    }

    for(int i=0; i<numActors[STATION]; i++)
    {
        _renderer->AddActor(objStations[i].actor);
    }
    //////////////////////////////////////////////////////////////////////////////////
    /// 3.Import Xmotion Dependency 3D Objects...
    ///
   // memcpy(this->objXmotion, other.objXmotion, sizeof(nXM));
    transformPointerXmotion = vtkSmartPointer<vtkTransform>::New();
    for(int i=0; i<numActors[XMOTION]; i++)
    {
        objXmotion[i].actor  = vtkActor::New();// middle of pointer
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();

        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objXmotion[i].actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        objXmotion[i].actor->SetMapper(mapper);
        objXmotion[i].actor->RotateZ(180);
        objXmotion[i].actor->SetProperty(other.objXmotion[i].actor->GetProperty());
    }

    for(int i=0; i<numActors[XMOTION]; i++)
    {
        _renderer->AddActor(objXmotion[i].actor);
    }
    //////////////////////////////////////////////////////////////////////////////////
    /// 4.Import Ymotion Dependency 3D Objects...
  //  memcpy(this->objZmotion, other.objZmotion, sizeof(nZM));
    transformPointerZmotion = vtkSmartPointer<vtkTransform>::New();
    for(int i=0; i<numActors[ZMOTION]; i++)
    {
        objZmotion[i].actor  = vtkActor::New();// middle of pointer
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();

        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objZmotion[i].actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        objZmotion[i].actor->SetMapper(mapper);
        objZmotion[i].actor->RotateZ(180);
        objZmotion[i].actor->SetProperty(other.objZmotion[i].actor->GetProperty());

    }
    for(int i=0; i<numActors[ZMOTION]; i++)
    {
        _renderer->AddActor(objZmotion[i].actor);
    }
    //////////////////////////////////////////////////////////////////////////////////
    /// 5.Import Link1 Dependency 3D Objects...
   // memcpy(this->objLink1, other.objLink1, sizeof(nL1));
    for(int i=0; i<numActors[LINK1]; i++)
    {
        objLink1[i].actor  = vtkActor::New();// middle of pointer
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();

        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objLink1[i].actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        objLink1[i].actor->SetMapper(mapper);
        objLink1[i].actor->RotateZ(180);
        objLink1[i].actor->SetProperty(other.objLink1[i].actor->GetProperty());

        memcpy(objLink1[i].param.posTargetEE, objLink1[i].actor->GetCenter(), sizeof(double)*3);
        transformPointerLink1[i] = vtkSmartPointer<vtkTransform>::New();
    }
    for(int i=0; i<numActors[LINK1]; i++)
    {
        _renderer->AddActor(objLink1[i].actor);
    }
    //////////////////////////////////////////////////////////////////////////////////
    /// 6.Import Link2 Dependency 3D Objects...
   // memcpy(this->objLink2, other.objLink2, sizeof(nL2));
    for(int i=0; i<numActors[LINK2]; i++)
    {
        objLink2[i].actor  = vtkActor::New();// middle of pointer
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();

        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objLink2[i].actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        objLink2[i].actor->SetMapper(mapper);
        objLink2[i].actor->RotateZ(180);
        objLink2[i].actor->SetProperty(other.objLink2[i].actor->GetProperty());

        memcpy(objLink2[i].param.posTargetEE, objLink2[i].actor->GetCenter(), sizeof(double)*3);
        transformPointerLink2[i] = vtkSmartPointer<vtkTransform>::New();

    }
    for(int i=0; i<numActors[LINK2]; i++)
    {
        _renderer->AddActor(objLink2[i].actor);
    }
    //////////////////////////////////////////////////////////////////////////////////
    /// 7.Import EndEffector Cup Dependency 3D Objects...
   // memcpy(this->objCupEE, other.objCupEE, sizeof(nCE));
    for(int i=0; i<numActors[CUPEE]; i++)
    {
        objCupEE[i].actor  = vtkActor::New();// middle of pointer
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();

        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objCupEE[i].actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        objCupEE[i].actor->SetMapper(mapper);
        objCupEE[i].actor->RotateZ(180);
        objCupEE[i].actor->SetProperty(other.objCupEE[i].actor->GetProperty());

        memcpy(objCupEE[i].param.posTargetEE, objCupEE[i].actor->GetCenter(), sizeof(double)*3);
        transformPointerEE[i] = vtkSmartPointer<vtkTransform>::New();

    }
    for(int i=0; i<numActors[CUPEE]; i++)
    {
        _renderer->AddActor(objCupEE[i].actor);
    }

    //////////////////////////////////////////////////////////////////////////////////
    /// 8.Import Arrow controller Dependency 3D Objects...
   // memcpy(this->objControlArrow, other.objControlArrow, sizeof(nAR));
    for(int i=0; i<numActors[CTRLARROW]; i++)
    {
        objControlArrow[i].actor  = vtkActor::New();// middle of pointer
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();

        vtkPolyData* pd = vtkPolyData::SafeDownCast(other.objControlArrow[i].actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        objControlArrow[i].actor->SetMapper(mapper);
        objControlArrow[i].actor->RotateZ(180);
        objControlArrow[i].actor->SetProperty(other.objControlArrow[i].actor->GetProperty());

        memcpy(objControlArrow[i].param.posTargetEE, objControlArrow[i].actor->GetCenter(), sizeof(double)*3);
        transformPointerCtrlArrow[i] = vtkSmartPointer<vtkTransform>::New();

    }
    for(int i=0; i<numActors[CTRLARROW]; i++)
    {
        _renderer->AddActor(objControlArrow[i].actor);
    }
    //////////////////////////////////////////////

    return *this;
}

void CRenderRobotArm::visibilityFrontViewer()
{
    for(int i=0; i<14; i++){
        objStations[i].actor->GetProperty()->SetOpacity(0.4);
        objStations[i].actor->SetPickable(false);
    }
    for(int i=0; i<3; i++)
    {
        objXmotion[i].actor->GetProperty()->SetOpacity(0.4);
        objXmotion[i].actor->SetPickable(false);
    }
    objXmotion[2].actor->GetProperty()->SetOpacity(0.99);

    objCupEE[1].actor->GetProperty()->SetOpacity(0.7);
    objCupEE[2].actor->SetVisibility(false);
    objControlArrow[0].actor->SetVisibility(false);
    objControlArrow[3].actor->SetVisibility(false);
}

void CRenderRobotArm::visibilityTopViewer()
{
    objStations[0].actor->SetPickable(false);
    objStations[0].actor->GetProperty()->SetOpacity(0.4);
    objStations[1].actor->SetVisibility(false);//"Frame_Bar_T1"
    objStations[2].actor->SetVisibility(false);
    objStations[3].actor->SetVisibility(false);
    objStations[4].actor->SetVisibility(false);
    objStations[5].actor->GetProperty()->SetOpacity(0.5);
    objStations[6].actor->GetProperty()->SetOpacity(0.5);

    objXmotion[0].actor->SetVisibility(false);
    objXmotion[1].actor->SetVisibility(false);
    objXmotion[2].actor->SetVisibility(false);
    objCupEE[2].actor->SetVisibility(false);
}

void CRenderRobotArm::setRenderer(vtkRenderWindow *renderWindow, vtkRenderer *renderer)
{
    _renderWindow = renderWindow;
    _renderer = renderer;


}




bool CRenderRobotArm::loadVRMLFileForZoeRobot(const char *filePath)
{

    ObjTemplate objHingeL[] = {
        {"Joint1",{0.2,0.2,0.2}},
        {"Joint2",{0.2,0.2,0.2}},
        {"Joint3",{0.2,0.2,0.2}},
        {"Joint4",{0.2,0.2,0.2}},
        {"Joint_EE",{0.2,0.2,0.2}}
    };

    ObjTemplate objStationsL[] = {
        {"Shelve",{0.063621,0.668339,0.8}},
        {"Frame_Bar_T1",{0.2,0.2,0.2}},
        {"Frame_Bar_T2",{0.2,0.2,0.2}},
        {"Frame_Corner_RT1",{0.15,0.15,0.15}},
        {"Frame_Corner_LT1",{0.15,0.15,0.15}},
        {"InsideConerBracket_4304_005",{1,1,0}},
        {"InsideConerBracket_4304_002",{1,1,0}},
        {"Frame_Corner_LT2",{0.2,0.2,0.2}},
        {"Frame_Corner_RT2",{0.2,0.2,0.2}},
        {"counterTop",{0.3,0.3,0.3}},
        {"Frame_Side_Column_R1",{0.2,0.2,0.2}},
        {"Frame_Side_Column_R2",{0.2,0.2,0.2}},
        {"Frame_Side_Column_L1",{0.2,0.2,0.2}},
        {"Frame_Side_Column_L2",{0.2,0.2,0.2}},

        {"Frame_Bar_B1",{0.2,0.2,0.2}},
        {"Frame_Bar_B2",{0.2,0.2,0.2}},
        {"Frame_Corner_LB1",{0.2,0.2,0.2}},
        {"Frame_Corner_LB2",{0.2,0.2,0.2}},


        {"Frame_Corner_RB1",{0.2,0.2,0.2}},
        {"Frame_Corner_RB2",{0.2,0.2,0.2}},




        {"Nema34_002",{0.1,0.1,0.1}},
        {"Nema34_003",{0.1,0.1,0.1}},
        {"Nema34_Holder_Assembly_v9",{0.3,0.3,0.3}},
        {"Nema34_Holder_Assembly_v9_001",{0.3,0.3,0.3}},
        {"Nema34_Pulley_Holder_Assembly_v9",{0.3,0.3,0.3}},
        {"Nema34_Pulley_Holder_Assembly_v9_001",{0.3,0.3,0.3}},

        {"Connector_CornerLB",{0.25,0.25,0.25}},
        {"Connector_CornerLB_back",{0.25,0.25,0.25}},
        {"Connector_CornerLT",{0.25,0.25,0.25}},
        {"Connector_CornerLT_back",{0.25,0.25,0.256}},
        {"Connector_CornerRB",{0.25,0.25,0.25}},
        {"Connector_CornerRB_back",{0.25,0.25,0.25}},
        {"Connector_CornerRT",{0.25,0.25,0.25}},//0
        {"Connector_CornerRT_back",{0.25,0.25,0.25}},//0
        {"Idle_Pulley_8mm",{0.2,0.2,0.2}},
        {"Idle_Pulley_8mm_001",{0.2,0.2,0.2}},
        {"Pulley_BL",{0.3,.3,.3}},
        {"Pulley_LT",{0.3,.3,.3}},
        {"PulleyShaft_8mm",{0.4,0.4,.4}},
        {"PulleyShaft_8mm_001",{0.4,0.4,.4}},

        {"B13-8530_028",{0.7,0.7,0.7}},
        {"B13-8530_030",{0.7,0.7,0.7}},
        {"B13-8530_032",{0.7,0.7,0.7}},
        {"B13-8530_035",{0.7,0.7,0.7}},

        {"DragCableChain_Bottom_MountingPlate_v1",{0.063621,0.668339,0.8}},
//        {"END-STOP_Sensor_004",{1,0,0}},
//        {"END-STOP_Sensor_006",{1,0,0}},
//        {"END-STOP_Sensor_007",{1,0,0}},
//        {"END-STOP_Sensor_008",{1,0,0}},

        {"Proximity_SensorOn3x3Holder_v8",{1,1,0}},
        {"Proximity_SensorOn3x3Holder_v8_001",{1,1,0}},


        {"InsideConerBracket_4304",{1,1,0}},
        {"InsideConerBracket_4304_001",{1,1,0}},

        {"InsideConerBracket_4304_003",{1,1,0}},
        {"InsideConerBracket_4304_004",{1,1,0}},

        {"InsideConerBracket_4304_006",{1,1,0}},
        {"InsideConerBracket_4304_007",{1,1,0}},
        {"InsideConerBracket_4304_008",{1,1,0}},
        {"InsideConerBracket_4304_009",{1,1,0}},
        {"InsideConerBracket_4304_010",{1,1,0}},
        {"InsideConerBracket_4304_011",{1,1,0}},
        {"InsideConerBracket_4304_012",{1,1,0}},
        {"InsideConerBracket_4304_013",{1,1,0}},
        {"InsideConerBracket_4304_014",{1,1,0}},
        {"InsideConerBracket_4304_015",{1,1,0}},
        {"InsideConerBracket_4304_016",{1,1,0}},
        {"InsideConerBracket_4304_017",{1,1,0}},
        {"InsideConerBracket_4304_018",{1,1,0}},
        {"InsideConerBracket_4304_019",{1,1,0}}
    };
    ObjTemplate objXmotionL[]={
        // Xmotion dependency
        {"LinearBearing6824_004",{0.19,0.19,0.19}},
        {"X-GuidePlate_Top_Assembly_v5",{1,1,0}},
        {"Moving_Column",{0.25,0.25,0.25}},

        {"BallScrewRod",{0.5,0.5,0.5}},
        {"Nema34_001",{0.1,0.1,0.1}},//0

        {"LinearBearing6824_001",{0.19,0.19,0.19}},


        {"BK15_fixture",{0.0012,0.122,0.6}},
        {"BF15_block",{0.0012,0.122,0.6}},

//        {"N75-3627_nut_001",{0.7, 0.7, 0.7}},
//        {"N75-3627_nut_002",{0.7, 0.7, 0.7}},
//        {"N75-3627_nut_003",{0.7, 0.7, 0.7}},


        {"Nema34_BallScrew_Holder_Assembly_v6",{1,1,0}},

//        {"Proximity_SensorOn3x3Holder_002",{1,1,0}},
//        {"Proximity_SensorOn3x3Holder_003",{1,1,0}},
//        {"END-STOP_Sensor_005",{1,0,0}},
//        {"END-STOP_Sensor_009",{1,0,0}},

        {"X-GuidePlate_Bottom_Assembly_v9",{1,1,0}},

    };
    ObjTemplate objZmotionL[]={
        ////////////////
        //Zmotion dependency

        {"Z_BallScrew_Holder_Assembly",{1,1,0}},
        {"ballscrewNut20",{0.3,0.3,0.3}},
        {"B3M-60T-11H-BF-ID10",{0.063621,0.668339,0.8}},
        {"B3M-60T-11H-BF_002",{0.063621,0.668339,0.8}},
        {"B3M-11M_Belt",{0.1,.1,0.1}},
        {"B3M-11M_Belt_002",{0.1,.1,0.1}},
        {"PillowBlockBearingKP8mm_004",{0,0,0}},
        {"PillowBlockBearingKP8mm_005",{0,0,0}},
        {"bolt_5mm_01_003",{0.2,0.2,0.2}},
        {"bolt_5mm_003",{0.2,0.2,0.2}},
        {"END-STOP_Sensor_001",{1,0,0}},
        {"END-STOP_Sensor_002",{1,0,0}},
        {"LinearBearing8623",{0.2,0.2,0.2}},
        {"LinearBearing6824_021",{0.2,0.2,0.2}},
        {"NO_RU28_D52-10_Steel_v2",{0.1,0.1,0.1}},
        {"Nema24_001",{0.1,0.1,0.1}},

    };

    ObjTemplate objLink1L[]={
        {"RobotArm_Link1",{1,1,0}},
        {"Nema23",{0.1,0.1,0.1}},
        {"CrossRollerBearingRU28_Hub_D10-20_v8",{0.2,0.2,0.2}},
        {"Stop_Rod_RJ1_1",{0.4,0,0}},
        {"Stop_Rod_RJ1_2",{0.4,0,0}},
        {"B3M-60T-11H-BF_003",{0.063621,0.668339,0.8}},
        {"B3M-11M_Belt_003",{0.1,.1,0.1}},
        {"END-STOP_Sensor",{0.4,0,0}},
        {"END-STOP_Sensor_003",{0.4,0,0}},
        {"NO_RU28_D52-10_Steel_v2_001",{0.1,0.1,0.1}},
    };
    ObjTemplate objLink2L[]={
        {"RobotArm_Link2_v3",{0.8,0.8,0}},
        {"CrossRollerBearingRU28_Hub_D10-20_001",{0.2,0.2,0.2}},
        {"Stop_Rod_RJ2_1",{0.4,0,0}},
        {"Stop_Rod_RJ2_2",{0.4,0,0}},

    };
    ObjTemplate objCupEEL[]={
        {"CupHolder",{0.2,0.2,0.8}},
        {"Cup",{0.9,0.9,0.9}},
        {"Joint_EERot",{0.8,0.8,0.8}},

    };
    ObjTemplate objControlArrowL[]={
        // End-Effector
        {"Arrow_MoveX_Top",{1,0.3,0.3}},
        {"Arrow_MoveX",{1,0.3,0.3}},
        {"Arrow_MoveZ",{0.0,0.0,1}},
        {"Arrow_RotCup",{.1,0.1,1}},
    };
//    ObjTemplate objTargetL[]={
//        // End-Effector
//        {"Target",{1,0,0}},
//    };



numActors[0] = sizeof(objHingeL)/sizeof(ObjTemplate);
numActors[1] = sizeof(objStationsL)/sizeof(ObjTemplate);
numActors[2] = sizeof(objXmotionL)/sizeof(ObjTemplate);
numActors[3] = sizeof(objZmotionL)/sizeof(ObjTemplate);
numActors[4] = sizeof(objLink1L)/sizeof(ObjTemplate);
numActors[5] = sizeof(objLink2L)/sizeof(ObjTemplate);
numActors[6] = sizeof(objCupEEL)/sizeof(ObjTemplate);
numActors[7] = sizeof(objControlArrowL)/sizeof(ObjTemplate);

for(int i=0; i<nActorGroup; i++) cout<<"numActors["<<i<<"]="<<numActors[i]<<endl;

for(int i=0; i<numActors[0]; i++) objHinge[i].copyDataFromArray(objHingeL[i]);
for(int i=0; i<numActors[1]; i++) objStations[i].copyDataFromArray(objStationsL[i]);
for(int i=0; i<numActors[2]; i++) objXmotion[i].copyDataFromArray(objXmotionL[i]);
for(int i=0; i<numActors[3]; i++) objZmotion[i].copyDataFromArray(objZmotionL[i]);
for(int i=0; i<numActors[4]; i++) objLink1[i].copyDataFromArray(objLink1L[i]);
for(int i=0; i<numActors[5]; i++) objLink2[i].copyDataFromArray(objLink2L[i]);
for(int i=0; i<numActors[6]; i++) objCupEE[i].copyDataFromArray(objCupEEL[i]);
for(int i=0; i<numActors[7]; i++) objControlArrow[i].copyDataFromArray(objControlArrowL[i]);


//memcpy(this->objHinge, objHingeL, sizeof(objHingeL));
//memcpy(this->objStations, objStationsL, sizeof(objStationsL));
//memcpy(this->objXmotion, objXmotionL, sizeof(objXmotionL));
//memcpy(this->objZmotion, objZmotionL, sizeof(objZmotionL));
//memcpy(this->objLink1, objLink1L, sizeof(objLink1L));
//memcpy(this->objLink2, objLink2L, sizeof(objLink2L));
//memcpy(this->objCupEE, objCupEEL, sizeof(objCupEEL));
//memcpy(this->objControlArrow, objControlArrowL, sizeof(objControlArrowL));






     axes[0] = vtkSmartPointer<vtkAxesActor>::New();
     axes[0]->SetTotalLength(200,200,200);
     axes[0]->SetAxisLabels(false);
     _renderer->AddActor(axes[0]);


    vtkSmartPointer<vtkVRMLImporter> importer= vtkSmartPointer<vtkVRMLImporter>::New();
    importer->SetFileName ( filePath );
    importer->Read();
    importer->Update();

    vtkMapper *mapper;

    //////////////////////////////////////////////////////////////////////////////////
    /// 0. Import Target 3D Objects...
    targetActor = static_cast<vtkActor*>(importer->GetVRMLDEFObject("Target"));
    targetActor->RotateZ(180);
    mapper = targetActor ->GetMapper();
    mapper->ScalarVisibilityOff();
    targetActor->SetVisibility(false);
    targetActor->GetProperty()->SetColor(1,1,1);
    targetActor->GetProperty()->SetOpacity(0.5);
    _renderer->AddActor(targetActor);

    //////////////////////////////////////////////////////////////////////////////////
    /// 1. Import Hinge 3D Objects...
     cout<<"------------------------------------------------------------------------"<<endl;
    for(int i=0; i<numActors[HINGE]; i++)
    {
        objHinge[i].actor  = static_cast<vtkActor*>(importer->GetVRMLDEFObject(objHinge[i].param.name));// middle of pointer
        objHinge[i].actor->RotateZ(180);

        mapper = objHinge[i].actor ->GetMapper();
        mapper->ScalarVisibilityOff();

        memcpy(objHinge[i].param.posTargetEE, objHinge[i].actor->GetCenter(), sizeof(double)*3);
        objHinge[i].actor->SetVisibility(false);
        cout<<"Hinge Name["<<i<<"] = "<<objHinge[i].param.name
           <<", pos=("<<objHinge[i].actor->GetCenter()[0]<<", "
           <<objHinge[i].actor->GetCenter()[1]<<", "
           <<objHinge[i].actor->GetCenter()[2]<<") "
           <<endl;

    }
    for(int i=0; i<numActors[HINGE]; i++)
    {
        _renderer->AddActor(objHinge[i].actor);
    }

    //////////////////////////////////////////////////////////////////////////////////
    /// 2. Import Stationary 3D Objects...
    cout<<"------------------------------------------------------------------------"<<endl;
    transformPointerStations = vtkSmartPointer<vtkTransform>::New();
    for(int i=0; i<numActors[STATION]; i++)
    {
        objStations[i].actor  = static_cast<vtkActor*>(importer->GetVRMLDEFObject(objStations[i].param.name));// middle of pointer
//        objStations[i].actor ->RotateY(180);
        objStations[i].actor->RotateZ(180);

        mapper = objStations[i].actor ->GetMapper();
        mapper->ScalarVisibilityOff();
        objStations[i].actor->GetProperty()->SetColor(objStations[i].color);
        objStations[i].actor->GetProperty()->SetAmbient(0.2);
        objStations[i].actor->GetProperty()->SetDiffuse(0.2);
        objStations[i].actor->GetProperty()->SetSpecular(0.7);
        objStations[i].actor->GetProperty()->SetSpecularPower(2.5);
        cout<<"station Name["<<i<<"] = "<<objStations[i].param.name
           <<", pos=("<<objStations[i].actor->GetCenter()[0]<<", "
           <<objStations[i].actor->GetCenter()[1]<<", "
           <<objStations[i].actor->GetCenter()[2]<<") "
           <<endl;

    }

    for(int i=0; i<numActors[STATION]; i++)
    {
        _renderer->AddActor(objStations[i].actor);
    }
    //    mkMatrix transX(4);
    //    objXmotion[i].actor->GetMatrix(transX.m_pData);
    //    printMat("transX",transX);

    //////////////////////////////////////////////////////////////////////////////////
    /// 3.Import Xmotion Dependency 3D Objects...
    cout<<"------------------------------------------------------------------------"<<endl;
    transformPointerXmotion = vtkSmartPointer<vtkTransform>::New();
    for(int i=0; i<numActors[XMOTION]; i++)
    {
        objXmotion[i].actor  = static_cast<vtkActor*>(importer->GetVRMLDEFObject(objXmotion[i].param.name));// middle of pointer
        objXmotion[i].actor ->RotateZ(180);

        mapper = objXmotion[i].actor ->GetMapper();
        mapper->ScalarVisibilityOff();
        objXmotion[i].actor->GetProperty()->SetColor(objXmotion[i].color);
        objXmotion[i].actor->GetProperty()->SetAmbient(0.2);
        objXmotion[i].actor->GetProperty()->SetDiffuse(.2);
        objXmotion[i].actor->GetProperty()->SetSpecular(0.7);
        objXmotion[i].actor->GetProperty()->SetSpecularPower(2.5);
        cout<<"X-Motion Name["<<i<<"] = "<<objXmotion[i].param.name
           <<", pos=("<<objXmotion[i].actor->GetCenter()[0]<<", "
           <<objXmotion[i].actor->GetCenter()[1]<<", "
           <<objXmotion[i].actor->GetCenter()[2]<<") "
           <<endl;
    }
//    double *xCenter = objXmotion[0].actor->GetCenter();
//    cout<<"x-motion: "<<xCenter[0]<<", "<<xCenter[1]<<", "<<xCenter[2]<<endl;

    for(int i=0; i<numActors[XMOTION]; i++)
    {
        _renderer->AddActor(objXmotion[i].actor);
    }

    //////////////////////////////////////////////////////////////////////////////////
    /// 4.Import Zmotion Dependency 3D Objects...
    cout<<"------------------------------------------------------------------------"<<endl;
    transformPointerZmotion = vtkSmartPointer<vtkTransform>::New();
    for(int i=0; i<numActors[ZMOTION]; i++)
    {
        objZmotion[i].actor  = static_cast<vtkActor*>(importer->GetVRMLDEFObject(objZmotion[i].param.name));// middle of pointer
       // actorPointer[i]->GetProperty()->SetColor(colorLists[i]);
        objZmotion[i].actor ->RotateZ(180);

        mapper = objZmotion[i].actor ->GetMapper();
        mapper->ScalarVisibilityOff();
        objZmotion[i].actor->GetProperty()->SetColor(objZmotion[i].color);
        objZmotion[i].actor->GetProperty()->SetAmbient(0.2);
        objZmotion[i].actor->GetProperty()->SetDiffuse(0.2);
        objZmotion[i].actor->GetProperty()->SetSpecular(0.7);
        objZmotion[i].actor->GetProperty()->SetSpecularPower(2.5);
        cout<<"Z-Motion Name["<<i<<"] = "<<objZmotion[i].param.name
           <<", pos=("<<objZmotion[i].actor->GetCenter()[0]<<", "
           <<objZmotion[i].actor->GetCenter()[1]<<", "
           <<objZmotion[i].actor->GetCenter()[2]<<") "
           <<endl;

    }
    for(int i=0; i<numActors[ZMOTION]; i++)
    {
        _renderer->AddActor(objZmotion[i].actor);
//        matZmotion[i]=traXYZ(0,0,0);
//        objZmotion[i].actor->GetMatrix(matZmotion[i].m_pData);

    }


    //////////////////////////////////////////////////////////////////////////////////
    /// 5.Import Link1 Dependency 3D Objects...
    for(int i=0; i<numActors[LINK1]; i++)
    {
        objLink1[i].actor  = static_cast<vtkActor*>(importer->GetVRMLDEFObject(objLink1[i].param.name));// middle of pointer
        objLink1[i].actor ->RotateZ(180);

        mapper = objLink1[i].actor ->GetMapper();
        mapper->ScalarVisibilityOff();
        objLink1[i].actor->GetProperty()->SetColor(objLink1[i].color);
        objLink1[i].actor->GetProperty()->SetAmbient(0.2);
        objLink1[i].actor->GetProperty()->SetDiffuse(0.2);
        objLink1[i].actor->GetProperty()->SetSpecular(0.7);
        objLink1[i].actor->GetProperty()->SetSpecularPower(2.5);

        memcpy(objLink1[i].param.posTargetEE, objLink1[i].actor->GetCenter(), sizeof(double)*3);
        transformPointerLink1[i] = vtkSmartPointer<vtkTransform>::New();
    }
    for(int i=0; i<numActors[LINK1]; i++)
    {
        _renderer->AddActor(objLink1[i].actor);
    }

    //////////////////////////////////////////////////////////////////////////////////
    /// 6.Import Link2 Dependency 3D Objects...
    for(int i=0; i<numActors[LINK2]; i++)
    {
        objLink2[i].actor  = static_cast<vtkActor*>(importer->GetVRMLDEFObject(objLink2[i].param.name));// middle of pointer
        objLink2[i].actor ->RotateZ(180);

        mapper = objLink2[i].actor ->GetMapper();
        mapper->ScalarVisibilityOff();
        objLink2[i].actor->GetProperty()->SetColor(objLink2[i].color);
        objLink2[i].actor->GetProperty()->SetAmbient(0.2);
        objLink2[i].actor->GetProperty()->SetDiffuse(0.2);
        objLink2[i].actor->GetProperty()->SetSpecular(0.7);
        objLink2[i].actor->GetProperty()->SetSpecularPower(2.0);

        memcpy(objLink2[i].param.posTargetEE, objLink2[i].actor->GetCenter(), sizeof(double)*3);
        transformPointerLink2[i] = vtkSmartPointer<vtkTransform>::New();

    }
    for(int i=0; i<numActors[LINK2]; i++)
    {
        _renderer->AddActor(objLink2[i].actor);
    }

    //////////////////////////////////////////////////////////////////////////////////
    /// 7.Import EndEffector Cup Dependency 3D Objects...
    for(int i=0; i<numActors[CUPEE]; i++)
    {
        objCupEE[i].actor  = static_cast<vtkActor*>(importer->GetVRMLDEFObject(objCupEE[i].param.name));// middle of pointer
        objCupEE[i].actor ->RotateZ(180);

        mapper = objCupEE[i].actor ->GetMapper();
        mapper->ScalarVisibilityOff();
        objCupEE[i].actor->GetProperty()->SetColor(objCupEE[i].color);
        objCupEE[i].actor->GetProperty()->SetAmbient(0.2);
        objCupEE[i].actor->GetProperty()->SetDiffuse(0.2);
        objCupEE[i].actor->GetProperty()->SetSpecular(0.6);
        objCupEE[i].actor->GetProperty()->SetSpecularPower(2);

        memcpy(objCupEE[i].param.posTargetEE, objCupEE[i].actor->GetCenter(), sizeof(double)*3);
        transformPointerEE[i] = vtkSmartPointer<vtkTransform>::New();

    }

    for(int i=0; i<numActors[CUPEE]; i++)
    {
        _renderer->AddActor(objCupEE[i].actor);
    }


    //////////////////////////////////////////////////////////////////////////////////
    /// 8.Import Arrow controller Dependency 3D Objects...
    for(int i=0; i<numActors[CTRLARROW]; i++)
    {
        objControlArrow[i].actor  = static_cast<vtkActor*>(importer->GetVRMLDEFObject(objControlArrow[i].param.name));// middle of pointer
        objControlArrow[i].actor ->RotateZ(180);

        mapper = objControlArrow[i].actor ->GetMapper();
        mapper->ScalarVisibilityOff();
        objControlArrow[i].actor->GetProperty()->SetColor(objControlArrow[i].color);
        objControlArrow[i].actor->GetProperty()->SetAmbient(0.2);
        objControlArrow[i].actor->GetProperty()->SetDiffuse(0.2);
        objControlArrow[i].actor->GetProperty()->SetSpecular(0.6);
        objControlArrow[i].actor->GetProperty()->SetSpecularPower(2);

        memcpy(objControlArrow[i].param.posTargetEE, objControlArrow[i].actor->GetCenter(), sizeof(double)*3);
        transformPointerCtrlArrow[i] = vtkSmartPointer<vtkTransform>::New();

    }
    for(int i=0; i<numActors[CTRLARROW]; i++)
    {
        _renderer->AddActor(objControlArrow[i].actor);
    }


    return true;
}

void CRenderRobotArm::resetZoeRobotPosition()
{
    vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
    mat->Identity();
    for(int i=0; i<numActors[2]; i++)
    {
        mat->Identity();
        objXmotion[i].actor->SetUserMatrix(mat);
    }
    for(int i=0; i<numActors[3]; i++)
    {
        objZmotion[i].actor->SetUserMatrix(mat);
    }
    for(int i=0; i<numActors[4]; i++)
    {
         objLink1[i].actor->SetUserMatrix(mat);
    }
    for(int i=0; i<numActors[5]; i++)
    {
         objLink2[i].actor->SetUserMatrix(mat);
    }
    for(int i=0; i<numActors[6]+1; i++)
    {
        objCupEE[i].actor->SetUserMatrix(mat);
    }
    _renderWindow->Render();
}


//void CRenderRobotArm::setFinalJointByMouseUp(double x, double y, double r1, double r2)
//{
////    robotKin.currentJointPos[0]+=x;
////    robotKin.currentJointPos[1]+=y;
////    robotKin.currentJointPos[2]+=r1;
////    robotKin.currentJointPos[3]+=r2;

////    /////////////////////////////////////////////
////    // Check Joint Limits
////    if(robotKin.currentJointPos[0]<=0.0) robotKin.currentJointPos[0]=0;
////    else if(robotKin.currentJointPos[0]>=robotKin.J1_UPPERLIMIT) robotKin.currentJointPos[0]=robotKin.J1_UPPERLIMIT;

////    if(robotKin.currentJointPos[1]<=0.0) robotKin.currentJointPos[1]=0;
////    else if(robotKin.currentJointPos[1]>=robotKin.J2_UPPERLIMIT) robotKin.currentJointPos[1]=robotKin.J2_UPPERLIMIT;

//}

void CRenderRobotArm::updateZoeRobotConfig()
{

    /////////////////////////////////////////
    T1 = traXYZ( robotKin.param.Px , 0, 0 );

    T2 = traXYZ( 0 , 0, robotKin.param.Pz )*T1;

    T3 = traXYZ( objHinge[2].param.posTargetEE[0] ,objHinge[2].param.posTargetEE[1] , objHinge[2].param.posTargetEE[2] )*T2;
    mkMatrix L3_2 = traXYZ( objHinge[3].param.posTargetEE[0]-objHinge[2].param.posTargetEE[0] ,
                            objHinge[3].param.posTargetEE[1]-objHinge[2].param.posTargetEE[1] ,
                            objHinge[3].param.posTargetEE[2]-objHinge[2].param.posTargetEE[2] );
    mkMatrix HingePos2 = traXYZ( objHinge[2].param.posTargetEE[0] ,objHinge[2].param.posTargetEE[1] , objHinge[2].param.posTargetEE[2] );
    T4  =  T2*HingePos2*rotZrad(robotKin.param.th1)*L3_2;
    mkMatrix L4_3 = traXYZ( objHinge[4].param.posTargetEE[0]-objHinge[3].param.posTargetEE[0] ,
                            objHinge[4].param.posTargetEE[1]-objHinge[3].param.posTargetEE[1] ,
                            objHinge[4].param.posTargetEE[2]-objHinge[3].param.posTargetEE[2] );
    mkMatrix HingePos4 = traXYZ( objHinge[3].param.posTargetEE[0] ,objHinge[3].param.posTargetEE[1] , objHinge[3].param.posTargetEE[2] );
    T5  =  T4*rotZrad(robotKin.param.th2)*L4_3;


    // 1.Move Objects along with X-axis
    transformPointerXmotion->SetMatrix(T1.m_pData);
    for(int i=0; i<numActors[XMOTION]; i++)
    {
        objXmotion[i].actor->SetUserTransform(transformPointerXmotion);

    }
    objControlArrow[0].actor->SetUserTransform(transformPointerXmotion);
    objControlArrow[1].actor->SetUserTransform(transformPointerXmotion);
    // 2.Move Objects along with Y-axis
    transformPointerZmotion->SetMatrix(T2.m_pData);
    for(int i=0; i<numActors[ZMOTION]; i++)
    {
        objZmotion[i].actor->SetUserTransform(transformPointerZmotion);
    }
    objControlArrow[0].actor->SetUserTransform(transformPointerZmotion);
    objControlArrow[2].actor->SetUserTransform(transformPointerZmotion);
    // 3.Rotate1 Objects about Y-axis
    for(int i=0; i<numActors[LINK1]; i++)
    {
        mkMatrix P3 = traXYZ( -objLink1[i].param.posTargetEE[0] ,-objLink1[i].param.posTargetEE[1] , -objLink1[i].param.posTargetEE[2] );
        mkMatrix L3_0 = traXYZ( objLink1[i].param.posTargetEE[0]-objHinge[2].param.posTargetEE[0] ,
                                objLink1[i].param.posTargetEE[1]-objHinge[2].param.posTargetEE[1] ,
                                objLink1[i].param.posTargetEE[2]-objHinge[2].param.posTargetEE[2] );

        mkMatrix T =  T3*rotZrad(robotKin.param.th1)*L3_0* P3;
        transformPointerLink1[i]->SetMatrix(T.m_pData);
        objLink1[i].actor->SetUserTransform(transformPointerLink1[i]);
    }

    // 4. Rotate2 Objects about Y-axis
    for(int i=0; i<numActors[LINK2]; i++)
    {
        mkMatrix P4 = traXYZ( -objLink2[i].param.posTargetEE[0],-objLink2[i].param.posTargetEE[1] , -objLink2[i].param.posTargetEE[2] );
        mkMatrix L4_0 = traXYZ( objLink2[i].param.posTargetEE[0]-objHinge[3].param.posTargetEE[0] ,
                               objLink2[i].param.posTargetEE[1]-objHinge[3].param.posTargetEE[1] ,
                               objLink2[i].param.posTargetEE[2]-objHinge[3].param.posTargetEE[2] );

        mkMatrix T = T4*rotZrad(robotKin.param.th2)*L4_0* P4;

        transformPointerLink2[i]->SetMatrix(T.m_pData);
        objLink2[i].actor->SetUserTransform(transformPointerLink2[i]);
    }
    // 5. Cup Position
    //mkMatrix Tcup;
    for(int i=0; i<numActors[CUPEE]; i++)
    {
        mkMatrix P5 = traXYZ( -objLink2[i].param.posTargetEE[0], -objLink2[i].param.posTargetEE[1] , -objLink2[i].param.posTargetEE[2] );
        mkMatrix L5_4 = traXYZ( objLink2[i].param.posTargetEE[0]-objHinge[4].param.posTargetEE[0] ,
                               objLink2[i].param.posTargetEE[1]-objHinge[4].param.posTargetEE[1] ,
                               objLink2[i].param.posTargetEE[2]-objHinge[4].param.posTargetEE[2] );

        mkMatrix T = T5*L5_4* P5;
        transformPointerEE[i]->SetMatrix(T.m_pData);
        objCupEE[i].actor->SetUserTransform(transformPointerEE[i]);
       // if(i==1) Tcup = T;
    }
//  //  printMat("Tcup", Tcup);
//    transformPointerEE[3]->SetMatrix(Tcup.m_pData);
//    objCupEE[3].actor->SetUserTransform(transformPointerEE[3]);
    // 6. Controll Arrow
    mkMatrix P5 = traXYZ( -objControlArrow[3].param.posTargetEE[0],-objControlArrow[3].param.posTargetEE[1] , -objControlArrow[3].param.posTargetEE[2] );
    mkMatrix L5_4 = traXYZ( objControlArrow[3].param.posTargetEE[0]-objHinge[4].param.posTargetEE[0] ,
                           objControlArrow[3].param.posTargetEE[1]-objHinge[4].param.posTargetEE[1] ,
                           objControlArrow[3].param.posTargetEE[2]-objHinge[4].param.posTargetEE[2] );

    mkMatrix T = T5*L5_4*P5;
    transformPointerCtrlArrow[3]->SetMatrix(T.m_pData);
    objControlArrow[3].actor->SetUserTransform(transformPointerCtrlArrow[3]);


}

MapObjActor *CRenderRobotArm::addTargetFromeFile(string name, string desc, string *type ,double *pos)
{
    mkMatrix Ttarget;
    vtkSmartPointer<vtkTransform> transformTarget = vtkSmartPointer<vtkTransform>::New();
    Ttarget = traXYZ(pos[0]+robotKin.offDisX, pos[1]+robotKin.offDisY, pos[2]+robotKin.offDisZ)*rotZrad(pos[3]+M_PI_2);
    transformTarget->SetMatrix(Ttarget.m_pData);
    MapObjActor *newActor = new MapObjActor;

    memcpy(newActor->param.posTargetEE, pos, sizeof(double)*4);    
    string value[4]={name, desc, type[0], type[1]};
    newActor->setData(value);

    newActor->addActor(transformTarget, targetActor, _renderer);
    objTarget.push_back(newActor);

    return newActor;
}


//void CRenderRobotArm::createSelectedTarget()
//{
//    mkMatrix Ttarget;
//    vtkSmartPointer<vtkTransform> transformTarget = vtkSmartPointer<vtkTransform>::New();
//    Ttarget = traXYZ(robotKin.param.EEx+robotKin.offDisX, robotKin.param.EEy+robotKin.offDisY, robotKin.param.EEz+robotKin.offDisZ)*rotZrad(robotKin.param.EEth+M_PI_2);
//    transformTarget->SetMatrix(Ttarget.m_pData);
//    selectedTarget = new MapObjActor;
//    selectedTarget->setName("SelectedTarget","OnlyOne");
//    //strcpy(newActor->name, name.c_str());
//    selectedTarget->addActor(transformTarget, targetActor, _renderer);
//    _renderWindow->Render();
//}

void CRenderRobotArm::moveSelectedTarget(double *pos)
{
    mkMatrix Ttarget;
    vtkSmartPointer<vtkTransform> transformTarget = vtkSmartPointer<vtkTransform>::New();
    Ttarget = traXYZ(pos[0]+robotKin.offDisX, pos[1]+robotKin.offDisY, pos[2]+robotKin.offDisZ)*rotZrad(pos[3]+M_PI_2);
    transformTarget->SetMatrix(Ttarget.m_pData);
    targetActor->SetUserTransform(transformTarget);
    targetActor->VisibilityOn();
    targetActor->GetProperty()->SetColor(1,0,0);
    _renderWindow->Render();
}

MapObjActor * CRenderRobotArm::addTarget(string value[])
{
    mkMatrix Ttarget;
    vtkSmartPointer<vtkTransform> transformTarget = vtkSmartPointer<vtkTransform>::New();
    Ttarget = traXYZ(robotKin.param.EEx+robotKin.offDisX, robotKin.param.EEy+robotKin.offDisY, robotKin.param.EEz+robotKin.offDisZ)*rotZrad(robotKin.param.EEth+M_PI_2);
    transformTarget->SetMatrix(Ttarget.m_pData);
    MapObjActor *newActor = new MapObjActor;
    newActor->param.posTargetEE[0] = robotKin.param.EEx;
    newActor->param.posTargetEE[1] = robotKin.param.EEy;
    newActor->param.posTargetEE[2] = robotKin.param.EEz;
    newActor->param.posTargetEE[3] = robotKin.param.EEth;
    newActor->setData(value);
    //strcpy(newActor->name, name.c_str());
    newActor->addActor(transformTarget, targetActor, _renderer);

    objTarget.push_back(newActor);
    _renderWindow->Render();
    return newActor;
    //objCupEE[3].actor->SetUserTransform(transformPointerEE[3]);
}

MapObjActor * CRenderRobotArm::searchTarget(string name)
{
    auto itr = std::find_if(objTarget.begin(), objTarget.end(),
                            [&](MapObjActor *i) {
        return (i && (std::string(i->param.name)==name));
    });
    if(itr!=objTarget.end())
    {
        return (*itr);
    }
    else return nullptr;
    /////////////////////////////////////
//    for(MapObjActor *obj : objTarget)
//    {
//        if(strcmp(obj->name,name.c_str())==0)
//        {
//            return obj;
//        }
//    }
//    return nullptr;
}

void CRenderRobotArm::updateTargetDistanceText(double dist)
{
    char str[128];
    sprintf(str,"Remain Distance: %3.2f [mm]", dist);
    //targetDistance->SetInput ( str );
}

void CRenderRobotArm::updateCartLocationText(char *loc)
{
    char str[128];
    sprintf(str,"Cart: %s", loc);
    //cartLocation->SetInput ( str );
}

void CRenderRobotArm::addOriginAxis()
{
    double l[3]={20,20,20};

     axes[0] = vtkSmartPointer<vtkAxesActor>::New();
     axes[0]->SetTotalLength(l);
     _renderer->AddActor(axes[0]);
}
void CRenderRobotArm::moveRobotStation(double x, double y, double z)
{
    // 1.Move Objects
    mkMatrix T1 = traXYZ( x , y, z );

    transformPointerStations->SetMatrix(T1.m_pData);

    for(int i=0; i<numActors[1]; i++)
    {
        objStations[i].actor->SetUserTransform(transformPointerStations);
    }
}


