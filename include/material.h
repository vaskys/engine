#ifndef MATERIAL_H
#define MATERIAL_H

#include "config.h"

class Material
{
    public:
        Material(string id);
        ~Material();

        short shader_id;
        
        void set_color_diffuse(float r,float g,float b);
        void set_color_specular(float r,float g,float b);

        glm::vec3 get_color_diffuse();
        glm::vec3 get_color_specular();

        unsigned int get_texture_diffuse();
        void set_texture_diffuse(unsigned int textura);
        void set_shading(bool v);

        string get_name_id();

        int get_shader_hodnota();

        bool get_ma_diffuse_texture();
        bool get_ma_shading();


        string get_shader_prefix_fragment();

        void ohodnot();

    private:
        glm::vec3 color_diffuse;
        glm::vec3 color_specular;
        
        unsigned int texture_diffuse;
        unsigned int texture_specular;
        unsigned int texture_normal;

        string name_id;

        bool ma_diffuse_texture;
        bool ma_shading;

        int shader_hodnota;
        string shader_prefix_fragment;
};

#endif
