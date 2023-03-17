#ifndef MKTOPCAMERA_H
#define MKTOPCAMERA_H

#include <QObject>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMath.h>
#include <vtkNamedColors.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>

#include "mainwindow.h"
#include "jntctrldialog.h"
#include "vtk_headers.h"
#include "renderrobotarm.h"
#include "../sharedfiles/mkRobotKin.h"
#include "qmkjointeditvlayout.h"
extern MKZoeRobotKin robotKin;
extern int winSize[2];
extern int viewMode;
extern CamViewer camView[10];
extern MainWindow *g_MainWindow;
class mkTopCamera : public vtkInteractorStyleTrackballCamera
{
private:
  vtkActor* LastPickedActor=nullptr;
  //vtkActor2D* LastPickedActor2D=nullptr;
  vtkProperty* LastPickedProperty;//= vtkProperty::New();

    // vtkInteractorStyle interface
private:
  bool isCupSelected=false, isLink1Selected=false, isLink2Selected=false;
  bool isRotArrowSelected=false, isMoveXSelected=false;
  double pre_rad=0;
public:
   vtkRenderer* renderer;
   CRenderRobotArm *myRobotRenderer=nullptr;
   CRenderRobotArm *myRobotRenderer2=nullptr;
   double clickPosStart[3];
   double* worldPos;
   int rev=0;
//   double curInput[3];
//   double curXLimit[2]={0};
//   double prevPosX=0;
//   double validEE[3];// Lx,EEx,EEy
   MKZoeRobotKin startRobotKin;

public:
  static mkTopCamera* New();
  vtkTypeMacro(mkTopCamera, vtkInteractorStyleTrackballCamera)

  mkTopCamera()
  {
    LastPickedActor = nullptr;
    LastPickedProperty = vtkProperty::New();
  }
  virtual ~mkTopCamera() override;
  void setRobotRenderer(void * robotRen, void * robotRen2)
  {
      myRobotRenderer = static_cast<CRenderRobotArm*>(robotRen);
      myRobotRenderer2 = static_cast<CRenderRobotArm*>(robotRen2);

  }


  virtual void OnLeftButtonDown() override
  {

    auto colors =   vtkSmartPointer<vtkNamedColors>::New();
    int* clickPos = this->GetInteractor()->GetEventPosition();
    // Pick from this location.
    auto picker = vtkSmartPointer<vtkPropPicker>::New();
    vtkPropCollection *pC = picker->GetPickList();


    picker->Pick(clickPos[0],  clickPos[1], 0,  renderer);
    this->LastPickedActor = picker->GetActor();
    if(this->LastPickedActor )
    {
       // cout << "Top View: Pressed left mouse button." << std::endl;
        int x = this->Interactor->GetEventPosition()[0];
        int y = this->Interactor->GetEventPosition()[1];
        vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();
        coordinate->SetCoordinateSystemToDisplay();
        coordinate->SetValue(x,y,0);
        worldPos = coordinate->GetComputedWorldValue(myRobotRenderer->_renderWindow->GetRenderers()->GetFirstRenderer());
        //std::cout << "World coordinate: " << worldPos[0] << ", " << worldPos[1] << ", " << worldPos[2] << std::endl;
        memcpy(clickPosStart, worldPos,sizeof(double)*3);

       // std::cout << this->LastPickedActor<<", Pick: " << clickPos[0] << ", " << clickPos[1] << ", "<<clickPos[2] << std::endl;
        this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
        rev=0;
        startRobotKin = robotKin;
        if(myRobotRenderer->objControlArrow[0].actor==LastPickedActor)
        {
            isMoveXSelected=true;

           // cout<<"++++++++++++++++++++++ Mouse Down ++++++++++++++++++++++"<<endl;
            //startRobotKin.printForKin();
           // cout<<"++++++++++++++++++++++ End Mouse Down ++++++++++++++++++++++"<<endl;
//            robotKin.bkParam = robotKin.param;
//            prevPosX=robotKin.curX;
        }
        else if(myRobotRenderer->objLink1[0].actor==LastPickedActor||myRobotRenderer->objLink1[1].actor==LastPickedActor)
        {

            isLink1Selected=true;
            robotKin.bkParam = robotKin.param;
        }
        else if(myRobotRenderer->objLink2[0].actor==LastPickedActor)
        {
            isLink2Selected=true;
            robotKin.bkParam = robotKin.param;
        }
        else if(myRobotRenderer->objCupEE[1].actor==this->LastPickedActor)
        {   // Select Cup
            isCupSelected=true;
            this->LastPickedActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
            robotKin.bkParam = robotKin.param;
            myRobotRenderer->_renderWindow->Render();
        }

        else if(myRobotRenderer->objControlArrow[3].actor==LastPickedActor)
        {   // Selcect TargetArrow to rotate robot about center of cup...
            isRotArrowSelected=true;
            robotKin.bkParam = robotKin.param;
        }
    }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

  }// ---------------- OnLeftButtonDown() -----------------------

  virtual void OnMouseMove() override
  {

         // std::cout << "Moveing left mouse button." << std::endl;
          int x = this->Interactor->GetEventPosition()[0];
          int y = this->Interactor->GetEventPosition()[1];
         // std::cout << "(x,y) = (" << x << "," << y << ")" << std::endl;
          vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();
          coordinate->SetCoordinateSystemToDisplay();
          coordinate->SetValue(x,y,0);

          // This doesn't produce the right value if the sphere is zoomed in???
          worldPos = coordinate->GetComputedWorldValue(myRobotRenderer->_renderWindow->GetRenderers()->GetFirstRenderer());
          if(isMoveXSelected)
          {
              double posX=robotKin.curX + worldPos[0]-clickPosStart[0];
              MKZoeRobotKin testRobotKin(robotKin);

               // cout<<"Before- Lx= "<<posX<<";, EEx= "<<robotKin.param.EEx<<";, EEy= "<<robotKin.param.EEy<<";, EEz= "<<testRobotKin.param.EEz<<";, th1= "<<robotKin.param.th1<<";, th2= "<<robotKin.param.th2<<";"<<endl;
                bool rec = testRobotKin.invKinMoveX(posX);
               // cout<<"After-response test    : "<<rec<<", EEx= "<<testRobotKin.param.EEx<<", EEy= "<<testRobotKin.param.EEy<<";, EEz= "<<testRobotKin.param.EEz<<", th1= "<<testRobotKin.param.th1<<", th2= "<<testRobotKin.param.th2<<endl;

              if( abs(startRobotKin.param.EEx-testRobotKin.param.EEx)<1.E-5 &&  abs(startRobotKin.param.EEy-testRobotKin.param.EEy)<1.E-5)
              {
                rev+=rec;
                //cout<<"EE position is same. rec="<<rec<<endl;
                if(rec){ // There is Solution and EE position remains at the same position...
                  //  cout<<"1. robotKin: pz="<<robotKin.param.Pz<<", "<<testRobotKin.param.Pz<<", EEz="<<robotKin.param.EEz<<", "<<testRobotKin.param.EEz<<endl;
                    robotKin = testRobotKin;
                   // cout<<"2. robotKin: pz="<<robotKin.param.Pz<<", "<<testRobotKin.param.Pz<<", EEz="<<robotKin.param.EEz<<", "<<testRobotKin.param.EEz<<endl;
                }
                else {

                     cout<<"No solution: posX="<<posX<<endl;
                }
                //g_MainWindow->jointCtrlDialogBox->updateSilder();
                g_MainWindow->JointEditVLayout->updateSilder();
                myRobotRenderer->updateZoeRobotConfig();
                myRobotRenderer->_renderWindow->Render();

                myRobotRenderer2->updateZoeRobotConfig();
                myRobotRenderer2->_renderWindow->Render();
              }
              else
              {
                  cout<<"EE position is NOT a same."<<endl;

              }
             // cout<<"------------------------------------------------------------------------------------"<<endl;
          }
          else if(isLink1Selected)
          {
              double posRJ1[3]={robotKin.offDisX+robotKin.T0[0].m_ppData[0][3],
                                robotKin.offDisY+robotKin.T0[0].m_ppData[1][3],
                                robotKin.offDisZ+robotKin.T0[0].m_ppData[2][3]};

              double vecA[2]={posRJ1[0]-clickPosStart[0], posRJ1[1]-clickPosStart[1]};
              double vecB[2]={posRJ1[0]-worldPos[0],      posRJ1[1]-worldPos[1]};
              double rad1 = atan2(vecA[0]*vecB[1] - vecA[1]*vecB[0], vecA[0]*vecB[0] + vecA[1]*vecB[1]);
              /////////////////////////////////////////////////////
              // Prevent from exeeding +-180 degree by rotation
              double totalRad1 = robotKin.curR1+rad1;

              double a1 = totalRad1*RTOD;
              double reg1 = a1-int(a1);
              int d1= int(a1)%360;
              totalRad1 = (d1+reg1)*DTOR;
             // cout<<"511.149="<<(d1+reg1)<<endl;
              if(abs(totalRad1)>=M_PI)
              {
                  if(totalRad1<0)
                    totalRad1 +=2*M_PI;
                  else if(totalRad1>0)
                    totalRad1 -=2*M_PI;
              }


              /////////////////////////////////////////////////////

              double pos[4]={robotKin.curX, robotKin.curZ, totalRad1, robotKin.curR2};

              robotKin.forKin(pos[0],pos[1],pos[2],pos[3]);
             // cout<<"Rot1: "<<rad1*RTOD<<", total Rot1: "<<totalRad1*RTOD<< ", Kin.th1: "<<robotKin.param.th1*RTOD<<endl;
              //g_MainWindow->jointCtrlDialogBox->updateSilder();
              g_MainWindow->JointEditVLayout->updateSilder();
              myRobotRenderer->_renderWindow->Render();

              myRobotRenderer->updateZoeRobotConfig();
              myRobotRenderer->_renderWindow->Render();
              myRobotRenderer2->updateZoeRobotConfig();//(pos[0],pos[1],pos[2],pos[3]);
              myRobotRenderer2->_renderWindow->Render();
          }
          else if(isLink2Selected)
          {
              double posRJ2[3]={robotKin.offDisX+robotKin.T0[1].m_ppData[0][3],
                                robotKin.offDisY+robotKin.T0[1].m_ppData[1][3],
                                robotKin.offDisZ+robotKin.T0[1].m_ppData[2][3]};

              double vecA[2]={posRJ2[0]-clickPosStart[0], posRJ2[1]-clickPosStart[1]};
              double vecB[2]={posRJ2[0]-worldPos[0],      posRJ2[1]-worldPos[1]};
              double rad2 = atan2(vecA[0]*vecB[1] - vecA[1]*vecB[0], vecA[0]*vecB[0] + vecA[1]*vecB[1]);//+-180

              /////////////////////////////////////////////////////
              // Prevent from exeeding +-180 degree by rotation
              double x1 = robotKin.T0[0].m_ppData[0][3]+robotKin.offDisX;
              double y1 = robotKin.T0[0].m_ppData[1][3]+robotKin.offDisY;

              double x2 = robotKin.T0[1].m_ppData[0][3]+robotKin.offDisX;
              double y2 = robotKin.T0[1].m_ppData[1][3]+robotKin.offDisY;

              double v1[2] = {x2-x1, y2-y1} ;  //# Vector 1
              double v2[2] = {worldPos[0]-x1, worldPos[1]-y1} ; // # Vector 1
              double xp =v1[0]*v2[1] - v1[1]*v2[0];//  # Cross product


              double tatalRad2 = robotKin.curR2+rad2;

              double a2 = tatalRad2*RTOD;
              double reg2 = a2-int(a2);
              int d2= int(a2)%360;
              tatalRad2 = (d2+reg2)*DTOR;

              if(abs(tatalRad2)>10.0*DTOR)
              {
                  if(xp>0)
                  {
                      if(tatalRad2<0)
                          tatalRad2 +=2*M_PI;
                  }
                  else if(xp<0)
                  {
                      if(tatalRad2>0)
                          tatalRad2 -=2*M_PI;
                  }
              }
              /////////////////////////////////////////////////////

               double pos[4]={robotKin.curX, robotKin.curZ, robotKin.curR1 ,  tatalRad2};
               robotKin.forKin(pos[0],pos[1],pos[2],pos[3]);
              // cout<<"Rot2: "<<rad2*RTOD<<", total Rot2: "<<tatalRad2*RTOD<< ", Kin.th2: "<<robotKin.param.th2*RTOD<<endl;
               //g_MainWindow->jointCtrlDialogBox->updateSilder();
               g_MainWindow->JointEditVLayout->updateSilder();
               myRobotRenderer->updateZoeRobotConfig();
               myRobotRenderer->_renderWindow->Render();

               myRobotRenderer2->updateZoeRobotConfig();
               myRobotRenderer2->_renderWindow->Render();

          }
          else if(isCupSelected)
          {
              double pos[2]={robotKin.curEEx + worldPos[0]-clickPosStart[0],
                             robotKin.curEEy + worldPos[1]-clickPosStart[1]};

               MKZoeRobotKin testRobotKin(robotKin);

                bool rec = testRobotKin.invKinEE_W_XYR(pos[0],pos[1], robotKin.curEEth);
                if(rec>0){
                    robotKin = testRobotKin;
                }
                else {
                    cout<<"No solution: CupSelected"<<endl;
                }

                myRobotRenderer->updateZoeRobotConfig();
                myRobotRenderer->_renderWindow->Render();

                myRobotRenderer2->updateZoeRobotConfig();
                myRobotRenderer2->_renderWindow->Render();
                //g_MainWindow->jointCtrlDialogBox->updateSilder();
                g_MainWindow->JointEditVLayout->updateSilder();


          }

          else if(isRotArrowSelected)
          {
              double posREE[3]={robotKin.offDisX+robotKin.T0[2].m_ppData[0][3],
                                robotKin.offDisY+robotKin.T0[2].m_ppData[1][3],
                                robotKin.offDisZ+robotKin.T0[2].m_ppData[2][3]};

              double vecA[2]={posREE[0]-clickPosStart[0], posREE[1]-clickPosStart[1]};
              double vecB[2]={posREE[0]-worldPos[0],      posREE[1]-worldPos[1]};
              double rad = atan2(vecA[0]*vecB[1] - vecA[1]*vecB[0], vecA[0]*vecB[0] + vecA[1]*vecB[1]);//+-180

              /////////////////////////////////////////////////////
              // Prevent from exceeding +-180 degree by rotation
              // Check if the mouse point is above or under line composed of JR4 and center of cup
              double x1 = robotKin.T0[1].m_ppData[0][3]+robotKin.offDisX;
              double y1 = robotKin.T0[1].m_ppData[1][3]+robotKin.offDisY;

              double x2 = robotKin.T0[2].m_ppData[0][3]+robotKin.offDisX;
              double y2 = robotKin.T0[2].m_ppData[1][3]+robotKin.offDisY;

              double v1[2] = {x2-x1, y2-y1} ;  //# Vector 1
              double v2[2] = {worldPos[0]-x1, worldPos[1]-y1} ; // # Vector 1
              double xp =v1[0]*v2[1] - v1[1]*v2[0];//  # Cross product


              double totalRad = robotKin.curEEth+rad;

              //cout<<"totalRad-before1:"<<totalRad*RTOD<<endl;

              // +++ If total angle is over 360 then modulate by 360...
              if(abs(totalRad)>=PI2)
              {
                  double a2 = totalRad*RTOD;
                  double reg2 = a2-int(a2);
                  int d2= int(a2)%360;
                  totalRad = (d2+reg2)*DTOR;
              }
              // +++ If total angle is greater than 180 then modify it under 180...
              if(abs(totalRad)>=M_PI)
              {
                  if(totalRad<0) totalRad +=PI2;
                  else totalRad = PI2-totalRad;
              }

              /////////////////////////////////////////////////////
               MKZoeRobotKin testRobotKin(robotKin);
               int rec = testRobotKin.invKinEE_W_R(totalRad);
              // cout<<rev<<", EE:R "<<rad*RTOD<<", tRot: "<<tatalRad<< ", EEx: "<<robotKin.param.EEx<< ", EEy: "<<robotKin.param.EEy<< ", EEth: "<<robotKin.param.EEth<<", th1: "<<robotKin.param.th1<<endl;
               if(rec>0){
                    robotKin = testRobotKin;
               }
               else {
                   cout<<"No solution: RotArrowSelected"<<endl;
               }


                   myRobotRenderer->updateZoeRobotConfig();
                   myRobotRenderer->_renderWindow->Render();

                   myRobotRenderer2->updateZoeRobotConfig();
                   myRobotRenderer2->_renderWindow->Render();
                   //g_MainWindow->jointCtrlDialogBox->updateSilder();
                   g_MainWindow->JointEditVLayout->updateSilder();


          }
      vtkInteractorStyleTrackballCamera::OnMouseMove();
  }// ----------- OnMouseMove() -------------------


  virtual void OnLeftButtonUp() override
  {
      int x = this->Interactor->GetEventPosition()[0];
      int y = this->Interactor->GetEventPosition()[1];
      vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();
      coordinate->SetCoordinateSystemToDisplay();
      coordinate->SetValue(x,y,0);
      //This doesn't produce the right value if the sphere is zoomed in???
      worldPos = coordinate->GetComputedWorldValue(myRobotRenderer->_renderWindow->GetRenderers()->GetFirstRenderer());
        pre_rad=0;

        if(isLink1Selected)
        {
            robotKin.updateCurrentJoint( );
        }
        else if(isLink2Selected)
        {
             robotKin.updateCurrentJoint( );
        }
        else if(isCupSelected)
        {
            if (this->LastPickedActor )
            {
              this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
            }
            robotKin.updateCurrentJoint();
        }
        else if(isRotArrowSelected )
        {
            robotKin.updateCurrentJoint();
        }
        else if(isMoveXSelected)
        {
            robotKin.updateCurrentJoint();
        }
        cout<<"MOUSE UP:: Lx="<<robotKin.param.Px<<", EEx="<<robotKin.param.EEx<<", EEy="<<robotKin.param.EEy
            <<", th1="<<robotKin.param.th1
            <<", th2="<<robotKin.param.th2
            <<endl;
        isCupSelected=false;
        isLink1Selected=false;
        isLink2Selected=false;
        isRotArrowSelected=false;
        isMoveXSelected=false;
        myRobotRenderer->updateZoeRobotConfig();
        //myRobotRenderer->addTarget();
        myRobotRenderer->_renderWindow->Render();

        myRobotRenderer2->updateZoeRobotConfig();
        //myRobotRenderer2->addTarget();
        myRobotRenderer2->_renderWindow->Render();
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();

        vtkCamera *cameraTop = renderer->GetActiveCamera();
        cout<<"-------------- Top View Camera Information -----------";
         cameraTop->Print(std::cout);

  }// ---------------- OnLeftButtonUp() -----------------------


  void addRenderer(vtkRenderer* ren)
  {
      renderer=ren;
  }


public:
    virtual void Rotate() override {}
    virtual  void Spin() override{}

    //virtual  void Pan() override{}
//    virtual  void Dolly() override{}


    virtual     void OnMouseWheelForward() override{}
    virtual     void OnMouseWheelBackward() override{}

public:


  // vtkInteractorStyle interface
public:

};

#endif // MKCAMERA_H


