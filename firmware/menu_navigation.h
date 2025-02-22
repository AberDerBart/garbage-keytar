#pragma once
#include "menu.h"

uint8_t menu_len(menu_item_t *);

void menu_open();
void menu_close();
void menu_toggle();

void menu_prev();
void menu_next();
void menu_parent();

void menu_inc();
void menu_dec();

void menu_select();
