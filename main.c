#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

const int FPS = 3000;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

struct block{
    float F;
    int M;
    float x;
    float y;
    int width;
    int height;
};

void draw_blocks(SDL_Renderer* renderer, struct block* blockbig, struct block* blocksmall,SDL_FRect* temprect);
int collision(struct block* blockbig, struct block* blocksmall);

int main(int argc, char* args[]){

    SDL_Window* window = SDL_CreateWindow("Collision", SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0), SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    struct block *blockbig = (struct block*)malloc(sizeof(struct block*));
    struct block *blocksmall = (struct block*)malloc(sizeof(struct block*));
    SDL_Rect* wall = (SDL_Rect*)malloc(sizeof(SDL_Rect));

    int collisions = 0;

    wall->h = 500;
    wall->w = 30;
    wall->x = 50;
    wall->y = 250;

    blockbig->F = -0.1;
    blockbig->M = 10000;
    blockbig->x = 500;
    blockbig->y = 500;
    blockbig->width = 100;
    blockbig->height = 100;


    blocksmall->F = 0;
    blocksmall->M = 1;
    blocksmall->width = 50;
    blocksmall->height = 50;
    blocksmall->x = 200;
    blocksmall->y = blockbig->y + blockbig->height - blocksmall->height;

    

    const float dt = 1000.0 / FPS;
    int starting_tick = 0;

    SDL_Event e;
    SDL_FRect rect;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        starting_tick = SDL_GetTicks();
        
        if(collision(blockbig,blocksmall)){
            collisions++;
            printf("%d\n",collisions);
        }
        if(blocksmall->x <= wall->x+wall->w){
            blocksmall->F *= -1;
            blocksmall->x = wall->x+wall->w;
            collisions++;
            printf("%d\n",collisions);

        }
        blockbig->x += blockbig->F * dt;
        blocksmall->x += blocksmall->F * dt;


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 1);
        SDL_RenderFillRect(renderer, wall);
        draw_blocks(renderer, blockbig,blocksmall,&rect);
        if (dt > SDL_GetTicks() - starting_tick) {
            SDL_Delay(dt - (SDL_GetTicks() - starting_tick));
        }
    }

    free(blocksmall);
    free(blockbig);
    free(wall);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
void draw_blocks(SDL_Renderer* renderer, struct block* blockbig, struct block* blocksmall, SDL_FRect* temprect){
    
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 1);

    temprect->h = blockbig->height;
    temprect->w = blockbig->width;
    temprect->x = blockbig->x;
    temprect->y = blockbig->y;
    SDL_RenderFillRectF(renderer,temprect);
    temprect->h = blocksmall->height;
    temprect->w = blocksmall->width;
    temprect->x = blocksmall->x;
    temprect->y = blocksmall->y;

    SDL_RenderFillRectF(renderer,temprect);

    SDL_RenderPresent(renderer);

}
int collision(struct block* blockbig, struct block* blocksmall){
    if(blockbig->x <= blocksmall->x + blocksmall->width){
        blockbig->F = (blocksmall->M * (blocksmall->F - blockbig->F) + blockbig->M * blockbig->F + blocksmall->M * blocksmall->F) / (blockbig->M + blocksmall->M);
        blocksmall->F = (blockbig->M * (blockbig->F - blocksmall->F) + blockbig->M * blockbig->F + blocksmall->M * blocksmall->F) / (blockbig->M + blocksmall->M);  
        return 1;
    }
    return 0;
}
