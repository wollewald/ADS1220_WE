/***************************************************************************
  Example sketch for the ADS1220_WE library

  This sketch shows how to use the excitation currents IDAC1 and IDAC2.

  Further information can be found soon on:

  https://wolles-elektronikkiste.de/         (German)
  https://wolles-elektronikkiste.de/en/      (English)

***************************************************************************/

#include <ADS1220_WE.h>
#include <SPI.h>

#define ADS1220_CS_PIN    7 // chip select pin
#define ADS1220_DRDY_PIN  6 // data ready pin 

ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);

void setup() {
  Serial.begin(9600);
  if (!ads.init()) {
    Serial.println("ADS1220 is not connected!");
    while (1);
  }
  //  ads.setCompareChannels(ADS1220_MUX_0_1); default seeting for this test
  ads.setGain(ADS1220_GAIN_1);
  ads.bypassPGA(true); // since the negative voltage is AVSS, we have to bypass PGA 

  /* The ADS1220 can provide two excitation currents, IDAC1 and IDAC2. It takes up to 200µs
     until the current is set up. The library includes a delay, so you don't have to add one.
     You can switch IDAC1 and IDAC2 on and off individually but the current is the same for 
     both.
     The ADS1220 will try to provide the voltage needed for the current you have chosen. This 
     voltage shall not exceed AVDD - 0.9V.

     ADS1220_IDAC_OFF         // default
     ADS1220_IDAC_10_MU_A     // set IDAC1/IDAC2 to 10 µA
     ADS1220_IDAC_50_MU_A     // 50 µA
     ADS1220_IDAC_100_MU_A    // 100 µA
     ADS1220_IDAC_250_MU_A    // 250 µA
     ADS1220_IDAC_500_MU_A    // 500 µA
     ADS1220_IDAC_1000_MU_A   // 1000 µA
     ADS1220_IDAC_1500_MU_A   // 1500 µA
  */
 ads.setIdacCurrent(ADS1220_IDAC_100_MU_A);

  /* You can choose to which pin IDAC1 and IDAC2 are directed. The parameters are self-explaining.
     ADS1220_IDAC_NONE
     ADS1220_IDAC_AIN0_REFP1
     ADS1220_IDAC_AIN1
     ADS1220_IDAC_AIN2
     ADS1220_IDAC_AIN3_REFN1
     ADS1220_IDAC_REFP0
     ADS1220_IDAC_REFN0
  */
  //  ads.setIdac1Routing(ADS1220_IDAC_AIN0_REFP1);
  //  ads.setIdac2Routing(ADS1220_IDAC_AIN2);

} // end of setup()

/* As a very simple test:
 *  - put a 10 kOhm resistor between AIN0/REFP1 and AIN1
 *  - connect AIN1 to AVSS
 *  - connect AIN2 to AIN0/REFP1 
 *  Then see what happens.
 */

void loop() {
  float result = 0.0;

  ads.setIdac1Routing(ADS1220_IDAC_AIN0_REFP1);
  result = ads.getVoltage_mV(); 
  Serial.print("Voltage, caused by IDAC1 [mV]:           ");
  Serial.println(result, 3);  // will be roughly 1000 mA, according to Ohm's law

  ads.setIdac2Routing(ADS1220_IDAC_AIN2);
  result = ads.getVoltage_mV(); 
  Serial.print("Voltage, caused by IDAC1 and IDAC2 [mV]: ");
  Serial.println(result, 3);  // will be roughly 2000 mA, according to Ohm's law

  ads.setIdac1Routing(ADS1220_IDAC_NONE);
  ads.setIdac2Routing(ADS1220_IDAC_NONE);
  Serial.println();
  
  delay(2000);
}
