#pragma once

//Wrap SDL calls for saner error handling
#define SDL_Init(...) ExpectedWrapper(SDL_Init(__VA_ARGS__))
#define SDL_CreateWindow(...) ExpectedWrapperNull<SDL_Window>(SDL_CreateWindow(__VA_ARGS__))