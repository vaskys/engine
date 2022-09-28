#include "animation.h"

Animation::Animation()
{
}

Animation::Animation(string path,Model *model)
{
    Assimp::Importer importer;
    const aiScene *scene=importer.ReadFile(path,aiProcess_Triangulate);
    aiAnimation *animation=scene->mAnimations[0];
    cas=animation->mDuration;
    tps=animation->mTicksPerSecond;
    read_hierarchy_node(root_node,scene->mRootNode);
    read_missing_bones(animation,*model);
}

Animation::~Animation()
{
    bone_map.clear();
    bones.clear();
}

Animation::Animation(Animation &animation)
{
    cas=animation.cas;
    tps=animation.tps;
    root_node=animation.root_node;
    bone_map=animation.bone_map;
    bones=animation.bones;
}

Bone *Animation::find_bone(string name)
{
    for(int i=0;i<bones.size();i++)
    {
        if(bones.at(i).get_bone_name() == name)
            return &bones.at(i);
    }
    return nullptr;
}

float Animation::get_ticks_per_second()
{
    return tps;
}

float Animation::get_duration()
{
    return cas;
}

const AssimpNodeData& Animation::get_root_node()
{
    return root_node;
}

const map<string,BONE_INFO>& Animation::get_bone_map()
{
    return bone_map;
}

void Animation::read_missing_bones(const aiAnimation*aiAnimation,Model &model)
{
    int size =aiAnimation->mNumChannels;
    auto boneInfoMap = model.get_bone_map();
    int boneCount = model.get_bone_count();
    for (int i = 0; i < size; i++)
    {
        auto channel =aiAnimation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        bones.push_back(Bone(channel->mNodeName.data,boneInfoMap[channel->mNodeName.data].id, channel));
    }
    bone_map= boneInfoMap;
}

void Animation::read_hierarchy_node(AssimpNodeData &dest,const aiNode *src)
{
    dest.name = src->mName.data;
    dest.transformation =AssimpToGLM(src->mTransformation);
    dest.childrenCount = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        read_hierarchy_node(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}



