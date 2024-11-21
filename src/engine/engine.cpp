#define GLEW_NO_GLU
#include "GL/glew.h"

#include "engine.h"
#include "SDL2/SDL.h"
#include "util/SDLWrapper.h"
#include "fmt/core.h"
#include "input/input.h"
#include "gl/gl_shader.h"
#include <magic_enum/magic_enum.hpp>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

#define _DEBUG

#ifdef _DEBUG
    #include "gl/gl_debug.h"
#endif

int Engine::add_primitive(Primitives::PrimitiveType type, float size) {
    switch (type) {
        case Primitives::CUBE:
            {
                GLsizei size_tmp = Primitives::glCreateCube(scene_data);
                return size_tmp;
            }
        default:
            throw std::runtime_error(fmt::format("Unknown Type: {}", magic_enum::enum_name(type)));
            break;
    }
}

Camera Engine::create_camera() {
    float aspect_ratio = (float)app.width/(float)app.height;
    // std::cout << aspect_ratio << std::endl;

    CameraData camera_data {
        .position = vec3(0.0f, 0.0f, 12.0f),
        .direction = vec3(0.0f, 0.0f, -1.0f),
        .up_vec = normalize(vec3(1.0f, 1.0f, 0.0f)),
        .fov = 45.0f,
        .aspect = aspect_ratio,
        .near = 0.1f,
        .far = 100.0f
    };

    return Camera(camera_data);
}


bool Engine::glInit() {

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
    }

    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    #ifdef _DEBUG
        // Initialise debug call-back
        glDebugInit();
    #endif

    //Use Vsync
    if( SDL_GL_SetSwapInterval( 0 ) < 0 )
    {
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }

    // Set up initial GL attributes
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the cleared back buffer to black
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Set the cleared back buffer to blue
    glCullFace(GL_BACK);                  // Set back-face culling
    glEnable(GL_CULL_FACE);               // Enable use of back/front face culling
    glEnable(GL_DEPTH_TEST);              // Enable use of depth testing
    glDisable(GL_STENCIL_TEST);           // Disable stencil test for speed

    // GLuint g_uiVAO; //Vertex Array Object, seems to store all the settings (?). Since OpenGL uses global state, binding this lets you context switch
    // GLuint g_uiVBO; //Vertex Buffer Object, stores vertex data GPU side
    // GLuint g_uiIBO; //Index Buffer Object, stores index data connecting vertices in groups of 3 to create triangles
    GLuint g_uiMainProgram;

    // Create vertex shader
    GLuint uiVertexShader;
    if (!GL_LoadShaderFile(uiVertexShader, GL_VERTEX_SHADER, "shaders/tutorial1_vert.glsl", 100))
        return false;
        
    // Create fragment shader
    GLuint uiFragmentShader;
    if (!GL_LoadShaderFile(uiFragmentShader, GL_FRAGMENT_SHADER, "shaders/tutorial1_frag.glsl", 200))
        return false;

    // Create program
    if (!GL_LoadShaders(g_uiMainProgram, uiVertexShader, uiFragmentShader))
        return false;

    // Clean up unneeded shaders
    glDeleteShader(uiVertexShader);
    glDeleteShader(uiFragmentShader);

    // Specify program to use
    glUseProgram(g_uiMainProgram);
    this->gl_programs.g_uiMainProgram = g_uiMainProgram;

    //tmp stuff
    add_primitive(Primitives::CUBE, 1.0f);

    //create camera
    this->main_camera.camera = std::make_unique<Camera>(create_camera());
    GLuint uiUBO = glCreateCamera(*(main_camera.camera));
    main_camera.g_uiUBO = uiUBO;
    // assume camera never changes
    // Bind camera UBO
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uiUBO);

    return true;
}


Engine::Engine(Game& game, int width, int height) {
    const bool g_bWindowFullscreen = false;
    const int WINDOW_FLAGS = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | (g_bWindowFullscreen * SDL_WINDOW_FULLSCREEN);
    const int RENDERER_FLAGS = SDL_RENDERER_ACCELERATED;

    this->app.game = &game;
    this->app.width = width;
    this->app.height = height;

    //Initialize SDL
    SDL_Init(SDL_INIT_VIDEO)
        .expect([]{ return fmt::format("SDL could not initialize! SDL_Error: {}", SDL_GetError()); });

    //Use OpenGL 4.6 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    // Turn on double buffering with a 24bit Z buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    #ifdef _DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    #endif

    //Create window
    this->app.window = SDL_CreateWindow( game.name().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, WINDOW_FLAGS )
        .expect([]{ return fmt::format("Window could not be created! SDL_Error: {}", SDL_GetError()); });

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    SDL_GLContext g_context = SDL_GL_CreateContext(app.window);
    if (g_context == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return;
    }

    //Initialize OpenGL
    if( !glInit() )
    {
        printf( "Unable to initialize OpenGL!\n" );
        return;
    }
}

Engine::~Engine() {
    this->shutdown();
}

void Engine::run() {
    Input input = Input();

    Uint32 old_elapsed = SDL_GetTicks();
    while (true) {
        Uint32 current_elapsed = SDL_GetTicks();
        float delta = (float)(current_elapsed - old_elapsed) / 1000.0f;
        prepareScene();
        input.reset_frame();
        input.doInput();
        update(input, delta);
        presentScene();
        SDL_Delay(16);

        old_elapsed = current_elapsed;
    }
};

void Engine::update(Input input, float delta) {
    const float SPEED = 10.0f;

    if (main_camera.camera) {
        if (input.isPressed(PlayerInputEvent::CAMERA_UP)) {
            main_camera.camera->translate(vec3{0.0f, SPEED * delta, 0.0f});
        }

        if (input.isPressed(PlayerInputEvent::CAMERA_DOWN)) {
            main_camera.camera->translate(vec3{0.0f, -SPEED * delta, 0.0f});
        }

        if (input.isPressed(PlayerInputEvent::CAMERA_LEFT)) {
            main_camera.camera->translate(vec3{-SPEED * delta, 0.0f, 0.0f});
        }

        if (input.isPressed(PlayerInputEvent::CAMERA_RIGHT)) {
            main_camera.camera->translate(vec3{SPEED * delta, 0.0f, 0.0f});
        }
    }
}

void Engine::prepareScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::presentScene(void)
{
    // Update main camera buffers
    if (main_camera.camera) {
        glBindCamera(*main_camera.camera, main_camera.g_uiUBO);
    }

    // Specify VAO to use
    for (unsigned i = 0; i < scene_data.mp_Meshes.size(); i++) {
        glBindVertexArray(scene_data.mp_Meshes[i].m_uiVAO);
        // std::cout << fmt::format("{}, {}, {}, {}", scene_data.mp_Meshes[0].m_uiVAO, scene_data.mp_Meshes[0].m_uiVBO, scene_data.mp_Meshes[0].m_uiIBO, scene_data.mp_Meshes[0].m_uiNumIndices) << std::endl;
        // std::cout << glm::to_string(this->main_camera->asViewProjectionMatrix()) << std::endl;
    
        // Draw the triangle
        glDrawElements(GL_TRIANGLES, scene_data.mp_Meshes[i].m_uiNumIndices, GL_UNSIGNED_INT, 0);
    }
    
    SDL_GL_SwapWindow(app.window);
}

void Engine::shutdown() {
    this->glShutdown();

    SDL_GL_DeleteContext( app.context );
    SDL_DestroyWindow( app.window );
    SDL_Quit();
};

void Engine::glShutdown() {
    // Release the shader program
    glDeleteProgram(gl_programs.g_uiMainProgram);

    // Delete Camera
    if (main_camera.camera) {
        glDeleteBuffers(1, &main_camera.g_uiUBO);
    }

    // Delete VBO and VAO, and IBO
    for (unsigned i = 0; i < scene_data.mp_Meshes.size(); i++) {
        glDeleteBuffers(1, &scene_data.mp_Meshes[i].m_uiVBO);
        glDeleteBuffers(1, &scene_data.mp_Meshes[i].m_uiIBO);
        glDeleteVertexArrays(1, &scene_data.mp_Meshes[i].m_uiVAO);
    }
}