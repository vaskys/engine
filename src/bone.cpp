#include "bone.h"

Bone::Bone(string name,int ID,const  aiNodeAnim*channel)
{
    this->name=name;
    this->id=ID;
    local_transform=glm::mat4(1.0f);

    pocet_pos=channel->mNumPositionKeys;
    pocet_rot=channel->mNumRotationKeys;
    pocet_scal=channel->mNumScalingKeys;

    for(int i=0;i<pocet_pos;i++)
    {
        aiVector3D pos=channel->mPositionKeys[i].mValue;
        float time_stamp=channel->mPositionKeys[i].mTime;

        KeyPosition data;
        data.position=glm::vec3(pos.x,pos.y,pos.z);
        data.timeStamp=time_stamp;
        positions.push_back(data);
    }

    for(int i=0;i<pocet_rot;i++)
    {
        aiQuaternion orientation=channel->mRotationKeys[i].mValue;
        float time_stamp=channel->mRotationKeys[i].mTime;

        KeyRotation data;
        data.orientation=AssimpQuatToGLM(orientation);
        data.timeStamp=time_stamp;
        rotations.push_back(data);
    }


    for(int i=0;i<pocet_scal;i++)
    {
        aiVector3D scale=channel->mScalingKeys[i].mValue;
        float time_stamp=channel->mScalingKeys[i].mTime;

        KeyScale data;
        data.scale=glm::vec3(scale.x,scale.y,scale.z);
        data.timeStamp=time_stamp;
        scalings.push_back(data);
    }

}

Bone::~Bone()
{
}

void Bone::update(float animation_time)
{
    glm::mat4 translation=interpolate_position(animation_time);
    glm::mat4 rotation=interpolate_rotation(animation_time);
    glm::mat4 scaling=interpolate_scaling(animation_time);

    local_transform=translation*rotation*scaling;
}

glm::mat4 Bone::get_local_tranform()
{
    return local_transform;
}

string Bone::get_bone_name()
{
    return name;
}

int Bone::get_bone_id()
{
    return id;
}

int Bone::get_position_index(float animation_time)
{
    for (int index = 0; index < pocet_pos- 1; ++index)
    {
        if (animation_time< positions[index + 1].timeStamp)
            return index;
    }
    return 0;
}

int Bone::get_rotation_index(float animation_time)
{
    for (int index = 0; index < pocet_rot- 1; ++index)
    {
        if (animation_time< rotations[index + 1].timeStamp)
            return index;
    }
    return 0;
}

int Bone::get_scaling_index(float animation_time)
{
    for (int index = 0; index < pocet_scal- 1; ++index)
    {
        if (animation_time< scalings[index + 1].timeStamp)
            return index;
    }
    return 0;
}



float Bone::get_scale_factor(float last_time,float next_time,float animation_time)
{
    float scaleFactor = 0.0f;
    float midWayLength = animation_time-last_time;
    float framesDiff = next_time-last_time;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::interpolate_position(float animation_time)
{
    if (1 ==pocet_pos)
        return glm::translate(glm::mat4(1.0f),positions[0].position);

    int p0Index =get_position_index(animation_time);
    int p1Index = p0Index + 1;
    float scaleFactor =get_scale_factor(positions[p0Index].timeStamp,positions[p1Index].timeStamp,animation_time);
    glm::vec3 finalPosition = glm::mix(positions[p0Index].position,positions[p1Index].position, scaleFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::interpolate_rotation(float animation_time)
{
    if (1 == pocet_rot)
    {
        glm::quat rotation = glm::normalize(rotations[0].orientation);
        return glm::toMat4(rotation);
    
    }
    int p0Index =get_rotation_index(animation_time);
    int p1Index = p0Index + 1;
    float scaleFactor =get_scale_factor(rotations[p0Index].timeStamp,rotations[p1Index].timeStamp,animation_time);
    glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation,rotations[p1Index].orientation, scaleFactor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolate_scaling(float animation_time)
{
    if (1 == pocet_scal)
            return glm::scale(glm::mat4(1.0f),scalings[0].scale);

    int p0Index =get_scaling_index(animation_time);
    int p1Index = p0Index + 1;
    float scaleFactor = get_scale_factor(scalings[p0Index].timeStamp,scalings[p1Index].timeStamp,animation_time);
    glm::vec3 finalScale = glm::mix(scalings[p0Index].scale,scalings[p1Index].scale,scaleFactor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}



