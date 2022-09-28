#include "mesh_node.h"
#include "engine.h"

MeshNode::MeshNode(string name,ModelNode *model_node) : Node(name)
{
    mesh_indices.clear();   
    this->model_node=model_node;
    can_modify=true;
    inherit_t=true;
}

MeshNode::~MeshNode() 
{
    mesh_indices.clear();
    model_node=NULL;
}

void MeshNode::copy(Node *target,ModelNode *m_node)
{
    MeshNode*nova=new MeshNode(get_name(),m_node);
    nova->set_transform(get_transform());
    nova->mesh_indices=mesh_indices;
    target->add_child(nova);

    for(int i=0;i<get_children().size();i++)
    {
        Node *decko=get_children().at(i);
        if(dynamic_cast<MeshNode*>(decko))
            ((MeshNode*)decko)->copy(nova,m_node);
        else
            decko->copy(nova);
    }
}

void MeshNode::update()
{
    Node::update();
//    debug_print("RENDERING "+get_name());
   gin_get_main_renderer()->draw(this);
}

