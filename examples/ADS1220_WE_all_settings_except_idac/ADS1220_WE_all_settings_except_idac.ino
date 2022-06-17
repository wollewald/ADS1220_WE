/***************************************************************************
* Example sketch for the ADS1220_WE library
*
* This sketch explains all settings except the ones for the programmable excitation
* current sources (IDAC). You can use the sketch as a basis for your own sketches.   
* 
* Further information can be found soon on:
*
* https://wolles-elektronikkiste.de/         (German)
* https://wolles-elektronikkiste.de/en/      (English)
* 
***************************************************************************/

#include <ADS1220_WE.h>
#include <SPI.h>

#define ADS1220_CS_PIN    7 // chip select pin
#define ADS1220_DRDY_PIN  6 // data ready pin 

/* Create your ADS1220 object */
ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);
/* Alternatively you can also pass the SPI object as reference */
// ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN, &SPI);

void setup(){
  Serial.begin(9600);
  if(!ads.init()){
    Serial.println("ADS1220 is not connected!");
    while(1);
  }

/* General settings / commands */
//  ads.start(); // wake up from power down and start measurement
//  ads.reset(); // resets the ADS1220; all settings will change to default
//  ads.powerDown(); // send the ADS1220 to sleep
//  ads.setSPIClockSpeed(8000000); // set SPI clock speed, default is 4 MHz

/* You set the channels to be measured with setCompareChannels(); You
 * can choose the following parameters:
 * Parameter                  Pos. Input     Neg. Input         Comment
 * ADS1220_MUX_0_1              AIN0           AIN1             default
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
 */ 
//  ads.setCompareChannels(ADS1220_MUX_1_3);

/* You can choose a gain between 1 (default) and 128 using setGain() if PGA is enabled (default).
 * If PGA is disabled you can still choose a gain factor up to 4. If PGA is enabled, the amplified 
 * voltage shall be between AVSS + 200mV and AVDD - 200mV. Outside this range linearity drops. 
 * For details see data sheet, section 8.3.2.1.
 * The parameters you can choose are:
 * ADS1220_GAIN_X with X = 1,2,4,8,16,32,64 or 128
 * With getGainFactor() you can query the gain. The function returns the effective gain and not the 
 * gain set in the register. Under certain conditions these are are different. E.g. the effective gain 
 * is set to 1 when external references are measured. 
 */
//  ads.setGain(ADS1220_GAIN_16);
//  ads.getGainFactor(); // returns the effective gain as a byte value
//  ads.bypassPGA(true); // true disables PGA, false enables PGA

/* The data rate is set with setDataRate(). The data rate also depends on the operating mode and 
 * the clock. If the internal clock is used or an external clock with 4.096 MHz the data rates are
 * as follows (per second):
 *  
 *  Level               Normal Mode      Duty-Cycle      Turbo Mode
 * ADS1220_DR_LVL_0          20               5               40         (default)
 * ADS1220_DR_LVL_1          45              11.25            90
 * ADS1220_DR_LVL_2          90              22.5            180
 * ADS1220_DR_LVL_3         175              44              350
 * ADS1220_DR_LVL_4         330              82.5            660
 * ADS1220_DR_LVL_5         600             150             1200
 * ADS1220_DR_LVL_6        1000             250             2000
 * 
 * The higher the data rate, the higher the noise (tables are provided in section 7.1 in the data sheet).
 * In single shot mode the conversion times equal the times in Normal Mode.
 */
//  ads.setDataRate(ADS1220_DR_LVL_2);  

/* Using setOperatingMode() you choose the operating mode. Possible parameters are:
 * ADS1220_NORMAL_MODE      ->  Normal Mode
 * ADS1220_DUTY_CYCLE_MODE  ->  Duty cycle mode. Saves power, but noise is higher. 
 * ADS1220_TURBO_MODE       ->  Turbo Mode for fast measurements 
 */
//  ads.setOperatingMode(ADS1220_DUTY_CYCLE_MODE);

/*  You can choose between a continuous and a single-shot (on demand) mode with setConversionMode();
 *  ADS1220_SINGLE_SHOT (default)
 *  ADS1220_CONTINUOUS  
 */
// ads.setConversionMode(ADS1220_CONTINUOUS);

/* In order to obtain temperature values, choose enableTemperatureSensor(true); false will disable
 * the temperature sensor. As long as the temperature sensor is enabled the ADS1220 is blocked for
 * this task. To obtain voltage values, you have to switch thesensor off. The temperature is queried 
 * with getTemperature(); 
 */
//  ads.enableTemperatureSensor(true);
//  ads.getTemperature(); // returns temperature as float
 
/* 
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
 * the reference. Be aware that this is not a measurement with highest precision. "Calibration" 
 * might be a bit misleading. You should take the lowest data rate (default) for most accurate 
 * results. You can use the following functions:
 * setRefp0Refn0AsVefAndCalibrate();
 * setRefp1Refn1AsVefAndCalibrate();
 * setAvddAvssAsVrefAndCalibrate();
 * setIntVRef();
 * The latter function sets the default settings. 
 */
//  ads.setVRefSource(ADS1220_VREF_REFP0_REFN0);
//  ads.setVRefValue_V(3.312);  // just an example
// or:
//  ads.setRefp0Refn0AsVefAndCalibrate(); //or:
//  ads.setRefp1Refn1AsVefAndCalibrate(); //or:
//  ads.setAvddAvssAsVrefAndCalibrate(); //or:
//  ads.setIntVRef();

/* You can set a filter to reduce 50 and or 60 Hz noise with setFIRFilter(); Parameters: 
 * ADS1220_NONE       no filter (default)
 * ADS1220_50HZ_60HZ  50Hz and 60Hz filter
 * ADS1220_50HZ       50Hz filter
 * ADS1220_60HZ       60Hz filter
 */
//  ads.setFIRFilter(ADS1220_50HZ_60HZ);

/* When data is ready the DRDY pin will turn from HIGH to LOW. In addition, also the DOUT pin 
 * can be set as a data ready pin. The function is setDrdyMode(), parameters are:
 * ADS1220_DRDY        only DRDY pin is indicating data readiness  (default);
 * ADS1220_DOUT_DRDY   DRDY and DOUT pin indicate data readiness
 */
//  ads.setDrdyMode(ADS1220_DOUT_DRDY);
  
} // end setup

void loop(){

/* The following functions query measured values from the ADS1220. If you request values in single-shot 
 * mode, the conversion will be initiated automatically. The value will be delivered once DRDY goes LOW. 
 * This ensures that you will receive "fresh" data. This is particularly important when you change 
 * channels.  
 */
//  ads.getVoltage_mV(); // get result in millivolts
//  ads.getVoltage_muV(); // get result in microvolts
//  ads.getRawData();  // get raw result (signed 24 bit as long int)
//  ads.getTemperature();  // get temperature (you need to enable the T-sensor before);
  delay(2000);
}
