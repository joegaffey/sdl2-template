#include <SDL2/SDL.h>
#include <iostream>

static int winWidth = 800;
static int winHeight = 600;
static int running = 1;

static const char *title = "SDL2 Template App";
static const char *imagePath = "sdl.bmp";
static const int speed = 5;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *winSurface;
SDL_Texture *imageTexture;
SDL_Rect imageRect;
SDL_GameController *controller;

// Startup ////////////////////////////////////////////////////////////////////////////////////////

void initControllers()
{
    int numJoy = SDL_NumJoysticks();
    std::cout << numJoy << " joysticks detected." << std::endl;

    for (int i = 0; i < numJoy; ++i)
    {
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
            if (controller)
            {
                std::cout << "Controller connected: " << SDL_GameControllerName(controller) << std::endl;
                break;
            }
            else
            {
                std::cerr << "Could not open controller %i: " << i << SDL_GetError() << std::endl;
            }
        }
    }

    if (!controller)
        std::cout << "No controllers found." << std::endl;
}

int initDisplay()
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    window = SDL_CreateWindow(title,
                              (DM.w / 2) - (winWidth / 2),
                              (DM.h / 2) - (winHeight / 2),
                              winWidth,
                              winHeight,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return 0;
    }
    return 1;
}

int initGraphics()
{
    SDL_Surface *imageSurface = SDL_LoadBMP(imagePath);
    if (!imageSurface)
    {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        return 0;
    }
    imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (!imageTexture)
    {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        return 0;
    }
    imageRect = {
        (winWidth - imageSurface->w) / 2,
        (winHeight - imageSurface->h) / 2,
        imageSurface->w,
        imageSurface->h
    };
    SDL_FreeSurface(imageSurface);
    return 1;
}

int init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK != 0))
    {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        return 0;
    }
    else
    {
        initControllers();
        if (!initDisplay())
            return 0;
        if (!initGraphics())
            return 0;
    }
    return 1;
}

// Event Handling /////////////////////////////////////////////////////////////////////////////////

void handleButtonPress(SDL_Event event)
{
    SDL_GameControllerButton button = (SDL_GameControllerButton)event.cbutton.button;
    switch (button)
    {
    case SDL_CONTROLLER_BUTTON_A:
        imageRect.y += speed;
        break;
    case SDL_CONTROLLER_BUTTON_B:
        imageRect.x += speed;
        break;
    case SDL_CONTROLLER_BUTTON_X:
        imageRect.x -= speed;
        break;
    case SDL_CONTROLLER_BUTTON_Y:
        imageRect.y -= speed;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        imageRect.y += speed;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        imageRect.x += speed;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        imageRect.x -= speed;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        imageRect.y -= speed;
        break;
    }
}

void handleKeyInput(SDL_Event event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_UP:
        imageRect.y -= speed;
        break;
    case SDLK_LEFT:
        imageRect.x -= speed;
        break;
    case SDLK_RIGHT:
        imageRect.x += speed;
        break;
    case SDLK_DOWN:
        imageRect.y += speed;
        break;
    case SDLK_w:
        imageRect.y -= speed;
        break;
    case SDLK_a:
        imageRect.x -= speed;
        break;
    case SDLK_d:
        imageRect.x += speed;
        break;
    case SDLK_s:
        imageRect.y += speed;
        break;
    }
}

void handleWindowEvent(SDL_Event event)
{
    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        winWidth = event.window.data1;
        winHeight = event.window.data2;
        imageRect = {(winWidth - imageRect.w) / 2, (winHeight - imageRect.h) / 2, imageRect.w, imageRect.h};
    }
}

// Game loop //////////////////////////////////////////////////////////////////////////////////////

void paint()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imageTexture, nullptr, &imageRect);
    SDL_RenderPresent(renderer);
}

void checkEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
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
}

// Main ///////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
    running = init();

    while (running)
    {
        checkEvents();
        paint();
    }

    if (controller)
        SDL_GameControllerClose(controller);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}