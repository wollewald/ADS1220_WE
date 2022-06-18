/***************************************************************************
* Example sketch for the ADS1220_WE library
*
* This sketch shows how the gain and PGA are set and how they change automatically 
* under certain conditions. Also the PGA is forced to be bypassed under certain 
* conditions. If you want to try this example, apply a voltage of < 250 mV to AIN0/
* AIN1 (differential) and to AIN2 (single-ended, i.e. vs. AVSS).
* 
* Further information can be found soon on:
*
* https://wolles-elektronikkiste.de/         (German)
* https://wolles-elektronikkiste.de/en/      (English)
* 
***************************************************************************/

#include <ADS1220_WE.h>
#include <SPI.h>

#define ADS1220_CS_PIN    7
#define ADS1220_DRDY_PIN  6

ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);

void setup(){
  Serial.begin(9600);
  ads.init();

/* You can choose a gain between 1 (default) and 128 using setGain() if PGA is enabled 
 * (default). If PGA is disabled you can still choose a gain factor up to 4. If PGA is 
 * enabled, the amplified voltage shall be between AVSS + 200mV and AVDD - 200mV. Outside 
 * this range linearity drops. For details check the data sheet, section 8.3.2.1.
 * 
 * If you apply a single-ended mode (negative AINx = AVSS), PGA must be bypassed. Accordingly, 
 * the maximum gain is 4. The library does these settings automatically.
 * 
 * For the measurement of reference voltages / supply voltage PGA will also be bypassed. In 
 * this case gain is 1.
 * 
 * The parameters you can choose for setGain() are:
 * ADS1220_GAIN_X with X = 1,2,4,8,16,32,64 or 128
 * 
 * With getGainFactor() you can query the gain. The function returns the effective gain and 
 * not the gain set in the register. Under certian conditions thes are are different. For 
 * example, the effective gain is set to 1 when external references are measured. 
 */
  ads.setGain(ADS1220_GAIN_8); // set gain to 8
  ads.bypassPGA(false);  // redundant, since this is default, but I wanted to highlight this setting. 
}

void loop(){
  ads.setCompareChannels(ADS1220_MUX_0_1);
  float result = ads.getVoltage_mV();
  byte gain = ads.getGainFactor();  // queries the (effective) gain
  Serial.print("AIN0/AIN1 [mV]: ");
  Serial.println(result);
  Serial.print("PGA bypassed (0: no, 1: yes): ");  
  Serial.println(ads.isPGABypassed()); // queries if PGA is bypassed, here the result will be 0
  Serial.print("Current gain: ");  // 
  
  Serial.println(gain);
  Serial.println();
   
  ads.setCompareChannels(ADS1220_MUX_2_AVSS);
  result = ads.getVoltage_mV();
  gain = ads.getGainFactor();
  Serial.print("AIN2/AVSS [mV]: ");
  Serial.println(result);
  Serial.print("PGA bypassed (0: no, 1: yes): "); // the PGA will be bypassed
  Serial.println(ads.isPGABypassed());
  Serial.print("Current gain: ");  // gain will be reduced to 4. 
  Serial.println(gain);
  Serial.println();

  ads.setCompareChannels(ADS1220_MUX_AVDD_M_AVSS_4);
  result = ads.getVoltage_mV();
  gain = ads.getGainFactor();
  Serial.print("AVDD/AVSS [mV]: ");
  Serial.println(result*4.0);
  Serial.print("PGA bypassed (0: no, 1: yes): "); // the PGA will be bypassed
  Serial.println(ads.isPGABypassed());
  Serial.print("Current gain: ");  // gain will be reduced to 1. 
  Serial.println(gain);
  Serial.println();
 
  delay(2000);
}
