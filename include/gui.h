#ifndef GUI_H
#define GUI_H

#include "config.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


void gui_init(SDL_Window *window,SDL_GLContext  &context);
void gui_clear();
void gui_process_sdl_event(SDL_Event &event);
void gui_new_frame(SDL_Window *window);
void gui_draw();
void gui_rescale();




void gui_test();
#endif
