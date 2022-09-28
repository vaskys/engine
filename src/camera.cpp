#include "camera.h"

Camera::Camera(float aspect_ratio,float fov,float z_near,float z_far)
{
    position=glm::vec3(0.0f,0.0f,0.0f);
    direction=glm::vec3(0.0f,0.0f,1.0f);
    up=glm::vec3(0.0f,1.0f,0.0f);
    projection=glm::mat4(1.0f);
    view=glm::mat4(1.0f);
    zNear=0.1f;
    zFar=100.0f;
    aspect_ratio=18.0f/9.0;
    euler=glm::vec3(0,0,0);

    set_fov(fov);
    set_zFar(z_far);
    set_zNear(z_near);
    set_aspect_ratio(aspect_ratio);

}

Camera::~Camera()
{

}

void Camera::look_at(glm::vec3 target)
{
    glm::vec3 smer=target-get_position();
    smer=glm::normalize(smer);
    set_direction(smer);
}

void Camera::update()
{
    glm::mat4 p=glm::perspective(glm::radians(get_fov()),get_aspect_ratio(),get_zNear(),get_zFar());
    glm::mat4 v=glm::lookAt(get_position(),get_position()+get_direction(),get_up());

    v=glm::rotate(v,glm::radians(euler.x),glm::vec3(1.0f,0.0f,0.0f));
    v=glm::rotate(v,glm::radians(euler.y),glm::vec3(0.0f,1.0f,0.0f));
    v=glm::rotate(v,glm::radians(euler.z),glm::vec3(0.0f,0.0f,1.0f));

    set_projection_matrix(p);
    set_view_matrix(v);

}

glm::vec3 Camera::get_euler()
{
    return euler;
}
glm::mat4 Camera::get_view_projection_matrix()
{
    return projection*view;
}

glm::vec3 Camera::get_direction()
{
    return this->direction;
}

glm::vec3 Camera::get_position()
{
    return this->position;
}

glm::vec3 Camera::get_up()
{
    return this->up;
}


glm::mat4 Camera::get_projection_matrix()
{
    return this->projection;
}

glm::mat4 Camera::get_view_matrix()
{
    return this->view;
}


float Camera::get_fov()
{
    return this->FOV;
}

float Camera::get_zFar()
{
    return this->zFar;
}

float Camera::get_zNear()
{
    return this->zNear;
}

float Camera::get_aspect_ratio()
{
    return this->aspect_ratio;
}

void Camera::set_position(glm::vec3 position)
{
    this->position=position;
}

void Camera::set_direction(glm::vec3 direction)
{
    this->direction=direction;
}

void Camera::set_up(glm::vec3 up)
{
    this->up=up;
}


void Camera::set_projection_matrix(glm::mat4 projection)
{
    this->projection=projection;
}

void Camera::set_view_matrix(glm::mat4 view_i)
{
    this->view=view_i;
}

void Camera::set_fov(float fov_i)
{
    this->FOV=fov_i;
}

void Camera::set_zFar(float far_i)
{
    this->zFar=far_i;
}

void Camera::set_zNear(float near)
{
    this->zNear=near;
}

void Camera::set_aspect_ratio(float aspect_r)
{
    this->aspect_ratio=aspect_r;
}

void Camera::set_euler(glm::vec3 e)
{
    euler=e;
}

//////////////////////////////////////////////////////////////






