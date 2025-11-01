/***************************************************************************
* Example sketch for the ADS1220_WE library
*
* This sketch shows how to change channels and get data. The sketch uses the 
* the default settings, e.g. the internal voltage reference. This limits the 
* maximum voltage that can be measured to 2.048 volts.
* 
* On my website you find:
*   1) An article about the ADS1220 and the use of this library
*      https://wolles-elektronikkiste.de/ads1220-4-kanal-24-bit-a-d-wandler  (German) 
*      https://wolles-elektronikkiste.de/en/4-channel-24-bit-adc-ads1220     (English)
* 
*   2) An article how to use the ADS1220 for typical applications, such as thermocouples, 
*      NTCs, RTDs or Wheatstone bridges:
*      https://wolles-elektronikkiste.de/ads1220-teil-2-anwendungen          (German)
*      https://wolles-elektronikkiste.de/en/ads1220-part-2-applications      (English)
* 
***************************************************************************/

#include <ADS1220_WE.h>
#include <SPI.h>

/* If you only have one SPI device, then you can save one pin by defining
  CS_PIN as -1 and connecting CS of the ADS1220 to GND */
#define ADS1220_CS_PIN    5 // chip select pin
#define ADS1220_DRDY_PIN  4 // data ready pin 

/* Create your ADS1220 object */
ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);
/* Alternatively you can also pass the SPI object as reference */
// ADS1220_WE ads = ADS1220_WE(&SPI, ADS1220_CS_PIN, ADS1220_DRDY_PIN);

void setup(){
  Serial.begin(9600);
  if(!ads.init()){
    Serial.println("ADS1220 is not connected!");
    while(1);
  }
/* The voltages to be measured need to be between negative VREF + 0.2 V and positive
 * VREF -0.2 V if PGA is enabled. For this basic example I disable PGA, to be on the 
 * safe side. */ 
 ads.bypassPGA(true);
}

/* 
 *  You set the channels to be measured with setCompareChannels(); You
 * can choose the following parameters:
 * Parameter                  Pos. Input     Neg. Input         Comment
 * ADS1220_MUX_0_1              AIN0           AIN1
 * ADS1220_MUX_0_2              AIN0           AIN2
 * ADS1220_MUX_0_3              AIN0           AIN3
 * ADS1220_MUX_1_2              AIN1           AIN2
 * ADS1220_MUX_1_3              AIN1           AIN3
 * ADS1220_MUX_2_3              AIN2           AIN2
 * ADS1220_MUX_1_0              AIN1           AIN0
 * ADS1220_MUX_3_2              AIN3           AIN2
 * ADS1220_MUX_0_AVSS           AIN0           AVSS           single-ended
 * ADS1220_MUX_1_AVSS           AIN1           AVSS           single-ended  
 * ADS1220_MUX_2_AVSS           AIN2           AVSS           single-ended
 * ADS1220_MUX_3_AVSS           AIN3           AVSS           single-ended
 * ADS1220_MUX_REFPX_REFNX_4   REFP0/REFP1   REFN0/REFN1     (REFPX-REFNX)/4; PGA bypassed
 * ADS1220_MUX_AVDD_M_AVSS_4    AVDD           AVSS           (AVDD-AVSS)/4; PGA bypassed
 * ADS1220_MUX_AVDD_P_AVSS_2  (AVDD+AVSS)/2 (AVDD+AVSS)/2     for self test
 * 
 * The last three modes use the internal reference (2.048 V) and gain = 1, independent of 
 * your settings.
 */


void loop(){
  float result = 0.0;
  long longResult = 0;
    
  ads.setCompareChannels(ADS1220_MUX_0_1);
  result = ads.getVoltage_mV();
  longResult = ads.getRawData();
  Serial.print("AIN0 vs. AIN1  [mV]: ");
  Serial.println(result);
  Serial.print("AIN0 vs. AIN1 (raw): ");  // raw data
  Serial.println(longResult);
  
  ads.setCompareChannels(ADS1220_MUX_0_AVSS);
  result = ads.getVoltage_mV();
  Serial.print("AIN0 vs. AVSS  [mV]: ");
  Serial.println(result);

  ads.setCompareChannels(ADS1220_MUX_1_AVSS);
  result = ads.getVoltage_mV();
  Serial.print("AIN1 vs. AVSS  [mV]: ");
  Serial.println(result);
  
  ads.setCompareChannels(ADS1220_MUX_2_AVSS);
  result = ads.getVoltage_muV();    // request result in microvolts
  Serial.print("AIN2 vs. AVSS  [µV]: ");
  Serial.println(result);
  
  ads.setCompareChannels(ADS1220_MUX_3_AVSS);
  result = ads.getVoltage_mV();
  Serial.print("AIN3 vs. AVSS  [mV]: ");
  Serial.println(result);
  
  ads.setCompareChannels(ADS1220_MUX_AVDD_M_AVSS_4); // voltage supply / 4
  result = ads.getVoltage_mV() * 4.0;
  Serial.print("Supply Voltage [mV]: ");
  Serial.println(result);

  result = ads.getVRef_V();                 // get the reference voltage
  Serial.print("Reference       [V]: ");
  Serial.println(result,3); 
  
  Serial.print("Temperature    [°C]: ");    // get the temperature
  result = ads.getTemperature();
  Serial.println(result);
  
  Serial.println();
 
  delay(2000);
}

