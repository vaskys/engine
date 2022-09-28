#include "env.h"

Env::Env()
{
    light_nodes.clear();
    set_global_light_default();
}

Env::~Env()
{
    light_nodes.clear();
}

void Env::set_global_light_default()
{
    global_light_dir=glm::vec3(1,0,0);
    global_light_color=glm::vec3(1,1,1);
    global_light_status=true;
}


void Env::add_light(LightNode *node)
{
    light_nodes.push_back(node);
}


vector<LightNode*> Env::get_lights()
{
    return light_nodes;
}

void Env::remove_light(LightNode *node)
{
    light_nodes.erase(remove(light_nodes.begin(),light_nodes.end(),node),light_nodes.end());
}

void Env::clear()
{
    light_nodes.clear();
}

void Env::set_global_light_dir(glm::vec3 dir)
{
    global_light_dir=dir;
}

void Env::set_global_light_color(glm::vec3 color)
{
    global_light_color=color;
}


void Env::set_global_light_status(bool status)
{
    global_light_status=status;
}

glm::vec3 Env::get_global_light_dir()
{
    return global_light_dir;
}

glm::vec3 Env::get_global_light_color()
{
    return global_light_color;
}


bool Env::get_global_light_status()
{
    return global_light_status;
}



