#include "orbit_camera.h"
#include "abs_sdl.h"

OrbitCamera::OrbitCamera(float aspect_ratio,float fov,float z_near,float z_far) : Camera(aspect_ratio,fov,z_near,z_far)
{
    rotation=glm::vec3(1.0f);
    v=0;
    h=0;
    radius=10.0f;
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::look_at(glm::vec3 t)
{
    target=t;
}

void OrbitCamera::update()
{
    controls();

    rotation.x= sin(h) *radius;
    rotation.z= cos(h) *radius;
    rotation.y= sin(v)*radius;

    glm::vec3 p=glm::vec3(rotation.x,-rotation.y,rotation.z)+target;
    set_position(p);

    glm::vec3 smer=target-get_position();
    smer=glm::normalize(smer);

    set_direction(smer);
    Camera::update();
}

void OrbitCamera::controls()
{
    if(abs_sdl::sdl_is_key_pressed_event(102))
    {
        if(abs_sdl::mouse_scroll_down())
        {
            radius-=0.5f;
        }
        else if(abs_sdl::mouse_scroll_up())
        {
            radius+=0.5f;
        }
    }
    else
    {
        if(abs_sdl::mouse_scroll_right())
            h+=0.1f;
        else if(abs_sdl::mouse_scroll_left())
            h-=0.1f;
        if(abs_sdl::mouse_scroll_up())
            v+=0.1f;
        else if(abs_sdl::mouse_scroll_down())
            v-=0.1f;
    }

    if(abs_sdl::sdl_is_key_pressed_event(106))
    {
        target+= get_direction() * abs_sdl::get_delta_time() * 2.0f;
    }
    else if(abs_sdl::sdl_is_key_pressed_event(107))
        target-= get_direction() * abs_sdl::get_delta_time() * 2.0f;

    
}



