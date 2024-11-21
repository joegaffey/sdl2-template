#include <SDL2/SDL.h>
#include <iostream>

int WIN_WIDTH = 800;
int WIN_HEIGHT = 600;

SDL_Rect offsetRect;

SDL_Surface* winSurface = nullptr;
SDL_GameController* controller = nullptr;
        
void performActionOnButtonPress(SDL_GameControllerButton button) {
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A:
            offsetRect.y--;
            break;
        case SDL_CONTROLLER_BUTTON_B:
            offsetRect.x--;
            break;
        case SDL_CONTROLLER_BUTTON_X:
            offsetRect.x++;
            break;
        case SDL_CONTROLLER_BUTTON_Y:
            offsetRect.y++;
            break;
    }
}

void performActionOnKeyInput(SDL_Event ev) {
    switch (ev.key.keysym.sym) {
        case SDLK_UP:
            offsetRect.y--;
            break;
        case SDLK_LEFT:
            offsetRect.x--;
            break;
        case SDLK_RIGHT:
            offsetRect.x++;
            break;
        case SDLK_DOWN:
            offsetRect.y++;
            break;
        case SDLK_w:
            offsetRect.y--;
            break;
        case SDLK_a:
            offsetRect.x--;
            break;
        case SDLK_d:
            offsetRect.x++;
            break;
        case SDLK_s:
            offsetRect.y++;
            break;
    }
}

void paint(SDL_Surface* winSurface, SDL_Surface* imageSurface, SDL_Window* window) {
    winSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(winSurface, nullptr, SDL_MapRGB(winSurface -> format, 255, 255, 255));
    SDL_BlitSurface(imageSurface, nullptr, winSurface, &offsetRect);
    SDL_UpdateWindowSurface(window);
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
        
        SDL_Window* window = SDL_CreateWindow("SDL2 Test", 
                                              (DM.w / 2) - (WIN_WIDTH / 2), 
                                              (DM.h / 2) - (WIN_HEIGHT / 2), 
                                              WIN_WIDTH, 
                                              WIN_HEIGHT, 
                                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        SDL_Surface* imageSurface = SDL_LoadBMP("sdl.bmp");
        if (!imageSurface) {
            std::cout << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        }

        offsetRect = {(WIN_WIDTH - imageSurface->w) / 2, (WIN_HEIGHT - imageSurface->h) / 2, 0, 0};
        
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
                case SDL_CONTROLLERBUTTONUP:
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        WIN_WIDTH = event.window.data1;
                        WIN_HEIGHT = event.window.data2;
                        offsetRect = {(WIN_WIDTH - imageSurface->w) / 2, (WIN_HEIGHT - imageSurface->h) / 2, 0, 0};
                    }
                    break;
                case SDL_KEYDOWN:
                    performActionOnKeyInput(event);
                    break;
            }
            paint(winSurface, imageSurface, window);
        }

        SDL_FreeSurface(imageSurface);
        SDL_DestroyWindow(window);
    }
    if (controller) {
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
    return 0;
}