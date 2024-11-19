#pragma once

#include "../game.h"

#include "GL/glew.h"
#include "SDL2/SDL.h"


class Engine {


    public:
        Engine(Game& game, int width, int height);
        ~Engine();

        void run();
        void shutdown();

    private:
        typedef struct {
            Game* game;
            // SDL_Renderer *renderer;
            SDL_Window *window;
            SDL_GLContext *context;
        } App;

        typedef struct {
            GLuint g_uiVAO;
            GLuint g_uiVBO;
            GLuint g_uiMainProgram;
        } GLContext;

        App app;
        GLContext gl_context;

        void prepareScene();
        void presentScene();

        bool glInit();
        void glShutdown();
};