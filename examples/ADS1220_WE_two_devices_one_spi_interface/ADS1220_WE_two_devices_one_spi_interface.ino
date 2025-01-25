/***************************************************************************
* Example sketch for the ADS1220_WE library
*
* This sketch shows how to use two ADS1220 modules attached to the same SPI 
* interface. This sketch was tested on an Arduino UNO R3 and Arduino UNO R4.
* For an Arduino UNO R4 (WIFI or Minima) pay attention to lines 29-32!
* 
***************************************************************************/


#include <ADS1220_WE.h>
#include <SPI.h>

// Pin definitions
#define ADS1220_1_CS_PIN    7   // Chip Select
#define ADS1220_1_DRDY_PIN  6   // Data Ready

#define ADS1220_2_CS_PIN    5   // Chip Select
#define ADS1220_2_DRDY_PIN  4   // Data Ready

// Create ADS1220 objects
ADS1220_WE ads1 = ADS1220_WE(ADS1220_1_CS_PIN, ADS1220_1_DRDY_PIN);
ADS1220_WE ads2 = ADS1220_WE(ADS1220_2_CS_PIN, ADS1220_2_DRDY_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  /* Choose SPI clock speed here. For reasons I don't know the sketch only works
     on an Arduino UNO R4 if you choose different SPI clock speeds for both devices */
//   ads1.setSPIClockSpeed(4000000); // set SPI clock speed, default is 4 MHz
//   ads2.setSPIClockSpeed(4000001); // For an Arduino R4 Minima or WIFI choose e.g. 4000001 (bizarre, I know!)
  
  digitalWrite(ADS1220_1_CS_PIN, HIGH);
  pinMode(ADS1220_1_CS_PIN, OUTPUT);
  digitalWrite(ADS1220_2_CS_PIN, HIGH);
  pinMode(ADS1220_2_CS_PIN, OUTPUT);

  Serial.println("CS of ADS1220 #1 and ADS1220 #2 forced to HIGH (inactive).");

  Serial.println("Initializing ADS1220 #1...");
  if (ads1.init()) {
    Serial.println("ADS1220 #1 connected and functioning!");
  } else {
    Serial.println("ERROR: ADS1220 #1 not responding.");
    //while (1);
  }
  delay(100);

  Serial.println("Initializing ADS1220 #2...");
  if (ads2.init()) {
    Serial.println("ADS1220 #2 connected and functioning!");
  } else {
    Serial.println("ERROR: ADS1220 #2 not responding.");
    //while (1);
  }
  delay(100);

  ads1.setGain(ADS1220_GAIN_1);
  ads1.setDataRate(ADS1220_DR_LVL_0);
  ads1.setConversionMode(ADS1220_CONTINUOUS);

  ads2.setGain(ADS1220_GAIN_1);
  ads2.setDataRate(ADS1220_DR_LVL_0);
  ads2.setConversionMode(ADS1220_CONTINUOUS);

  float vRef1 = ads1.getVRef_V();
  Serial.print("VRef (ADS1220 #1): ");
  Serial.print(vRef1, 3);
  Serial.println(" V");

  float vRef2 = ads2.getVRef_V();
  Serial.print("VRef (ADS1220 #2): ");
  Serial.print(vRef2, 3);
  Serial.println(" V");

  delay(3000);

}

void loop() { 

  ads1.setCompareChannels(ADS1220_MUX_0_1);
  float result1 = ads1.getVoltage_mV();
  Serial.print("1. AIN0 vs. AIN1  [mV]: ");
  Serial.println(result1);

  ads2.setCompareChannels(ADS1220_MUX_0_1);
  float result2 = ads2.getVoltage_mV();
  Serial.print("2. AIN0 vs. AIN1  [mV]: ");
  Serial.println(result2);
  
  Serial.println();
  delay(1000);
}