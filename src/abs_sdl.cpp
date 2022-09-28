#include "abs_sdl.h"
#include "gui.h"

SDL_Window *window;
SDL_Event event;
SDL_GLContext context;

int mouse_x=0;
int mouse_y=0;

int last_mouse_x=0;
int last_mouse_y=0;

bool left_mouse_press=false;
bool right_mouse_press=false;

short scroll_y=-1;
short scroll_x=-1;

bool KEYS[322];
bool window_close;

bool enable_gui=true;
bool render_gui=true;

float delta_time=0;
float ticks=0;

float last_time;
int local_FPS=0;
int FPS=0;

int l_screenW;
int l_screenH;

bool screen_resize=false;

bool abs_sdl::sdl_init(int screenW,int screenH,string title,bool resize,bool fullscreen)
{
    l_screenW=screenW;
    l_screenH=screenH;

    if(SDL_Init(SDL_INIT_EVERYTHING) !=0)
    {
        debug_print("SDL INIT ERROR");
        return false;
    }
    
    if(screenW == 0 || screenH == 0)
    {
        SDL_DisplayMode dm;
        if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
        {
             SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
             return 1;
        }
        screenW=dm.w;
        screenH=dm.h;
    }
    int w_flags = SDL_WINDOW_OPENGL;
    if(resize)
        w_flags |= SDL_WINDOW_RESIZABLE;
    if(fullscreen)
        w_flags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,screenW,screenH,w_flags);
    if(window == nullptr)
    {
        debug_print("SDL WINDOW ERROR");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 2);

    context = SDL_GL_CreateContext(window);
    glEnable(GL_DEPTH_TEST);

    for(int i=0;i<322;i++)
        KEYS[i]=false;

    window_close=false;

    glViewport(0,0,screenW,screenH);

    if(enable_gui)
        gui_init(window,context);
   
    last_time=SDL_GetTicks();
    set_vsync(false);
    return true;
}

void abs_sdl::sdl_clear()
{
    if(enable_gui)
        gui_clear();
    debug_print("SDL CLEANUP");
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

int abs_sdl::get_fps()
{
    return FPS;
}

int abs_sdl::get_screenW()
{
    return l_screenW;
}

int abs_sdl::get_screenH()
{
    return l_screenH;
}

bool abs_sdl::get_screen_resize()
{
    return screen_resize;
}

void abs_sdl::set_vsync(bool i)
{
    SDL_GL_SetSwapInterval(i);
}

float abs_sdl::get_delta_time()
{
    return delta_time;
}
//sdl eventy
void abs_sdl::sdl_pool_events()
{
    screen_resize=false;
    bool ok=true;
    scroll_y=-1;
    scroll_x=-1;

    delta_time=(SDL_GetTicks() - ticks) / 1000.0f;
    ticks=SDL_GetTicks();
    local_FPS++;

    if((ticks - last_time) > 1.000f)
    {
        FPS=local_FPS;
        local_FPS=0;
        last_time+= 1000.0f;
    }

    while(SDL_PollEvent(&event) && ok )
    {

        SDL_GetMouseState(&mouse_x,&mouse_y);
        if(enable_gui)
            gui_process_sdl_event(event);

        switch(event.type)
        {
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    l_screenW=event.window.data1;
                    l_screenH=event.window.data2;
                    glViewport(0, 0,l_screenW,l_screenH);
                    screen_resize=true;
                }
                break;
            case SDL_QUIT:
                window_close=true;
                ok=false;
                break;
            case SDL_KEYDOWN:
                //debug_print(to_string(event.key.keysym.sym));
                if(event.key.keysym.sym<322)
                    KEYS[event.key.keysym.sym]=true;
                break;
            case SDL_KEYUP:
                if(event.key.keysym.sym<322)
                    KEYS[event.key.keysym.sym]=false;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button==SDL_BUTTON_LEFT)
                    left_mouse_press=true;
                else if(event.button.button==SDL_BUTTON_RIGHT)
                    right_mouse_press=true;
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button==SDL_BUTTON_LEFT)
                    left_mouse_press=false;
                else if(event.button.button==SDL_BUTTON_RIGHT)
                    right_mouse_press=false;
                break;
            case SDL_MOUSEWHEEL:
                if(event.wheel.y>0)
                    scroll_y=0; //hore
                else if(event.wheel.y<0)
                    scroll_y=1; //hole
                if(event.wheel.x>0)
                    scroll_x=0; //lava
                else if(event.wheel.x<0)
                    scroll_x=1; //prava
                break;
            default:
                break;

        }
    }
}

bool abs_sdl::sdl_is_window_close_event()
{
    return window_close;
}

bool abs_sdl::sdl_is_key_pressed_event(int key)
{
    return KEYS[key];
}

bool abs_sdl::sdl_is_mouse_button_down(bool button)
{
    if(button)
        return left_mouse_press;
    else
        return right_mouse_press;
}

bool abs_sdl::sdl_is_mouse_movement()
{
    if(event.type==SDL_MOUSEMOTION)
    {
        return true;
    }

    return false;
}

int abs_sdl::sdl_get_mouse_x()
{
    return mouse_x;
}

int abs_sdl::sdl_get_mouse_y()
{
    return mouse_y;
}

int abs_sdl::get_mouse_horizontal()
{
    int offset=abs_sdl::sdl_get_mouse_x()-last_mouse_x;
    last_mouse_x=abs_sdl::sdl_get_mouse_x();
    return offset;
}

int abs_sdl::get_mouse_vertical()
{
    int offset=abs_sdl::sdl_get_mouse_y()-last_mouse_y;
    last_mouse_y=abs_sdl::sdl_get_mouse_y();
    return offset;

}

bool abs_sdl::mouse_scroll_down()
{
    if(scroll_y==1)
        return true;
    return false;
}

bool abs_sdl::mouse_scroll_up()
{
    if(scroll_y==0)
        return true;
    return false;
}

bool abs_sdl::mouse_scroll_left()
{
    if(scroll_x==1)
        return true;
    return false;
}

bool abs_sdl::mouse_scroll_right()
{
    if(scroll_x==0)
        return true;
    return false;
}

void abs_sdl::opengl_clear_color()
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void abs_sdl::draw_gui()
{
    if(enable_gui && render_gui)
    {
        gui_new_frame(window);
        gui_draw();
    }
}


void abs_sdl::opengl_swap_buffer()
{
    SDL_GL_SwapWindow(window);
}
void abs_sdl::set_gui(bool input)
{
    render_gui=input;
    enable_gui=input;
}

