#pragma once

#include "../game.h"

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "gl/gl_scene.h"
#include "render/camera.h"
#include <memory>

class Engine {


    public:
        Engine(Game& game, int width, int height);
        ~Engine();

        void run();
        void shutdown();

        int add_primitive(Primitives::PrimitiveType type, float size);
        int add_mesh();
        Camera create_camera();

    private:
        typedef struct {
            Game* game;
            // SDL_Renderer *renderer;
            SDL_Window *window;
            SDL_GLContext *context;
            int width;
            int height;
        } App;

        typedef struct {
            GLuint g_uiMainProgram;
        } GLPrograms;

        typedef struct {
            GLuint g_uiUBO;
            std::unique_ptr<Camera> camera;
        } GLCamera;

        App app;
        SceneData scene_data;
        GLCamera main_camera;
        GLPrograms gl_programs;

        void prepareScene();
        void presentScene();

        bool glInit();
        void glShutdown();
};