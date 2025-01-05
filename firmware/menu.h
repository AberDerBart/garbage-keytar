#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct menu_item_t {
  char *label;
  struct menu_item_t *parent;
  struct menu_item_t *children[];
} menu_item_t;

typedef struct {
  menu_item_t *current;
  uint8_t index;
} menu_state_t;

void menu_init();

uint8_t menu_len(menu_item_t *);

void menu_open();
void menu_close();
void menu_toggle();

void menu_prev();
void menu_next();
void menu_parent();

void menu_select();

bool menu_render();