#ifndef ANIMATION_H
#define ANIMATION_H


#include "config.h"
#include "bone.h"
#include "model.h"

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
    public:
        Animation();
        Animation(string path,Model *model);
        ~Animation();

        Animation(Animation &animation);

        float get_ticks_per_second();
        float get_duration();
        Bone *find_bone(string name);
        const AssimpNodeData& get_root_node();
        const map<string,BONE_INFO> &get_bone_map();

    private:
        void read_missing_bones(const aiAnimation*aiAnimation,Model &model);
        void read_hierarchy_node(AssimpNodeData &dest,const aiNode *src);


        float cas;
        int tps;
        vector<Bone> bones;
        AssimpNodeData root_node;
        map<string,BONE_INFO> bone_map;
};


#endif
