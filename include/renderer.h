#ifndef RENDERER_H
#define RENDERER_H

#include "config.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "mesh_node.h"
#include "animator.h"


#include <map>

class Renderer
{
    public:
        Renderer();
        ~Renderer();

        void begin(glm::mat4 VP);
        void begin();
        void draw(Model *model,glm::mat4 transform);
//        void draw(ModelNode *node);
        void draw(MeshNode *node);

        void end();

        void clear();
    private:
        void push(MESH_T mesh,DefaultShader*shader);
        void filter();

        vector<MESH_T> meshe_to_draw;

        map<int,DefaultShader*> shadre;

        glm::mat4 camera_projection;
        glm::mat4 camera_view;
        glm::vec3 cam_pos;

        void search_nodes(glm::mat4 glob,Model *model,MeshNode *node);
        void search_node_for_meshes(glm::mat4 glob,Model *model,MeshNode *node);

        string default_shader_code_vertex;
        string default_shader_code_fragment;

        void load_shader_code();
        void generate_shader(int hodnota,string vertex_prefix,string fragment_prefix);


        void unload_unused_shaders();
};

#endif
