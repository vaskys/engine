#ifndef MESH_H
#define MESH_H

#include "config.h"
#include "material.h"
#include <map>



typedef struct
{
    int id;
    glm::mat4 offset;
}BONE_INFO;

typedef struct
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 UV;

    int bone_id[MAX_BONES_INFLUENCE];
    float weights[MAX_BONES_INFLUENCE];
}VERTEX;

class Mesh
{
    public:
        Mesh();
        Mesh(string id);
        ~Mesh();

        string id_name;
        
        unsigned int get_VAO();
        int get_num_vertices();
        int get_num_indices();
        int get_num_v_pre_gen();
        
        void generate(bool bones);
        void add_vertex(VERTEX vertex);
        void add_vertex(glm::vec3 pos,glm::vec2 uv,glm::vec3 normal);

        void add_index(unsigned int index);
        void set_material(Material *material);
        Material *get_material();

        vector<VERTEX> get_vertices();
        vector<unsigned int> get_indices();

    private:
        vector<VERTEX> vertices;
        vector<unsigned int> indices;

        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;
               
        Material *material;

        unsigned int pocet_v;
        unsigned int pocet_i;


};

#endif
