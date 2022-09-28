#include "material.h"

Material::Material(string id)
{
    name_id=id;
    color_diffuse=glm::vec3(1.0f,1.0f,1.0f);
    color_specular=glm::vec3(1.0f,1.0f,1.0f);

    texture_diffuse=0;
    texture_normal=0;
    texture_specular=0;
    shader_id=0;


    ma_diffuse_texture=false;
    ma_shading=true;

    shader_hodnota=0;
}

Material::~Material()
{
    debug_print("Material delete");
}

string Material::get_name_id()
{
    return name_id;
}

void Material::set_color_diffuse(float r,float g,float b)
{
    color_diffuse=glm::vec3(r,g,b);
}

void Material::set_color_specular(float r, float g, float b)
{
    color_specular=glm::vec3(r,g,b);
}

glm::vec3 Material::get_color_diffuse()
{
    return color_diffuse;
}

glm::vec3 Material::get_color_specular()
{
    return color_specular;
}

unsigned int Material::get_texture_diffuse()
{
    return texture_diffuse;
}

void Material::set_texture_diffuse(unsigned int textura)
{
    texture_diffuse=textura;
    ma_diffuse_texture=true;
}

void Material::set_shading(bool v)
{
    ma_shading=v;
}

int Material::get_shader_hodnota()
{
    return shader_hodnota;
}

bool Material::get_ma_diffuse_texture()
{
    return ma_diffuse_texture;
}

string Material::get_shader_prefix_fragment()
{
    return shader_prefix_fragment;
}

void Material::ohodnot()
{
    shader_prefix_fragment="#version 330 core \n";
    shader_hodnota=0;
/*    if(ma_diffuse_texture)
    {
        shader_hodnota++;
        shader_prefix_fragment+="#define ma_texture_diffuse \n";
    }
*/
    if(ma_diffuse_texture)
    {
        shader_hodnota=1;
        shader_prefix_fragment+="#define ma_texture_diffuse \n";
    }

    if(ma_shading)
    {
        shader_hodnota = 2;
        shader_prefix_fragment+="#define ma_shading \n";

    }

    if(ma_shading && ma_diffuse_texture)
    {
        shader_hodnota = 3;
        shader_prefix_fragment+="#define ma_shading \n";
        shader_prefix_fragment+="#define ma_texture_diffuse \n";
    }


}




