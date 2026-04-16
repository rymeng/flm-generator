#include "framework.h"

struct default_device_information const FlashDevice = {
    FRAMEWORK_VERSION,
    "RT32F7",
    ONCHIP_DEVICE,
    0x08000000UL,
    0x00080000UL,
    512UL,
    0UL,
    0xFFU,
    1000UL,
    1000UL,
    {
        {1024UL, 0UL},
        SECTOR_TYPE_LIST_END_SYMBOL
    }
};
