#ifndef CAMERA_H
#define CAMERA_H

#include "config.h"

class Camera
{
    public:
        Camera(float aspect_ratio,float fov,float z_near,float z_far);
        ~Camera();

        virtual void update();

        glm::vec3 get_position();
        glm::vec3 get_direction();
        glm::vec3 get_up();
        glm::vec3 get_euler();

        glm::mat4 get_projection_matrix();
        glm::mat4 get_view_matrix();

        float get_fov();
        float get_zNear();
        float get_zFar();
        float get_aspect_ratio();

        void set_position(glm::vec3 position);
        void set_direction(glm::vec3 direction);
        void set_up(glm::vec3 up);


        void set_projection_matrix(glm::mat4 projection);
        void set_view_matrix(glm::mat4 view);

        void set_fov(float fov);
        void set_zNear(float near);
        void set_zFar(float far);
        void set_aspect_ratio(float aspect_r);
        void set_euler(glm::vec3 e);
    
        glm::mat4 get_view_projection_matrix();

        virtual void look_at(glm::vec3 target);

    private:
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;
        glm::vec3 euler;

        glm::mat4 projection;
        glm::mat4 view;

        float FOV;
        float zNear;
        float zFar;
        float aspect_ratio;

};



#endif
