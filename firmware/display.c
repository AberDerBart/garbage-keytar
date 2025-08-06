#include "display.h"

#include "battery.h"
#include "hardware/i2c.h"
#include "menu/main.h"
#include "midi_ble.h"
#include "pico-ssd1306/ssd1306.h"
#include "settings.h"
#include "stdio.h"
#include "string.h"
#include "ui/menu.h"
#include "ui/stack.h"
#include "ui/status.h"

#define SDA_PIN 8
#define SCL_PIN 9

#define KEYBOARD_POS 0, 0
#define BLUETOOTH_POS 48, 0
#define CHARGE_POS 96, 0
#define BATTERY_POS 104, 0
#define KEY_MAPPING_POS 64, 0

void draw_info();
void draw_menu();

ssd1306_t disp;

bool display_initialized = false;

void redraw();

void display_init() {
  printf("init display\n");

  // init hardware
  i2c_init(i2c0, 400000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SDA_PIN);
  gpio_pull_up(SCL_PIN);

  disp.external_vcc = false;
  display_initialized = ssd1306_init(&disp, 128, 64, 0x3c, i2c0);

  if (!display_initialized) {
    printf("failed to initialize display\n");
    return;
  }

  ssd1306_set_rotation(&disp, ROT_180);

  ui_init(&disp, &ui_status);

  ui_render();
}

void display_task() {
  if (!display_initialized) {
    return;
  }

  return;
}

ssd1306_t* get_display() { return &disp; }