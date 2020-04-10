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

const static unsigned int SCREENWIDTH = 1024, SCREENHEIGHT = 512;

void quit(const std::string& error) {
    std::cout << error << std::endl;
    exit(-1);
}

void CheckSDLError(int line = -1)
{
    std::string error = SDL_GetError();

    if (error != "")
    {
        std::cout << "SLD Error : " << error << std::endl;

        if (line != -1)
            std::cout << "\nLine : " << line << std::endl;

        SDL_ClearError();
    }
}


int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        quit("Failed to init SDL2");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


    auto window = SDL_CreateWindow("Programming",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREENWIDTH,
        SCREENHEIGHT,
        SDL_WINDOW_OPENGL
    );

    //SDL_CaptureMouse(SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    glewInit();


    glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);

    std::cout << glGetString(GL_VERSION) << std::endl;

    std::atomic<bool> keys[0xFF] = { false };
    std::atomic<int> mouse_delta[2] = { 0 };
    
    Game g = Game(SCREENWIDTH, SCREENHEIGHT, keys, mouse_delta);
    bool running = true;

    glEnable(GL_DEPTH_TEST);

    std::thread tick([&]() {
        auto tick_limiter = TimeLimiter(60);
        while (running) {
            g.tick(tick_limiter.dt());

            mouse_delta[0] = 0;
            mouse_delta[1] = 0;
            tick_limiter.sleep();
        }
    });

    int frame_counter = 0;
    const int fps = 60;
    auto frame_limiter = TimeLimiter(fps);
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                }

                if (event.key.keysym.sym > 0xFF) continue;
                keys[(char)event.key.keysym.sym] = true;
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym > 0xFF) continue;
                keys[(char)event.key.keysym.sym] = false;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                if (SDL_GetRelativeMouseMode() == SDL_FALSE) continue;
                mouse_delta[0] = event.motion.xrel;
                mouse_delta[1] = event.motion.yrel;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                }
            }
        }

        if (frame_counter > fps) {
            char title[50];
            snprintf(title, 50, "Programming FPS: %f", frame_limiter.dt() * fps * fps);
            SDL_SetWindowTitle(window, title);
            frame_counter = 0;
        }
        ++frame_counter;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        g.render();

        SDL_GL_SwapWindow(window);
        frame_limiter.sleep();
    }

    tick.join();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}