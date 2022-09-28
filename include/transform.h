#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "config.h"

class Transform
{
    public:
        Transform();
        ~Transform();

        glm::mat4 get_local_tranform();
        glm::mat4 get_global_transform();

        glm::vec3 get_position();
        glm::vec3 get_scale();
        glm::vec3 get_euler();

        glm::vec3 get_position_global();
        glm::vec3 get_euler_global();
    
        bool dirty;
        void set_local_transform(glm::vec3 pos,glm::vec3 scale,glm::vec3 euler);
        void set_local_transform(glm::mat4 transform);
        void set_global_transform(glm::mat4 transform);
    private:

        glm::mat4 local_transform;
        glm::mat4 global_transform;

        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 euler;


        void cal_local_tranform();
    
};



#endif
