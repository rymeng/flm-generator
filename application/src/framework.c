#include "framework.h"

#include "device_algorithm.h"

int Init(
    unsigned long address,
    unsigned long clock,
	unsigned long function)
{
    /* Only for old version uVision debugger */
    (void)clock;

    uint32_t ret = __init((uint32_t)address, (uint32_t)function);

    return (int)ret;
}

int UnInit(unsigned long function)
{
    uint32_t ret = __deinit((uint32_t)function);

    return (int)ret;
}

int EraseChip(void)
{
    uint32_t ret = __erase_bank();

    return (int)ret;
}

int EraseSector(unsigned long address)
{
    uint32_t ret = __erase_sector((uint32_t)address);

    return (int)ret;
}

int ProgramPage(
    unsigned long address,
    unsigned long size,
    unsigned char *buffer)
{
    uint32_t ret = __program((uint32_t)address, (uint32_t)size, (uint8_t *)buffer);

    return (int)ret;
}

unsigned long Verify(
    unsigned long address,
    unsigned long size,
    unsigned char *buffer)
{
    int ret = __verify((uint32_t)address, (uint32_t)size, (uint8_t *)buffer);

    return ret;
}
