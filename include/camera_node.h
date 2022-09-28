#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include "Node.h"
#include "camera.h"

class CameraNode : public Node
{
    public:
        CameraNode(string n);
        virtual ~CameraNode();

        void update();
        void clear();
        void copy(Node *target);

        Camera *get_camera();
    private:
        Camera *camera;

};

#endif
