#ifndef MODEL_H
#define MODEL_H

#include "config.h"
#include "mesh.h"
#include "material.h"
#include "Node.h"

class Model
{
    public:
        Model(string n);
        ~Model();

        void add_mesh(Mesh *mesh);
        int get_mesh_count();

        Mesh *get_mesh_at_index(int i);
       // ModelNode*get_root_node();
        Node*get_root_node();


        string get_name();
        map<string,BONE_INFO> get_bone_map();
        int get_bone_count();


        void add_bone_info(BONE_INFO bone_i,string name);

    private:
        string name;
        vector<Mesh*> meshe;

        Node*root_node;

        map<string,BONE_INFO> bone_map;
        int bone_count;

};

#endif
