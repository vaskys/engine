#ifndef ABS_SDL_H
#define ABS_SDL_H

#include "config.h"
//abstrackia sdl2

namespace abs_sdl
{
    bool sdl_init(int screenW,int screenH,string title,bool resize,bool fullscreen);
    void sdl_clear();
    void sdl_pool_events();

    bool sdl_is_window_close_event();
    bool sdl_is_key_pressed_event(int key);
    bool sdl_is_mouse_button_down(bool button);
    
    bool sdl_is_mouse_movement();

    int sdl_get_mouse_x();
    int sdl_get_mouse_y();

    void opengl_clear_color();
    void opengl_swap_buffer();

    int get_mouse_vertical();
    int get_mouse_horizontal();

    bool mouse_scroll_down();
    bool mouse_scroll_up();
    bool mouse_scroll_left();
    bool mouse_scroll_right();

    void set_gui(bool input);
    void draw_gui();


    float get_delta_time();
    int get_fps();

    void set_vsync(bool i);

    int get_screenW();
    int get_screenH();
    bool get_screen_resize();
};




#endif
