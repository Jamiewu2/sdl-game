#define GLEW_NO_GLU
#include "GL/glew.h"

#include "engine.h"
#include "SDL2/SDL.h"
#include "util/SDLWrapper.h"
#include "fmt/core.h"
#include "input/input.h"
#include "gl/gl_shader.h"

#define _DEBUG
#ifdef _DEBUG
    #include "gl/gl_debug.h"
#endif

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

    GLuint g_uiVAO; //Vertex Array Object, seems to store all the settings (?). Since OpenGL uses global state, binding this lets you context switch
    GLuint g_uiVBO; //Vertex Buffer Object, stores vertex data GPU side
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

    // Create a Vertex Array Object
    glGenVertexArrays(1, &g_uiVAO);
    glBindVertexArray(g_uiVAO);

    // Create VBO data
    GLfloat fVertexData[] =
    {
        -0.5f, 0.5f,
         0.0f, -0.5f,
         0.5f, 0.5f
    };

    // Create Vertex Buffer Object
    glGenBuffers(1, &g_uiVBO); // find 1 unused id to create a buffer, g_uiVBO_id = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, g_uiVBO); //actually create the buffer, of type GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(fVertexData), fVertexData, GL_STATIC_DRAW); // add the data

    // Specify location of data within buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Specify program to use
    glUseProgram(g_uiMainProgram);

    this->gl_context.g_uiVAO = g_uiVAO;
    this->gl_context.g_uiVBO = g_uiVBO;
    this->gl_context.g_uiMainProgram = g_uiMainProgram;

    return true;
}


Engine::Engine(Game& game, int width, int height) {
    const bool g_bWindowFullscreen = false;
    const int WINDOW_FLAGS = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | (g_bWindowFullscreen * SDL_WINDOW_FULLSCREEN);
    const int RENDERER_FLAGS = SDL_RENDERER_ACCELERATED;

    this->app.game = &game;

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
    while (true) {
        prepareScene();
        doInput();
        presentScene();
        SDL_Delay(16);
    }
};

void Engine::shutdown() {
    this->glShutdown();

    SDL_GL_DeleteContext( app.context );
    SDL_DestroyWindow( app.window );
    SDL_Quit();
};

void Engine::prepareScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::presentScene(void)
{
    // Specify VAO to use
    glBindVertexArray(gl_context.g_uiVAO);
    
    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(app.window);
}

void Engine::glShutdown() {
    // Release the shader program
    glDeleteProgram(gl_context.g_uiMainProgram);

    // Delete VBO and VAO
    glDeleteBuffers(1, &gl_context.g_uiVBO);
    glDeleteVertexArrays(1, &gl_context.g_uiVAO);
}