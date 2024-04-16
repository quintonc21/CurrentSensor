/*
 * Author: Ben Westcott
 * Date created: 8/18/23
 */
#include <ml_tc0.h>
#include <ml_tc_common.h>

void TC0_init(void)
{
    TC_disable(TC0);
    TC_swrst(TC0);

    TC0->COUNT16.CTRLA.bit.PRESCALER = PRESCALER_DIV1;
    TC0->COUNT16.CTRLA.bit.MODE = MODE_COUNT16;
    TC0->COUNT16.CTRLA.bit.PRESCSYNC = PRESCSYNC_PRESC;

    TC0->COUNT16.WAVE.bit.WAVEGEN = WAVEGEN_MFRQ;

    TC0->COUNT16.CC[0].reg = TC_COUNT16_CC_CC(50);
    while(TC0->COUNT16.SYNCBUSY.bit.CC0)
    {
        /* Wait for sync */
    }

    TC_set_oneshot(TC0);

    ML_TC_CLR_INTFLAGS(TC0);

    //ML_TC_OVF_INTSET(TC0);
    //NVIC_EnableIRQ(TC0_IRQn);
    //NVIC_SetPriority(TC0_IRQn, 2);

    //ML_TC_OVF_CLR_INTFLAG(TC0);
}

/*
const ml_pin_settings tc0_cc0_pin = 
{
    PORT_GRP_A, 4, PF_E, PP_EVEN, OUTPUT_PULL_DOWN, DRIVE_ON
};

void TC0_CC0_pinout(void)
{
    peripheral_port_init(&tc0_cc0_pin);
}*/

void TC0_intset
(
    _Bool ovf, _Bool err, _Bool mc0, _Bool mc1, 
    _Bool nvic_enable, 
    const uint32_t nvic_prilvl
)
{
    TC0->COUNT16.INTENSET.bit.OVF = ovf;
    TC0->COUNT16.INTENSET.bit.ERR = err;
    TC0->COUNT16.INTENSET.bit.MC0 = mc0;
    TC0->COUNT16.INTENSET.bit.MC1 = mc1;

    if(nvic_enable)
    {
        NVIC_EnableIRQ(TC0_IRQn);
        NVIC_SetPriority(TC0_IRQn, nvic_prilvl);
    }
}

void TC0_intclr
(
    _Bool ovf, _Bool err, _Bool mc0, _Bool mc1,
    _Bool nvic_disable
)
{    
    TC0->COUNT16.INTENCLR.bit.OVF = ovf;
    TC0->COUNT16.INTENCLR.bit.ERR = err;
    TC0->COUNT16.INTENCLR.bit.MC0 = mc0;
    TC0->COUNT16.INTENCLR.bit.MC1 = mc1;

    if(nvic_disable)
    {
        NVIC_DisableIRQ(TC0_IRQn);
    }

}
