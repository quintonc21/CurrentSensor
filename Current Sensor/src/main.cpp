/*
 * Author: Quinton Clark
 * Date created: 4/12/2024
 * 
 * For use of Polulu +-15 A current sensor
 * 
*/

#include <Arduino.h>
#include <ml_clocks.h>
#include <ml_adc_common.h>
#include <ml_adc0.h>
#include <ml_port.h>

//to configure the code, input no current through the sensor, and subtract the base value from the calced base
//then, since the adc is linear, input a current, take the difference and 
//find the conversion rate from base count to expected current

//1 means DC input 0 means AC input
int DC_bit = 1;

const ml_pin_settings adc_pin = {PORT_GRP_B, 8, PF_B, PP_EVEN, ANALOG, DRIVE_OFF};


void setup(void)
{
  MCLK_init();
  GCLK_init();

  Serial.begin(115200);
  
  ADC0_init();
  peripheral_port_init(&adc_pin);

   ADC_enable(ADC0);
   ADC_swtrig_start(ADC0);

  ADC0->INTENSET.bit.RESRDY = true;

}

uint16_t result = 0;
double total = 0;
double base = 0;
int count = 0;
float current = 0;
float maximum = 0;
int maxCount = 0;
void loop(void)
{
  if(ADC0->INTFLAG.bit.RESRDY == true)
  {
    result = (uint16_t)ADC0->RESULT.reg;
    //DC
    if (DC_bit == 1){

      base = 1982.3; //change base to measured value
      total += result;   
      count++;
      if(count == 100000) {
        total = total/count;
        Serial.print(total);
        Serial.print(" base - ");
        current = (total- base)*3.5; 
        Serial.print(current, 1);
        Serial.println(" mAmps ");
        count=0;
        total = 0;
      }
    }
    //AC
    else if(DC_bit == 0){
      base = 2001; //set base value here from measurements
      if(result > maximum) {
        maximum = result;
      }
      count++;
      if(count == 40000) {
        total += maximum;
        maxCount++;
        if(maxCount == 20) {
          total = total/maxCount;
          Serial.print(total);
          Serial.print(" max(base) - "); 
          current = ((total - base)*3.5)/sqrt(2); 
          Serial.print(current, 2);
          Serial.println(" mAmps(RMS)");
          total = 0;
          maxCount = 0;
        }
        count=0;
        maximum = 0;
      }
    } 
  }
}