#ifndef USG_H
#define USG_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define USG_DEFAULT_WIDTH 128
#define USG_DEFAULT_HEIGHT 64

typedef enum {
  USG_MODE_CLEAR = 0,
  USG_MODE_SET = 1,
  USG_MODE_XOR = 2
} usg_draw_mode_t;

typedef struct {
  uint16_t width;
  uint16_t height;
  uint8_t *buffer;
  const void *font;
} usg_t;

void usg_Init(usg_t *dev, uint8_t *buffer, uint16_t width, uint16_t height);

void usg_ClearBuffer(usg_t *dev);

void usg_DrawPixel(usg_t *dev, int16_t x, int16_t y, usg_draw_mode_t mode);

void usg_DrawLine(usg_t *dev, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  usg_draw_mode_t mode);

void usg_DrawFrame(usg_t *dev, int16_t x, int16_t y, uint16_t w, uint16_t h,
                   usg_draw_mode_t mode);

void usg_DrawBox(usg_t *dev, int16_t x, int16_t y, uint16_t w, uint16_t h,
                 usg_draw_mode_t mode);

void usg_DrawDisc(usg_t *dev, int16_t x0, int16_t y0, uint16_t r,
                  usg_draw_mode_t mode);

void usg_DrawTriangle(usg_t *dev, int16_t x0, int16_t y0, int16_t x1,
                      int16_t y1, int16_t x2, int16_t y2, usg_draw_mode_t mode);

void usg_DrawXBM(usg_t *dev, int16_t x, int16_t y, uint16_t w, uint16_t h,
                 const uint8_t *bitmap, usg_draw_mode_t mode);

void usg_SetFont(usg_t *dev, const void *font);

void usg_DrawStr(usg_t *dev, int16_t x, int16_t y, const char *str,
                 usg_draw_mode_t mode);

void usg_PrintAnsiStatic(const usg_t *dev);

#endif

#ifdef USG_IMPLEMENTATION

void usg_Init(usg_t *dev, uint8_t *buffer, uint16_t width, uint16_t height) {
  dev->width = width;
  dev->height = height;
  dev->buffer = buffer;
  dev->font = NULL;
}

void usg_ClearBuffer(usg_t *dev) {
  if (dev->buffer) {
    memset(dev->buffer, 0, (dev->width * dev->height) / 8);
  }
}

void usg_DrawPixel(usg_t *dev, int16_t x, int16_t y, usg_draw_mode_t mode) {
  if (x < 0 || x >= dev->width || y < 0 || y >= dev->height)
    return;

  uint16_t idx = x + (y / 8) * dev->width;
  uint8_t bit = 1 << (y % 8);

  switch (mode) {
  case USG_MODE_SET:
    dev->buffer[idx] |= bit;
    break;
  case USG_MODE_CLEAR:
    dev->buffer[idx] &= ~bit;
    break;
  case USG_MODE_XOR:
    dev->buffer[idx] ^= bit;
    break;
  }
}

void usg_DrawLine(usg_t *dev, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  usg_draw_mode_t mode) {
  int16_t dx = x0 < x1 ? x1 - x0 : x0 - x1;
  int16_t sx = x0 < x1 ? 1 : -1;
  int16_t dy = y0 < y1 ? y1 - y0 : y0 - y1;
  int16_t sy = y0 < y1 ? 1 : -1;
  int16_t err = (dx > dy ? dx : -dy) / 2;
  int16_t e2;

  for (;;) {
    usg_DrawPixel(dev, x0, y0, mode);
    if (x0 == x1 && y0 == y1)
      break;
    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
  }
}

void usg_PrintAnsiStatic(const usg_t *dev) {
  printf("\033[H\033[J");

  for (uint16_t y = 0; y < dev->height; y++) {
    for (uint16_t x = 0; x < dev->width; x++) {
      uint16_t idx = x + (y / 8) * dev->width;
      uint8_t bit = 1 << (y % 8);

      if (dev->buffer[idx] & bit) {
        printf("\033[42m  \033[0m");
      } else {
        printf("\033[40m░░\033[0m");
      }
    }
    printf("\n");
  }
}

void usg_DrawXBM(usg_t *dev, int16_t x, int16_t y, uint16_t w, uint16_t h,
                 const uint8_t *bitmap, usg_draw_mode_t mode) {
  uint16_t byte_width = (w + 7) / 8;
  for (uint16_t j = 0; j < h; j++) {
    for (uint16_t i = 0; i < w; i++) {
      if (bitmap[j * byte_width + i / 8] & (1 << (i % 8))) {
        usg_DrawPixel(dev, x + i, y + j, mode);
      }
    }
  }
}

void usg_DrawBox(usg_t *dev, int16_t x, int16_t y, uint16_t w, uint16_t h,
                 usg_draw_mode_t mode) {
#warning "Isn't the best approach yet"
  for (int16_t i = 0; i < h; i++) {
    usg_DrawLine(dev, x, y + i, x + w - 1, y + i, mode);
  }
}

void usg_DrawFrame(usg_t *dev, int16_t x, int16_t y, uint16_t w, uint16_t h,
                   usg_draw_mode_t mode) {
#warning "Isn't the best approach yet"
  int16_t x2 = x + w - 1;
  int16_t y2 = y + h - 1;
  usg_DrawLine(dev, x, y, x2, y, mode);
  usg_DrawLine(dev, x, y2, x2, y2, mode);
  usg_DrawLine(dev, x, y, x, y2, mode);
  usg_DrawLine(dev, x2, y, x2, y2, mode);
}

void usg_DrawDisc(usg_t *dev, int16_t x0, int16_t y0, uint16_t r,
                  usg_draw_mode_t mode) {
#warning "Isn't the best approach yet"
  if (!dev || !dev->buffer || r == 0)
    return;

  int16_t x = r;
  int16_t y = 0;
  int16_t err = 1 - x;

  while (x >= y) {
    usg_DrawLine(dev, x0 - x, y0 + y, x0 + x, y0 + y, mode);
    usg_DrawLine(dev, x0 - x, y0 - y, x0 + x, y0 - y, mode);

    if (y != x) {
      usg_DrawLine(dev, x0 - y, y0 + x, x0 + y, y0 + x, mode);
      usg_DrawLine(dev, x0 - y, y0 - x, x0 + y, y0 - x, mode);
    }

    y++;

    if (err < 0) {
      err += 2 * y + 1;
    } else {
      x--;
      err += 2 * (y - x) + 1;
    }
  }
}

void usg_DrawStr(usg_t *dev, int16_t x, int16_t y, const char *str,
                 usg_draw_mode_t mode) {
#warning "Not implement yet"
}

#endif
