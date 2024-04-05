#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    SDL_Texture *backgroundTexture;
    SDL_Texture *character1Texture;
    SDL_Texture *character2Texture;
    const char *dialogText;
} Scene;

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path);

void renderScene(SDL_Renderer *renderer, Scene *scene, TTF_Font *font);

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    // Create a window and renderer
    SDL_Window *window = SDL_CreateWindow("Road To Heaven", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Load textures
    SDL_Texture *menuTexture = loadTexture(renderer, "../images/menu.png");
    SDL_Texture *newGameButtonTexture = loadTexture(renderer, "../images/new_game_button.png");
    SDL_Texture *exitButtonTexture = loadTexture(renderer, "../images/exit_button.png");
    SDL_Texture *backgroundTexture = loadTexture(renderer, "../images/background.png");
    SDL_Texture *character1Texture = loadTexture(renderer, "../images/character1.png");
    SDL_Texture *character2Texture = loadTexture(renderer, "../images/character2.png");

    TTF_Font *font = TTF_OpenFont("../assets/fonts/OpenSans/OpenSans-Regular.ttf", 24);
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    Scene mainMenuScene = {menuTexture, NULL, NULL, NULL};
    Scene scene1 = {backgroundTexture, character1Texture, character2Texture, "This is scene 1."};
    Scene scene2 = {backgroundTexture, character1Texture, character2Texture, "This is scene 2."};

    bool running = 1;
    SDL_Event e;
    Scene *currentScene = &mainMenuScene;
    bool inMainMenu = true;

    // Main loop
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (inMainMenu && mouseX >= 300 && mouseX <= 500 && mouseY >= 200 && mouseY <= 250) {
                    // New game button clicked, switching to the first scene...
                    currentScene = &scene1;
                    inMainMenu = false;
                } else if (inMainMenu && mouseX >= 300 && mouseX <= 500 && mouseY >= 300 && mouseY <= 350) {
                    // Exit button clicked, quit the game
                    running = false;
                }
            } else if (!inMainMenu && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                if (currentScene == &scene1) {
                    currentScene = &scene2;
                } else if (currentScene == &scene2) {
                    currentScene = &scene1;
                }
            }
        }

        SDL_RenderClear(renderer);

        renderScene(renderer, currentScene, font);
        SDL_RenderPresent(renderer);
    }

    // Free resources and close SDL
    SDL_DestroyTexture(menuTexture);
    SDL_DestroyTexture(newGameButtonTexture);
    SDL_DestroyTexture(exitButtonTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(character1Texture);
    SDL_DestroyTexture(character2Texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path) {
    SDL_Surface *surface = SDL_LoadBMP(path);
    if (!surface) {
        printf("Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void renderScene(SDL_Renderer *renderer, Scene *scene, TTF_Font* font) {
    SDL_RenderCopy(renderer, scene->backgroundTexture, NULL, NULL);

    if (scene->character1Texture) {
        SDL_Rect character1Rect = {100, 100, 200, 400};
        SDL_RenderCopy(renderer, scene->character1Texture, NULL, &character1Rect);
    }
    if (scene->character2Texture) {
        SDL_Rect character2Rect = {500, 100, 200, 400};
        SDL_RenderCopy(renderer, scene->character2Texture, NULL, &character2Rect);
    }

    if (scene->dialogText) {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, scene->dialogText, textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {50, 500, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}
