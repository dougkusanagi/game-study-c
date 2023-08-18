#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constants.h"

SDL_Window *window;
SDL_Renderer *renderer;

typedef struct
{
    int x, y, w, h;
} Ledge;

typedef struct
{
    int x, y, w, h;
    SDL_Texture *animation_idle[13];
} Entity;

typedef struct
{
    Entity player;

    Ledge ledges[100];

    SDL_Texture *floor;

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

    SDL_Rect rect = {game->player.x, game->player.y, game->player.w, game->player.h};

    // for (int i = 0; i < 13; i++)
    // {
    //     SDL_RenderCopyEx(game->renderer, game->player.animation_idle[i], NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    //     SDL_RenderPresent(game->renderer);
    // }

    SDL_RenderCopyEx(game->renderer, game->player.animation_idle[0], NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(game->renderer);
}

void loadPlayerAnimations(GameState *game)
{
    SDL_Surface *surface;
    char filename[30];

    for (int i = 0; i < 13; i++)
    {
        sprintf(filename, "img/penitent/idle/idle%d.png", i + 1);
        surface = IMG_Load(filename);

        if (!surface)
        {
            printf("Failed to load image: %s\n", SDL_GetError());
            SDL_Quit();
            exit(EXIT_FAILURE);
        }

        game->player.animation_idle[i] = SDL_CreateTextureFromSurface(game->renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void loadGame(GameState *game)
{
    loadPlayerAnimations(game);

    game->player.x = 100;
    game->player.y = 100;
    game->player.w = 63;
    game->player.h = 73;
}

int initializeWindow(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return FALSE;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);

    if (!window)
    {
        printf("Failed to create window: %s\n", SDL_GetError());
        return FALSE;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return FALSE;
    }

    return TRUE;
}

int main(int argc, char *argv[])
{
    initializeWindow();

    GameState game;
    game.renderer = renderer;

    loadGame(&game);

    int done = 0;

    while (!done)
    {
        done = processEvents(window, &game);

        render(&game);
    }

    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return EXIT_SUCCESS;
}
