#include "input.h"

void Input::doInput(void)
{
    if (is_new_frame) {
        for (auto& state: player_just_pressed_state) 
        {
            state.second = false;
        }

        for (auto& state: player_just_released_state) 
        {
            state.second = false;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
                switch (event.type)
                {
                    case SDL_QUIT:
                        exit(0);
                        break;

                    case SDL_KEYDOWN:
                    {
                        SDL_Keycode key = event.key.keysym.sym;
                        if (player_input_map.count(key)) {
                            PlayerInputEvent player_event = player_input_map[key];
                            player_input_state[player_event] = true;
                            player_just_pressed_state[player_event] = true;
                        }
                        break;
                    }
                    case SDL_KEYUP:
                    {
                        SDL_Keycode key = event.key.keysym.sym;
                        if (player_input_map.count(key)) {
                            PlayerInputEvent player_event = player_input_map[key];
                            player_input_state[player_event] = false;
                            player_just_released_state[player_event] = true;
                        }
                        break;
                    }
                    default:
                        break;
            }
        }
        is_new_frame = false;
    }
}

void Input::reset_frame() {
    this->is_new_frame = true;
}

bool Input::isPressed(PlayerInputEvent input_event) {
    return player_input_state[input_event];
}

bool Input::isJustPressed(PlayerInputEvent input_event) {
    return player_just_pressed_state[input_event];
}

bool Input::isJustReleased(PlayerInputEvent input_event) {
    return player_just_released_state[input_event];
}