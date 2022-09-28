#include "player_con.h"
#include "engine.h"

PlayerCon::PlayerCon()
{
    attack=false;
    pohyb=glm::vec3(0,0,0);
    speed=100.0f;
    dir=glm::vec3(0,0,1);
}

PlayerCon::~PlayerCon()
{
    Controler::clear();
}

void PlayerCon::init()
{

    main=(FNode*)gin_get_active_scene()->get_child("player");    

    main->get_obj()->setAngularFactor(btVector3(0,0,0));
    

    m_node=(ModelNode*)main->get_children().at(0);


    gin_get_asset_manager()->load_animation("models/anim/idle.fbx","models/anim/base.fbx");
    gin_get_asset_manager()->load_animation("models/anim/walk.fbx","models/anim/base.fbx");
    gin_get_asset_manager()->load_animation("models/anim/back.fbx","models/anim/base.fbx");
    gin_get_asset_manager()->load_animation("models/anim/attack.fbx","models/anim/base.fbx");
    gin_get_asset_manager()->load_animation("models/anim/kill.fbx","models/anim/base.fbx");


    m_node->get_animator()->add_animation(gin_get_asset_manager()->get_animation("models/anim/idle.fbx"),"idle");
    m_node->get_animator()->add_animation(gin_get_asset_manager()->get_animation("models/anim/walk.fbx"),"walk");
    m_node->get_animator()->add_animation(gin_get_asset_manager()->get_animation("models/anim/back.fbx"),"back");
    m_node->get_animator()->add_animation(gin_get_asset_manager()->get_animation("models/anim/attack.fbx"),"attack");
    m_node->get_animator()->add_animation(gin_get_asset_manager()->get_animation("models/anim/kill.fbx"),"kill");

}


void PlayerCon::update()
{
    gin_get_active_camera()->look_at(main->get_transform().get_position_global());

    main->get_obj()->activate(true);
    main->get_obj()->setAngularVelocity(btVector3(0,0,0));            

    pohyb=glm::vec3(0,0,0);

    btQuaternion o=main->get_obj()->getOrientation();
    glm::quat rot=glm::quat(); 
    rot.x=o.getX();
    rot.y=o.getY();
    rot.z=o.getZ();
    rot.w=o.getW();
    
    if(!attack)
    {
        if(gin_is_key_pressed(119))
        {
            m_node->get_animator()->play("walk",true);
            pohyb=rot*dir * speed * gin_get_delta_time();
        }
        else if(gin_is_key_pressed(115))
        {
            m_node->get_animator()->play("back",true);
            pohyb=rot*(-dir) * speed * gin_get_delta_time();
        }
        else if(gin_is_key_pressed(99))
        {
            gin_get_active_scene()->env.set_global_light_status(true);
        }
        else
        {
            m_node->get_animator()->play("idle",true);
        }

        if(gin_is_key_pressed(97))
        {
            main->get_obj()->setAngularVelocity(btVector3(0,1,0) * speed * gin_get_delta_time());            
        }
        else if(gin_is_key_pressed(100))
        {
            main->get_obj()->setAngularVelocity(btVector3(0,-1,0) * speed * gin_get_delta_time());            
        }
        if(gin_is_mouse_button_down(1))
        {
            attack=true;
            m_node->get_animator()->play("attack",false);
        }
    }
    else
    {
        attack=m_node->get_animator()->is_playing();
    }

    main->get_obj()->setLinearVelocity(btVector3(pohyb.x,0,pohyb.z));
}

void PlayerCon::clear()
{
    main=nullptr;
    m_node=nullptr;
}
