#include "fnode.h"
#include "engine.h"



void recurs_mesh_obj(Node *node,btTriangleMesh *mesh_shape,Model *model)
{
     for(int i=0;i<node->get_children().size();i++)
     {
         Node *decko=node->get_children().at(i);

         if(dynamic_cast<MeshNode*>(node->get_children().at(i)))
         {
             decko=(MeshNode*)node->get_children().at(i);

             for(int j=0;j<((MeshNode*)decko)->mesh_indices.size();j++)
             {
                 int mesh_index=((MeshNode*)decko)->mesh_indices.at(j);
                 Mesh *mesh=model->get_mesh_at_index(mesh_index);

                 for(int index=0;index<mesh->get_num_indices()-3;index+=3)
                 {
                     unsigned int index_jedna=mesh->get_indices().at(index);
                     unsigned int index_dva=mesh->get_indices().at(index+1);
                     unsigned int index_tri=mesh->get_indices().at(index+2);

                     VERTEX jedna=mesh->get_vertices().at(index_jedna);
                     VERTEX dva=mesh->get_vertices().at(index_dva);
                     VERTEX tri=mesh->get_vertices().at(index_tri);

                     glm::vec4 vertex_pos_jedna=decko->get_transform().get_global_transform() * glm::vec4(jedna.pos,1.0f);
                     glm::vec4 vertex_pos_dva=decko->get_transform().get_global_transform() * glm::vec4(dva.pos,1.0f);
                     glm::vec4 vertex_pos_tri=decko->get_transform().get_global_transform() * glm::vec4(tri.pos,1.0f);

                     mesh_shape->addTriangle(
                             btVector3(vertex_pos_jedna.x,vertex_pos_jedna.y,vertex_pos_jedna.z),
                             btVector3(vertex_pos_dva.x,vertex_pos_dva.y,vertex_pos_dva.z),
                             btVector3(vertex_pos_tri.x,vertex_pos_tri.y,vertex_pos_tri.z)
                             );
                 }
             }
         }
         recurs_mesh_obj(decko,mesh_shape,model);
     }
}

FNode::FNode(string n,ModelNode* node) : Node(n)
{
     can_modify=true;
     inherit_t=false;
    
     model_shape_pointer=node;
     mass = 0.0f;
     shape_type="mesh_c";
  
     mesh_shape=new btTriangleMesh();
     Model *model=node->model;

     recurs_mesh_obj(node,mesh_shape,model);
    
     btTransform t;
     t.setIdentity();
     t.setOrigin(btVector3(0,0,0));
     
     shape=new btBvhTriangleMeshShape(mesh_shape,false);
     shape->setLocalScaling(btVector3(1.0f,1.0f,1.0f));

     btVector3 localInertia(0, 0, 0);
     shape->calculateLocalInertia(mass,localInertia);
     
     btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,NULL,shape, localInertia);
     obj=new btRigidBody(rbInfo);
     obj->setWorldTransform(t);

     btDiscreteDynamicsWorld *bt_world=gin_get_fworld()->get_bt_world();
     bt_world->addRigidBody(obj);

}



FNode::FNode(string n) : Node(n)
{
    model_shape_pointer=NULL;
    mesh_shape=NULL;

    can_modify=true;
    inherit_t=false;
    
    mass=0.0f;
    shape_type="box";

    size=glm::vec3(1,1,1);
    radius=1.0f;
    height=1.0f;

    btTransform t;
    t.setFromOpenGLMatrix(glm::value_ptr(get_transform().get_global_transform()));

    
    btVector3 localInertia(0, 0, 0);
    shape=new btBoxShape(btVector3(1,1,1));
    shape->calculateLocalInertia(mass,localInertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,NULL,shape, localInertia);
    obj=new btRigidBody(rbInfo);

    btDiscreteDynamicsWorld *bt_world=gin_get_fworld()->get_bt_world();
    obj->setWorldTransform(t);
    bt_world->addRigidBody(obj);
}

FNode::~FNode()
{
    btDiscreteDynamicsWorld *bt_world=gin_get_fworld()->get_bt_world();
    bt_world->removeRigidBody(obj);
    
    if(obj)
        delete obj;
    if(shape)
        delete shape;
    if(mesh_shape)
        delete mesh_shape;
}

btRigidBody*FNode::get_obj()
{
    return obj;
}

btCollisionShape*FNode::get_shape()
{
    return shape;
}

void FNode::set_mass(float i)
{
    if(!gin_get_f_update())
    {
        btDiscreteDynamicsWorld *bt_world=gin_get_fworld()->get_bt_world();
        bt_world->removeRigidBody(obj);

        mass=i;
        btVector3 localInertia(0, 0, 0);
        obj->getCollisionShape()->calculateLocalInertia(mass,localInertia);
        obj->setMassProps(mass,localInertia);

        bt_world->addRigidBody(obj);
    }
}

float FNode::get_mass()
{
    return mass;
}

string FNode::get_shape_type()
{
    return shape_type;
}

void FNode::set_shape(string type)
{
    if(type == "box")
    {
        shape=new btBoxShape(btVector3(size.x,size.y,size.z));
        obj->setCollisionShape(shape);
        set_mass(mass);
        shape_type="box";
    }
    else if(type == "sphere")
    {
        shape=new btSphereShape(radius);
        obj->setCollisionShape(shape);
        set_mass(mass);
        shape_type="sphere";
    }
    else if(type == "capsule")
    {
        shape=new btCapsuleShape(radius,height);
        obj->setCollisionShape(shape);
        set_mass(mass);
        shape_type="capsule";
   }
}

void FNode::update()
{
    if(gin_get_f_update())
    {
        Transform transform_i;
        btTransform t=obj->getWorldTransform();

        glm::mat4 global=get_transform().get_global_transform();
        t.getOpenGLMatrix(glm::value_ptr(global));
        transform_i.set_local_transform(global);
        Node::set_transform(transform_i);
    }
    Node::update();
}


void FNode::set_transform(Transform transform_i)
{
    if(!gin_get_f_update())
    {
        Node::set_transform(transform_i);

        btTransform t;
        t.setFromOpenGLMatrix(glm::value_ptr(get_transform().get_local_tranform()));
        obj->setWorldTransform(t);
        obj->setLinearVelocity(btVector3(0,0,0));
        obj->setAngularVelocity(btVector3(0,0,0));
    }
}

void FNode::set_size(glm::vec3 size_i)
{
    size=size_i;
}

void FNode::set_radius(float i)
{
    radius=i;
}

void FNode::set_height(float i)
{
    height=i;
}

glm::vec3 FNode::get_size()
{
    return size;
}

float FNode::get_radius()
{
    return radius;
}

float FNode::get_height()
{
    return height;
}

void FNode::copy(Node *target)
{
    FNode *nova=new FNode("fobj");
    nova->set_transform(get_transform());

    nova->set_height(get_height());
    nova->set_radius(get_radius());
    nova->set_size(get_size());
    nova->set_shape(get_shape_type());

    nova->set_mass(get_mass());
    target->add_child(nova);

    for(int i=0;i<get_children().size();i++)
    {
        Node *decko=get_children().at(i); 
        decko->copy(nova);
    }
}

ModelNode *FNode::get_model_shape_pointer()
{
    return model_shape_pointer;
}

