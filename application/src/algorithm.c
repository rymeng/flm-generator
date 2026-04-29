#include "algorithm.h"

/* Import framework macros */
#include "framework.h"

uint32_t __mem_region_init(uint32_t address, uint32_t function)
{
    uint32_t ret = RESULT_OK;

    if ((address >= FLASH_BANK1_MAIN_BLOCK_BASE_ADDR) &&
        (address <= FLASH_BANK1_MAIN_BLOCK_END_ADDR)) {
        if ((function == FUNCTION_ERASE) || (function == FUNCTION_PROGRAM)) {
            /* Unlock FMC 1 */
            FLASH->UNLOCKR1.reg = FMC_KEY1;
            __DSB();
            FLASH->UNLOCKR1.reg = FMC_KEY2;
            __DSB();

            /* Check FMC 1 lock status */
            if (FLASH->CR1.reg & FLASH_CR1_LOCK_Msk) {
                ret = RESULT_ERROR;
            }
        }
    } else if ((address >= FLASH_BANK2_MAIN_BLOCK_BASE_ADDR) &&
               (address <= FLASH_BANK2_MAIN_BLOCK_END_ADDR)) {
        if ((function == FUNCTION_ERASE) || (function == FUNCTION_PROGRAM)) {
            /* Unlock FMC 2 */
            FLASH->UNLOCKR2.reg = FMC_KEY1;
            __DSB();
            FLASH->UNLOCKR2.reg = FMC_KEY2;
            __DSB();

            /* Check FMC 2 lock status */
            if (FLASH->CR2.reg & FLASH_CR2_LOCK_Msk) {
                ret = RESULT_ERROR;
            }
        }
    } else if ((address >= ITCM_BASE_ADDR) && (address <= ITCM_END_ADDR)) {
        ;
    } else if ((address >= DTCM_BASE_ADDR) && (address <= DTCM_END_ADDR)) {
        ;
    } else if ((address >= SRAM1_BASE_ADDR) && (address <= SRAM1_END_ADDR)) {
        ;
    } else if ((address >= SRAM2_BASE_ADDR) && (address <= SRAM2_END_ADDR)) {
        ;
    } else {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __mem_region_deinit(uint32_t function)
{
    uint32_t ret = RESULT_OK;

    if ((function == FUNCTION_ERASE) || (function == FUNCTION_PROGRAM)) {
        /* Lock FMC 1 & FMC 2 */
        FLASH->CR1.reg |= FLASH_CR1_LOCK_Msk;
        FLASH->CR2.reg |= FLASH_CR2_LOCK_Msk;
        __DSB();

        /* Check FMC 1 & FMC 2 lock status */
        if (!(FLASH->CR1.reg & FLASH_CR1_LOCK_Msk) ||
            !(FLASH->CR2.reg & FLASH_CR2_LOCK_Msk)) {
            ret = RESULT_ERROR;
        }
    }

    return ret;
}

uint32_t __mem_region_erase_bank(void)
{
    uint32_t ret = RESULT_OK;

    /* Unlock FMC 1 */
    FLASH->UNLOCKR1.reg = FMC_KEY1;
    __DSB();
    FLASH->UNLOCKR1.reg = FMC_KEY2;
    __DSB();

    /* Unlock FMC 2 */
    FLASH->UNLOCKR2.reg = FMC_KEY1;
    __DSB();
    FLASH->UNLOCKR2.reg = FMC_KEY2;
    __DSB();

    /* Clear EOP flag */
    FLASH->SR1.reg |= FLASH_SR1_EOP_Msk;
    FLASH->SR2.reg |= FLASH_SR2_EOP_Msk;
    __DSB();

    /* Enable main block erasure */
    FLASH->CR1.reg |= FLASH_CR1_BEREN_Msk;
    FLASH->CR2.reg |= FLASH_CR2_BEREN_Msk;
    __DSB();

    /* Start main block erasure */
    FLASH->CR1.reg |= FLASH_CR1_START_Msk;
    FLASH->CR2.reg |= FLASH_CR2_START_Msk;

    /* Wait for erasure start */
    while ((FLASH->CR1.reg & FLASH_CR1_START_Msk) ||
        (FLASH->CR2.reg & FLASH_CR2_START_Msk)) {
        ;
    }

    /* Wait for erasure done */
    while ((FLASH->SR1.reg & FLASH_SR1_BUSY_Msk) ||
        (FLASH->SR2.reg & FLASH_SR2_BUSY_Msk)) {
        ;
    }

    /* Disable main block erasure */
    FLASH->CR1.reg &= ~FLASH_CR1_BEREN_Msk;
    FLASH->CR2.reg &= ~FLASH_CR2_BEREN_Msk;
    __DSB();

    /* Check FMC 1 EOP flag */
    if (FLASH->SR1.reg & FLASH_SR1_EOP_Msk) {
        /* Clear FMC 1 EOP flag */
        FLASH->SR1.reg |= FLASH_SR1_EOP_Msk;
        __DSB();
    } else {
        ret = RESULT_ERROR;
    }

    /* Check FMC 2 EOP flag */
    if (FLASH->SR2.reg & FLASH_SR2_EOP_Msk) {
        /* Clear FMC 2 EOP flag */
        FLASH->SR2.reg |= FLASH_SR2_EOP_Msk;
        __DSB();
    } else {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __mem_region_erase_sector(uint32_t address)
{
    uint32_t ret = RESULT_OK;

    if ((address >= FLASH_BANK1_MAIN_BLOCK_BASE_ADDR) &&
        (address <= FLASH_BANK1_MAIN_BLOCK_LAST_ERASE_UNIT_ADDR)) {
        /* Clear EOP flag */
        FLASH->SR1.reg |= FLASH_SR1_EOP_Msk;
        __DSB();

        /* Write destination address */
        FLASH->AR1.reg = address;

        /* Enable sector erasure */
        FLASH->CR1.reg |= FLASH_CR1_SEREN_Msk;
        __DSB();

        /* Start erasing */
        FLASH->CR1.reg |= FLASH_CR1_START_Msk;

        /* Wait for erasing start */
        while (FLASH->CR1.reg & FLASH_CR1_START_Msk) {
            ;
        }

        /* Wait for erasing done */
        while (FLASH->SR1.reg & FLASH_SR1_BUSY_Msk) {
            ;
        }

        /* Disable sector erasure */
        FLASH->CR1.reg &= ~FLASH_CR1_SEREN_Msk;
        __DSB();

        /* Check FMC 1 EOP flag */
        if (FLASH->SR1.reg & FLASH_SR1_EOP_Msk) {
            /* Clear FMC 1 EOP flag */
            FLASH->SR1.reg |= FLASH_SR1_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= FLASH_BANK1_MAIN_BLOCK_BASE_ADDR) &&
               (address <= FLASH_BANK2_MAIN_BLOCK_LAST_ERASE_UNIT_ADDR)) {
        /* Clear EOP flag */
        FLASH->SR2.reg |= FLASH_SR2_EOP_Msk;
        __DSB();

        /* Write destination address */
        FLASH->AR2.reg = address;

        /* Enable sector erasure */
        FLASH->CR2.reg |= FLASH_CR2_SEREN_Msk;
        __DSB();

        /* Start erasing */
        FLASH->CR2.reg |= FLASH_CR2_START_Msk;

        /* Wait for erasing start */
        while (FLASH->CR2.reg & FLASH_CR2_START_Msk) {
            ;
        }

        /* Wait for erasing done */
        while (FLASH->SR2.reg & FLASH_SR2_BUSY_Msk) {
            ;
        }

        /* Disable sector erasure */
        FLASH->CR2.reg &= ~FLASH_CR2_SEREN_Msk;
        __DSB();

        /* Check FMC 2 EOP flag */
        if (FLASH->SR2.reg & FLASH_SR2_EOP_Msk) {
            /* Clear FMC 2 EOP flag */
            FLASH->SR2.reg |= FLASH_SR2_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= ITCM_BASE_ADDR) &&
               (address <= ITCM_LAST_ERASE_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else if ((address >= DTCM_BASE_ADDR) &&
               (address <= DTCM_LAST_ERASE_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else if ((address >= SRAM1_BASE_ADDR) &&
               (address <= SRAM1_LAST_ERASE_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else if ((address >= SRAM2_BASE_ADDR) &&
               (address <= SRAM2_LAST_ERASE_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __mem_region_program(uint32_t address, uint32_t size, uint8_t *buffer)
{
    uint32_t ret = RESULT_OK;

    if ((address >= FLASH_BANK1_MAIN_BLOCK_BASE_ADDR) &&
        (address <= FLASH_BANK1_MAIN_BLOCK_LAST_PROGRAM_UNIT_ADDR)) {
        /* Data filling */
        uint32_t aligned_size = ALIGN(size, PROGRAM_SIZE);
        if (aligned_size != size) {
            memset(buffer + size, 0xFF, aligned_size - size);
            size = aligned_size;
        }

        /* Enable programming */
        FLASH->CR1.reg |= FLASH_CR1_PGEN_Msk;
        __DSB();

        /* Program */
        for (uint32_t i = 0UL; i < size; i += PROGRAM_SIZE) {
            for (uint32_t j = 0UL; j < PROGRAM_SIZE; j += 8UL) {
                *(uint64_t *)(address + i + j) = *(uint64_t *)(buffer + i + j);
                __DSB();
            }

            /* Wait for programming done */
            while (FLASH->SR1.reg & FLASH_SR1_BUSY_Msk) {
                ;
            }
        }

        /* Disable programming */
        FLASH->CR1.reg &= ~FLASH_CR1_PGEN_Msk;
        __DSB();

        /* Check FMC 1 EOP flag */
        if (FLASH->SR1.reg & FLASH_SR1_EOP_Msk) {
            /* Clear FMC 1 EOP flag */
            FLASH->SR1.reg |= FLASH_SR1_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= FLASH_BANK2_MAIN_BLOCK_BASE_ADDR) &&
               (address <= FLASH_BANK2_MAIN_BLOCK_LAST_PROGRAM_UNIT_ADDR)) {
        /* Data filling */
        uint32_t aligned_size = ALIGN(size, PROGRAM_SIZE);
        if (aligned_size != size) {
            memset(buffer + size, 0xFF, aligned_size - size);
            size = aligned_size;
        }

        /* Enable programming */
        FLASH->CR2.reg |= FLASH_CR2_PGEN_Msk;
        __DSB();

        /* Program */
        for (uint32_t i = 0UL; i < size; i += PROGRAM_SIZE) {
            for (uint32_t j = 0UL; j < PROGRAM_SIZE; j += 8UL) {
                *(uint64_t *)(address + i + j) = *(uint64_t *)(buffer + i + j);
                __DSB();
            }

            /* Wait for programming done */
            while (FLASH->SR2.reg & FLASH_SR2_BUSY_Msk) {
                ;
            }
        }

        /* Disable programming */
        FLASH->CR2.reg &= ~FLASH_CR2_PGEN_Msk;
        __DSB();

        /* Check FMC 2 EOP flag */
        if (FLASH->SR2.reg & FLASH_SR2_EOP_Msk) {
            /* Clear FMC 2 EOP flag */
            FLASH->SR2.reg |= FLASH_SR2_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= ITCM_BASE_ADDR) &&
               (address <= ITCM_LAST_PROGRAM_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else if ((address >= DTCM_BASE_ADDR) &&
               (address <= DTCM_LAST_PROGRAM_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else if ((address >= SRAM1_BASE_ADDR) &&
               (address <= SRAM1_LAST_PROGRAM_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __mem_region_verify(uint32_t address, uint32_t size, uint8_t *buffer)
{
    /* Byte-by-byte verify */
    for (uint32_t i = 0UL; i < size; ++i) {
        if (*(uint8_t *)(address + i) != *(buffer + i)) {
            return address + i;
        }
    }

    return address + size;
}
