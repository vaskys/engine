#include "shader.h"
#include <fstream>
#include <sstream>


BaseShader::BaseShader(string vertex_path,string fragment_path)
{
    string vertex_code;
    string fragment_code;

    fstream vertex_subor;
    fstream fragment_subor;

    vertex_subor.open(vertex_path);
    fragment_subor.open(fragment_path);
    
    stringstream vertex_stream;
    stringstream fragment_stream;

    vertex_stream<<vertex_subor.rdbuf();
    fragment_stream<<fragment_subor.rdbuf();

    vertex_code=vertex_stream.str();
    fragment_code=fragment_stream.str();

    vertex_subor.close();
    fragment_subor.close();
    
    compile(vertex_code,fragment_code);
    register_uniforms();

}

BaseShader::BaseShader(string vertex_kod,string fragment_kod,bool bruh)
{
    compile(vertex_kod,fragment_kod);
    register_uniforms();
}

BaseShader::~BaseShader()
{
    glDeleteShader(id);
    debug_print("Shader delete");
}

void BaseShader::compile(string vertex_code,string fragment_code)
{
    unsigned int vertex;
    unsigned int fragment;

    int status;
    char log[512];

    const char* shader_code_vertex=vertex_code.c_str();
    const char* shader_code_fragemt=fragment_code.c_str();

    vertex=glCreateShader(GL_VERTEX_SHADER);    
    glShaderSource(vertex,1,&shader_code_vertex,NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex,GL_COMPILE_STATUS,&status);
    if(!status)
    {
        glGetShaderInfoLog(vertex,512,NULL,log);
        debug_print("vertex error "+string(log));

    }
    else
        debug_print("vertex gucci");

    fragment=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1,&shader_code_fragemt,NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment,GL_COMPILE_STATUS,&status);
    if(!status)
    {
        glGetShaderInfoLog(fragment,512,NULL,log);
        debug_print("fragmet error "+string(log));
    }
    else
        debug_print("fragment gucci");

    id=glCreateProgram();
    glAttachShader(id,vertex);
    glAttachShader(id,fragment);
    glLinkProgram(id);

    glGetProgramiv(id,GL_LINK_STATUS,&status);
    if(!status)
    {
        glGetProgramInfoLog(this->id,512,NULL,log);
        debug_print("shader link error "+string(log));
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void BaseShader::use()
{
    glUseProgram(id);
}

unsigned int BaseShader::get_id()
{
    return id;
}


void BaseShader::register_uniforms()
{

}

void BaseShader::send_float_uniform(const string name,float v) 
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), v); 
}

void BaseShader::send_int_uniform(const string name,int v) 
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), v); 
}

void BaseShader::send_vec3_uniform(const string name,glm::vec3 v)
{
    glUniform3f(glGetUniformLocation(id,name.c_str()),v.x,v.y,v.z);
}

void BaseShader::send_mat4_uniform(const string name,glm::mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(id,name.c_str()),1,GL_FALSE,glm::value_ptr(mat));
}


DefaultShader::DefaultShader(string vertex_code_i,string fragment_code_i,bool bruh) : BaseShader(vertex_code_i,fragment_code_i,bruh)
{
//    compile(vertex_code_i,fragment_code_i);
    register_uniforms();

    meshe.clear();
}

DefaultShader::DefaultShader(string vertex_path,string fragment_path) : BaseShader(vertex_path,fragment_path)
{
}

DefaultShader::~DefaultShader()
{
    BaseShader::~BaseShader();
    meshe.clear();
}


void DefaultShader::add_mesh(MESH_T*mesh)
{
    meshe.push_back(mesh);
}

vector<MESH_T*> DefaultShader::get_meshe()
{
    return meshe;
}

void DefaultShader::clear_meshe()
{
    meshe.clear();
}

void DefaultShader::send_projection_uniform(glm::mat4 projection)
{
    glUniformMatrix4fv(projection_uniform,1,GL_FALSE,glm::value_ptr(projection));
}
void DefaultShader::send_view_uniform(glm::mat4 view_i)
{
    glUniformMatrix4fv(view_uniform,1,GL_FALSE,glm::value_ptr(view_i));
}

void DefaultShader::send_model_uniform(glm::mat4 model)
{
    glUniformMatrix4fv(model_uniform,1,GL_FALSE,glm::value_ptr(model));
}

void DefaultShader::send_diffuse_color_uniform(glm::vec3 color)
{
    glUniform3f(diffuse_color_uniform,color.x,color.y,color.z);
}

void DefaultShader::send_diffuse_texutre_uniform(unsigned int texture_id)

{
    glActiveTexture(GL_TEXTURE0+texture_id);
    glUniform1i(diffuse_texture_uniform,texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void DefaultShader::send_camera_pos_uniform(glm::vec3 pos)
{
    glUniform3f(camera_pos_uniform,pos.x,pos.y,pos.z);
}

void DefaultShader::send_bone_transform(glm::mat4 bone_transform,int id)
{
    glUniformMatrix4fv(bones_transform_uniform[id],1,GL_FALSE,glm::value_ptr(bone_transform));
}

void DefaultShader::send_light_color(glm::vec3 color, int i)
{
    glUniform3f(lights_color_uniform[i],color.x,color.y,color.z);
}
void DefaultShader::send_light_pos(glm::vec3 pos, int i)
{
    glUniform3f(lights_pos_uniform[i],pos.x,pos.y,pos.z);
}

void DefaultShader::send_light_sila(float sila,int i)
{
    glUniform1f(lights_sila_uniform[i],sila);
}

void DefaultShader::send_lights_pocet(unsigned int i)
{
    glUniform1i(lights_pocet_aktiv_uniform,i);    
}

void DefaultShader::send_global_light_color(glm::vec3 color)
{
    glUniform3f(light_global_color_uniform,color.x,color.y,color.z);
}

void DefaultShader::send_global_light_dir(glm::vec3 dir)
{
    glUniform3f(light_global_dir_uniform,dir.x,dir.y,dir.z);
}

void DefaultShader::send_global_light_status(bool status)
{
    glUniform1i(light_global_status_uniform,status);
}

void DefaultShader::register_uniforms()
{
    projection_uniform=glGetUniformLocation(id,"projection");
    view_uniform=glGetUniformLocation(id,"view");
    camera_pos_uniform=glGetUniformLocation(id,"camera_pos");
    model_uniform=glGetUniformLocation(id,"model");
    diffuse_color_uniform=glGetUniformLocation(id,"diffuse_color");
    diffuse_texture_uniform=glGetUniformLocation(id,"texture_diffuse");

    for(int i=0;i<100;i++)
    {
        string u_id="finalBonesMatrices["+to_string(i)+"]";
        bones_transform_uniform[i]=glGetUniformLocation(id,u_id.c_str());
    }

    for(int i=0;i<4;i++)
    {
        string id_pos="svetla["+to_string(i) + "].pos";
        string id_color="svetla["+to_string(i) + "].color";
        string id_sila="svetla["+to_string(i) + "].sila";

        lights_pos_uniform[i]=glGetUniformLocation(id,id_pos.c_str());
        lights_color_uniform[i]=glGetUniformLocation(id,id_color.c_str());
        lights_sila_uniform[i]=glGetUniformLocation(id,id_sila.c_str());
    }
    lights_pocet_aktiv_uniform=glGetUniformLocation(id,"active_svetla");

    light_global_dir_uniform=glGetUniformLocation(id,"global_light_dir");
    light_global_color_uniform=glGetUniformLocation(id,"global_light_color");
    light_global_status_uniform=glGetUniformLocation(id,"global_light_status");

}


