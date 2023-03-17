#ifndef MKFRONTCAMERA_H
#define MKFRONTCAMERA_H

#include <QObject>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMath.h>
#include <vtkNamedColors.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkTransform.h>
#include <vtkProperty.h>
#include "mainwindow.h"
#include "jntctrldialog.h"
#include "vtk_headers.h"
#include "renderrobotarm.h"
#include "../sharedfiles/mkRobotKin.h"
#include "qmkjointeditvlayout.h"
#include "qmktargeteditvlayout.h"

extern MKZoeRobotKin robotKin;
extern int winSize[2];
extern int viewMode;
extern CamViewer camView[10];
extern MainWindow *g_MainWindow;
class mkFrontCamera : public vtkInteractorStyleTrackballCamera
{
private:
  vtkActor* LastPickedActor;//=nullptr;
  vtkProperty* LastPickedProperty;//= vtkProperty::New();

    // vtkInteractorStyle interface
private:
  bool isCupSelected=false;
  bool isMoveXSelected=false;
  bool isMoveZSelected=false;
  bool isKeyShiftPressed=false;
  bool isKeyAltPressed=false;
  bool isLeftMouseDown=false;
  bool isRightMouseDown=false;
public:
   vtkRenderer* renderer;
   CRenderRobotArm *myRobotRenderer=nullptr;
   CRenderRobotArm *myRobotRenderer2=nullptr;
   double clickPosStart[3], cupPosEnd[3];
   double mousePos1[2], mousePos2[2];// Save mouse position on display
   double* worldPos;
   vtkNew<vtkCoordinate> coordinate ;


public:
  static mkFrontCamera* New();
  vtkTypeMacro(mkFrontCamera, vtkInteractorStyleTrackballCamera)

  mkFrontCamera()
  {
    LastPickedActor = nullptr;
    LastPickedProperty = vtkProperty::New();
  }
  virtual ~mkFrontCamera() override;
  void setRobotRenderer(void * robotRen, void * robotRen2)
  {
      myRobotRenderer = static_cast<CRenderRobotArm*>(robotRen);
      myRobotRenderer2 = static_cast<CRenderRobotArm*>(robotRen2);
  }


  virtual void OnLeftButtonDown() override
  {
//      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
//      return;
    isLeftMouseDown=true;
    if(isKeyAltPressed) {
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        return;
    }
//    if(isKeyShiftPressed) {
//        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
//        return;
//    }
    auto colors =   vtkSmartPointer<vtkNamedColors>::New();
    int* clickPos = this->GetInteractor()->GetEventPosition();
    // Pick from this location.
    auto picker = vtkSmartPointer<vtkPropPicker>::New();
    picker->Pick(clickPos[0], clickPos[1], clickPos[2]*0, renderer);

    this->LastPickedActor = picker->GetActor();
    if(this->LastPickedActor )
    {

        std::cout <<endl<<"Pressed left mouse button." << std::endl;

        int x = this->Interactor->GetEventPosition()[0];
        int y = this->Interactor->GetEventPosition()[1];
        mousePos1[0]=x; mousePos1[1]=y;
        std::cout << "-------- Start(x,y) = (" << x << "," << y << "), " ;
        vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();
        coordinate->SetCoordinateSystemToDisplay();
        coordinate->SetValue(x,y,0);

        // This doesn't produce the right value if the sphere is zoomed in???
        worldPos = coordinate->GetComputedWorldValue(myRobotRenderer->_renderer);
       // worldPos = coordinate->GetComputedValue(myRobotRenderer->_renderer);
       // std::cout << "World coordinate: " << worldPos[0] << ", " << worldPos[1] << ", " << worldPos[2] << std::endl;
        memcpy(clickPosStart, worldPos,sizeof(double)*3);

        //////////////////////////////////////////////////////////////
        // When Cup hides the target then get Target actor first...
        if(myRobotRenderer->objCupEE[1].actor==this->LastPickedActor)
        {

            myRobotRenderer->objCupEE[1].actor->SetPickable(false);
            auto picker_target = vtkSmartPointer<vtkPropPicker>::New();
            picker_target->Pick(clickPos[0], clickPos[1], clickPos[2]*0, renderer);
            vtkActor *cupActor = picker_target->GetActor();
            for(MapObjActor *objTarget: g_MainWindow->renderRobotFront->objTarget)
            {
                if(objTarget->actor==cupActor)
                {
                    cout<<"I found Target:"<<objTarget->param.name<<endl;
                    g_MainWindow->TargetEditVLayout->setSelectionTarget(objTarget);
                    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
                    myRobotRenderer->objCupEE[1].actor->SetPickable(true);
                    return;
                }
            }
            myRobotRenderer->objCupEE[1].actor->SetPickable(true);
        }
        // -----------------------------------------------------------
        //////////////////////////////////////////////////////////
        this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());


        for(MapObjActor *objTarget: g_MainWindow->renderRobotFront->objTarget)
        {
            if(objTarget->actor==this->LastPickedActor)
            {
                cout<<"I found Target:"<<objTarget->param.name<<endl;
                g_MainWindow->TargetEditVLayout->setSelectionTarget(objTarget);
                vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

                return;
            }
        }
        myRobotRenderer->objCupEE[0].actor->SetPickable(true);
        if(myRobotRenderer->objControlArrow[1].actor==this->LastPickedActor)
        {
            cout<<"Clicked MoveX"<<endl;
            this->LastPickedActor->GetProperty()->SetOpacity(1);
            isMoveXSelected=true;
        }
        else if(myRobotRenderer->objControlArrow[2].actor==this->LastPickedActor)
        {
            cout<<"Clicked Arrow_MoveZ"<<endl;
            this->LastPickedActor->GetProperty()->SetOpacity(1);
            isMoveZSelected=true;
        }
        else if(myRobotRenderer->objCupEE[1].actor==this->LastPickedActor || myRobotRenderer->objCupEE[0].actor==this->LastPickedActor)
        {
             cout<<"Clicked Cup"<<endl;
            this->LastPickedActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
            myRobotRenderer->_renderWindow->Render();
            isCupSelected=true;
        }
//        else
//        {
//            for(MapObjActor *objTarget: g_MainWindow->renderRobotFront->objTarget)
//            {
//                if(objTarget->actor==this->LastPickedActor)
//                {
//                    cout<<"I found Target:"<<objTarget->param.name<<endl;
//                    g_MainWindow->TargetEditVLayout->setSelectionTarget(objTarget);
//                    break;
//                }
//            }
//        }
    }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

  }// ---------------- OnLeftButtonDown() -----------------------

  virtual void OnMouseMove() override
  {
//vtkInteractorStyleTrackballCamera::OnMouseMove();
//return;
      if( isLeftMouseDown && isKeyAltPressed)
      {
      //    cout<<"rotation on mouse move"<<endl;
          vtkInteractorStyleTrackballCamera::Rotate();
//          this->Rotate();
          return;
      }
//      else if( isLeftMouseDown && isKeyShiftPressed)
//      {
//         // cout<<"zoom"<<endl;
//          vtkInteractorStyleTrackballCamera::Pan();
//          return;
//      }

        // std::cout << "Moveing left mouse button." << std::endl;
        int x = this->Interactor->GetEventPosition()[0];
        int y = this->Interactor->GetEventPosition()[1];
        mousePos2[0]=x; mousePos2[1]=y;

        coordinate->SetCoordinateSystemToDisplay();
        coordinate->SetValue(x,y,0);
       // coordinate->Print(std::cout);
        worldPos = coordinate->GetComputedWorldValue(myRobotRenderer->_renderWindow->GetRenderers()->GetFirstRenderer());
//        double *ldv = coordinate->GetComputedDoubleViewportValue(myRobotRenderer->_renderer);
//        cout<<"GetComputedLocalDisplayValue: "<<ldv[0]<<", "<<ldv[1]<<", "<<ldv[2]<<endl;
        double dx = x-mousePos1[0];
        double dy = y-mousePos1[1];
        double l = sqrt(dx*dx + dy*dy);
         vtkCamera *cameraFront = renderer->GetActiveCamera();
         double ang = cameraFront->GetViewAngle()*0.5;
         double w = sin(ang*DTOR)*cameraFront->GetDistance();
         double r = w/winSize[1];
        if(isMoveXSelected)
        {
            if(viewMode==0){// For Parallel Projection
                double pos[4]={worldPos[0]+robotKin.curX-clickPosStart[0],
                       robotKin.curZ,
                       robotKin.curR1,robotKin.curR2};
                robotKin.forKin(pos[0],pos[1],pos[2],pos[3]);
            }
            else {
                double pos[4]={robotKin.curX+dx*r,
                               robotKin.curZ,
                               robotKin.curR1,robotKin.curR2};
                robotKin.forKin(pos[0],pos[1],pos[2],pos[3]);
            }
           // g_MainWindow->jointCtrlDialogBox->updateSilder();
            g_MainWindow->JointEditVLayout->updateSilder();
            myRobotRenderer->updateZoeRobotConfig();
            myRobotRenderer->_renderWindow->Render();

            if(myRobotRenderer2) {
                myRobotRenderer2->updateZoeRobotConfig();
                myRobotRenderer2->_renderWindow->Render();
            }
        }
        else if(isMoveZSelected)
        {

//             cout << "Moving (x,y) = (" << x << "," << y << "), " ;
//             cout<<"dx,dy: "<<dx<<", "<<dy<<", r="<<r<<", distance:"<<cameraFront->GetDistance()<<", winSize:"<<winSize[0]<<", "<<winSize[1]<<endl;

             if(viewMode==0){// For Parallel Projection
                 //            cout<<"worldPos: "<<worldPos[0]-clickPosStart[0]<<", "<<worldPos[1]-clickPosStart[1]<<", "<<worldPos[2]-clickPosStart[2]<<", J1: "<<robotKin.curX<<endl;
                             double pos[4]={robotKin.curX,
                                    worldPos[2]+robotKin.curZ-clickPosStart[2],
                                    robotKin.curR1,robotKin.curR2};
             robotKin.forKin(pos[0],pos[1],pos[2],pos[3]);
             }
             else {// For Perspective Projection
                 double pos[4]={robotKin.curX,
                                dy*r+robotKin.curZ,
                                robotKin.curR1,robotKin.curR2};
                 robotKin.forKin(pos[0],pos[1],pos[2],pos[3]);
             }

            //g_MainWindow->jointCtrlDialogBox->updateSilder();
            g_MainWindow->JointEditVLayout->updateSilder();
            myRobotRenderer->updateZoeRobotConfig();
            myRobotRenderer->_renderWindow->Render();

            if(myRobotRenderer2) {
                myRobotRenderer2->updateZoeRobotConfig();
                myRobotRenderer2->_renderWindow->Render();
            }
           // robotKin.printForKin();
        }
        else if(isCupSelected)
        {
//            cout << "Moving (x,y) = (" << x << "," << y << "), " ;
//            cout<<"worldPos: "<<worldPos[0]<<", "<<worldPos[1]<<", "<<worldPos[2]<<", J1: "<<robotKin.curX<<endl;
             if(viewMode==0){// For Parallel Projection
                double pos[4]={worldPos[0]+robotKin.curX-clickPosStart[0],
                       worldPos[2]+robotKin.curZ-clickPosStart[2],
                       robotKin.curR1,robotKin.curR2};
                robotKin.forKin(pos[0],pos[1],pos[2],pos[3]);
             }
             else {
                 double pos[4]={
                        robotKin.curX+dx*r,
                        robotKin.curZ+dy*r,
                        robotKin.curR1,robotKin.curR2};
                 robotKin.forKin(pos[0],pos[1],pos[2],pos[3]);
             }
            //g_MainWindow->jointCtrlDialogBox->updateSilder();
            g_MainWindow->JointEditVLayout->updateSilder();
            myRobotRenderer->updateZoeRobotConfig();
            myRobotRenderer->_renderWindow->Render();

            if(myRobotRenderer2) {
                myRobotRenderer2->updateZoeRobotConfig();
                myRobotRenderer2->_renderWindow->Render();
            }
        }
        //
        vtkInteractorStyleTrackballCamera::OnMouseMove();
  }// ----------- OnMouseMove() -------------------


  virtual void OnLeftButtonUp() override
{
//    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
//    return;
    isLeftMouseDown=false;
    if(isMoveXSelected)
    {
        if (this->LastPickedActor)
        {
          this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
        }
        robotKin.updateCurrentJoint();
        isMoveXSelected=false;
    }
    else if(isMoveZSelected)
    {
        if (this->LastPickedActor)
        {
          this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
        }
        robotKin.updateCurrentJoint();
        isMoveZSelected=false;
    }
    else if(isCupSelected)
    {
        if (this->LastPickedActor)
        {
          this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
        }

        robotKin.updateCurrentJoint();//( worldPos[0]-clickPosStart[0], worldPos[2]-clickPosStart[2],0,0);

        isCupSelected=false;
    }

    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}// ---------------- OnLeftButtonUp() -----------------------


  void addRenderer(vtkRenderer* ren)
  {
      renderer=ren;
  }


public:
    virtual void Rotate() override
  {

      return;
      vtkCamera *cameraFront = renderer->GetActiveCamera();
      cameraFront->Print(std::cout);
      return;

//      //vtkMatrix4x4 * tf = cameraFront->GetEyeTransformMatrix();
//      //vtkMatrix4x4 * tf = cameraFront->GetProjectionTransformMatrix(renderer);
//      vtkMatrix4x4 * tf = cameraFront->GetViewTransformMatrix();
//      //cout<<"rotate"<<tf<<endl;
//      //vtkTransform * tf = cameraFront->GetModelViewTransformObject();
//      //vtkMatrix4x4 *mat = tf->GetMatrix();
//      //cout<<mat->GetElement(0,0)<<", "<<mat->GetElement(0,1)<<", "<<mat->GetElement(0,3)<<endl;
//      mkMatrix mm;
//      mm.SetData(4,4,tf->GetData());
//      printMat("mm",mm);

  //    vtkInteractorStyleTrackballCamera::Rotate();
  }

    virtual     void OnMouseWheelForward() override{}
    virtual     void OnMouseWheelBackward() override{}

public:
  virtual void OnKeyDown() override
  {
      // Get the keypress
    //vtkCamera *cameraFront = renderer->GetActiveCamera();
    vtkRenderWindowInteractor *rwi = this->Interactor;
    std::string key = rwi->GetKeySym();
    // Output the key that was pressed
    std::cout << "Pressed " << key << std::endl;
    //cameraFront->Print(std::cout);
    if(key=="Alt_L" && viewMode!=0 ){
       // std::cout << "  ->Pressed Shift Key" << key << std::endl;
        isKeyAltPressed=true;
        return;
    }


     //vtkInteractorStyleTrackballCamera::OnKeyDown();
  }
  virtual void OnKeyUp() override
  {
      //std::cout << " Key UP"  << std::endl;
      isKeyShiftPressed=false;
      isKeyAltPressed = false;
      vtkInteractorStyleTrackballCamera::OnKeyUp();
  }

};
#endif // MKCAMERA_H
