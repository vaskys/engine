#include "fyzika.h"
#include "engine.h"

FWorld::FWorld()
{
    gravity=glm::vec3(0,-10,0);

    drawer=new FDraw();

    bt_collision_config=new btDefaultCollisionConfiguration();
    bt_collision_dispatcher=new btCollisionDispatcher(bt_collision_config);
    bt_broadphase=new btDbvtBroadphase();
    bt_solver=new btSequentialImpulseConstraintSolver();
    bt_world=new btDiscreteDynamicsWorld(bt_collision_dispatcher,bt_broadphase,bt_solver,bt_collision_config);

    bt_world->setDebugDrawer(drawer);
    bt_world->setGravity(btVector3(gravity.x,gravity.y,gravity.z));
}

void FWorld::set_gravity(glm::vec3 g_i)
{
    gravity=g_i;
    bt_world->setGravity(btVector3(gravity.x,gravity.y,gravity.z));

}

glm::vec3 FWorld::get_gravity()
{
    return gravity;
}

FWorld::~FWorld()
{
    delete bt_world;
    delete bt_solver;
    delete bt_broadphase;
    delete bt_collision_dispatcher;
    delete bt_collision_config;

    delete drawer;
}

void FWorld::draw()
{
    drawer->get_shader()->use();
    drawer->get_shader()->send_mat4_uniform("projection",gin_get_active_camera()->get_projection_matrix());
    drawer->get_shader()->send_mat4_uniform("view",gin_get_active_camera()->get_view_matrix());

    bt_world->debugDrawWorld();
    drawer->push_draw();
}

void FWorld::update()
{
    bt_world->stepSimulation(1.0f/60.0f,5,1.0f/60.0f);
}

btDiscreteDynamicsWorld* FWorld::get_bt_world()
{
    return bt_world;
}


void FWorld::set_all_obj(bool v)
{
    for(int i=0;i<bt_world->getNumCollisionObjects();i++)
    {
        bt_world->getCollisionObjectArray().at(i)->activate(v);
    }
}

