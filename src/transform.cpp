#include "transform.h"

Transform::Transform()
{
    position=glm::vec3(0.0f,0.0f,0.0f);
    scale=glm::vec3(1.0f,1.0f,1.0f);
    euler=glm::vec3(0.0f,0.0f,0.0f);

    local_transform=glm::mat4(1.0f);
    global_transform=glm::mat4(1.0f);

    cal_local_tranform();

    dirty=true;
}

Transform::~Transform()
{
}


void Transform::cal_local_tranform()
{
    local_transform=glm::mat4(1.0f);
    local_transform=glm::translate(local_transform,position);
    local_transform=glm::rotate(local_transform,glm::radians(euler.x),glm::vec3(1.0f,0.0f,0.0f));
    local_transform=glm::rotate(local_transform,glm::radians(euler.y),glm::vec3(0.0f,1.0f,0.0f));
    local_transform=glm::rotate(local_transform,glm::radians(euler.z),glm::vec3(0.0f,0.0f,1.0f));
    local_transform=glm::scale(local_transform,scale);

    dirty=true;
}

void Transform::set_local_transform(glm::mat4 t)
{
//    local_transform=t;
    this->position=t[3];
    for (int i = 0; i < 3; ++i)
    {
        this->scale[i] = glm::length(t[i]);
        t[i] /= this->scale[i];
    }
    this->euler = glm::eulerAngles(glm::quat_cast(t));
    this->euler.x=glm::degrees(this->euler.x);
    this->euler.y=glm::degrees(this->euler.y);
    this->euler.z=glm::degrees(this->euler.z);
  
    cal_local_tranform();
}

void Transform::set_local_transform(glm::vec3 pos,glm::vec3 scale,glm::vec3 rot)
{
    this->position=pos;
    this->scale=scale;
    this->euler=rot;

    cal_local_tranform();
}

void Transform::set_global_transform(glm::mat4 t)
{
    this->global_transform=t;
}

glm::vec3 Transform::get_position()
{
    return this->position;
}

glm::vec3 Transform::get_position_global()
{
    return glm::vec3(global_transform[3]);    
}

glm::vec3 Transform::get_euler()
{
    return this->euler;
}

glm::vec3 Transform::get_euler_global()
{
    glm::vec3 r;
    glm::extractEulerAngleXYZ(global_transform,r.x,r.y,r.z);
    return glm::vec3(glm::degrees(r.x),glm::degrees(r.y),glm::degrees(r.z));
}

glm::vec3 Transform::get_scale()
{
    return this->scale;
}

glm::mat4 Transform::get_local_tranform()
{
    return this->local_transform;
}

glm::mat4 Transform::get_global_transform()
{
    return this->global_transform;
}


