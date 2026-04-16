#include "framework.h"

#include "device_algorithm.h"

int Init(
    unsigned long address,
    unsigned long clock,
	unsigned long function)
{
    /* Only for old version uVision debugger */
    (void)clock;

    int ret = __init((uint32_t)address, (uint32_t)function);

    return ret;
}

int UnInit(unsigned long function)
{
    int ret = __deinit((uint32_t)function);

    return ret;
}

int EraseChip(void)
{
    int ret = __erase_bank();

    return ret;
}

int EraseSector(unsigned long address)
{
    int ret = __erase_sector((uint32_t)address);

    return ret;
}

int ProgramPage(
    unsigned long address,
    unsigned long size,
    unsigned char *buffer)
{
    int ret = __program((uint32_t)address, (uint32_t)size, (uint8_t *)buffer);

    return ret;
}

unsigned long Verify(
    unsigned long address,
    unsigned long size,
    unsigned char *buffer)
{
    int ret = __verify((uint32_t)address, (uint32_t)size, (uint8_t *)buffer);

    return ret;
}
