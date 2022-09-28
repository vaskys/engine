#include "scene.h"
#include "engine.h"


#include "animation.h"
#include "animator.h"
#include "fnode.h"

#include <fstream>

Scene::Scene(string n) : Node(n)
{
}

Scene::~Scene()
{
}

void Scene::reset()
{
    delete_child_nodes();
    set_name("default scene");
    env.set_global_light_default();
}

bool b=true;
void Scene::run()
{
    while(!gin_is_window_close_event())
    {
        gin_pool_events();


        gin_clear_color(0.0f,0.0f,0.0f,0.0f);
        gin_update_active_camera();

        cameras.clear();
        env.clear(); // <------------ !!!!!!!!!!!!!
        gin_get_main_renderer()->begin();
        update();
        gin_get_main_renderer()->end();
        gin_swap_buffer();
    }
}

void recurs_update(Node *node)
{
    for(int i=0;i<node->get_children().size();i++)
    {
        Node *child=node->get_children().at(i);
        child->update();
        recurs_update(child);
    }
}

void Scene::update()
{
    for(int i=0;i<get_children().size();i++)
    {
        Node *child=get_children().at(i);
        child->update();

        recurs_update(child);
    }
}
void Scene::clear()
{
    cameras.clear();
    env.clear();
    delete_nodes(this);
}


void Scene::add_model_node(string model_name)
{
    Model *model=gin_get_asset_manager()->get_model(model_name);
    ModelNode*root=(ModelNode*)model->get_root_node();
    root->copy(this); 

}

void Scene::add_light()
{
    LightNode *node=new LightNode("light");
    add_child(node);
//    env.add_light(node);
}

void Scene::add_empty()
{
    Node *node=new Node("empty");
    add_child(node);
}

void Scene::add_camera_node()
{
    CameraNode *node=new CameraNode("camera");
    cameras.push_back(node);
    add_child(node);
}


void Scene::add_fobj()
{
    FNode *node=new FNode("fobj");
    add_child(node);
}

void Scene::add_mesh_fobj(ModelNode *m_node)
{
    FNode *node=new FNode("fmeshobj",m_node);
    add_child(node);
}


//////
void Scene::export_node(Node *node,ofstream &output)
{
    output << node->get_name() <<endl;
    output << to_string(node->get_children().size()) <<endl;
    
    if(dynamic_cast<Scene*>(node))
    {
        output << "scene" <<endl;

        output << to_string(((Scene*)node)->env.get_global_light_status()) <<endl;
        output << to_string(((Scene*)node)->env.get_global_light_dir().x) <<endl;
        output << to_string(((Scene*)node)->env.get_global_light_dir().y) <<endl;
        output << to_string(((Scene*)node)->env.get_global_light_dir().z) <<endl;

        output << to_string(((Scene*)node)->env.get_global_light_color().x) <<endl;
        output << to_string(((Scene*)node)->env.get_global_light_color().y) <<endl;
        output << to_string(((Scene*)node)->env.get_global_light_color().z) <<endl;
    }
    else
    {

        if(dynamic_cast<ModelNode*>(node))
        {
            output << "model" <<endl;
            output << ((ModelNode*)node)->model->get_name() << endl;
        }
        else if(dynamic_cast<MeshNode*>(node))
        {
            output << "mesh" <<endl;
            int size_m=((MeshNode*)node)->mesh_indices.size();
            output << to_string(size_m) <<endl;
            for(int i=0;i<size_m;i++)
            {
                output<< to_string(((MeshNode*)node)->mesh_indices.at(i)) <<endl;
            }

        }
        else if(dynamic_cast<LightNode*>(node))
        {
            output << "light" <<endl;
            output << to_string(((LightNode*)node)->get_diff_farba().x) <<endl; 
            output << to_string(((LightNode*)node)->get_diff_farba().y) <<endl; 
            output << to_string(((LightNode*)node)->get_diff_farba().z) <<endl; 
            output << to_string(((LightNode*)node)->get_sila()) <<endl; 
        }
        else if(dynamic_cast<CameraNode*>(node))
        {
            output << "camera" <<endl;
        }
        else if(dynamic_cast<FNode*>(node))
        {
            output << "fnode" <<endl;
            output << ((FNode*)node)->get_shape_type() <<endl;
            output << to_string(((FNode*)node)->get_mass()) <<endl;
            output << to_string(((FNode*)node)->get_radius()) <<endl;
            output << to_string(((FNode*)node)->get_height()) <<endl;
            output << to_string(((FNode*)node)->get_size().x) <<endl;
            output << to_string(((FNode*)node)->get_size().y) <<endl;
            output << to_string(((FNode*)node)->get_size().z) <<endl;

            if(((FNode*)node)->get_shape_type() == "mesh_c")
            {
                output<<((FNode*)node)->get_model_shape_pointer()->get_name() << endl;
            }
        }
        else
            output << "empty" <<endl;

        output << to_string(node->get_transform().get_position().x) <<endl;
        output << to_string(node->get_transform().get_position().y) <<endl;
        output << to_string(node->get_transform().get_position().z) <<endl;

        output << to_string(node->get_transform().get_euler().x) <<endl;
        output << to_string(node->get_transform().get_euler().y) <<endl;
        output << to_string(node->get_transform().get_euler().z) <<endl;
      
        output << to_string(node->get_transform().get_scale().x) <<endl;
        output << to_string(node->get_transform().get_scale().y) <<endl;
        output << to_string(node->get_transform().get_scale().z) <<endl;
    }

    for(int i=0;i<node->get_children().size();i++)
    {
        Node *decko=node->get_children().at(i);
        export_node(decko,output);
    }
}

void Scene::save(string n)
{
    ofstream output(n);
    export_node(this,output);    
}


void Scene::import_node(Node *parent,ifstream &input,string line)
{
    bool c_flag=false;

    string input_name ="";
    int input_pocet=0;
    string input_type;

    getline(input,line);
    input_name=line;

    getline(input,line);
    input_pocet=stoi(line);

    getline(input,line);
    input_type=line;

    if(input_type == "scene")
    {
        getline(input,line);
        bool input_light_status=stoi(line);

        glm::vec3 input_light_dir=glm::vec3(0,0,0);
        getline(input,line);
        input_light_dir.x=stof(line);
        getline(input,line);
        input_light_dir.y=stof(line);
        getline(input,line);
        input_light_dir.z=stof(line);

        glm::vec3 input_light_color=glm::vec3(0,0,0);
        getline(input,line);
        input_light_color.x=stof(line);
        getline(input,line);
        input_light_color.y=stof(line);
        getline(input,line);
        input_light_color.z=stof(line);

        ((Scene*)parent)->set_name(input_name);
        ((Scene*)parent)->env.set_global_light_dir(input_light_dir);
        ((Scene*)parent)->env.set_global_light_color(input_light_color);
        ((Scene*)parent)->env.set_global_light_status(input_light_status);

        for(int i=0;i<input_pocet;i++)
            import_node(parent,input,line);        
    }
    else
    {
        Node *node;
        if(input_type == "model" )
        {
            getline(input,line);
            string model_path=line;
            
            node=new ModelNode(input_name,gin_get_asset_manager()->get_model(model_path));
            loader_pomocna=(ModelNode*)node;
        }
        else if(input_type == "mesh" )
        {
            node=new MeshNode(input_name,loader_pomocna);
            int mesh_indices_size=0;
            getline(input,line);
            mesh_indices_size=stoi(line);
            for(int i=0;i<mesh_indices_size;i++)
            {
                getline(input,line);
                int index=stoi(line);
                ((MeshNode*)node)->mesh_indices.push_back(index);
            }
        }
        else if(input_type == "light" )
        {
            node=new LightNode(input_name);

            glm::vec3 farba=glm::vec3(1,1,1);
            float sila=1.0f;

            getline(input,line);
            farba.x=stof(line);
            getline(input,line);
            farba.y=stof(line);
            getline(input,line);
            farba.z=stof(line);
            getline(input,line);
            sila=stof(line);

            ((LightNode*)node)->set_color(farba);
            ((LightNode*)node)->set_sila(sila);
        }
        else if(input_type == "camera" )
        {
            node=new CameraNode(input_name);
        }
        else if(input_type == "fnode" )
        {
            string shape_type="box";
            float mass=0.0f;
            float radius=1.0f;
            float height=1.0f;
            glm::vec3 size=glm::vec3(1,1,1);
        
            getline(input,line);
            shape_type=line;
            getline(input,line);
            mass=stof(line);
            getline(input,line);
            radius=stof(line);
            getline(input,line);
            height=stof(line);
            getline(input,line);
            size.x=stof(line);
            getline(input,line);
            size.y=stof(line);
            getline(input,line);
            size.z=stof(line);

            string model_shape_pointer_name;
            if(shape_type == "mesh_c")
            {
                getline(input,line);
                model_shape_pointer_name=line;
                loader_mesh_shape_pomocna.push_back(model_shape_pointer_name);
                node=new Node(input_name);

            }
            else
            {
                node=new FNode(input_name);
                ((FNode*)node)->set_size(size);
                ((FNode*)node)->set_radius(radius);
                ((FNode*)node)->set_height(height);
                ((FNode*)node)->set_shape(shape_type);
                ((FNode*)node)->set_mass(mass);
            }
            
        }
        else
            node=new Node(input_name);

        glm::vec3 input_pos=glm::vec3(0,0,0);
        getline(input,line);
        input_pos.x=stof(line);
        getline(input,line);
        input_pos.y=stof(line);
        getline(input,line);
        input_pos.z=stof(line);

        glm::vec3 input_rot=glm::vec3(0,0,0);
        getline(input,line);
        input_rot.x=stof(line);
        getline(input,line);
        input_rot.y=stof(line);
        getline(input,line);
        input_rot.z=stof(line);

        glm::vec3 input_scale=glm::vec3(0,0,0);
        getline(input,line);
        input_scale.x=stof(line);
        getline(input,line);
        input_scale.y=stof(line);
        getline(input,line);
        input_scale.z=stof(line);

        Transform t;
        t.set_local_transform(input_pos,input_scale,input_rot);
        node->set_transform(t);

        parent->add_child(node);

        for(int i=0;i<input_pocet;i++)
            import_node(node,input,line);        
    }
}

void Scene::load(string n)
{
    bool f_state=gin_get_f_update();

    gin_set_f_update(false);
    reset();

    ifstream input(n);
    string line;
    
    import_node(this,input,line);
    loader_pomocna=nullptr;

    update();
    for(int i=0;i<loader_mesh_shape_pomocna.size();i++)
    {
        ModelNode *n = (ModelNode*)get_child(loader_mesh_shape_pomocna.at(i));
        add_mesh_fobj(n);
    }
    loader_mesh_shape_pomocna.clear();

    gin_set_f_update(f_state);
}

void Scene::init_con()
{
    init_con(this);
}

void Scene::init_con(Node *node)
{
    for(int i=0;i<node->get_children().size();i++)
    {
       if(node->get_children().at(i)->get_controler())
            node->get_children().at(i)->get_controler()->init();        
        
        init_con(node->get_children().at(i));
    }
}

