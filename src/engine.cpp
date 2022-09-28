#include "engine.h"

Scene *gin_active_scene;
Asset_manager *asset_manager;
Renderer *main_renderer;
Camera *main_camera;
Camera *default_cam;

FrameBuffer *main_buffer;
FBOShader*fbo_shader;

FWorld *f_world;
bool draw_f=false;
bool update_f=true;
bool editor=false;

void gin_set_editor(bool v)
{
    editor=v;
    abs_sdl::set_gui(v);
}

void gin_init_default_camera()
{
    default_cam=new OrbitCamera(18.0f/9.0f,45.0f,0.1f,10000.0f);
    ((OrbitCamera*)default_cam)->look_at(glm::vec3(0.0f,0.0f,0.0f));
    default_cam->set_position(glm::vec3(0.0f,20.0f,-10.0f));

    gin_set_active_camera(default_cam);
}

void gin_set_fdraw(bool v)
{
    draw_f=v;
}

bool gin_get_fdraw()
{
    return draw_f;
}

void gin_update_active_camera()
{
    main_camera->update();
}

void gin_set_active_camera(Camera *camera)
{
    main_camera=camera;
}

Camera *gin_get_active_camera()
{
    return main_camera;
}

Camera *gin_get_default_camera()
{
    return default_cam;
}

void gin_init_renderer()
{
    main_renderer=new Renderer();
}
Renderer *gin_get_main_renderer()
{
    return main_renderer;
}

void gin_init_default_scene()
{
    gin_active_scene=new Scene("default scene");
}
void gin_set_active_scene(Scene *scene)
{
    gin_active_scene=scene;
}

Scene* gin_get_active_scene()
{
    return gin_active_scene;
}

void gin_clear_scene()
{
    gin_active_scene->clear();
}

void gin_run_active_scene()
{
    gin_active_scene->run();
}

void gin_init_asset_manager()
{
    asset_manager=new Asset_manager();
}

Asset_manager *gin_get_asset_manager()
{
    return asset_manager;
}

bool gin_init(int screenW,int screenH,string window_name,bool resize,bool fullscreen)
{

    abs_sdl::sdl_init(screenW,screenH,window_name,resize,fullscreen);
    gin_init_asset_manager();
    gin_init_renderer();

    ///init fbo shader
    main_buffer=new FrameBuffer(gin_get_screen_w(),gin_get_screen_h());
    fbo_shader=new FBOShader("shaders/fbo_vertex.glsl","shaders/fbo_fragment.glsl");
    
    f_world=new FWorld();
    gin_set_editor(false); 

    return true;
}

bool gin_screen_resize()
{
    return abs_sdl::get_screen_resize();
}

FrameBuffer *gin_get_main_frame_buffer()
{
    return main_buffer;
}

int gin_get_screen_w()
{
    return abs_sdl::get_screenW();
}

int gin_get_screen_h()
{
    return abs_sdl::get_screenH();
}

void gin_clear()
{
    gin_clear_scene();
    asset_manager->clear();
    delete asset_manager;

    main_renderer->clear();
    delete main_renderer;

//    delete main_camera;
    main_camera=nullptr;

    delete main_buffer;
    delete fbo_shader;

    delete default_cam;

    delete f_world;

    abs_sdl::sdl_clear();
}


int gin_get_fps()
{
    return abs_sdl::get_fps();
}


void gin_set_vsync(bool i)
{
    abs_sdl::set_vsync(i);
}


float gin_get_delta_time()
{
    return abs_sdl::get_delta_time();
}

void gin_pool_events()
{
    abs_sdl::sdl_pool_events();
}

bool gin_is_window_close_event()
{
    return abs_sdl::sdl_is_window_close_event();
}

bool gin_is_key_pressed(int key)
{
    return abs_sdl::sdl_is_key_pressed_event(key);
}

bool gin_is_mouse_button_down(bool button)
{
    return abs_sdl::sdl_is_mouse_button_down(button);
}

bool gin_is_mouse_movement()
{
    return abs_sdl::sdl_is_mouse_movement();
}

int gin_get_mouse_x()
{
    return abs_sdl::sdl_get_mouse_x();
}

int gin_get_mouse_y()
{
    return abs_sdl::sdl_get_mouse_y();
}

int gin_get_mouse_vertical()
{
    return abs_sdl::get_mouse_vertical();
}

int gin_get_mouse_horizontal()
{
    return abs_sdl::get_mouse_horizontal();
}

void gin_clear_color(float r,float g,float b,float a)
{
    if(update_f)
        f_world->update();

    if(gin_screen_resize())
    {
        main_buffer->clear();
        main_buffer=new FrameBuffer(gin_get_screen_w(),gin_get_screen_h());
        debug_print("FBO RESIZE");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, main_buffer->id);
    glEnable(GL_DEPTH_TEST);
    abs_sdl::opengl_clear_color();
}

void gin_swap_buffer()
{
    if(draw_f)
    {
        f_world->draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    if(editor)
        abs_sdl::draw_gui();
    else
    {
        fbo_shader->use();
        glBindVertexArray(gin_get_asset_manager()->get_model("gin_quad")->get_mesh_at_index(0)->get_VAO());
        glActiveTexture(GL_TEXTURE0+main_buffer->texture);
        fbo_shader->send_int_uniform("fbo_buffer",main_buffer->texture);        
        glBindTexture(GL_TEXTURE_2D, main_buffer->texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    abs_sdl::opengl_swap_buffer();
}


bool gin_mouse_scroll_down()
{
    return abs_sdl::mouse_scroll_down();
}

bool gin_mouse_scroll_up()
{
    return abs_sdl::mouse_scroll_up();
}

bool gin_mouse_scoll_left()
{
    return abs_sdl::mouse_scroll_left();
}

bool gin_mouse_scroll_right()
{
    return abs_sdl::mouse_scroll_right();
}


FWorld* gin_get_fworld()
{
    return f_world;
}

void gin_set_f_update(bool v)
{
    update_f=v;
    f_world->set_all_obj(v);
}

bool gin_get_f_update()
{
    return update_f;
}







