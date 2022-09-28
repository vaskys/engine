#include "mesh.h"

Mesh::Mesh()
{
    vertices.clear();
    indices.clear();
}


Mesh::Mesh(string id)
{
    id_name=id;
    vertices.clear();
    indices.clear();
}

Mesh::~Mesh()
{
    vertices.clear();
    indices.clear();

    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&VAO);

    debug_print("mesh delete "+id_name);
}

int Mesh::get_num_indices()
{
    return pocet_i;
}

int Mesh::get_num_v_pre_gen()
{
    return vertices.size();
}


int Mesh::get_num_vertices()
{
    return pocet_v;
}

unsigned int Mesh::get_VAO()
{
    return VAO;
}

void Mesh::generate(bool bones)
{
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(VERTEX),&vertices[0],GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0], GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VERTEX),(void*)0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX,normal));

    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX,UV));

    if(bones)
    {
        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(3, 4,GL_INT,sizeof(VERTEX), (void*)offsetof(VERTEX,bone_id));


        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4,GL_FLOAT,GL_FALSE,sizeof(VERTEX), (void*)offsetof(VERTEX,weights));
    }

    glBindVertexArray(0);
    
    pocet_v=vertices.size();
    pocet_i=indices.size();

   // vertices.clear();
   // indices.clear();
}

void Mesh::add_vertex(VERTEX vertex)
{
    vertices.push_back(vertex);
}

void Mesh::add_vertex(glm::vec3 pos,glm::vec2 uv,glm::vec3 normal)
{
    VERTEX v;
    v.pos=pos;
    v.UV=uv;
    v.normal=normal;

    add_vertex(v);
}


void Mesh::add_index(unsigned int index)
{
    indices.push_back(index);
}


void Mesh::set_material(Material *material)
{
    this->material=material;
}

Material* Mesh::get_material()
{
    return material;
}


vector<VERTEX> Mesh::get_vertices()
{
    return vertices;
}


vector<unsigned int> Mesh::get_indices()
{
    return indices;
}


