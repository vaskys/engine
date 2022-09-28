#ifndef SCENE_H
#define SCENE_H


#include "config.h"
#include "Node.h"

#include <set>
#include "env.h"
#include "camera_node.h"
#include "model_node.h"

class Scene : public Node
{
    public:
        Scene(string n);
        ~Scene();
        
        void save(string n);
        void load(string n);
        
        void run();
        void update();
        void clear();

        void add_empty();
        void add_model_node(string model_name);
        void add_camera_node();
        void add_light();
        void add_fobj();
        void add_mesh_fobj(ModelNode *model_node);


        void reset();
        void init_con();

        Env env;
        vector<CameraNode*> cameras;
    private:
        ModelNode*loader_pomocna;
        vector<string> loader_mesh_shape_pomocna;

        void import_node(Node *parent,ifstream &input,string line);
        void export_node(Node *node,ofstream &output);

        void init_con(Node *node);
};

#endif
