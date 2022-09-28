#include "model_node.h"
#include "engine.h"

ModelNode::ModelNode(string name,Model *model) : Node(name)
{
    can_modify=true;
    inherit_t=true;
    this->model=model;
    if(has_bones())
         this->animator=new Animator();
}

ModelNode::~ModelNode()
{
//    if(animator !=nullptr)
  //      delete animator;
}

void ModelNode::update()
{
    Node::update();
    if(has_bones())
    {
        animator->update(gin_get_delta_time());
    }
}

bool ModelNode::has_bones()
{
    if(model->get_bone_count() > 0)
        return true;
    return false;
}

Animator *ModelNode::get_animator()
{
    return animator;
}

void ModelNode::copy(Node *target)
{
    ModelNode*nova=new ModelNode(get_name(),model);
    nova->set_transform(get_transform());
    target->add_child(nova);

    for(int i=0;i<get_children().size();i++)
    {
        Node *decko=get_children().at(i);
        if(dynamic_cast<MeshNode*>(decko))
            ((MeshNode*)decko)->copy(nova,nova);
        else
            decko->copy(nova);
    }

}

