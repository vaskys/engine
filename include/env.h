#ifndef ENV_H
#define ENV_H

#include "config.h"
#include "lights.h"

class Env
{
    public:
        Env();
        ~Env();

        void add_light(LightNode *node);
        vector<LightNode*> get_lights();
        void remove_light(LightNode *node);
        void clear();

        glm::vec3 get_global_light_dir();
        glm::vec3 get_global_light_color();
        bool get_global_light_status();

        void set_global_light_dir(glm::vec3 dir);
        void set_global_light_color(glm::vec3 color);
        void set_global_light_status(bool status);

        void set_global_light_default();
    private:
        vector<LightNode*> light_nodes;


        glm::vec3 global_light_dir;
        glm::vec3 global_light_color;
        bool global_light_status;
};


#endif
