
#include "mkTopCamera.h"
vtkStandardNewMacro(mkTopCamera);
mkTopCamera::~mkTopCamera()
{
    LastPickedProperty->Delete();
}



