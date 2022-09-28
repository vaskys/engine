#include "first_person_camera.h"
#include "abs_sdl.h"

float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

FirstPersonCamera::FirstPersonCamera(float aspect_ratio,float fov,float z_near,float z_far) : Camera(aspect_ratio,fov,z_near,z_far)
{
    //neviem
}



FirstPersonCamera::~FirstPersonCamera()
{
}


void FirstPersonCamera::update()
{
    Camera::update();
    controls();
}

void FirstPersonCamera::controls()
{
    if(abs_sdl::sdl_is_key_pressed_event(119))
    {
        set_position(get_position()+get_direction()*0.1f);
    }
    else if(abs_sdl::sdl_is_key_pressed_event(97))
    {
        set_position(get_position()-glm::normalize(glm::cross(get_direction(),get_up()))*0.1f);
    }
    else if(abs_sdl::sdl_is_key_pressed_event(100))
    {
        set_position(get_position()+glm::normalize(glm::cross(get_direction(),get_up()))*0.1f);
    }
    else if(abs_sdl::sdl_is_key_pressed_event(115))
    {
        set_position(get_position()-get_direction()*0.1f);
    }

    float xoffset = abs_sdl::sdl_get_mouse_x()- lastX;
    float yoffset = lastY - abs_sdl::sdl_get_mouse_y(); 
    lastX = abs_sdl::sdl_get_mouse_x();
    lastY = abs_sdl::sdl_get_mouse_y();

    float sensitivity = 0.5f;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 smer;
    smer.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    smer.y = sin(glm::radians(pitch));
    smer.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   
    set_direction(glm::normalize(smer));
}

