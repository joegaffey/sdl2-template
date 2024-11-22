#include <SDL2/SDL.h>
#include <iostream>

int WIN_WIDTH = 800;
int WIN_HEIGHT = 600;

SDL_Renderer* renderer;
SDL_Rect imageRect;
SDL_Texture* texture;
SDL_Surface* winSurface;
SDL_GameController* controller;
        
void performActionOnButtonPress(SDL_GameControllerButton button) {
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A: imageRect.y--; break;
        case SDL_CONTROLLER_BUTTON_B: imageRect.x--; break;
        case SDL_CONTROLLER_BUTTON_X: imageRect.x++; break;
        case SDL_CONTROLLER_BUTTON_Y: imageRect.y++; break;
    }
}

void performActionOnKeyInput(SDL_Event ev) {
    switch (ev.key.keysym.sym) {
        case SDLK_UP: imageRect.y--; break;
        case SDLK_LEFT: imageRect.x--; break;
        case SDLK_RIGHT: imageRect.x++; break;
        case SDLK_DOWN: imageRect.y++; break;
        case SDLK_w: imageRect.y--; break; 
        case SDLK_a: imageRect.x--; break;
        case SDLK_d: imageRect.x++; break;
        case SDLK_s: imageRect.y++; break;
    }
}

void paint() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, &imageRect);
    SDL_RenderPresent(renderer);
}

int main(int argc, char const *argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK != 0))
        std::cout << "Error: " << SDL_GetError();
    else 
    {
        int numJoy = SDL_NumJoysticks();
        std::cout << numJoy << " joysticks detected.\n";

        for (int i = 0; i < numJoy; ++i) {
            if (SDL_IsGameController(i)) {
                controller = SDL_GameControllerOpen(i);
                if (controller) {
                    std::cout << "Controller connected: \n" << SDL_GameControllerName(controller);
                    break;
                } else {
                    std::cerr << "Could not open game controller %i: \n" << i << SDL_GetError();
                }
            }
        }

        if (!controller) {
            std::cout << "No game controllers found.\n";
        }

        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        
        SDL_Window* window = SDL_CreateWindow("SDL2 Template", 
                                              (DM.w / 2) - (WIN_WIDTH / 2), 
                                              (DM.h / 2) - (WIN_HEIGHT / 2), 
                                              WIN_WIDTH, 
                                              WIN_HEIGHT, 
                                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return -1;
        }

        SDL_Surface* imageSurface = SDL_LoadBMP("sdl.bmp");
        if (!imageSurface) {
            std::cout << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        }
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        imageRect = {(WIN_WIDTH - imageSurface->w) / 2, (WIN_HEIGHT - imageSurface->h) / 2, imageSurface->w, imageSurface->h};
        SDL_FreeSurface(imageSurface);

        int running = 1;
        
        while(running) 
        {
            SDL_Event event;
            SDL_PollEvent(&event);
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    performActionOnButtonPress((SDL_GameControllerButton)event.cbutton.button);
                    break;
                case SDL_KEYDOWN:
                    performActionOnKeyInput(event);
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        WIN_WIDTH = event.window.data1;
                        WIN_HEIGHT = event.window.data2;
                        imageRect = {(WIN_WIDTH - imageRect.w) / 2, (WIN_HEIGHT - imageRect.h) / 2, imageRect.w, imageRect.h};
                    }
                    break;
            }
            paint();
        }

        SDL_DestroyWindow(window);
    }
    if (controller) {
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
    return 0;
}