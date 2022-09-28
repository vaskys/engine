#include "lights.h"
#include "model.h"
#include "engine.h"

LightNode::LightNode(string n) : Node(n)
{
    diff_farba=glm::vec3(1.0f,1.0f,1.0f);
    sila=1.0f;
    inherit_t=true;
    can_modify=true;
}

LightNode::~LightNode()
{
}

void LightNode::update()
{
    Node::update();

    if(gin_get_fdraw())
    {
        Model *model=gin_get_asset_manager()->get_model("gin_cube");
        gin_get_main_renderer()->draw(model,get_transform().get_global_transform());
    }
    gin_get_active_scene()->env.add_light(this);

}

void LightNode::copy(Node *target)
{
    LightNode*nova=new LightNode(get_name());
    nova->set_transform(get_transform());
    nova->set_color(get_diff_farba());
    nova->set_sila(get_sila());
    target->add_child(nova);

    for(int i=0;i<get_children().size();i++)
    {
        Node *decko=get_children().at(i);
        decko->copy(nova);
    }

}

glm::vec3 LightNode::get_diff_farba()
{
    return diff_farba;
}

float LightNode::get_sila()
{
    return sila;
}


void LightNode::set_sila(float v)
{
    sila=v;
}

void LightNode::set_color(glm::vec3 color)
{
    diff_farba=color;
}
