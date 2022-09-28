#include "animator.h"



Animator::Animator()
{
    current_time=0.0f;
    current_anim=nullptr;
    final_bones_transform.reserve(100);

    for(int i=0;i<100;i++)
    {
        final_bones_transform.push_back(glm::mat4(1.0f));
    }

    loop=false;
    playing=false;
    current_anim_id="";
}


Animator::~Animator()
{
    clear();
}


void Animator::update(float dt)
{
    if(current_anim != nullptr)
    {
        if(playing)
        {
            current_time+=current_anim->get_ticks_per_second() * dt;
            current_time=fmod(current_time,current_anim->get_duration());
            cal_bone_transform(&current_anim->get_root_node(),glm::mat4(1.0f));
        }
        if(current_time + 1.0f > current_anim->get_duration() )
        {
            if(loop)
                playing=true;
            else
            {
                playing=false;
            }
        }
    }
}


void Animator::add_animation(Animation *animation,string name)
{
    if(!animacie.count(name))
    {
        Animation *anim=new Animation(*animation);
        debug_print("ADD ANIM "+to_string(anim->get_duration()));
        animacie.insert({name,anim});
   }
}

void Animator::remove_animation(string name)
{
    map<string,Animation*>::iterator it;
    it=animacie.find(name);
    delete it->second;
    animacie.erase(it,animacie.end());
}

void Animator::play(string id,bool loop_i)
{
    if(current_anim_id !=id)
    {
        current_anim=animacie.at(id);
        current_anim_id=id;
        current_time=0.0f;
        loop=loop_i;
        playing=true;
    }
}

bool Animator::is_playing()
{
    return playing;
}

void Animator::cal_bone_transform(const AssimpNodeData*node,glm::mat4 parent_transform)
{
    string node_name=node->name;
    glm::mat4 node_transform=node->transformation;
    Bone *bone=current_anim->find_bone(node_name);

    if(bone)
    {
        bone->update(current_time);
        node_transform=bone->get_local_tranform();
    }
    glm::mat4 global=parent_transform * node_transform;
    auto bone_map = current_anim->get_bone_map();

    if(bone_map.count(node_name))
    {
        int index=bone_map[node_name].id;
        glm::mat4 offset=bone_map[node_name].offset;
        final_bones_transform[index]=global*offset;
    }

    for(int i=0;i<node->childrenCount;i++)
    {
        cal_bone_transform(&node->children[i],global);
    }
}

vector<glm::mat4> Animator::get_final_bones_transform()
{
    return final_bones_transform;
}

void Animator::clear()
{
    for(auto a : animacie)
    {
//        delete a.second;
    }
    animacie.clear();
}

