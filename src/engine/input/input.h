#pragma once

#include "SDL2/SDL.h"
#include <unordered_map>

// void doInput(void)
// {
//     SDL_Event event;

//     while (SDL_PollEvent(&event))
//     {
//         switch (event.type)
//         {
//             case SDL_QUIT:
//                 exit(0);
//                 break;

//             case SDL_KEYDOWN:

//                 break;

//             default:
//                 break;
//         }
//     }
// }

//Ideally this would be decoupled from the engine code but I'm not sure how to do that
enum PlayerInputEvent {
    CAMERA_UP,
    CAMERA_DOWN,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    QUIT_GAME
};

class Input {
    public:
        std::unordered_map<SDL_Keycode, PlayerInputEvent> player_input_map = {
            {SDLK_KP_8, CAMERA_UP},
            {SDLK_KP_2, CAMERA_DOWN},
            {SDLK_KP_4, CAMERA_LEFT},
            {SDLK_KP_6, CAMERA_RIGHT}
        };

        std::unordered_map<PlayerInputEvent, bool> player_input_state;
        std::unordered_map<PlayerInputEvent, bool> player_just_pressed_state;
        std::unordered_map<PlayerInputEvent, bool> player_just_released_state;

        void doInput();
        void reset_frame();

        bool isPressed(PlayerInputEvent input_event);
        bool isJustPressed(PlayerInputEvent input_event);
        bool isJustReleased(PlayerInputEvent input_event);

    private:
        bool is_new_frame = false;

};