#include "board.h"

void board_uart_init(void)
{
    /* Enable peripheral clock */
    RCC->UNLOCKR.reg = 0x55AA6699UL;
    __DSB();
    RCC->AHBEN1.reg |= RCC_AHBEN1_GPIOAEN_Msk;
    RCC->APBEN5.reg |= RCC_APBEN5_UART1EN_Msk;
    __DSB();
    RCC->UNLOCKR.reg = 0x55AA6698UL;
    __DSB();

    /* Disable enhanced protection */
    SYSCTRL->UNLOCKR.reg = 0x55AA6699UL;
    __DSB();
    SYSCTRL->EPROTR3.reg |= SYSCTRL_EPROTR3_GPIOADIS_Msk;
    __DSB();
    SYSCTRL->UNLOCKR.reg = 0x55AA6698UL;
    __DSB();

    /* Initialize PA01 as UART1 TX pin */
    GPIOA->AFR1.reg &= ~GPIO_AFR1_PIN12_Msk;
    GPIOA->AFGR1.reg &= ~GPIO_AFGR1_PIN12_Msk;
    GPIOA->AFGR1.reg |= 0x1UL << GPIO_AFGR1_PIN12_Pos;
    GPIOA->AFR1.reg |= 0x2UL << GPIO_AFR1_PIN12_Pos;
    GPIOA->AMSELR.reg &= ~GPIO_AMSELR_PIN12_Msk;

    /* Reset UART1 */
    UART1->CR1.reg |= UART_CR1_RST_Msk;
    __DSB();

    /* Release UART1 */
    UART1->CR1.reg &= ~UART_CR1_RST_Msk;
    __DSB();

    /* Set UART1 baud */
    UART1->CR2.reg = (0x6UL << UART_CR2_OSR_Pos) |
                     (0x1UL << UART_CR2_DESEN_Pos) |
                     (0x95UL << UART_CR2_DIV_Pos);
    __DSB();

    /* Enable UART1 TX module */
    UART1->CR3.reg |= UART_CR3_TXEN_Msk;
    __DSB();

    return;
}

_ARMABI __attribute__((__nonnull__(2))) int fputc(int c, FILE *stream)
{
    (void)stream;

    while ((UART1->CSR1.reg & UART_CSR1_TXRDY_Msk) != UART_CSR1_TXRDY_Msk) {
        ;
    }
    UART1->DR.reg = c;
    while ((UART1->CSR1.reg & UART_CSR1_TXC_Msk) != UART_CSR1_TXC_Msk) {
        ;
    }

    return c;
}
