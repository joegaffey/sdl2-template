#include <SDL2/SDL.h>
#include <iostream>

int WIN_WIDTH = 800;
int WIN_HEIGHT = 600;
char* IMAGE_PATH = "sdl.bmp";

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* winSurface;
SDL_GameController* controller;
SDL_Texture* imageTexture;
SDL_Rect imageRect;

// Startup ////////////////////////////////////////////////////////////////////////////////////////

void initControllers() {
    int numJoy = SDL_NumJoysticks();
    std::cout << numJoy << " joysticks detected." << std::endl;

    for (int i = 0; i < numJoy; ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                std::cout << "Controller connected: " << SDL_GameControllerName(controller) << std::endl;
                break;
            } else {
                std::cerr << "Could not open game controller %i: " << i << SDL_GetError() << std::endl;
            }
        }
    }

    if (!controller) {
        std::cout << "No game controllers found." << std::endl;
    }
}

int initDisplay() {
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    
    window = SDL_CreateWindow("SDL2 Template",
                              (DM.w / 2) - (WIN_WIDTH / 2),
                              (DM.h / 2) - (WIN_HEIGHT / 2),
                              WIN_WIDTH,
                              WIN_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return 0;
    }
    return 1;
}

int initGraphics() {
    SDL_Surface* imageSurface = SDL_LoadBMP(IMAGE_PATH);
    if (!imageSurface) {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        return 0;
    }
    imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (!imageTexture) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        return 0;
    }
    imageRect = {(WIN_WIDTH - imageSurface->w) / 2, (WIN_HEIGHT - imageSurface->h) / 2, imageSurface->w, imageSurface->h};
    SDL_FreeSurface(imageSurface);
    return 1;
}

// Rendering //////////////////////////////////////////////////////////////////////////////////////

void paint() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imageTexture, nullptr, &imageRect);
    SDL_RenderPresent(renderer);
}

// Event Handling /////////////////////////////////////////////////////////////////////////////////

void handleButtonPress(SDL_Event event) {
    SDL_GameControllerButton button = (SDL_GameControllerButton)event.cbutton.button;
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A: imageRect.y--; break;
        case SDL_CONTROLLER_BUTTON_B: imageRect.x--; break;
        case SDL_CONTROLLER_BUTTON_X: imageRect.x++; break;
        case SDL_CONTROLLER_BUTTON_Y: imageRect.y++; break;
    }
}

void handleKeyInput(SDL_Event event) {
    switch (event.key.keysym.sym) {
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

void handleWindowEvent(SDL_Event event) {
    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        WIN_WIDTH = event.window.data1;
        WIN_HEIGHT = event.window.data2;
        imageRect = {(WIN_WIDTH - imageRect.w) / 2, (WIN_HEIGHT - imageRect.h) / 2, imageRect.w, imageRect.h};
    }
}

// Main ///////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
    int running = 1;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK != 0)) {
        std::cout << "Error: " << SDL_GetError();
        running = 0;
    }
    else {
        initControllers();
        running = initDisplay();
        running = initGraphics();
    }
    
    while(running) 
    {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                handleButtonPress(event);
                break;
            case SDL_KEYDOWN:
                handleKeyInput(event);
                break;
            case SDL_WINDOWEVENT:
                handleWindowEvent(event);
                break;
        }
        paint();
    }

    SDL_DestroyWindow(window);
    if (controller)
        SDL_GameControllerClose(controller);
    SDL_Quit();
    return 0;
}