#define USG_IMPLEMENTATION
#include "usg.h"

static uint8_t my_screen_buffer[USG_DEFAULT_WIDTH * USG_DEFAULT_HEIGHT / 8];

const uint8_t player_sprite[] = {0x3C, 0x42, 0xA5, 0x81,
                                 0xA5, 0x99, 0x42, 0x3C};

int main(void) {
  usg_t screen;

  usg_Init(&screen, my_screen_buffer, 64, 64); // Initialize the screen

  usg_ClearBuffer(&screen); // Clears the screen buffer

  usg_DrawDisc(&screen, 12, 18, 5, USG_MODE_SET);

  usg_DrawXBM(&screen, 28, 14, 8, 8, player_sprite,
      USG_MODE_SET); // Draw a custom bitmap

  usg_DrawBox(&screen, 48, 14, 10, 10,
      USG_MODE_SET); // Draw a box *using lines*
                     
  usg_DrawLine(&screen, 0, 36, 63, 36, USG_MODE_SET); // Draw a line

  usg_DrawFrame(&screen, 16, 45, 32, 10,
      USG_MODE_SET); // Draw a frame *using lines*

  usg_DrawStr(&screen, 32, 61, "Testing functions",
      USG_MODE_SET); // Draw a string *Not implement*
                     
  usg_PrintAnsiStatic(
      &screen); // Print an static console representation of the buffer


  return 0;
}
