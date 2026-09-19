#define USG_IMPLEMENTATION
#include "fonts/basic_font.h"
#include "usg.h"
#include <SDL2/SDL.h>

#define SCALE 8

static uint8_t my_screen_buffer[USG_DEFAULT_WIDTH * USG_DEFAULT_HEIGHT / 8];

static const uint8_t player_sprite[] = {0x3C, 0x42, 0xA5, 0x81,
                                        0xA5, 0x99, 0x42, 0x3C};

void usg_RenderSDL(const usg_t *dev, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 255, 128, 255);

  for (uint16_t y = 0; y < dev->height; y++) {
    for (uint16_t x = 0; x < dev->width; x++) {
      uint16_t idx = x + (y / 8) * dev->width;
      uint8_t bit = 1 << (y % 8);

      if (dev->buffer[idx] & bit) {
        SDL_Rect rect = {
            .x = x * SCALE, .y = y * SCALE, .w = SCALE - 1, .h = SCALE - 1};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return 1;
  }

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (SDL_CreateWindowAndRenderer(64 * SCALE, 64 * SCALE, 0, &window,
                                  &renderer) < 0) {
    SDL_Quit();
    return 1;
  }

  SDL_SetWindowTitle(window, "USG Library - SDL2 Simulation");

  usg_t screen;
  usg_Init(&screen, my_screen_buffer, 64, 64);
  usg_ClearBuffer(&screen);

  bool running = true;
  SDL_Event event;
  int r = 5;

  usg_DrawDisc(&screen, 32, 32, r, USG_MODE_SET);

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
          r++;
          break;
        case SDLK_DOWN:
          if (r > 1)
            r--;
          break;
        case SDLK_ESCAPE:
          running = false;
          break;
        }
      }
    }

    usg_ClearBuffer(&screen);

    usg_DrawDisc(&screen, 32, 32, r, USG_MODE_SET);
    usg_SetFont(&screen, usg_font_5x7_data);
    usg_DrawStr(&screen, 12, 28, "Testing", USG_MODE_XOR);

    usg_RenderSDL(&screen, renderer);
    SDL_Delay(16);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
