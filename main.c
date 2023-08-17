#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

typedef struct
{
    int x, y;
    SDL_Texture *texture;
} Entity;

typedef struct
{
    Entity player;
    SDL_Renderer *renderer;
} GameState;

int processEvents(SDL_Window *window, GameState *game)
{
    SDL_Event event;
    int done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
            if (window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
            }
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = 1;
                break;
            }
            break;

        case SDL_QUIT:
            done = 1;
            break;
        }
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_UP])
    {
        game->player.y -= 10;
    }
    if (keys[SDL_SCANCODE_DOWN])
    {
        game->player.y += 10;
    }
    if (keys[SDL_SCANCODE_LEFT])
    {
        game->player.x -= 10;
    }
    if (keys[SDL_SCANCODE_RIGHT])
    {
        game->player.x += 10;
    }

    return done;
}

void render(GameState *game)
{
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderClear(game->renderer);

    SDL_Rect rect = {game->player.x, game->player.y, 63, 73};
    SDL_RenderCopy(game->renderer, game->player.texture, NULL, &rect);

    SDL_RenderPresent(game->renderer);
}

void loadGame(GameState *game)
{
    SDL_Surface *surface;
    surface = IMG_Load("img/penitent.png");

    if (!surface)
    {
        printf("Failed to load image: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    game->player.texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    game->player.x = 100;
    game->player.y = 100;
}

int main(int argc, char *argv[])
{
    GameState game;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "Hello World",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    game.renderer = renderer;

    loadGame(&game);

    int done = 0;

    while (!done)
    {
        done = processEvents(window, &game);

        render(&game);
    }

    SDL_DestroyTexture(game.player.texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return EXIT_SUCCESS;
}
