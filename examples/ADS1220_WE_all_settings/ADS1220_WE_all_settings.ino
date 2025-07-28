/***************************************************************************
  Example sketch for the ADS1220_WE library

  This sketch explains all settings. You can use the sketch as a basis for your
  own sketches.

  On my website you find:
     1) An article about the ADS1220 and the use of this library
        https://wolles-elektronikkiste.de/ads1220-4-kanal-24-bit-a-d-wandler  (German) 
        https://wolles-elektronikkiste.de/en/4-channel-24-bit-adc-ads1220     (English)
 
    2) An article how to use the ADS1220 for typical applications, such as thermocouples, 
       NTCs, RTDs or Wheatstone bridges:
       https://wolles-elektronikkiste.de/ads1220-teil-2-anwendungen          (German)
       https://wolles-elektronikkiste.de/en/ads1220-part-2-applications      (English)

***************************************************************************/

#include <ADS1220_WE.h>
#include <SPI.h>

#define ADS1220_CS_PIN    7 // chip select pin
#define ADS1220_DRDY_PIN  6 // data ready pin 

/* Create your ADS1220 object */
ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);
/* Alternatively you can also pass the SPI object as reference */
// ADS1220_WE ads = ADS1220_WE(&SPI, ADS1220_CS_PIN, ADS1220_DRDY_PIN);

void setup() {
  Serial.begin(9600);
  if (!ads.init()) {
    Serial.println("ADS1220 is not connected!");
    while (1);
  }

  /* General settings / commands */
  //  ads.start(); // wake up from power down and start measurement
  //  ads.reset(); // resets the ADS1220; all settings will change to default
  //  ads.powerDown(); // send the ADS1220 to sleep
  //  ads.setSPIClockSpeed(8000000); // set SPI clock speed, default is 4 MHz

  /* You set the channels to be measured with setCompareChannels(); You
     can choose the following parameters:
     Parameter                  Pos. Input     Neg. Input         Comment
     ADS1220_MUX_0_1              AIN0           AIN1             default
     ADS1220_MUX_0_2              AIN0           AIN2
     ADS1220_MUX_0_3              AIN0           AIN3
     ADS1220_MUX_1_2              AIN1           AIN2
     ADS1220_MUX_1_3              AIN1           AIN3
     ADS1220_MUX_2_3              AIN2           AIN2
     ADS1220_MUX_1_0              AIN1           AIN0
     ADS1220_MUX_3_2              AIN3           AIN2
     ADS1220_MUX_0_AVSS           AIN0           AVSS (=AGND)   single-ended
     ADS1220_MUX_1_AVSS           AIN1           AVSS           single-ended
     ADS1220_MUX_2_AVSS           AIN2           AVSS           single-ended
     ADS1220_MUX_3_AVSS           AIN3           AVSS           single-ended
     ADS1220_MUX_REFPX_REFNX_4   REFP0/REFP1   REFN0/REFN1     (REFPX-REFNX)/4; PGA bypassed
     ADS1220_MUX_AVDD_M_AVSS_4    AVDD           AVSS          (AVDD-AVSS)/4; PGA bypassed
     ADS1220_MUX_AVDD_P_AVSS_2    AVDD           AVSS           AVDD+AVSS)/2
     The last three modes use the internal reference (2.048 V) and gain = 1, independent of
     your settings.
  */
  //  ads.setCompareChannels(ADS1220_MUX_0_3);

  /* You can choose a gain between 1 (default) and 128 using setGain() if PGA is enabled
     (default). If PGA is disabled you can still choose a gain factor up to 4. If PGA is
     enabled, the amplified voltage shall be between AVSS + 200mV and AVDD - 200mV. Outside
     this range linearity drops. For details check the data sheet, section 8.3.2.1.

     If you apply a single-ended mode (negative AINx = AVSS), PGA must be bypassed. Accordingly,
     the maximum gain is 4. The library does these settings automatically.

     For the measurement of reference voltages / supply voltage PGA will also be bypassed. In
     this case gain is 1.

     The parameters you can choose for setGain() are:
     ADS1220_GAIN_X with X = 1,2,4,8,16,32,64 or 128

     With getGainFactor() you can query the gain. The function returns the effective gain and
     not the gain set in the register. Under certian conditions thes are are different. For
     example, the effective gain is set to 1 when external references are measured.
  */
  ads.setGain(ADS1220_GAIN_1);
  //  ads.getGainFactor(); // returns the effective gain as a byte value
  //  ads.bypassPGA(true); // true disables PGA, false enables PGA
  //  ads.isPGABypassed(); // returns true, if PGA is bypassed

  /* The data rate level with setDataRate(). The data rate itself also depends on the operating
     mode and the clock. If the internal clock is used or an external clock with 4.096 MHz the data
     rates are as follows (per second):

      Level               Normal Mode      Duty-Cycle      Turbo Mode
     ADS1220_DR_LVL_0          20               5               40         (default)
     ADS1220_DR_LVL_1          45              11.25            90
     ADS1220_DR_LVL_2          90              22.5            180
     ADS1220_DR_LVL_3         175              44              350
     ADS1220_DR_LVL_4         330              82.5            660
     ADS1220_DR_LVL_5         600             150             1200
     ADS1220_DR_LVL_6        1000             250             2000

     The higher the data rate, the higher the noise (tables are provided in section 7.1 in the
     data sheet). In single-shot mode the conversion times equal the times in Normal Mode.
  */
  //  ads.setDataRate(ADS1220_DR_LVL_2);

  /* Using setOperatingMode() you choose the operating mode. Possible parameters are:
     ADS1220_NORMAL_MODE      ->  Normal Mode
     ADS1220_DUTY_CYCLE_MODE  ->  Duty cycle mode. Saves power, but noise is higher.
     ADS1220_TURBO_MODE       ->  Turbo Mode for fast measurements
  */
  //  ads.setOperatingMode(ADS1220_DUTY_CYCLE_MODE);

  /*  You can choose between a continuous and a single-shot (on demand) mode with
      setConversionMode(). Parameters are:
      ADS1220_SINGLE_SHOT (default)
      ADS1220_CONTINUOUS
  */
  // ads.setConversionMode(ADS1220_CONTINUOUS);

  /*  When you request a result, the program waits by default until the DRDY-pin is LOW because
      this indicates that data is available. You can change this behaviour by activating the 
      non-blocking mode. But then, in single-shot mode, you have to start conversions manually 
      with start() and check by yourself that data is available. In continuous mode you just need
      to ensure that data is available. Data is not available for short time after reading or if 
      changing channels.
  */
  // ads.setNonBlockingMode(true);

  /* In order to obtain temperature values, choose enableTemperatureSensor(true); false will
     disable the temperature sensor. As long as the temperature sensor is enabled the ADS1220
     is blocked for this task. To obtain voltage values, you have to switch the sensor off. The
     temperature is queried with getTemperature();
  */
  //  ads.enableTemperatureSensor(true);
  //  ads.getTemperature(); // returns temperature as float

  /*
     setVRefSource() sets the the reference voltage source. Parameters are:
     ADS1220_VREF_INT          int. reference 2.048 V (default)
     ADS1220_VREF_REFP0_REFN0  ext. reference = Vrefp0 - Vrefn0
     ADS1220_VREF_REFP1_REFN1  ext. reference = Vrefp1 - Vrefn1 (REFP1=AIN0, REFN1=AIN3)
     ADS1220_VREF_AVDD_AVSS    ext. reference = supply voltage

     If you use the above options you also have to set the value of vRef "manually":
     setVRefValue_V(vRef in volts);

     Alternatively, you can set the reference voltage source and let the ADS1220 measure
     the reference. Be aware that this is not a measurement with highest precision.
     "Calibration" might be a bit misleading. You should take the lowest data rate (default)
     for most accurate results. You can use the following functions:
     setRefp0Refn0AsVefAndCalibrate();
     setRefp1Refn1AsVefAndCalibrate();
     setAvddAvssAsVrefAndCalibrate();
     setIntVRef();
     The latter function sets the default settings.

     Be aware that VREFPx must be >= VREFNx + 0.75V.
  */
  //  ads.setVRefSource(ADS1220_VREF_REFP0_REFN0);
  //  ads.setVRefValue_V(3.312);  // just an example
  //  or:
  //  ads.setRefp0Refn0AsVefAndCalibrate(); //or:
  //  ads.setRefp1Refn1AsVefAndCalibrate(); //or:
  //  ads.setAvddAvssAsVrefAndCalibrate(); //or:
  //  ads.setIntVRef();
  //  to query VRef:
  //  ads.getVRef_V(); // returns VRef as float

  /* You can set a filter to reduce 50 and or 60 Hz noise with setFIRFilter(); Parameters:
     ADS1220_NONE       no filter (default)
     ADS1220_50HZ_60HZ  50Hz and 60Hz filter
     ADS1220_50HZ       50Hz filter
     ADS1220_60HZ       60Hz filter
  */
  //  ads.setFIRFilter(ADS1220_50HZ_60HZ);

  /* When data is ready the DRDY pin will turn from HIGH to LOW. In addition, also the DOUT pin
     can be set as a data ready pin. The function is setDrdyMode(), parameters are:
     ADS1220_DRDY        only DRDY pin is indicating data readiness  (default);
     ADS1220_DOUT_DRDY   DRDY and DOUT pin indicate data readiness
  */
  //  ads.setDrdyMode(ADS1220_DOUT_DRDY);


  /* There is a switch between AIN3/REFN1 and AVSS. You can use this option to save power in
     bridge sensor applications.
     ADS1220_ALWAYS_OPEN    The switch is always open.
     ADS1220_SWITCH         Switch automatically closes when the START/SYNC command is sent
                            and opens when the POWERDOWN command is issued.
  */
  // ads.setLowSidePowerSwitch(ADS1220_SWITCH);

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
  // ads.setIdacCurrent(ADS1220_IDAC_50_MU_A);

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
  //  ads.setIdac2Routing(ads1220IdacRouting route);

} // end of setup()

void loop() {
  float result = 0.0;

  /* The following functions query measured values from the ADS1220. If you request values in
     single-shot mode, the conversion will be initiated automatically. The value will be delivered
     once DRDY goes LOW. This ensures that you will receive "fresh" data. This is particularly
     important when you change channels.
  */
  result = ads.getVoltage_mV(); // get result in millivolts
  //  ads.getVoltage_muV(); // get result in microvolts
  //  ads.getRawData();  // get raw result (signed 24 bit as long int)
  //  ads.getTemperature();  // get temperature (you need to enable the T-sensor before);
  Serial.println(result, 3);
  
  delay(2000);
}
