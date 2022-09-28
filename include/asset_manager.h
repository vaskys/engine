#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H 

#include "config.h"

#include <map>
#include "mesh.h"
#include "material.h"
#include "model.h"
#include "animation.h"

class Asset_manager
{
    public:
        Asset_manager();
        ~Asset_manager();

        bool load_model(string path);
        bool load_animation(string path,string model_id);

        void clear();
        Model *get_model(string id);
        Animation *get_animation(string id);
        unsigned int get_texture(string id);

        Material *get_default_material();
        Material *get_material(string n);

        void init_default();

        set<string> get_list_of_models();

    private:
        map<string,Model*> models;
        map<string,Material*> materials;
        map<string,unsigned int> textures;
        map<string,Animation*> animacie;

        Assimp::Importer importer;

        bool load_texture(string path,string file_name);
        void unload_texture(string id);
        void unload_all_textures();

        void init_basic_shapes();
        void init_cube();
        void init_plane();
        void init_quad();


        void process_meshe(const aiScene *scene,Model *model,string path);
        void set_v_bone_default(VERTEX &vertex);
};

#endif
