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


/*  Using default clock 4MHz, TMR_CLK = F_CPU / PRESCALER = 4MHz / 64 = 62.5 kHz */
#define PERIOD_EXAMPLE_VALUE   15624  /* T = 250ms, F = 62.5kHz/(15624+1) = 4 Hz */


#include <avr/io.h>
#include <avr/interrupt.h>

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
    /* enable overflow interrupt */
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    
    /* set Normal mode */
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
    
    /* disable event counting */
    TCA0.SINGLE.EVCTRL &= ~TCA_SINGLE_CNTAEI_bm;
    
    /* set the period */
    TCA0.SINGLE.PER = PERIOD_EXAMPLE_VALUE;  
    
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc         /* set clock source (sys_clk/64) */
                      | TCA_SINGLE_ENABLE_bm;                /* start timer */
}

void PORT_init(void)
{
    /* set pin 5 of PORT F as output */
    PORTF.DIRSET = PIN5_bm;
}

ISR(TCA0_OVF_vect)
{
    /* Toggle pin 5 of PORT F */
    PORTF.OUTTGL = PIN5_bm;
    
    /* Clear the interrupt flag */
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

int main(void)
{
    PORT_init();
    TCA0_init();
    
    /* enable global interrupts */
    sei();
    
    while (1) 
    {
        ;
    }
}