#include <adc_register.h>

void ADC_Register_Init(void)
{
    /* 1. Enable GPIOA clock */
    RCC->AHB2ENR |= (1 << 0);

    /* 2. Enable ADC clock */
    RCC->AHB2ENR |= (1 << 13);

    /* 3. PA0 -> Analog mode */
    GPIOA->MODER &= ~(3 << 0); //clear
    GPIOA->MODER |=  (3 << 0); //set

    /* 4. Connect PA0 to ADC */
    GPIOA->ASCR |= (1 << 0);

    /* 5. Select ADC clock = SYSCLK */
    RCC->CCIPR &= ~(3 << 28);
    RCC->CCIPR |=  (3 << 28);

    /* 6. Exit ADC deep-power-down */
    ADC1->CR &= ~(1 << 29);

    /* 7. Enable ADC voltage regulator */
    ADC1->CR |= (1 << 28);

    /* Small delay */
    for (volatile uint32_t i = 0; i < 1000; i++);

    /* 8. Start calibration */
    ADC1->CR |= (1 << 31);

    /* Wait for calibration */
    while (ADC1->CR & (1 << 31));

    /* 9. Sampling time for Channel 5
       bits 17:15 = 111 */
    ADC1->SMPR1 &= ~(7 << 15);
    ADC1->SMPR1 |=  (7 << 15);

    /* 10. Select Channel 5 as first conversion */
    ADC1->SQR1 &= ~(0x1F << 6);
    ADC1->SQR1 |=  (5 << 6);

    /* 11. One conversion only */
    ADC1->SQR1 &= ~(0xF << 0);

    /* 12. 12-bit resolution */
    ADC1->CFGR &= ~(3 << 3);

    /* 13. Right alignment */
    ADC1->CFGR &= ~(1 << 5);

    /* 14. Single conversion mode */
    ADC1->CFGR &= ~(1 << 13);

    /* 15. Software trigger */
    ADC1->CFGR &= ~(3 << 10);

    /* 16. Clear ADRDY */
    ADC1->ISR |= (1 << 0);

    /* 17. Enable ADC */
    ADC1->CR |= (1 << 0);

    /* 18. Wait until ADC ready */
    while (!(ADC1->ISR & (1 << 0)));
}


uint32_t ADC_Register_Read(void)
{
    /* Start conversion */
    ADC1->CR |= (1 << 2);

    /* Wait for EOC */
    while (!(ADC1->ISR & (1 << 2)));

    /* Return ADC result */
    return ADC1->DR;
}
