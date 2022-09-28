#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "config.h"
#include "animation.h"

class Animator
{
    public:
        Animator();
        ~Animator();

        void update(float dt);
        void play(string name,bool loop_i);

        bool is_playing();
        vector<glm::mat4> get_final_bones_transform();

        void add_animation(Animation *animation,string name);
        void remove_animation(string name);

        void clear();
    private:
        void cal_bone_transform(const AssimpNodeData*node,glm::mat4 parent_transform);

        vector<glm::mat4> final_bones_transform;

        Animation *current_anim;
        string current_anim_id;

        float current_time;

        bool loop;
        bool playing;

        map<string,Animation*> animacie; 

};


#endif
