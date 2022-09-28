#include "engine.h"
#include "player_con.h"

void demo()
{
    gin_init(1280,720,"demo",true,false);

    gin_init_default_scene();
    gin_init_default_camera();

    gin_get_active_scene()->load("demo.txt"); 

    gin_get_active_scene()->get_child("player")->set_controler(new PlayerCon());
    gin_get_active_scene()->init_con();
   
    //gin_set_fdraw(true);

    gin_run_active_scene();
    gin_clear();
    
}

void editor()
{
    gin_init(1280,720,"test",true,false);
    gin_set_editor(true);
    gin_set_f_update(false);
    gin_set_fdraw(true);
    

    gin_get_asset_manager()->load_model("models/anim/base.fbx");
    gin_get_asset_manager()->load_animation("models/anim/animacka.fbx","models/anim/base.fbx");
    
    gin_init_default_scene();
    gin_init_default_camera();
    
    gin_get_active_scene()->add_light();
    gin_get_active_scene()->add_model_node("models/kocka/neviem.fbx");
    gin_get_active_scene()->add_model_node("models/anim/base.fbx");

    gin_get_active_scene()->add_empty();
    gin_get_active_scene()->add_empty();
    gin_get_active_scene()->add_empty();
    gin_get_active_scene()->add_empty();

    ModelNode *model_node=(ModelNode*)gin_get_active_scene()->get_child("models/anim/base.fbx");
    model_node->get_animator()->add_animation(gin_get_asset_manager()->get_animation("models/anim/animacka.fbx"),"test");
    model_node->get_animator()->play("test",true);
    
    gin_run_active_scene();
    gin_clear();

}

int main()
{
//    editor();
    demo();
    return 0;
}





