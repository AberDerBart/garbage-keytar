#include "ui/stack.h"

#include <stdlib.h>

#include "display.h"

typedef struct ui_stack_layer_t {
  ui_element_t* element;
  struct ui_stack_layer_t* up;
} ui_stack_layer_t;

typedef struct ui_stack_t {
  ui_stack_layer_t* current;
  uint8_t stack_size;
  ui_element_t* default_element;
  ssd1306_t* display;
} ui_stack_t;

ui_stack_t ui_stack = {
    .current = NULL,
    .stack_size = 0,
    .default_element = NULL,
    .display = NULL,
};

void ui_init(ssd1306_t* display, ui_element_t* default_element) {
  ui_stack.display = display;
  ui_stack.default_element = default_element;
}

void ui_push(ui_element_t* element) {
  ui_stack_layer_t* new_layer = malloc(sizeof(ui_stack_layer_t));
  new_layer->element = element;
  new_layer->up = ui_stack.current;

  ui_stack.current = new_layer;
  ui_stack.stack_size++;
};

void ui_pop() {
  if (!ui_stack.current) {
    return;
  }

  ui_stack_layer_t* popped = ui_stack.current;

  if (popped->element->free) {
    (popped->element->free)(popped->element);
  }

  ui_stack.current = popped->up;
  ui_stack.stack_size--;

  free(popped);
}

void ui_render() {
  if (!ui_stack.display) {
    return;
  }

  ui_element_t* element = NULL;
  if (ui_stack.current) {
    element = ui_stack.current->element;
  } else if (ui_stack.default_element) {
    element = ui_stack.default_element;
  } else {
    // no element to render
    return;
  }

  ui_pos_t pos = {
    x : 0,
    y : 0,
  };

  element->render(element, ui_stack.display, pos, true);
  ssd1306_show(ui_stack.display);
}

void ui_navigate(ui_nav_t nav) {
  if (!ui_stack.current) {
    if (!ui_stack.default_element) {
      return;
    }
    if (!ui_stack.default_element->navigate) {
      return;
    }
    (ui_stack.default_element->navigate)(ui_stack.current->element, nav);
    ui_render();
    return;
  }
  if (!ui_stack.current->element->navigate) {
    return;
  }
  switch (nav) {
    case RETURN:
      ui_pop();
      break;
    case CLOSE:
      ui_clear();
      break;
    default:
      ui_stack.current->element->navigate(ui_stack.current->element, nav);
  }

  ui_render();
}

void ui_clear() {
  while (ui_stack.current) {
    ui_pop();
  }
}

void ui_set_default_element(ui_element_t* element) {
  ui_stack.default_element = element;
}