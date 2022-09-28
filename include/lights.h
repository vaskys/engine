#ifndef LIGHTS_H
#define LIGHTS_H

#include "config.h"
#include "Node.h"

class LightNode: public Node
{
    public:
        LightNode(string Node);
        virtual ~LightNode();
        
        void copy(Node *target);
        void update();

        glm::vec3 get_diff_farba();
        float get_sila();


        void set_sila(float v);
        void set_color(glm::vec3 color);

    private:
        glm::vec3 diff_farba;
        float sila;
};



#endif
