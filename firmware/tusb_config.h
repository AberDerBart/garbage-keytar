#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// COMMON CONFIGURATION
//--------------------------------------------------------------------

// defined by compiler flags for flexibility
#ifndef CFG_TUSB_MCU
#error CFG_TUSB_MCU must be defined
#endif

#if CFG_TUSB_MCU == OPT_MCU_LPC43XX || CFG_TUSB_MCU == OPT_MCU_LPC18XX || \
    CFG_TUSB_MCU == OPT_MCU_MIMXRT10XX
#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_HOST | OPT_MODE_HIGH_SPEED)
#else
#define CFG_TUSB_RHPORT0_MODE OPT_MODE_HOST
#endif

#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN __attribute__((aligned(4)))
#endif

//--------------------------------------------------------------------
// CONFIGURATION
//--------------------------------------------------------------------

#define CFG_TUH_HUB 0
#define CFG_TUH_HID 1
#define CFG_TUH_HID_KEYBOARD 1
#define CFG_TUH_HID_MOUSE 0
#define CFG_TUSB_HOST_HID_GENERIC 0  // (not yet supported)
#define CFG_TUH_MSC 0
#define CFG_TUH_CDC 0

#define CFG_TUSB_HOST_DEVICE_MAX \
  (CFG_TUH_HUB ? 5 : 1)  // normal hub has 4 ports

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
