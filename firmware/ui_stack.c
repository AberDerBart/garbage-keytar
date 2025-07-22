#include "ui_stack.h"

#include <stdlib.h>

#include "display.h"

ui_stack_t ui_stack = {
    .current = NULL,
    .stack_size = 0,
    .default_element = NULL,
};

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

void ui_render(ssd1306_t* display) {
  if (!ui_stack.current) {
    if (!ui_stack.default_element) {
      return;
    }
    ui_stack.default_element->render(ui_stack.default_element, display);
    return;
  }
  ui_stack.current->element->render(ui_stack.current->element, display);
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
    ui_render(get_display());
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

  ui_render(get_display());
}

void ui_clear() {
  while (ui_stack.current) {
    ui_pop();
  }
}

void ui_set_default_element(ui_element_t* element) {
  ui_stack.default_element = element;
}