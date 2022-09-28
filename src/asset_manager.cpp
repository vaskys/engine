#include "asset_manager.h"
#include "model_node.h"
#include "mesh_node.h"

Asset_manager::Asset_manager()
{
    models.clear();
    materials.clear();
    textures.clear();
    init_default();
}

void Asset_manager::init_default()
{
    Material *default_material=new Material("default_material");
    materials.insert({default_material->get_name_id(),default_material});
    default_material->ohodnot();

    Material *default_shadeless=new Material("default_shadeless");
    materials.insert({default_shadeless->get_name_id(),default_shadeless});
    default_shadeless->set_shading(false);
    default_shadeless->ohodnot();

    init_basic_shapes();

}

Asset_manager::~Asset_manager()
{
    models.clear();
    materials.clear();
}

void search_for_nodes(ModelNode *model_node,aiNode *a_node,Node *node,string model_name)
{
    for(int i=0;i<a_node->mNumChildren;i++)
    {
        aiNode* a_child=a_node->mChildren[i];
        if(a_child->mNumMeshes > 0)
        {
            MeshNode*child;
            child=new MeshNode(a_child->mName.C_Str(),model_node);
            for(int j=0;j<a_child->mNumMeshes;j++)
               child->mesh_indices.push_back(a_child->mMeshes[j]);

            Transform transform;
            transform.set_local_transform(AssimpToGLM(a_child->mTransformation));
            child->set_transform(transform);

            search_for_nodes(model_node,a_child,child,model_name);
            node->add_child(child);

        }
    }
}


void Asset_manager::process_meshe(const aiScene *scene,Model *model,string path)
{
    aiColor4D ai_color(0.0f,0.0f,0.0f,1.0f);

    vector<VERTEX> vertices;
    for(int i=0;i<scene->mNumMeshes;i++)
    {
        vertices.clear();
        Mesh *mesh=new Mesh();
        aiMesh *mesh_data=scene->mMeshes[i];
        for(int j=0;j<mesh_data->mNumVertices;j++)
        {
            glm::vec3 pos=glm::vec3(mesh_data->mVertices[j].x,mesh_data->mVertices[j].y,mesh_data->mVertices[j].z);
            glm::vec3 normal=glm::vec3(mesh_data->mNormals[j].x,mesh_data->mNormals[j].y,mesh_data->mNormals[j].z); 
            glm::vec2 uv=glm::vec2(0.0f,0.0f);
            if(mesh_data->mTextureCoords[0])
            {
                uv.x=mesh_data->mTextureCoords[0][j].x;
                uv.y=mesh_data->mTextureCoords[0][j].y;
            }

            VERTEX v;
            v.pos=pos;
            v.normal=normal;
            v.UV=uv;
            set_v_bone_default(v);
//            mesh->add_vertex(v);
            vertices.push_back(v);

        }
        for(int j=0;j<mesh_data->mNumFaces;j++)
        {
            aiFace face=mesh_data->mFaces[j];
            for(int c=0;c<face.mNumIndices;c++)
            {
                mesh->add_index(face.mIndices[c]);
            }
        }
        if(mesh_data->mMaterialIndex >=0)
        {

            int m_index=mesh_data->mMaterialIndex;

            aiMaterial *ai_material=scene->mMaterials[m_index];
            string m_name=ai_material->GetName().C_Str();

            if(materials.count(m_name))
            {
                mesh->set_material(materials.at(m_name));
            }
            else
            {
                Material *mat=new Material(m_name);
                aiGetMaterialColor(ai_material,AI_MATKEY_COLOR_DIFFUSE,&ai_color);
                mat->set_color_diffuse(ai_color.r,ai_color.g,ai_color.b);

                aiString texture_name;
                ai_material->GetTexture(aiTextureType_DIFFUSE,0,&texture_name);
                
                string empty_check=texture_name.C_Str();
                if(empty_check!= "")
                {
                    bool c=load_texture(path,texture_name.C_Str());
                    if(c)
                        mat->set_texture_diffuse(get_texture(texture_name.C_Str()));
                }
                mat->ohodnot();
                materials.insert({mat->get_name_id(),mat});
                mesh->set_material(materials.at(m_name));
            }
        }

        //BONE STUFF
        for(int j=0;j< mesh_data->mNumBones;j++)
        {
            int bone_id=-1;
            string bone_name = mesh_data->mBones[j]->mName.C_Str();
//            debug_print(bone_name);
            if(!model->get_bone_map().count(bone_name))
            {
                BONE_INFO b_info;
                b_info.id=model->get_bone_count();
                b_info.offset=AssimpToGLM(mesh_data->mBones[j]->mOffsetMatrix);
                model->add_bone_info(b_info,bone_name);
                bone_id=model->get_bone_count()-1;
            }
            else
            {
                bone_id=model->get_bone_map().at(bone_name).id;
            }

            if(bone_id != -1)
            {
                for(int w=0;w<mesh_data->mBones[j]->mNumWeights;w++)
                {
                    float weight=mesh_data->mBones[j]->mWeights[w].mWeight;
                    int v_index=mesh_data->mBones[j]->mWeights[w].mVertexId;
                    
                    for(int k=0;k<MAX_BONES_INFLUENCE;k++)
                    {
                        if(vertices.at(v_index).bone_id[k] < 0)
                        {
                            vertices.at(v_index).bone_id[k]=bone_id;
                            vertices.at(v_index).weights[k]=weight;
                            break;
                        }
                    }
               }
            }
        }
        for(int j=0;j< vertices.size();j++)
            mesh->add_vertex(vertices.at(j));
        
        if(model->get_bone_map().size() > 0)
            mesh->generate(true);
        else
            mesh->generate(false);
        model->add_mesh(mesh);

        vertices.clear();
    }
 
}
bool Asset_manager::load_model(string file_name)
{
    string path= file_name.substr(0, file_name.find_last_of("\\/"));
//    debug_print("TOTO JE PATH" + path);

    if(models.count(file_name))
    {
        debug_print(file_name+" uz nacitane");
        return false;
    }

    const aiScene *scene = importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_FlipUVs );
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        debug_print("ERROR::ASSIMP::"+string(importer.GetErrorString()));
        return false; 
    }
    
    Model *model=new Model(file_name);
    aiNode *node=scene->mRootNode;
    
    process_meshe(scene,model,path);

    Transform transform;
    transform.set_local_transform(AssimpToGLM(node->mTransformation));

    ModelNode *model_root=(ModelNode*)model->get_root_node();
    model_root->set_transform(transform);
    search_for_nodes(model_root,node,(MeshNode*)model_root,file_name);
    model->get_root_node()->update_t_children();

    //meshe
    debug_print("MESHE: "+to_string(scene->mNumMeshes));

    models.insert({file_name,model});


    return true;
}


bool Asset_manager::load_texture(string path,string file_name)
{
    if(textures.count(file_name) || file_name=="")
    {
        return false;
    }

    unsigned int textura;
    glGenTextures(1,&textura);
    glBindTexture(GL_TEXTURE_2D,textura);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    string complete_path=path+"/"+file_name;
    SDL_Surface* Surface = IMG_Load(complete_path.c_str());
    debug_print(complete_path);
    
    int Mode = GL_RGB;
    if(Surface)
    {
        if(Surface->format->BytesPerPixel == 4) 
        {
            Mode = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        textures.insert({file_name,textura});
        SDL_FreeSurface(Surface);
        //
        return true;
    }
    else
    {
        debug_print("X "+complete_path);
        return false;
    }
}


void Asset_manager::clear()
{
    for(auto p: materials)
    {
        delete p.second;
    }
    for(auto p : models)
    {
        delete p.second;
    }
    unload_all_textures();
    models.clear();
    materials.clear();
    textures.clear();
}

Model* Asset_manager::get_model(string id)
{
    if(models.count(id))
    {
        return models.at(id);
    }
    else
    {
        debug_print("NOT LOADED, LOADING NOW "+id);
        load_model(id);
        return get_model(id);
    }
}

unsigned int Asset_manager::get_texture(string id)
{
    return textures.at(id);
}

void Asset_manager::unload_texture(string id)
{
    unsigned int textura=get_texture(id);
    glBindTexture(GL_TEXTURE_2D,textura);
    glDeleteTextures(1,&textura);
    textures.erase(id);
}

void Asset_manager::unload_all_textures()
{
    for(auto t: textures)
    {
        unsigned int textura=get_texture(t.first);
        glBindTexture(GL_TEXTURE_2D,textura);
        glDeleteTextures(1,&textura);
    }
}


Material* Asset_manager::get_default_material()
{
    return materials.at("default_material");
}

Material* Asset_manager::get_material(string n)
{
    return materials.at(n);
}

///PRIMITIES N SHIT



void Asset_manager::init_basic_shapes()
{
    init_plane();
    init_quad();
    init_cube();
}

void Asset_manager::init_quad()
{
    string shape_name="gin_quad";
    Model *model=new Model(shape_name);
    Mesh *mesh=new Mesh("gin_quad_mesh");

    Transform transform;
    transform.set_local_transform(glm::mat4(1.0f));

    //model->get_root_node()->root_mesh_node->mesh_indices.push_back(0);
    //
    MeshNode *root_mesh=new MeshNode("quad_mesh",(ModelNode*)model->get_root_node());
    root_mesh->mesh_indices.push_back(0);
    root_mesh->can_modify=false;
    model->get_root_node()->add_child(root_mesh);
 //   model->get_root_node()->mesh_indices.push_back(0);
    
    //fixni normals xD
    mesh->add_vertex(glm::vec3(-1.0f,1.0f,0.0f),glm::vec2(0.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(-1.0f,-1.0f,0.0f),glm::vec2(0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(1.0f,-1.0f,0.0f),glm::vec2(1.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(-1.0f,1.0f,0.0f),glm::vec2(0.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(1.0f,-1.0f,0.0f),glm::vec2(1.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(1.0f,1.0f,0.0f),glm::vec2(1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f));

    for(int i=0;i<mesh->get_num_v_pre_gen();i++)
        mesh->add_index(i);

    mesh->generate(false);
    mesh->set_material(get_default_material());
    model->add_mesh(mesh);
    models.insert({shape_name,model});
   
}

void Asset_manager::init_plane()
{
    string shape_name="gin_plane";
    Model *model=new Model(shape_name);
    Mesh *mesh=new Mesh("gin_plane_mesh");

    Transform transform;
    transform.set_local_transform(glm::mat4(1.0f));

//    model->get_root_node()->root_mesh_node->mesh_indices.push_back(0);
    MeshNode *root_mesh=new MeshNode("plane_mesh",(ModelNode*)model->get_root_node());
    root_mesh->can_modify=false;
    root_mesh->mesh_indices.push_back(0);
    model->get_root_node()->add_child(root_mesh);

//    model->get_root_node()->mesh_indices.push_back(0);
//    model->get_root_node()->set_transform(transform);
    
    //fixni normals xD
    mesh->add_vertex(glm::vec3(5.0f,-0.5f,5.0f),glm::vec2(0.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(-5.0f,-0.5f,5.0f),glm::vec2(0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(-5.0f,-0.5f,-5.0f),glm::vec2(1.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(5.0f,-0.5f,5.0f),glm::vec2(0.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(-5.0f,-0.5f,-5.0f),glm::vec2(1.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));
    mesh->add_vertex(glm::vec3(5.0f,-0.5f,-5.0f),glm::vec2(1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f));

    for(int i=0;i<mesh->get_num_v_pre_gen();i++)
        mesh->add_index(i);

    mesh->generate(false);
    mesh->set_material(get_default_material());
    model->add_mesh(mesh);
    models.insert({shape_name,model});
}



void Asset_manager::init_cube()
{
    string shape_name="gin_cube";
    Model *model=new Model(shape_name);
    Mesh *mesh=new Mesh("gin_cube_mesh");

    Transform transform;
    transform.set_local_transform(glm::mat4(1.0f));

    //model->get_root_node()->root_mesh_node->mesh_indices.push_back(0);
    //
    MeshNode*root_mesh=new MeshNode("cube_mesh",(ModelNode*)model->get_root_node());
    root_mesh->can_modify=false;
    root_mesh->mesh_indices.push_back(0);
    model->get_root_node()->add_child(root_mesh);
 //   model->get_root_node()->mesh_indices.push_back(0);
    

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    for(int i=0;i<288;i+=8)
    {
        VERTEX v;
        v.pos=glm::vec3(vertices[i],vertices[i+1],vertices[i+2]);
        v.normal=glm::vec3(vertices[i+3],vertices[i+4],vertices[i+5]);
        v.UV=glm::vec2(vertices[i+6],vertices[i+7]);
        mesh->add_vertex(v);
    }

    for(int i=0;i<mesh->get_num_v_pre_gen();i++)
        mesh->add_index(i);

    mesh->generate(false);
    mesh->set_material(get_material("default_shadeless"));
    model->add_mesh(mesh);
    models.insert({shape_name,model});

}



set<string> Asset_manager::get_list_of_models()
{
    set<string> zoznam;

    for(auto m : models)
        zoznam.insert(m.first);
    
    return zoznam;
}


///ANIM STUFFF
//
bool Asset_manager::load_animation(string path,string model_id)
{
    if(animacie.count(path))
    {
        return false;
    }
    Model *model=get_model(model_id);
    Animation *anim=new Animation(path,model);
    if(anim !=NULL)
    {
        animacie.insert({path,anim});
        return true;
    }
    return false;
}

Animation *Asset_manager::get_animation(string id)
{
    if(animacie.count(id))
        return animacie.at(id);
    else
    {
        return nullptr;
    }
}

void Asset_manager::set_v_bone_default(VERTEX &vertex)
{
    for(int i=0;i<MAX_BONES_INFLUENCE;i++)
    {
        vertex.bone_id[i]=-1;
        vertex.weights[i]=0.0f;
    }
}




