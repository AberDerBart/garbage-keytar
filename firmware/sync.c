#include "sync.h"
#include "pico/sync.h"

critical_section_t critical_section;

void sync_init() {
  critical_section_init(&critical_section);
}

void enter_critical() {
  critical_section_enter_blocking(&critical_section);
}

void exit_critical() {
  critical_section_exit(&critical_section);
}
