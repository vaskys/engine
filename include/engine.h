#ifndef ENGINE_H
#define ENGINE_H

#include "config.h"
#include "abs_sdl.h"
#include "asset_manager.h"
#include "shader.h"
#include "renderer.h"
#include "first_person_camera.h"
#include "orbit_camera.h"
#include "scene.h"

#include "framebuffer.h"
#include "FBOShader.h"
#include "mesh_node.h"
#include "model_node.h"
#include "fyzika.h"

FrameBuffer *gin_get_main_frame_buffer();

int gin_get_screen_w();
int gin_get_screen_h();

//neskor pridaj window tagy 
bool gin_init(int screenW,int screenH,string window_name,bool resize,bool fullscreen);
bool gin_screen_resize();
void gin_clear();
void gin_pool_events();
float gin_get_delta_time();
int gin_get_fps();
void gin_set_vsync(bool i);

bool gin_is_window_close_event();
bool gin_is_key_pressed(int key);
bool gin_is_mouse_button_down(bool button);
bool gin_is_mouse_movement();


int gin_get_mouse_x();
int gin_get_mouse_y();

void gin_set_editor(bool v);

void gin_clear_color(float r,float g,float b,float a);
void gin_swap_buffer();

int gin_get_mouse_vertical();
int gin_get_mouse_horizontal();

bool gin_mouse_scroll_down();
bool gin_mouse_scroll_up();
bool gin_mouse_scroll_left();
bool gin_mouse_scroll_right();


void gin_init_default_scene();
Scene *gin_get_active_scene();
void gin_set_active_scene(Scene *scene);
void gin_clear_scene();
void gin_run_active_scene();

Asset_manager* gin_get_asset_manager();
Renderer* gin_get_main_renderer();

void gin_update_active_camera();
void gin_init_default_camera();
void gin_set_active_camera(Camera *camera);
Camera *gin_get_active_camera();
Camera *gin_get_default_camera();

void gin_set_fdraw(bool v);
bool gin_get_fdraw();

void gin_set_f_update(bool v);
bool gin_get_f_update();
FWorld *gin_get_fworld();

#endif
