#ifndef MKGLOBALDATA_H
#define MKGLOBALDATA_H
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkProperty.h>

#ifndef RESOURCE_3DFILE_PATH
//#define RESOURCE_3DFILE_PATH const_cast<char*>("/Users/mikekim/Developer/ZoeRoboticsPlanner/3Dfiles/zoerobotV3.wrl")
#define RESOURCE_3DFILE_PATH const_cast<char*>("/Users/mikekim/Developer/ZoeRobotics_project/IStackerRobot/sharedfiles/3Dfiles/zoerobotV3.wrl")

#endif

#ifndef TARGET_SAVE_PATH
//#define TARGET_SAVE_PATH const_cast<char*>("/Users/mikekim/Developer/ZoeRoboticsPlanner/targetlists.xml")
#define TARGET_SAVE_PATH const_cast<char*>("/Users/mikekim/Developer/ZoeRobotics_project/IStackerRobot/sharedfiles/customerFiles/targetlists.xml")
#endif

#ifndef NUMJNT
#define NUMJNT 4
#endif

enum EE_AXIS {EEX=0, EEY, EEZ, EET};
enum MOTION_TYPE {MOVE_X, MOVE_Z, FLY_TO, FOLD_ARM, PLACE_TARGET, GRAB_TARGET, CIRCULAR_MOTION, SPIRAL_MOTION};

enum EEMOVE_TYPE {MT_JOINT, MT_LINEAR};
enum DESTINATION_TYPE {DT_TARGET, DT_GATE};

class MotionParam
{
public:
    int moveType=1;
    int targetType=1;


    char name[58];
    char desc[128];
    double posTargetEE[4]={0,0,0.0};//EEx,EEy,EEz,EEth;

    int type;
private:
    ////////////////////////////////////////
    // Define Static Data and access
    enum OPERATION_SELECT {OP_TARGET, OP_MOVE};
    static std::map<std::string, int>& opTypeName(int select)
    {
        static std::map<std::string, int>  mapTargetType {{"FINAL",1},{"APPROACH",2}};
        static std::map<std::string, int>  mapMoveType {{"JOINT",1},{"CARTESIAN",2}};
        static std::map<std::string, int> empty;
        switch(select)
        {
        case OP_TARGET:
            return mapTargetType;
        case OP_MOVE:
            return mapMoveType;
        default:
            return empty;
        }
    }

    int getOpTypeByName(int select, std::string name) {
        std::map<std::string, int> &object = opTypeName(select);
        if(object.empty()) return -1;
        return object[name];

    }
    std::string  getOpTypeByValue(int select, int n)
    {
        std::map<std::string, int> &object = opTypeName(select);
        auto it = std::find_if(object.begin(),object.end(),
                                  [n]( const std::pair<std::string, int> & t){
            return (t.second==n);
        });
        if(it!=object.end())
        {
            return it->first;
        }
        else return std::string("");
    }

public:
    void setTargetType(std::string name)
    {
        int val = getOpTypeByName(OP_TARGET, name);
        if(val>0)  targetType = val;
        else targetType=-1;
    }
    std::string getTargetType()
    {
        return getOpTypeByValue(OP_TARGET, targetType);
    }

    void setMoveType(std::string name)
    {
        int val = getOpTypeByName(OP_MOVE, name);
        if(val>0)  moveType = val;
        else moveType=-1;
    }
    std::string getMoveType()
    {
        return getOpTypeByValue(OP_MOVE, moveType);
    }


public:
    MotionParam(){ }
    MotionParam( MotionParam &other)
    {
        strcpy(name, other.name);
        strcpy(desc, other.desc);
        memcpy(posTargetEE, other.posTargetEE, sizeof(double)*4);
        moveType = other.moveType;
        targetType = other.targetType;
        type = other.type;
    }
    ~MotionParam(){}
    MotionParam & operator = (MotionParam & other)
    {
        // std::cout<<"1. called MotionParam Base : operator=()"<<endl;
        strcpy(name, other.name);
        strcpy(desc, other.desc);
        memcpy(posTargetEE, other.posTargetEE, sizeof(double)*4);
        moveType = other.moveType;
        targetType = other.targetType;
        type = other.type;
        return *this;
    }
};
////////////////////////////////////////////////////////////////
// Define Static and function;
//const char *MotionParam::destTypeName[2]={"TARGET","GATE"};
//const char *MotionParam::moveTypeName[2]={"JOINT","LINEAR"};
//const char *MotionParam::getDestTypeName(int type){return destTypeName[type];}
//const char *MotionParam::getMoveTypeName(int type){return moveTypeName[type];}
/////////////////////////////////////////////////////////////////////////////////////
class FoldArmParam: public MotionParam
{
public:
    // 1. rot[0]: Arm#1 rotation[deg]
    // 2. rot[1]: Arm#2 rotation[deg]
    double rot[2]={0.0};// Degree
public:
    FoldArmParam(){}
    FoldArmParam( FoldArmParam &other):MotionParam(other)
    {
        memcpy(rot, other.rot, sizeof(double)*2);
    }
    ~FoldArmParam(){}
    FoldArmParam & operator = (FoldArmParam & other)
    {
        // std::cout<<"2.called FoldArmParam : operator=()"<<endl;
        MotionParam::operator=(other);
        memcpy(rot, other.rot, sizeof(double)*2);
        return *this;
    }
};
class PlaceTargetParam: public MotionParam
{
public:
    char targetName[128];
    double metric[4]={0.0};
    // 1. metric[0]: Above from Target[mm]
    // 2. metric[1]: Below from Target[mm]
    // 3. metric[2]: Extracted from Target[mm]
    // 4. metric[3]: Speed[mm/sec]

public:
    PlaceTargetParam(){}
    PlaceTargetParam( PlaceTargetParam &other):MotionParam(other)
    {
        memcpy(metric, other.metric, sizeof(double)*4);
    }
    ~PlaceTargetParam(){}
    PlaceTargetParam & operator = (PlaceTargetParam & other)
    {
        // std::cout<<"2.called FoldArmParam : operator=()"<<endl;
        MotionParam::operator=(other);
        memcpy(metric, other.metric, sizeof(double)*4);
        return *this;
    }
};

typedef struct _TRAJECTORY_TIME {
    double deltaT;
    double transitT;
    double finalT;
    double velocity;
    int operationModeNum;
    int currentOperationModeNum;
}trajectoryTime;
typedef struct CAMVIEWER {
    double clipping[2];
    double focalPoint[3];
    double pos[3];
    double viewUP[3];
    double viewAng;
} CamViewer;
typedef struct OBJECT_TEMPLATE {
    char const * name;
    // char *desc;
    double color[3];
    //    double pos[3];
    //    vtkActor* actor;
} ObjTemplate;
///////////////////////////////////////////////
class MapObjActor {
public:
    MotionParam param;
    //    char  name[56];
    //    char desc[128];
    //    double pos[4]={0,0,0.0};//EEx,EEy,EEz,EEth;
    double color[3]={0,0,0};
    vtkActor* actor=nullptr;
    vtkRenderer *_renderer=nullptr;
public:
    MapObjActor()
    {
        // cout<< "+++ MAPOBJACTOR constructor"<<endl;
    }

    ~MapObjActor()
    {

    }
    MapObjActor(MapObjActor & obj)
    {
        param=obj.param;
        //        strcpy(name, obj.name);
        //        strcpy(desc, obj.desc);
        //        memcpy(pos, obj.pos, sizeof(double)*4);
        memcpy(color, obj.color, sizeof(double)*3);

        actor  = vtkActor::New();
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();
        vtkPolyData* pd = vtkPolyData::SafeDownCast(obj.actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        actor->SetMapper(mapper);
        actor->RotateZ(180);
        actor->SetProperty(obj.actor->GetProperty());
        actor->SetUserTransform(obj.actor->GetUserTransform());
        _renderer = obj._renderer;
        _renderer->AddActor(actor);
    }
    MapObjActor & operator=(MapObjActor & obj)
    {
        param=obj.param;
        //        strcpy(name, obj.name);
        //        strcpy(desc, obj.desc);
        //        memcpy(pos, obj.pos, sizeof(double)*4);
        memcpy(color, obj.color, sizeof(double)*3);

        actor  = vtkActor::New();
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();
        vtkPolyData* pd = vtkPolyData::SafeDownCast(obj.actor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        actor->SetMapper(mapper);
        actor->RotateZ(180);
        actor->SetProperty(obj.actor->GetProperty());
        actor->SetUserTransform(obj.actor->GetUserTransform());
        _renderer = obj._renderer;
        _renderer->AddActor(actor);
        return *this;
    }
//    void setName(std::string n, std::string d)
//    {
//        if(n.size()>0) strcpy(this->param.name, n.c_str());
//        if(n.size()>0) strcpy(this->param.desc, d.c_str());
//    }
    void setData(std::string val[])
    {
         strcpy(this->param.name, val[0].c_str());
        strcpy(this->param.desc, val[1].c_str());
        if(val[2].size()>0) this->param.setTargetType(val[2]);
        if(val[3].size()>0) this->param.setMoveType(val[3]);
        setColorObj();
    }
    void copyDataFromArray(ObjTemplate &obj)
    {
        strcpy(param.name, obj.name);
        memcpy(color, obj.color, sizeof(double)*3);
    }
    void addActor(vtkTransform *T, vtkActor *otherActor, vtkRenderer *renderer)
    {
        // memcpy(pos, obj.pos, sizeof(double)*3);
        _renderer = renderer;
        actor  = vtkActor::New();
        vtkSmartPointer<vtkPolyData> copy = vtkSmartPointer<vtkPolyData>::New();
        vtkPolyData* pd = vtkPolyData::SafeDownCast(otherActor->GetMapper()->GetInput());
        copy->DeepCopy(pd);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(copy);
        mapper->ScalarVisibilityOff();
        actor->SetMapper(mapper);
        actor->RotateZ(180);

        vtkSmartPointer<vtkProperty> property = vtkSmartPointer<vtkProperty>::New();
        property->DeepCopy(otherActor->GetProperty()); // dactor->GetProperty()
        actor->SetProperty(property);
        //cout<<"param.getTargetType():"<<param.getTargetType()<<endl;
        setColorObj();
        actor->SetUserTransform(T);
        _renderer->AddActor(actor);
    }
    void removeActor()
    {   std::cout<<"MapObjActor::removed actor!"<<endl;
        if(!_renderer) return;
        if(actor) _renderer->RemoveActor(actor);
        actor=nullptr;
    }
    void setColorObj()
    {
        if(!actor) return;
        if(param.getTargetType()=="Target"){
            actor->SetScale(1,1,1);
            actor->GetProperty()->SetColor(1,1,1);
        }
        else if(param.getTargetType()=="APPROACH"){
            actor->SetScale(0.5,1,0.5);
            actor->GetProperty()->SetColor(0.,0.,0.3);
        }
    }

} ;
//////////////////////////////////////////////////////
///
typedef struct _GLOBAL_DATA_COLLECTION {
    double prevJointConfigure[NUMJNT];
    double jointConfigure[NUMJNT];
    double curJointPos[NUMJNT];
    double curEEPos[NUMJNT];// EEx, EEy, EEz, EEth
    trajectoryTime trTime;

}globalSharedData;

//////////////////////////////
// Zoe Robot Kinematics Parameters
/*
typedef struct
{
    double th1, th2; // joint angle
    double Px,Py, Pz;      // x,y,z(Elevation) position of robot

    double EEx, EEy,EEz;  // x,y, z position of End-Effector
    double EEth; // tip angle of End-Effector

} KIN_PARAM;
*/
//////////////////////////////////////////////////////////////
typedef struct _CIRCLE_PACKET_
{
    char packet[128];
    uint8_t CMD;
    uint8_t jobID;
//    double scale = 100.0;
    double speed=0;
    double radius=0;
    double cenPosX=0, cenPosY=0;
    double EETheta=0;
    double arcAng=0;
//    uint8_t numRepeat=1;
//    char* pack() {
//        sprintf(packet, "G%d S%ld R%ld X%ld Y%ld T%ld A%ld J%d;",
//                CMD,
//                lround(scale*speed),
//                lround(scale*radius),
//                lround(scale*cenPosX),
//                lround(scale*cenPosY),
//                lround(scale*EETheta),// [deg]
//                lround(scale*arcAng),//[deg]
//                jobID
//                );
//    }

}CircleProfile;
//////////////////////////////////////////////////////////////
typedef struct _SPIRAL_PACKET_
{
    char packet[128];
    uint8_t CMD;
    uint8_t jobID;
    double speed=0;
    double radius=0;
    double cenPosX=0, cenPosY=0;
    double EETheta=0;
    double arcAng=0;
    double posZ=0;
    double heightZ=0;
}SpiralProfile;
// ... storing I.K motion for timer

typedef struct
{
  // ...OUTPUT...
  uint8_t jobID;
  uint8_t result;//
  unsigned long steps;   // total steps(pluse)
  unsigned long Na;      // pulse counts for acceleration area
  unsigned long Nac;     // pulse counts for acceleration and constant areas
  unsigned long NNb;     // pulse counts for deceleration area
  unsigned long Cn_acc0; // fisrt period time for 1st pulse of acceleration on Timer.
  int8_t dir;       // step motor direction 1:CCW, -1:CC
} SpeedProfile;// 28bytes
#endif // MKGLOBALDATA_H
