#include "midi_ble_client.h"

#include "ble_midi_client.h"
#include "pico/cyw43_arch.h"

void midi_ble_client_init()
{
    printf("init midi ble client: ");

    if (cyw43_arch_init())
    {
        printf("failed to initialise cyw43_arch\n");
        return;
    }

    const char client_name[] = "Keytar";
    ble_midi_client_init(client_name, strlen(client_name), IO_CAPABILITY_DISPLAY_ONLY, SM_AUTHREQ_SECURE_CONNECTION | SM_AUTHREQ_MITM_PROTECTION | SM_AUTHREQ_BONDING);

    ble_midi_client_scan_begin();
    printf("done\n");
}

void midi_ble_client_write(uint8_t len, uint8_t *msg)
{
    if (!ble_midi_client_is_connected)
    {
        return;
    }

    ble_midi_client_stream_write(len, msg);
}