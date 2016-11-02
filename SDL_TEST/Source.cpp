#pragma comment(lib, "C:/Users/Cameron/Documents/Programming/C++/SDL/SDL_TEST/SDL/lib/x86/SDL2.lib")
#pragma comment(lib, "runtimeobject.lib")
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

enum KeyPressSurface {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_COUNT
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* splash = NULL;
SDL_Surface* keyPressSurface[KEY_PRESS_SURFACE_COUNT];
SDL_Surface* currentSurface = NULL;

bool init();
bool loadMedia();
void close();
SDL_Surface* loadSurface(std::string path);
bool surfaceFailure(SDL_Surface* surface);
std::vector<int> keyPressStack;

int main(int argc, char* args[]) {
    std::cout << "Welcome, to ZOMBOCOM.\n";
    if (!init()) {
        std::cout << "Failed to initialize.\n";
    }
    else {
        if (!loadMedia()) {
            std::cout << "Critical Error: Failed to load media.\nShe cannae handle it captain.\n";
        }
        else {
            bool quit = false;

            SDL_Event e;
            int splashtime = 10000;
            currentSurface = keyPressSurface[KEY_PRESS_SURFACE_DEFAULT];

            std::cout << "Preparing to take user input.\n";

            static SDL_Rect screenRekt;
            screenRekt.x = 0;
            screenRekt.y = 0;
            screenRekt.w = SCREEN_WIDTH;
            screenRekt.h = SCREEN_HEIGHT;

            while (!quit) { //Main loop
                if(SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) { //When user clicks X, quit the loop
                        quit = true;
                        std::cout << "User closed window. Maybe we should do something.\n";
                    }
                }
                //Clear Screen
                SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

                if (splashtime > 0) { //Splash
                    SDL_BlitSurface(splash, NULL, screenSurface, NULL);
                    splashtime--;
                }
                else if (splashtime == 0) {
                    currentSurface = keyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
                        if (e.type == SDL_KEYDOWN) {
                            switch (e.key.keysym.sym) {
                            case SDLK_UP:
                                currentSurface = keyPressSurface[KEY_PRESS_SURFACE_UP];
                                break;
                            case SDLK_DOWN:
                                currentSurface = keyPressSurface[KEY_PRESS_SURFACE_DOWN];
                                break;
                            case SDLK_LEFT:
                                currentSurface = keyPressSurface[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                currentSurface = keyPressSurface[KEY_PRESS_SURFACE_RIGHT];
                                break;
                            }
                        }

                    SDL_BlitScaled(currentSurface, NULL, screenSurface, &screenRekt);

                }
                SDL_UpdateWindowSurface(window);
            }
        }
    }

    close();
	
	return 0;
}

bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL_Init Call, test for errors
		std::cout << "SDL could not initialize! SDL_Error: \n   " << SDL_GetError() << std::endl;
		success = false;
	} 
	else { //If no errors, initialize the window
		window = SDL_CreateWindow("OH GOD I AM NOT GOOD WITH COMPUTERS PLEASE TO HELP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        std::cout << "SDL turned on this time.\n";
		if (window == NULL) { //Check if window created
			std::cout << "Window could not be created! SDL_Error: \n   " << SDL_GetError();
		} 
		else { //Window was created
			screenSurface = SDL_GetWindowSurface(window);
            std::cout << "Oh boy we made a window.\n";
		}
	}
	return success;
}

bool surfaceFailure(SDL_Surface* surface) {
    if (surface == NULL) {
        std::cout << "Unable to load image!" << std::endl;
        return true; //Failure!
    }
    else {
        return false;
    }
}

bool loadMedia() {
	bool success = true;
    //Load bmp
	splash = loadSurface("bmp/splash.bmp");
    if (surfaceFailure(splash)) {
        success = false;
    }
    keyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("bmp/prompt.bmp");
    if (surfaceFailure(keyPressSurface[KEY_PRESS_SURFACE_DEFAULT])) {
        success = false;
    }
    keyPressSurface[KEY_PRESS_SURFACE_UP] = loadSurface("bmp/up.bmp");
    if (surfaceFailure(keyPressSurface[KEY_PRESS_SURFACE_UP])) {
        success = false;
    }
    keyPressSurface[KEY_PRESS_SURFACE_DOWN] = loadSurface("bmp/down.bmp");
    if (surfaceFailure(keyPressSurface[KEY_PRESS_SURFACE_DOWN])) {
        success = false;
    }
    keyPressSurface[KEY_PRESS_SURFACE_LEFT] = loadSurface("bmp/left.bmp");
    if (surfaceFailure(keyPressSurface[KEY_PRESS_SURFACE_LEFT])) {
        success = false;
    }
    keyPressSurface[KEY_PRESS_SURFACE_RIGHT] = loadSurface("bmp/right.bmp");
    if (surfaceFailure(keyPressSurface[KEY_PRESS_SURFACE_RIGHT])) {
        success = false;
    }

	
	return success;
}

void close() {
    std::cout << "Shuttin' it down.\n";
	SDL_FreeSurface(screenSurface);
	screenSurface = NULL;

	SDL_FreeSurface(splash);
	splash = NULL;

    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Surface* loadSurface(std::string path) { //Returns a surface from a bitmap at a path
    SDL_Surface* optimizedSurface = NULL;
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) {
        std::cout << "Unable to load " << path << " SDL Error: " << SDL_GetError() << std::endl;
    }
    else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, NULL);
        SDL_FreeSurface(loadedSurface);
        std::cout << "Optimizing surface...\n";
    }
    return optimizedSurface;
}