#include "model.h"

#include "model_node.h"

Model::Model(string n)
{
    meshe.clear();
    name=n;
    bone_count=0;

    root_node=new ModelNode(n,this);
    root_node->set_parent(NULL);
}

Model::~Model()
{
    for(auto p : meshe)
    {
        delete p;
    }
    meshe.clear();

    //delete nodes deletuje nodu co zadam + vsetky deti
    root_node->delete_nodes(root_node);
}

void Model::add_mesh(Mesh *mesh)
{
    meshe.push_back(mesh);
}

int Model::get_mesh_count()
{
    return meshe.size();
}

Mesh* Model::get_mesh_at_index(int i)
{
    return meshe[i];
}

 //ModelNode* Model::get_root_node()
 //{
 //    return root_node;
 //}
Node* Model::get_root_node()
{
    return root_node;
}

string Model::get_name()
{
    return name;
}

map<string,BONE_INFO> Model::get_bone_map()
{
    return bone_map;
}
int Model::get_bone_count()
{
    return bone_count;
}


void Model::add_bone_info(BONE_INFO bone_i,string name)
{
    bone_map.insert({name,bone_i});
    bone_count++;
}
