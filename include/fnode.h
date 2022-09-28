#ifndef FNODE_H
#define FNODE_H

#include "config.h"
#include "Node.h"

#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "model_node.h"

class FNode : public Node
{
    public:
        FNode(string n);
        FNode(string n,ModelNode *node);
        virtual ~FNode();

        void update();
        void copy(Node *target);

        btRigidBody*get_obj();
        btCollisionShape *get_shape();

        float get_mass();
        void set_mass(float i);

        void set_transform(Transform t);
        void set_shape(string type);
        string get_shape_type();

        void set_size(glm::vec3 size);
        void set_radius(float i);
        void set_height(float i);

        glm::vec3 get_size();
        float get_radius();
        float get_height();

        ModelNode *get_model_shape_pointer();

    private:
        btRigidBody*obj;
        btCollisionShape *shape;
        btTriangleMesh *mesh_shape;
        ModelNode *model_shape_pointer;

        float mass;
        string shape_type;

        glm::vec3 size;
        float radius;
        float height;
};




#endif
