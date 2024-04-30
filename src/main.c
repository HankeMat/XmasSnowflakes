/**
 * @file main.c
 * @author Hanke Matej (HAN0390)
 * @brief 
 * @version 0.1
 * @date 2023-12-08
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "sdl.h"
#include <time.h>

typedef struct {
    int id;
    int speed;
    float angle;
    SDL_Rect rect;
} Data;

typedef struct SnowFlake {
    Data data;
    struct SnowFlake *prev, *next;
} SnowFlake;

SnowFlake *first = NULL;

int rng(int highRange, int lowRange){
    return (rand() %(highRange - lowRange + 1)) + lowRange;
}

void add(int id, int speed, int dimension, float angle, SDL_Rect rect){
	SnowFlake *new;							
	new = (SnowFlake*)malloc(sizeof(SnowFlake));	
	
	if(NULL == new){					
		printf("Not enough space\n");
		exit(1);
	}
	
	new->next = NULL;
	new->prev = NULL;
	
    new->data.id = id;
	new->data.speed = speed;				
	new->data.angle = angle;
    new->data.rect = rect;
    new->data.rect.h = dimension;
    new->data.rect.w = dimension;
	
	if(NULL != first){					
		first->prev = new;				
		new->next = first;
		first = new;
	}else{
		first = new;					
	}
}

int removeSnowflake(int id){
	SnowFlake *test;
	int isThere = 0;
	
	for(test = first; test!=NULL; test = test->next){	
		if(test->data.id == id){					
			if(test->prev == NULL && test->next == NULL){
				first = NULL;
                fprintf(stderr, "Snowflake %d deleted\n", test->data.id);
				free(test);
				isThere = 1;
				return 0;
			}else if(test->next == NULL){
				test->prev->next = NULL;
                fprintf(stderr, "Snowflake %d deleted\n", test->data.id);
				free(test);
				isThere = 1;
				return 0;
			}else if(test->prev == NULL){
				test->next->prev = NULL;
				first = test->next;
                fprintf(stderr, "Snowflake %d deleted\n", test->data.id);
				free(test);
				isThere = 1;
				return 0;
			}else{
				test->next->prev = test->prev;
				test->prev->next = test->next;
                fprintf(stderr, "Snowflake %d deleted\n", test->data.id);
				free(test);
				isThere = 1;
				return 0;
			}
		}
	}
	
	if(0 == isThere){
		printf("No snow flake with this id\n");
		system("pause");
		return -1;
	}
	return -1;
}

void clearBcg(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

int main() {
    srand(time(NULL));
    SDL_Context init = sdl_context_init("Snow Flakes", 1200, 600);

    SDL_Surface* snowFlake = SDL_LoadBMP("src/snowflake.bmp");
    Uint32 BlackColor = SDL_MapRGB(snowFlake->format, 0, 0, 0);
    SDL_SetColorKey(snowFlake, SDL_ENABLE, BlackColor);
    SDL_Texture* snowFlakeTexture = SDL_CreateTextureFromSurface(init.renderer ,snowFlake);
    SDL_FreeSurface(snowFlake);

    SDL_Event event;
    int running = true;
    int id = 0;
    
    while (running) {
        int winHeight;
        int winWidth;
        SDL_GetWindowSize(init.window, &winWidth, &winHeight);
        // Catching an event
        while (SDL_PollEvent(&event)) { 
            // If event is type quit (crossing the window) then set running to zero which stops the main loop
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    int dimensionValue = rng(200, 30);
                    int speedValue = rng(7, 1);
                    SDL_Rect rect;
                    rect.x = mouseX - (dimensionValue / 2);
                    rect.y = mouseY - (dimensionValue / 2);

                    add(id , speedValue, dimensionValue, 0.0, rect);
                    fprintf(stderr, "SnowFlake %d created\n", id);
                    id++;
                }
            }
        }

        clearBcg(init.renderer);

        SnowFlake* temp;							

        for(temp = first;temp != NULL;temp = temp->next){
            SDL_RenderCopyEx(init.renderer, snowFlakeTexture, 0, &temp->data.rect, temp->data.angle, NULL, SDL_FLIP_NONE);
            temp->data.angle += temp->data.speed;
            temp->data.rect.y += temp->data.speed;
            if (temp->data.rect.y > winHeight) {
                removeSnowflake(temp->data.id);
            }
	    }

        SDL_RenderPresent(init.renderer);
    }


    sdl_context_free(&init);
    return 1;
}
