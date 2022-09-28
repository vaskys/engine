#ifndef FYZIKA_H
#define FYZIKA_H

#include "config.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "FDraw.h"

class FWorld
{
    public:
        FWorld();
        ~FWorld();

        void update();
        void draw();

        btDiscreteDynamicsWorld *get_bt_world();
        void set_all_obj(bool v);

        void set_gravity(glm::vec3 g_i);
        glm::vec3 get_gravity();
    private:
        btBroadphaseInterface *bt_broadphase;
        btDefaultCollisionConfiguration *bt_collision_config;
        btCollisionDispatcher *bt_collision_dispatcher;
        btSequentialImpulseConstraintSolver *bt_solver;
        btDiscreteDynamicsWorld *bt_world;

        FDraw *drawer;

        glm::vec3 gravity;
};



#endif
