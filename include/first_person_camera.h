#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "camera.h"

class FirstPersonCamera : public Camera
{
    public:
        FirstPersonCamera(float aspect_ratio,float fov,float z_near,float z_far);
        ~FirstPersonCamera();

        void update();

        void controls();
};

#endif
