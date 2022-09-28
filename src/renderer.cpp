#include "renderer.h"
#include <fstream>
#include <sstream>

#include "engine.h"
#include <vector>
#include "config.h"

Renderer::Renderer()
{
 //   primary_shader=shader;
    meshe_to_draw.clear();
    load_shader_code();
    shadre.clear();
}

Renderer::~Renderer()
{
}

void Renderer::begin()
{
    meshe_to_draw.clear();
    camera_projection=gin_get_active_camera()->get_projection_matrix();
    camera_view=gin_get_active_camera()->get_view_matrix();
    cam_pos=gin_get_active_camera()->get_position();
}


void Renderer::search_node_for_meshes(glm::mat4 glob,Model *model,MeshNode *node)
{

    for(int j=0;j<node->mesh_indices.size();j++)
    {
        MESH_T t;
        t.mesh=model->get_mesh_at_index(node->mesh_indices[j]);
        t.global_transform=glob*node->get_parent()->get_transform().get_local_tranform()*node->get_transform().get_local_tranform();
        t.has_bones=false;
        meshe_to_draw.push_back(t);
    }
}

void Renderer::search_nodes(glm::mat4 glob,Model *model,MeshNode*node)
{
    search_node_for_meshes(glob,model,node);
    for(int i=0;i<node->get_children().size();i++)
    {
        MeshNode *child=(MeshNode*)node->get_children().at(i);
        search_nodes(glob,model,child);
    }
}

void Renderer::draw(Model *model,glm::mat4 transform)
{
    Node *main=model->get_root_node();
    for(int i=0;i<main->get_children().size();i++)
    {
        search_nodes(transform,model,(MeshNode*)main->get_children().at(i));
    }
}

 //void Renderer::draw(ModelNode *node)
 //{
 //     
 //}
 //
 
void Renderer::draw(MeshNode *node)
{
    Model *model=node->model_node->model;

    for(int i=0;i<node->mesh_indices.size();i++)
    {
        MESH_T t;
        t.mesh=model->get_mesh_at_index(node->mesh_indices[i]);
        t.global_transform=node->get_transform().get_global_transform();
        t.has_bones=node->model_node->has_bones();
        t.animator=node->model_node->get_animator();
        meshe_to_draw.push_back(t);
    }
}

void find_closes_lights(Scene *scene,DefaultShader*shader,glm::mat4 transform)
{
    glm::vec3 pos=transform[3];
    glm::vec3 global_light_pos;

    Env env=scene->env;
    vector<pair<float,int>> v_s;
    for(int i=0;i<env.get_lights().size();i++)
    {
        global_light_pos=env.get_lights().at(i)->get_transform().get_global_transform()[3];
        float d=glm::distance(pos,global_light_pos);
        v_s.push_back(make_pair(d,i));
    }
    sort(v_s.begin(),v_s.end());

    int size=v_s.size();
    if(size > 4)
        size=4;
    
    shader->send_global_light_color(env.get_global_light_color());
    shader->send_global_light_dir(env.get_global_light_dir());
    shader->send_global_light_status(env.get_global_light_status());

    shader->send_lights_pocet(size);
    for(int i=0;i<size;i++)
    {
        
        int index=v_s[i].second;
        global_light_pos=env.get_lights().at(index)->get_transform().get_global_transform()[3]; 
        
        shader->send_light_pos(global_light_pos,i);
        shader->send_light_color(env.get_lights().at(index)->get_diff_farba(),i);
        shader->send_light_sila(env.get_lights().at(index)->get_sila(),i);
    }
}
void Renderer::end()
{
    filter();
    Scene *scene=gin_get_active_scene();

    for(auto s : shadre)
    {
        s.second->use();
        s.second->send_projection_uniform(camera_projection);
        s.second->send_view_uniform(camera_view);
        s.second->send_camera_pos_uniform(cam_pos);

        for(int i=0;i<s.second->get_meshe().size();i++)
        {
            if(s.first == 3 || s.first == 2 || s.first== 5 || s.first==6)
                find_closes_lights(scene,s.second,s.second->get_meshe().at(i)->global_transform);
            if(s.first == 1 || s.first == 3)
            {
                //textury
            }
            push(*s.second->get_meshe().at(i),s.second);
        }

        //debug_print(to_string(s.second->get_meshe().size()));

    }
}

void Renderer::filter()
{
    for(auto s : shadre)
        s.second->clear_meshe();

    for(int i=0;i<meshe_to_draw.size();i++)
    {
        int shader_hodnota=meshe_to_draw.at(i).mesh->get_material()->get_shader_hodnota();
        bool has_bones=meshe_to_draw.at(i).has_bones;
        string vertex_prefix="#version 330 core \n";
        if(has_bones)
        {
            if(shader_hodnota == 1)
                shader_hodnota=4;
            else if(shader_hodnota  == 2)
                shader_hodnota = 5;
            else if(shader_hodnota == 3 )
                shader_hodnota=6;
            vertex_prefix+="#define ma_kosti \n";
        }

        if(!shadre.count(shader_hodnota))
        {
            string fragment_prefix=meshe_to_draw.at(i).mesh->get_material()->get_shader_prefix_fragment();
            generate_shader(shader_hodnota,vertex_prefix,fragment_prefix);
        }
        
        DefaultShader*shader=shadre.at(shader_hodnota);
        shader->add_mesh(&meshe_to_draw.at(i));
    }
}

void Renderer::push(MESH_T t,DefaultShader*shader)
{
     if(t.has_bones)
     {
         for(int i=0;i<t.animator->get_final_bones_transform().size();i++)
         {
             shader->send_bone_transform(t.animator->get_final_bones_transform().at(i),i);
         }
      }
 
     shader->send_model_uniform(t.global_transform);
     shader->send_diffuse_color_uniform(t.mesh->get_material()->get_color_diffuse());
     shader->send_diffuse_texutre_uniform(t.mesh->get_material()->get_texture_diffuse());
 
     glBindVertexArray(t.mesh->get_VAO());
     glDrawElements(GL_TRIANGLES, t.mesh->get_num_indices(), GL_UNSIGNED_INT, 0);
     t.mesh=nullptr;
     t.animator=nullptr;
     t.has_bones=false;
}

void Renderer::generate_shader(int hodnota,string vertex_prefix,string fragment_prefix)
{
    debug_print("GENERUJEM SHADER "+to_string(hodnota));
    DefaultShader*shader=new DefaultShader(vertex_prefix+default_shader_code_vertex,fragment_prefix+default_shader_code_fragment,true);
//    debug_print("VERTEX KOD :"+vertex_prefix+default_shader_code_vertex);
//    debug_print("FRAGMENT KOD"+fragment_prefix+default_shader_code_fragment);
    shadre.insert({hodnota,shader});
}


void Renderer::clear()
{
    meshe_to_draw.clear();
    for(auto s : shadre)
        delete s.second;

    shadre.clear();
}

void Renderer::load_shader_code()
{
    //temp xD
    string vertex_path="shaders/vertex.glsl";
    string fragment_path="shaders/fragment.glsl";

    fstream vertex_subor;
    fstream fragment_subor;

    vertex_subor.open(vertex_path);
    fragment_subor.open(fragment_path);
    
    stringstream vertex_stream;
    stringstream fragment_stream;

    vertex_stream<<vertex_subor.rdbuf();
    fragment_stream<<fragment_subor.rdbuf();

    default_shader_code_vertex=vertex_stream.str();
    default_shader_code_fragment=fragment_stream.str();

    vertex_subor.close();
    fragment_subor.close();
}
