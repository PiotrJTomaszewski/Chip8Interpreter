#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_memory_editor.h"

#include "system.h"
#include "display_renderer.h"

class GUI {
public:
    GUI(System &system);
    ~GUI();
    void display();
    bool get_should_close();
private:
    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    System &system;
    bool should_close;
    MemoryEditor mem_edit;
    SDL_Window *window;
    SDL_GLContext gl_context;
    ImGuiIO io;
private:
    void handle_events();
    void display_main_menu();
    void display_cpu();
    void display_screen();
    DisplayRenderer display_renderer;
};
