#include "stm8s.h"

#define LED_GPIO_PORT    (GPIOB)
#define LED_GPIO_PIN     (GPIO_PIN_5)

volatile uint8_t timer_flag = 0;

 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
    TIM4->SR1 &= ~TIM4_SR1_UIF; // Clear interrupt flag
    timer_flag = 1;             // Signal flag
}

void TIM4_Config_ms(uint8_t ms)
{
    // Stop timer before configuration
    TIM4->CR1 &= ~TIM4_CR1_CEN;
    // Disable update interrupt
    TIM4->IER &= ~TIM4_IER_UIE;
    // Clear interrupt flag
    TIM4->SR1 &= ~TIM4_SR1_UIF;

    // Set prescaler to 128 (clock/128)
    TIM4->PSCR = 0x07;
    // 1 ms delay
    TIM4->ARR = 15 * ms;

    // Reset counter
    TIM4->CNTR = 0;

    // Enable update interrupt
    TIM4->IER |= TIM4_IER_UIE;

    // Start timer
    TIM4->CR1 |= TIM4_CR1_CEN;
}

void DelayMs(uint16_t ms)
{
    timer_flag = 0;
    TIM4_Config_ms(ms);
    rim();                       // Enable global interrupts
    wfi();                       // Enter deep sleep, wait for interrupt
    while (!timer_flag);         // Wait for ISR to set flag
    TIM4->CR1 &= ~TIM4_CR1_CEN;  // Stop timer
    TIM4->IER &= ~TIM4_IER_UIE;  // Disable timer interrupt
}

void main(void)
{
  uint16_t i = 0;

  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN,  GPIO_MODE_OUT_PP_LOW_FAST);

  while (1)
  {
    if (i <= 20)
    {
      GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN);
    }
    else if (i <= 500)
    {
      GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN);
    }
    else
    {
      i = 0;
    }
    i++;
    DelayMs(17);
  }
}


/**
  C:\Users\xxx\.platformio\packages\tool-stm8flash>stm8flash.exe -u -c stlinkv2 -p STM8S103F3P6
  Determine OPT area
  Due to its file extension (or lack thereof), "Workaround" is considered as RAW BINARY format!
  Unlocked device. Option bytes reset to default state.
  Bytes written: 11
  */ 

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif