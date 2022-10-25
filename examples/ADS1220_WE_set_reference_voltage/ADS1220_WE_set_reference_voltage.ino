/***************************************************************************
* Example sketch for the ADS1220_WE library
*
* This sketch shows how to set reference voltages (vRef). The maximum voltage 
* you can measure is vRef. However there are some restrictions if using PGA,
* which is the default. 
* For this example connect reference voltages to REFP0/REFN0 and REFP1(AIN0) 
* and REFN1(AIN3). You can then use AIN1 and AIN2 as two single-ended inputs
* or one differential input.  
* 
* On my website you find:
*   1) An article about the ADS1220 and the use of this library
*       https://wolles-elektronikkiste.de/ads1220-4-kanal-24-bit-a-d-wandler  (German) 
*       https://wolles-elektronikkiste.de/en/4-channel-24-bit-adc-ads1220     (English)
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
#define ADS1220_DRDY_PIN  6 // data ready pin 

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
}

/* For better readability of the code, the setting options are explained here and not
 * in loop where the functions are used.
 * 
 * You set the channels to be measured with setCompareChannels(); You
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
 * ADS1220_MUX_0_AVSS           AIN0           AVSS (=AGND)   single-ended
 * ADS1220_MUX_1_AVSS           AIN1           AVSS           single-ended  
 * ADS1220_MUX_2_AVSS           AIN2           AVSS           single-ended
 * ADS1220_MUX_3_AVSS           AIN3           AVSS           single-ended
 * ADS1220_MUX_REFPX_REFNX_4   REFP0/REFP1   REFN0/REFN1     (REFPX-REFNX)/4; PGA bypassed
 * ADS1220_MUX_AVDD_M_AVSS_4    AVDD           AVSS          (AVDD-AVSS)/4; PGA bypassed
 * ADS1220_MUX_AVDD_P_AVSS_2    AVDD           AVSS           AVDD+AVSS)/2
 * The last three modes use the internal reference (2.048 V) and gain = 1, independent of 
 * your settings.
 * 
 * setVRefSource() sets the the reference voltage source. Parameters are:
 * ADS1220_VREF_INT          int. reference 2.048 V (default)
 * ADS1220_VREF_REFP0_REFN0  ext. reference = Vrefp0 - Vrefn0
 * ADS1220_VREF_REFP1_REFN1  ext. reference = Vrefp1 - Vrefn1 (REFP1=AIN0, REFN1=AIN3)
 * ADS1220_VREF_AVDD_AVSS    ext. reference = supply voltage
 * 
 * If you use the above options you also have to set the value of vRef "manually":
 * setVRefValue_V(vRef in volts);
 * 
 * Alternatively, you can set the reference voltage source and let the ADS1220 measure 
 * the reference. Be aware that this is not a measurement with highest precision. 
 * "Calibration" might be a bit misleading. You should take the lowest data rate (default) 
 * for most accurate results. You can use the following functions:
 * setRefp0Refn0AsVefAndCalibrate();
 * setRefp1Refn1AsVefAndCalibrate();
 * setAvddAvssAsVrefAndCalibrate();
 * setIntVRef();
 * The latter function sets the default settings. 
 * 
 * Be aware that VREFPx must be >= VREFNx + 0.75V.
 */

void loop(){
  float result = 0.0;
    
  Serial.println("1. Standard internal voltage reference");
  result = ads.getVRef_V();                 // get the reference voltage 
  Serial.print("Reference, default      [V]: ");
  Serial.println(result,3); 
  
  Serial.println("2. REFP0/REFN0 as voltage reference, vRef manually set");
  ads.setVRefSource(ADS1220_VREF_REFP0_REFN0);
  ads.setVRefValue_V(3.295);    // set reference voltage in volts
  result = ads.getVRef_V();
  Serial.print("Reference, manually set [V]: ");
  Serial.println(result,3); 

  Serial.println("3. REFP0/REFN0 as voltage reference, vRef measured by ADS1220");
  ads.setRefp0Refn0AsVefAndCalibrate();
  result = ads.getVRef_V();
  Serial.print("Reference, measured     [V]: ");
  Serial.println(result,3); 

  Serial.println("4. REFP1/REFN1 as voltage reference, vRef measured by ADS1220");
  ads.setRefp1Refn1AsVefAndCalibrate();
  result = ads.getVRef_V();
  Serial.print("Reference, measured     [V]: ");
  Serial.println(result,3);
  
  Serial.println("5. AVDD/AVSS as voltage reference, vRef measured by ADS1220");
  ads.setAvddAvssAsVrefAndCalibrate();
  result = ads.getVRef_V();
  Serial.print("Reference, measured     [V]: ");
  Serial.println(result,3); 
 
  Serial.println("With 5. you can measure up to AVDD, just try.");
  ads.setCompareChannels(ADS1220_MUX_2_AVSS);
  result = ads.getVoltage_mV();
  Serial.print("AIN2 vs. AVSS          [mV]: ");
  Serial.println(result);

  ads.setIntVRef(); // switch back to standard vRef
  Serial.println();
 
  delay(2000);
}
