#include <iostream>
#include <vector>
#include <string>
#include <expected>

#include <fmt/core.h>
#include "SDL2/SDL.h"

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
   // vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!", "and CMake"};

   // for (const string& word : msg)
   // {
   //    cout << word << " ";
   // }
   // cout << endl;

   // SDL_Init(SDL_INIT_EVERYTHING);


   //The window we'll be rendering to
   SDL_Window* window = nullptr;
   
   //The surface contained by the window
   SDL_Surface* screenSurface = nullptr;

   //Initialize SDL
   if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
      throw std::runtime_error(fmt::format("SDL could not initialize! SDL_Error: {}", SDL_GetError()));

   //Create window
   window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
   if( window == NULL )
      throw std::runtime_error(fmt::format("Window could not be created! SDL_Error: %s\n", SDL_GetError()));
      
   //Get window surface
   screenSurface = SDL_GetWindowSurface( window );

   //Fill the surface white
   SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
   
   //Update the surface
   SDL_UpdateWindowSurface( window );

   //Hack to get window to stay up
   SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }

   //Destroy window
   SDL_DestroyWindow( window );

   //Quit SDL subsystems
   SDL_Quit();

   return 0;
   
}