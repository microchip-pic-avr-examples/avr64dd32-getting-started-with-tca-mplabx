/**
* \file main.c
*
* \brief Main source file.
*
(c) 2022 Microchip Technology Inc. and its subsidiaries.
    Subject to your compliance with these terms, you may use this software and
    any derivatives exclusively with Microchip products. It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*
*/

/*   Using default clock 4MHz,  TMR_CLK = F_CPU / PRESCALER = 4MHz / 16 = 250kHz */
#define PERIOD_EXAMPLE_VALUE_L       249    /* fpwm = 250kHz/(249+1) = 1.000 kHz */
#define PERIOD_EXAMPLE_VALUE_H        74    /* fpwm = 250kHz/(74+1)  = 3.333 kHz */
#define DUTY_CYCLE_EXAMPLE_VALUE_L    ((PERIOD_EXAMPLE_VALUE_L+1)/2) /* 50% duty cycle */
#define DUTY_CYCLE_EXAMPLE_VALUE_H    ((PERIOD_EXAMPLE_VALUE_H+1)/3) /* 33% duty cycle */

#include <avr/io.h>

/* Default fuses configuration:
- BOD disabled
- Oscillator in High-Frequency Mode
- UPDI pin active(WARNING: DO NOT CHANGE!)
- RESET pin used as GPIO
- CRC disabled
- MVIO enabled for dual supply
- Watchdog Timer disabled
*/
FUSES =
{
    .BODCFG = ACTIVE_DISABLE_gc | LVL_BODLEVEL0_gc | SAMPFREQ_128Hz_gc | SLEEP_DISABLE_gc,
    .BOOTSIZE = 0x0,
    .CODESIZE = 0x0,
    .OSCCFG = CLKSEL_OSCHF_gc,
    .SYSCFG0 = CRCSEL_CRC16_gc | CRCSRC_NOCRC_gc | RSTPINCFG_GPIO_gc | UPDIPINCFG_UPDI_gc,
    .SYSCFG1 = MVSYSCFG_DUAL_gc | SUT_0MS_gc,
    .WDTCFG = PERIOD_OFF_gc | WINDOW_OFF_gc,
};

void TCA0_init(void);
void PORT_init(void);

void TCA0_init(void)
{
    /* set waveform output on PORT C */ 
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTC_gc;
    
    /* enable split mode */
    TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;                 
    
    TCA0.SPLIT.CTRLB = TCA_SPLIT_HCMP0EN_bm     /* enable compare channel 0 for the higher byte */
                     | TCA_SPLIT_LCMP0EN_bm;    /* enable compare channel 0 for the lower byte */
    
       
    
    /* set the PWM frequencies and duty cycles */
    TCA0.SPLIT.LPER  = PERIOD_EXAMPLE_VALUE_L;                          
    TCA0.SPLIT.LCMP0 = DUTY_CYCLE_EXAMPLE_VALUE_L;                           
    TCA0.SPLIT.HPER  = PERIOD_EXAMPLE_VALUE_H;                             
    TCA0.SPLIT.HCMP0 = DUTY_CYCLE_EXAMPLE_VALUE_H;                               
    
    TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV16_gc    /* set clock source (sys_clk/16) */
                     | TCA_SPLIT_ENABLE_bm;         /* start timer */
}

void PORT_init(void)
{
    PORTC.DIRSET = PIN0_bm    /* set pin 0 of PORT C as output */
                 | PIN3_bm;   /* set pin 3 of PORT C as output */
}

int main(void)
{
    PORT_init();
    TCA0_init();
    
    while (1) 
    {
        ;
    }
}

