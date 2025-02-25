#include "config.h"

#include <assert.h>
#include <string.h>

#include "adsr.h"
#include "keyboard_matrix.h"
#include "pico/flash.h"

#define CONFIG_FLASH_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)
#define CONFIG_VERSION 1
#define CONFIG_MAGIC_NUMBER 0x4e62

typedef struct {
  uint16_t magic_number;
  uint16_t version;

  uint8_t keymap_index;
  adsr_settings_t adsr;
} config_t;

const config_t *saved_config =
    (const config_t *)(XIP_BASE + CONFIG_FLASH_OFFSET);

static_assert(sizeof(config_t) < FLASH_PAGE_SIZE);

void load_internal() {
  if (saved_config->magic_number != CONFIG_MAGIC_NUMBER) {
    // magic number does not match, this is no config, ignore
    return;
  }

  if (saved_config->version != CONFIG_VERSION) {
    // version does not match, ignore
    return;
  }

  // apply config data
  set_keymap_by_index(saved_config->keymap_index);
  memcpy(&adsr_settings, &(saved_config->adsr), sizeof(adsr_settings));
}

void config_init() {
  flash_safe_execute_core_init();

  flash_safe_execute(load_internal, NULL, 1000);
}

void save_internal() {
  config_t config;
  config.magic_number = CONFIG_MAGIC_NUMBER;
  config.version = CONFIG_VERSION;

  // collect config data
  config.keymap_index = get_keymap_index();
  memcpy(&config.adsr, &adsr_settings, sizeof(adsr_settings));

  // only write if necessary
  if (!memcmp(&config, saved_config, sizeof(config_t))) {
    return;
  }

  // write to flash
  uint8_t buffer[FLASH_PAGE_SIZE];
  memset(buffer, 0, FLASH_PAGE_SIZE);
  memcpy(buffer, &config, sizeof(config));

  flash_range_erase(CONFIG_FLASH_OFFSET, FLASH_SECTOR_SIZE);
  flash_range_program(CONFIG_FLASH_OFFSET, buffer, FLASH_PAGE_SIZE);
}

void config_save() { flash_safe_execute(save_internal, NULL, 1000); }