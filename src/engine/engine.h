#pragma once

#include "../game.h"

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "gl/gl_scene.h"

class Engine {


    public:
        Engine(Game& game, int width, int height);
        ~Engine();

        void run();
        void shutdown();

        int add_primitive(Primitives::PrimitiveType type, float size);
        int add_mesh();

    private:
        typedef struct {
            Game* game;
            // SDL_Renderer *renderer;
            SDL_Window *window;
            SDL_GLContext *context;
        } App;

        typedef struct {
            GLuint g_uiMainProgram;
        } GLPrograms;

        App app;
        SceneData scene_data;
        GLPrograms gl_programs;

        void prepareScene();
        void presentScene();

        bool glInit();
        void glShutdown();
};

