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
    const int keypad_display_order[16] = {
        0x1, 0x2, 0x3, 0xC,
        0x4, 0x5, 0x6, 0xD,
        0x7, 0x8, 0x9, 0xE,
        0xA, 0x0, 0xB, 0xF
    };
    static const ImU32 active_key_bg_color = 0xFFFFFFFF;
    const ImVec4 active_key_fg_color = ImVec4(0, 0, 0, 1);
    System &system;
    bool should_close;
    MemoryEditor mem_edit;
    SDL_Window *window;
    SDL_GLContext gl_context;
    ImGuiIO io;
    DisplayRenderer display_renderer;
private:
    void handle_events();
    void display_main_menu();
    void display_cpu();
    void display_screen();
    void display_io();
};
