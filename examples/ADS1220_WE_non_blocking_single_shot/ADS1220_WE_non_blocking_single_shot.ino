/***************************************************************************
* Example sketch for the ADS1220_WE library
*
* This sketch shows how to use the non-blocking mode for single-shot measurements.
* Apart from this, the sketch uses the default settings, e.g. the internal voltage 
* reference. This limits the maximum voltage that can be measured to 2.048 volts.
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

#define ADS1220_CS_PIN    7 // chip select pin
#define ADS1220_DRDY_PIN  2 // data ready interrupt pin 
#define MEASUREMENT_PERIOD 2000

volatile bool dataIsReady = false;

ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);

void setup(){
  Serial.begin(9600);
  if(!ads.init()){
    Serial.println("ADS1220 is not connected!");
    while(1);
  }
/* The voltages to be measured need to be between negative VREF + 0.2 V and positive
 * VREF -0.2 V if PGA is enabled. For this basic example I disable PGA, to be on the 
 * safe side. */ 
 ads.setNonBlockingMode(true); // switch ton non-blocking mode
 
 if(ads.getNonBlockingMode()){ // check non-blocking mode status
  Serial.println("Non-blocking mode is activated");
 }
 else Serial.println("Non-Blocking mode is deactivated ");

 ads.bypassPGA(true);
 
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
 * ADS1220_MUX_AVDD_P_AVSS_2    AVDD           AVSS           (AVDD+AVSS)/2
 * 
 * The last three modes use the internal reference (2.048 V) and gain = 1, independent of 
 * your settings.
 */
 ads.setCompareChannels(ADS1220_MUX_0_1);
 attachInterrupt(digitalPinToInterrupt(ADS1220_DRDY_PIN), dataReadyISR, FALLING);
}

void loop(){
  static unsigned long lastMeasurement = 0;
  float result = 0.0;
      
  if(dataIsReady){
    result = ads.getVoltage_mV();
    Serial.print("AIN0 vs. AIN1  [mV]: ");
    Serial.println(result);
    Serial.flush();
    dataIsReady = false; 
  }

  if(millis()-lastMeasurement > MEASUREMENT_PERIOD){
    // ads.setCompareChannels(ADS1220_MUX_0_1); // if you want to change channels, do it here
    ads.start(); // start measurement manually
    lastMeasurement = millis();
  }
}

void dataReadyISR(){
  dataIsReady = true;
}
