/*
* dio.c
* this is a digital i/o module that handles leds and buttons
* manages led state with rate limiting to prevent rapid changes
*/

#include "dio.h"

// internal led stuff
static uint8_t led_state = 0;          // current led state
static uint32_t led_rate_ms = 100;     // default cooldown time (100ms)
static uint8_t led_update_allowed = 0; // flag to check if we can update leds

// function pointer for button callback
static void (*button_callback)(void) = 0x00;


// handles button press interrupts
void EXTI0_IRQHandler(void)
{
    // call the button handler if we have one
    if (button_callback != 0x00) {
        button_callback();
    }

    // clear the interrupt flag
    EXTI->PR |= EXTI_PR_PR0;
}


// TIM2 IRQ Handler for led rate limiting
void TIM2_IRQHandler(void)
{
    // re-enable LED updates
    led_update_allowed = 1;

    // stop the timer
    TIM2->CR1 &= ~TIM_CR1_CEN;

    // clear the interrupt flag
    TIM2->SR &= ~TIM_SR_UIF;
}

// enable clocks of perifs
static void enable_clocks(void)
{
    // enable gpio clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;

    // enable tim2 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

//initialise board
static void initialise_board(void)
{
    //config led pins (PE8-PE15) as outputs
    uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
    *led_output_registers = 0x5555;
}

// config timer for led rate limiting
static void configure_timer(void)
{
    // config tim2 for 1ms interrupts
    TIM2->PSC = 7999;  // 8mhz / 8000 = 1khz
    TIM2->ARR = led_rate_ms;

    // enable update interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // configure NVIC for tim2 interrupt
    NVIC_SetPriority(TIM2_IRQn, 1);  // higher priority than button
    NVIC_EnableIRQ(TIM2_IRQn);

    // dont start yet, wait till requested
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

//enable button interrupt
static void enable_interrupt(void)
{
    __disable_irq();

    // enable system conifgs
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // configure PA0 as EXTI0 source
    SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PA;

    // configure EXTI0 to trigger on rising edge
    EXTI->RTSR |= EXTI_RTSR_TR0;

    // enable EXTI0 interrupt
    EXTI->IMR |= EXTI_IMR_MR0;

    // configure NVIC for EXTI0 interrupt
    NVIC_SetPriority(EXTI0_IRQn, 2);
    NVIC_EnableIRQ(EXTI0_IRQn);

    // re-enable interrupts
    __enable_irq();
}

//initialise digital io with callback function
void dio_init(void (*callback)(void))
{
    //set the button callback function
    button_callback = callback;

    //initialise hardware
    enable_clocks();
    initialise_board();
    configure_timer();
    enable_interrupt();

    //initialise led state
    led_state = 0x00;
    led_update_allowed = 1;

    // apply initial state
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
    *led_register = led_state;
}

//get the current state, and return it.
uint8_t dio_get_led_state(void)
{
    return led_state;
}

//set the leds state, taking a uint8 as input
void dio_set_led_state(uint8_t state)
{
    // only update if allowed
    if (led_update_allowed) {
        // update the state
        led_state = state;

        // update led hardware immediately
        uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
        *led_register = led_state;

        // stop updates during cooldown
        led_update_allowed = 0;

        // reset counter
        TIM2->CNT = 0;

        // start timer to re-enable updates after delay
        TIM2->CR1 |= TIM_CR1_CEN;
    }
}


//conif max rate leds can change state
void dio_set_led_rate(uint32_t rate_ms)
{
    // update rate limit
    led_rate_ms = (rate_ms < 1) ? 1 : rate_ms;  // ensure min of 1ms

    // update timer period
    TIM2->ARR = led_rate_ms - 1;
}
