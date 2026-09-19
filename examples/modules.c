#define USG_IMPLEMENTATION
#include "fonts/basic_font.h"
#include "usg.h"

static uint8_t my_screen_buffer[USG_DEFAULT_WIDTH * USG_DEFAULT_HEIGHT / 8];

const uint8_t player_sprite[] = {0x3C, 0x42, 0xA5, 0x81,
                                 0xA5, 0x99, 0x42, 0x3C};

int main(void) {
  usg_t screen;

  usg_Init(&screen, my_screen_buffer, 64, 64);

  usg_ClearBuffer(&screen);

  usg_DrawDisc(&screen, 12, 18, 5, USG_MODE_SET);

  usg_DrawXBM(&screen, 28, 14, 8, 8, player_sprite, USG_MODE_SET);

  usg_DrawBox(&screen, 48, 14, 10, 10, USG_MODE_SET);

  usg_DrawLine(&screen, 0, 36, 63, 40, USG_MODE_SET);

  usg_DrawFrame(&screen, 16, 45, 32, 10, USG_MODE_SET);

  usg_SetFont(&screen, usg_font_5x7_data);

  usg_DrawStr(&screen, 5, 27, "ABC120", USG_MODE_SET);

  usg_PrintAnsiStatic(&screen);

  return 0;
}
