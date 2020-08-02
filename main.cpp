#include <iostream>

#include <thread>
#include <mutex>
#include <functional>
#include <chrono>
#include <cstdio>

#include "SDL.h"
#include "GL/glew.h"

#include "Game.h"
#include "TimeLimiter.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

const static size_t SCREENWIDTH = 1000, SCREENHEIGHT = 1000;

void quit(const std::string& error) {
    std::cout << error << std::endl;
    exit(-1);
}

void CheckSDLError(int line = -1)
{
    std::string error = SDL_GetError();

    if (error != "") {
        std::cout << "SLD Error : " << error << std::endl;

        if (line != -1) {
            std::cout << "\nLine : " << line << std::endl;
        }

        SDL_ClearError();
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        quit("Failed to init SDL2");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

   
    auto window = SDL_CreateWindow("Programming",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREENWIDTH,
        SCREENHEIGHT,
        SDL_WINDOW_OPENGL
    );

    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    glewInit();
    glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Setup Dear ImGUI 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
   
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 440");

    std::atomic<bool> keys[0xFF] = { false };
    std::atomic<int> mouse_delta[2] = { 0 };
    
    Game g = Game(SCREENWIDTH, SCREENHEIGHT, keys, mouse_delta);
    bool running = true;

    int frame_counter = 0;
    const int fps = 60;
  
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double delta_time = 0;

    while (running) {
        last = now;
        now = SDL_GetPerformanceCounter();
        delta_time = (double)((now - last) / (double)SDL_GetPerformanceFrequency());

        auto current_mouse_mode = SDL_GetRelativeMouseMode() == SDL_TRUE;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) { 
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_SetRelativeMouseMode((SDL_bool)(!current_mouse_mode));
                    current_mouse_mode = !current_mouse_mode;
                    for (size_t i = 0; i < 0xFF; i++) keys[i] = false;
                }

                if (!current_mouse_mode) continue;
                if (event.key.keysym.sym > 0xFF) continue;
                keys[(char)event.key.keysym.sym] = true;
            }
            else if (event.type == SDL_KEYUP) {

                if (!current_mouse_mode) continue;
                if (event.key.keysym.sym > 0xFF) continue;
                keys[(char)event.key.keysym.sym] = false;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                if (!current_mouse_mode) continue;
                mouse_delta[0] = event.motion.xrel;
                mouse_delta[1] = event.motion.yrel;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    //SDL_SetRelativeMouseMode(SDL_TRUE);
                }
            }
        }

        if (frame_counter > fps) {
            char title[50];
            snprintf(title, 50, "Programming FPS: %f", (float)(1/delta_time));
            SDL_SetWindowTitle(window, title);
            frame_counter = 0;
        }
        ++frame_counter;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        g.tick(static_cast<float>(delta_time));
        g.render(SDL_GetRelativeMouseMode() == SDL_FALSE);

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);

        mouse_delta[0] = 0;
        mouse_delta[1] = 0;
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}