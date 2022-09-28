#ifndef BONE_H
#define BONE_H

#include "config.h"


struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};


class Bone
{
    public:
        Bone(string name,int ID,const aiNodeAnim* channel);
        ~Bone();

        void update(float animation_time);
        glm::mat4 get_local_tranform();
        string get_bone_name();
        int get_bone_id();

        int get_position_index(float animation_time);
        int get_rotation_index(float animation_time);
        int get_scaling_index(float animation_time);

    private:
        vector<KeyPosition> positions;
        vector<KeyRotation> rotations;
        vector<KeyScale> scalings;

        glm::mat4 local_transform;
        string name;
        int id;

        int pocet_pos;
        int pocet_rot;
        int pocet_scal;

        float get_scale_factor(float last_time,float next_time,float animation_time);
        glm::mat4 interpolate_position(float animation_time);
        glm::mat4 interpolate_rotation(float animation_time);
        glm::mat4 interpolate_scaling(float animation_time);
};

#endif
