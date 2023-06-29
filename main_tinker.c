#include "wr_i2c.h"
#include "si5351_tinker.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
  i2c_init();

  const int32_t correction = 978;
  si5351_Init(correction);

  si5351PLLConfig_t pll_conf;
  si5351OutputConfig_t out_conf;
  int32_t Fclk = 24000000; // 7 MHz

  si5351_CalcIQ(Fclk, &pll_conf, &out_conf);

  
  /* `phaseOffset` is a 7bit value, calculated from Fpll, Fclk and desired phase shift.
  * To get N° phase shift the value should be round( (N/360)*(4*Fpll/Fclk) )
  * Two channels should use the same PLL to make it work. There are other restrictions.
  * Please see AN619 for more details.
  *
  * si5351_CalcIQ() chooses PLL and MS parameters so that:
  *   Fclk in [1.4..100] MHz
  *   out_conf.div in [9..127]
  *   out_conf.num = 0
  *   out_conf.denum = 1
  *   Fpll = out_conf.div * Fclk.
  * This automatically gives 90° phase shift between two channels if you pass
  * 0 and out_conf.div as a phaseOffset for these channels.
  */
  uint8_t phaseOffset = (uint8_t)out_conf.div;
  si5351_SetupOutput(0, SI5351_PLL_A, SI5351_DRIVE_STRENGTH_4MA, &out_conf, 0);
  si5351_SetupOutput(1, SI5351_PLL_A, SI5351_DRIVE_STRENGTH_4MA, &out_conf, phaseOffset);
/*
  <F12>
  * The order is important! Setup the channels first, then setup the PLL.
  * Alternatively you could reset the PLL after setting up PLL and channels.
  * However since _SetupPLL() always resets the PLL this would only cause
  * sending extra I2C commands.
  */
  si5351_SetupPLL(SI5351_PLL_A, &pll_conf);
  si5351_EnableOutputs((1<<0) | (1<<1));
  printf("finished\n");
}
