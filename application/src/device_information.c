#include "device_framework.h"

struct FlashDevice const FlashDevice = {
    FLASH_DRV_VERS,
    "RT32F7",
    ONCHIP,
    0x08000000UL,
    0x00080000UL,
    512UL,
    0UL,
    0xFFU,
    1000UL,
    1000UL,
    {
        {1024UL, 0UL},
        SECTOR_END
    }
};
