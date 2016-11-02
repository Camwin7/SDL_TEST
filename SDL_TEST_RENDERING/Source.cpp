#pragma comment(lib, "C:/Users/Cameron/Documents/Programming/C++/SDL/SDL_TEST/SDL/lib/x86/SDL2.lib")
//#pragma comment(lib, "runtimeobject.lib")
#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

enum keyPressTexture {
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
//SDL_Surface* screenSurface = NULL;
SDL_Texture* splash = NULL;
SDL_Texture* keyPressTexture[KEY_PRESS_SURFACE_COUNT];
//SDL_Surface* currentSurface = NULL;
SDL_Texture* currentTexture = NULL;
SDL_Renderer* renderman = NULL;

bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(std::string path);
SDL_Surface* loadSurface(std::string path);
bool surfaceFailure(SDL_Surface* surface);
bool textureFailure(SDL_Texture* texture);

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
            currentTexture = keyPressTexture[KEY_PRESS_SURFACE_DEFAULT];

            std::cout << "Preparing to take user input.\n";

            /*static SDL_Rect screenRekt;
            screenRekt.x = 0;
            screenRekt.y = 0;
            screenRekt.w = SCREEN_WIDTH;
            screenRekt.h = SCREEN_HEIGHT;*/

            while (!quit) { //Main loop
                if(SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) { //When user clicks X, quit the loop
                        quit = true;
                        std::cout << "User closed window. Maybe we should do something.\n";
                    }
                }
                //Clear Screen
                //SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
                SDL_RenderClear(renderman);

                if (splashtime > 0) { //Splash
                    //SDL_BlitSurface(splash, NULL, screenSurface, NULL);
                    SDL_RenderCopy(renderman, splash, NULL, NULL);
                    splashtime--;
                }
                else if (splashtime == 0) {
                    currentTexture = keyPressTexture[KEY_PRESS_SURFACE_DEFAULT];
                        if (e.type == SDL_KEYDOWN) {
                            switch (e.key.keysym.sym) {
                            case SDLK_UP:
                                currentTexture = keyPressTexture[KEY_PRESS_SURFACE_UP];
                                break;
                            case SDLK_DOWN:
                                currentTexture = keyPressTexture[KEY_PRESS_SURFACE_DOWN];
                                break;
                            case SDLK_LEFT:
                                currentTexture = keyPressTexture[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                currentTexture = keyPressTexture[KEY_PRESS_SURFACE_RIGHT];
                                break;
                            }
                        }

                    //SDL_BlitScaled(currentSurface, NULL, screenSurface, &screenRekt);
                        SDL_RenderCopy(renderman, currentTexture, NULL, NULL);
                }
                //SDL_UpdateWindowSurface(window);
                SDL_RenderPresent(renderman);
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
			//screenSurface = SDL_GetWindowSurface(window);
            renderman = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderman == NULL) {
                std::cout << "Renderer initialized.\n";
            }
            else {
                SDL_SetRenderDrawColor(renderman, 0xff, 0xff, 0xff, 0xff);
                //maybe png loading someday
            }
            std::cout << "Oh boy we made a window. Now with more Renderer.\n";
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

bool textureFailure(SDL_Texture* texture) {
    if (texture == NULL) {
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
	splash = loadTexture("bmp/splash.bmp");
    if (textureFailure(splash)) {
        success = false;
    }
    keyPressTexture[KEY_PRESS_SURFACE_DEFAULT] = loadTexture("bmp/prompt.bmp");
    if (textureFailure(keyPressTexture[KEY_PRESS_SURFACE_DEFAULT])) {
        success = false;
    }
    keyPressTexture[KEY_PRESS_SURFACE_UP] = loadTexture("bmp/up.bmp");
    if (textureFailure(keyPressTexture[KEY_PRESS_SURFACE_UP])) {
        success = false;
    }
    keyPressTexture[KEY_PRESS_SURFACE_DOWN] = loadTexture("bmp/down.bmp");
    if (textureFailure(keyPressTexture[KEY_PRESS_SURFACE_DOWN])) {
        success = false;
    }
    keyPressTexture[KEY_PRESS_SURFACE_LEFT] = loadTexture("bmp/left.bmp");
    if (textureFailure(keyPressTexture[KEY_PRESS_SURFACE_LEFT])) {
        success = false;
    }
    keyPressTexture[KEY_PRESS_SURFACE_RIGHT] = loadTexture("bmp/right.bmp");
    if (textureFailure(keyPressTexture[KEY_PRESS_SURFACE_RIGHT])) {
        success = false;
    }

	
	return success;
}

void close() {
    std::cout << "Shuttin' it down.\n";
	//SDL_FreeSurface(screenSurface);
	//screenSurface = NULL;
    SDL_DestroyTexture(currentTexture);
    currentTexture = NULL;
	SDL_DestroyTexture(splash);
	splash = NULL;

    SDL_DestroyRenderer(renderman);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadTexture(std::string path) {
    SDL_Texture* theTexture = NULL;
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) {
        std::cout << "Unable to load " << path << " SDL Error: " << SDL_GetError() << std::endl;
    }
    else {
        theTexture = SDL_CreateTextureFromSurface(renderman, loadedSurface);
        if (theTexture == NULL) {
            std::cout << "Unable to create texture; fucking shit. It was the one from " << path << std::endl;
        }
    }
    SDL_FreeSurface(loadedSurface);
    return theTexture;
}

//This function does not optimize anymore
SDL_Surface* loadSurface(std::string path) { //Returns a surface from a bitmap at a path
    SDL_Surface* optimizedSurface = NULL;
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) {
        std::cout << "Unable to load " << path << " SDL Error: " << SDL_GetError() << std::endl;
    }
    else {
        //optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, NULL);
        SDL_FreeSurface(loadedSurface);
        std::cout << "Optimizing surface...\n";
    }
    return optimizedSurface;
}