#include "ImGuiFileDialog.h"
#include "gui.h"

GUI::GUI(System &system): system(system), display_renderer(system.get_io().display) {
    // From https://github.com/ocornut/imgui/blob/master/examples/example_sdl_opengl3/main.cpp
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        // TODO: Throw exception
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    should_close = false;
    mem_edit.ReadOnly = true;
}

GUI::~GUI() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GUI::display() {
    std::string file_path;

    handle_events();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    display_main_menu();
    display_cpu();
    mem_edit.DrawWindow("Stack", system.get_cpu().get_stack(), system.get_cpu().get_stack_size());
    mem_edit.DrawWindow("Memory", system.get_memory().get_memory_raw(), system.get_memory().get_memory_size());
    display_screen();
    display_io();

    if (ImGuiFileDialog::Instance()->Display("ROMKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            file_path = ImGuiFileDialog::Instance()->GetFilePathName();
            system.load_rom_file(file_path);
            system.restart();
        }
        ImGuiFileDialog::Instance()->Close();
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}


bool GUI::get_should_close() {
    return should_close;
}

void GUI::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            should_close = true;
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
            should_close = true;
        } else if (event.type == SDL_KEYDOWN) {
            auto &keypad = system.get_io().keypad;
            switch (event.key.keysym.sym) {
                case SDLK_1:
                    keypad.set_pressed(0x1);
                    break;
                case SDLK_2:
                    keypad.set_pressed(0x2);
                    break;
                case SDLK_3:
                    keypad.set_pressed(0x3);
                    break;
                case SDLK_4:
                    keypad.set_pressed(0xC);
                    break;
                case SDLK_q:
                    keypad.set_pressed(0x4);
                    break;
                case SDLK_w:
                    keypad.set_pressed(0x5);
                    break;
                case SDLK_e:
                    keypad.set_pressed(0x6);
                    break;
                case SDLK_r:
                    keypad.set_pressed(0xD);
                    break;
                case SDLK_a:
                    keypad.set_pressed(0x7);
                    break;
                case SDLK_s:
                    keypad.set_pressed(0x8);
                    break;
                case SDLK_d:
                    keypad.set_pressed(0x9);
                    break;
                case SDLK_f:
                    keypad.set_pressed(0xE);
                    break;
                case SDLK_z:
                    keypad.set_pressed(0xA);
                    break;
                case SDLK_x:
                    keypad.set_pressed(0x0);
                    break;
                case SDLK_c:
                    keypad.set_pressed(0xB);
                    break;
                case SDLK_v:
                    keypad.set_pressed(0xF);
                    break;
                default:
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            auto &keypad = system.get_io().keypad;
            switch (event.key.keysym.sym) {
                case SDLK_1:
                    keypad.set_released(0x1);
                    break;
                case SDLK_2:
                    keypad.set_released(0x2);
                    break;
                case SDLK_3:
                    keypad.set_released(0x3);
                    break;
                case SDLK_4:
                    keypad.set_released(0xC);
                    break;
                case SDLK_q:
                    keypad.set_released(0x4);
                    break;
                case SDLK_w:
                    keypad.set_released(0x5);
                    break;
                case SDLK_e:
                    keypad.set_released(0x6);
                    break;
                case SDLK_r:
                    keypad.set_released(0xD);
                    break;
                case SDLK_a:
                    keypad.set_released(0x7);
                    break;
                case SDLK_s:
                    keypad.set_released(0x8);
                    break;
                case SDLK_d:
                    keypad.set_released(0x9);
                    break;
                case SDLK_f:
                    keypad.set_released(0xE);
                    break;
                case SDLK_z:
                    keypad.set_released(0xA);
                    break;
                case SDLK_x:
                    keypad.set_released(0x0);
                    break;
                case SDLK_c:
                    keypad.set_released(0xB);
                    break;
                case SDLK_v:
                    keypad.set_released(0xF);
                    break;
                default:
                    break;
            }
        }
    }
}

void GUI::display_main_menu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("ROM")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) {
                ImGuiFileDialog::Instance()->OpenDialog("ROMKey", "Choose a ROM file", ".ch8", ".");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void GUI::display_cpu() {
    ImGui::Begin("CPU");
    auto &cpu = system.get_cpu();
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "General Registers");
    if (ImGui::BeginTable("#gen_reg_table", 2)) {
        for (int id = 0; id < cpu.get_general_reg_count(); ++id) {
            ImGui::TableNextColumn();
            ImGui::Text("%1X: 0x%02X", id, cpu.get_general_reg(id));
        }
        ImGui::EndTable();
    }
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Other Registers");
    ImGui::Text("PC: 0x%04X", cpu.get_pc());
    ImGui::Text("SP: 0x%02X", cpu.get_sp());
    ImGui::Text("Memory addr: 0x%04X", cpu.get_mem_addr_reg());
    ImGui::End();
}

void GUI::display_screen() {
    const int SCREEN_SCALE = 10;
    ImGui::Begin("Screen");
    display_renderer.render_if_dirty();
    int width = display_renderer.get_width() * SCREEN_SCALE;
    int height = display_renderer.get_height() * SCREEN_SCALE;
    ImGui::Image(reinterpret_cast<ImTextureID>(display_renderer.get_texture()), ImVec2(width, height));
    ImGui::End();
}

void GUI::display_io() {
    auto &timers = system.get_io().timers;
    auto &keypad = system.get_io().keypad;
    int key_id;

    ImGui::Begin("IO");
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Timers");
    ImGui::Text("Delay: 0x%02X", timers.get_delay_timer());
    ImGui::Text("Sound: 0x%02X", timers.get_sound_timer());

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Keypad");
    if (ImGui::BeginTable("#keypad_table", 4)) {
        for (int i = 0; i < 4; ++i) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 10.f);
        }

        for (int i = 0; i < keypad.get_key_count(); ++i) {
            key_id = keypad_display_order[i];
            ImGui::TableNextColumn();
            if (keypad.get_is_pressed(key_id)) {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, active_key_bg_color);
                ImGui::TextColored(active_key_fg_color, "%01X", key_id);
            } else {
                ImGui::Text("%01X", key_id);
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}
