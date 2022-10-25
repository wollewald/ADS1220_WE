/******************************************************************************
 *
 * This is a library for the 24 bit, 4 channel ADS1220 A/D Converter
 *
 * I added several examples sketches which should enable you to use the library.
 *
 * You are free to use it, change it or build on it. In case you like it, it would
 * be cool if you give it a star.
 *
 * If you find bugs, please inform me!
 *
 * Written by Wolfgang (Wolle) Ewald
 * 
 * On my website you find:
 *
 *   1) An article about the ADS1220 and the use of this library
 *      https://wolles-elektronikkiste.de/ads1220-4-kanal-24-bit-a-d-wandler  (German) 
 *      https://wolles-elektronikkiste.de/en/4-channel-24-bit-adc-ads1220     (English)
 *
 *   2) An article how to use the ADS1220 for typical applications, such as thermocouples, 
 *      NTCs, RTDs or Wheatstone bridges:
 *      https://wolles-elektronikkiste.de/ads1220-teil-2-anwendungen          (German)
 *      https://wolles-elektronikkiste.de/en/ads1220-part-2-applications      (English)
 *
 ******************************************************************************/

#ifndef ADS1220_WE_H_
#define ADS1220_WE_H_

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include<SPI.h>

typedef enum ADS1220_MUX {
    ADS1220_MUX_0_1     = 0x00,   //default
    ADS1220_MUX_0_2     = 0x10,
    ADS1220_MUX_0_3     = 0x20,
    ADS1220_MUX_1_2     = 0x30,
    ADS1220_MUX_1_3     = 0x40,
    ADS1220_MUX_2_3     = 0x50,
    ADS1220_MUX_1_0     = 0x60,
    ADS1220_MUX_3_2     = 0x70,
    ADS1220_MUX_0_AVSS  = 0x80,
    ADS1220_MUX_1_AVSS  = 0x90,
    ADS1220_MUX_2_AVSS  = 0xA0,
    ADS1220_MUX_3_AVSS  = 0xB0,
    ADS1220_MUX_REFPX_REFNX_4 = 0xC0,
    ADS1220_MUX_AVDD_M_AVSS_4 = 0xD0,
    ADS1220_MUX_AVDD_P_AVSS_2 = 0xE0
} ads1220Mux;

typedef enum ADS1220_GAIN {
    ADS1220_GAIN_1   = 0x00,   //default
    ADS1220_GAIN_2   = 0x02,
    ADS1220_GAIN_4   = 0x04,
    ADS1220_GAIN_8   = 0x06,
    ADS1220_GAIN_16  = 0x08,
    ADS1220_GAIN_32  = 0x0A,
    ADS1220_GAIN_64  = 0x0C,
    ADS1220_GAIN_128 = 0x0E
} ads1220Gain;

typedef enum ADS1220_DATA_RATE {
    ADS1220_DR_LVL_0 = 0x00,   // default
    ADS1220_DR_LVL_1 = 0x20,
    ADS1220_DR_LVL_2 = 0x40,
    ADS1220_DR_LVL_3 = 0x60,
    ADS1220_DR_LVL_4 = 0x80,
    ADS1220_DR_LVL_5 = 0xA0,
    ADS1220_DR_LVL_6 = 0xC0
} ads1220DataRate;

typedef enum ADS1220_OP_MODE {      
    ADS1220_NORMAL_MODE     = 0x00,  // default
    ADS1220_DUTY_CYCLE_MODE = 0x08,
    ADS1220_TURBO_MODE      = 0x10
} ads1220OpMode;

typedef enum ADS1220_CONV_MODE {
    ADS1220_SINGLE_SHOT     = 0x00,  // default
    ADS1220_CONTINUOUS      = 0x04
} ads1220ConvMode;

typedef enum ADS1220_VREF{
    ADS1220_VREF_INT            = 0x00,  // default
    ADS1220_VREF_REFP0_REFN0    = 0x40,
    ADS1220_VREF_REFP1_REFN1    = 0x80,
    ADS1220_VREF_AVDD_AVSS      = 0xC0
} ads1220VRef;

typedef enum ADS1220_FIR{
    ADS1220_NONE        = 0x00,   // default
    ADS1220_50HZ_60HZ   = 0x10,
    ADS1220_50HZ        = 0x20,
    ADS1220_60HZ        = 0x30
} ads1220FIR;

typedef enum ADS1220_PSW {
    ADS1220_ALWAYS_OPEN = 0x00,  // default
    ADS1220_SWITCH      = 0x08
} ads1220PSW;

typedef enum ADS1220_IDAC_CURRENT {
    ADS1220_IDAC_OFF        = 0x00,  // defaulr
    ADS1220_IDAC_10_MU_A    = 0x01,
    ADS1220_IDAC_50_MU_A    = 0x02,
    ADS1220_IDAC_100_MU_A   = 0x03,
    ADS1220_IDAC_250_MU_A   = 0x04,
    ADS1220_IDAC_500_MU_A   = 0x05,
    ADS1220_IDAC_1000_MU_A  = 0x06,
    ADS1220_IDAC_1500_MU_A  = 0x07
} ads1220IdacCurrent;

typedef enum ADS1220_IDAC_ROUTING {
    ADS1220_IDAC_NONE       = 0x00,  // default
    ADS1220_IDAC_AIN0_REFP1 = 0x01,
    ADS1220_IDAC_AIN1       = 0x02,
    ADS1220_IDAC_AIN2       = 0x03,
    ADS1220_IDAC_AIN3_REFN1 = 0x04,
    ADS1220_IDAC_REFP0      = 0x05,
    ADS1220_IDAC_REFN0      = 0x06,
} ads1220IdacRouting;

typedef enum ADS1220_DRDY_MODE {
    ADS1220_DRDY      = 0x00,   // default
    ADS1220_DOUT_DRDY = 0x02
} ads1220DrdyMode;

/* other */

class ADS1220_WE
{
    public:

        //ADS1220 SPI commands
        static constexpr uint8_t ADS1220_RESET       {0x06};    
        static constexpr uint8_t ADS1220_START       {0x08};    //Send the START/SYNC command (08h) to start converting in continuous conversion mode
        static constexpr uint8_t ADS1220_PWRDOWN     {0x02};
        static constexpr uint8_t ADS1220_RDATA       {0x10};
        static constexpr uint8_t ADS1220_WREG        {0x40};    // write register
        static constexpr uint8_t ADS1220_RREG        {0x20};    // read register

        /* registers */
        static constexpr uint8_t ADS1220_CONF_REG_0  {0x00};
        static constexpr uint8_t ADS1220_CONF_REG_1  {0x01};
        static constexpr uint8_t ADS1220_CONF_REG_2  {0x02};
        static constexpr uint8_t ADS1220_CONF_REG_3  {0x03};

        /* other */
        static constexpr float ADS1220_RANGE {8388607.0}; // = 2^23 - 1 as float
    
        /* Constructors */
        ADS1220_WE(int cs, int drdy) : _spi{&SPI}, csPin{cs}, drdyPin{drdy} {}
        ADS1220_WE(SPIClass *s, int cs, int drdy) : _spi{s}, csPin{cs}, drdyPin{drdy} {}
            
        /* Commands */
        uint8_t init();
        void start();
        void reset();
        void powerDown();
        
        /* Configuration Register 0 settings */
        void setCompareChannels(ads1220Mux mux);
        void setGain(ads1220Gain gain);
        uint8_t getGainFactor();
        void bypassPGA(bool bypass);
        bool isPGABypassed();
        
        /* Configuration Register 1 settings */
        void setDataRate(ads1220DataRate rate);
        void setOperatingMode(ads1220OpMode mode);
        void setConversionMode(ads1220ConvMode mode);
        void enableTemperatureSensor(bool enable);
        void enableBurnOutCurrentSources(bool enable);
        
        /* Configuration Register 2 settings */
        void setVRefSource(ads1220VRef vRefSource);
        void setFIRFilter(ads1220FIR fir);
        void setLowSidePowerSwitch(ads1220PSW psw);
        void setIdacCurrent(ads1220IdacCurrent current);
        
        /* Configuration Register 3 settings */
        void setIdac1Routing(ads1220IdacRouting route);
        void setIdac2Routing(ads1220IdacRouting route);
        void setDrdyMode(ads1220DrdyMode mode);
        
        /* Other settings */
        void setSPIClockSpeed(unsigned long clock);
        void setVRefValue_V(float refVal);
        float getVRef_V();
        void setAvddAvssAsVrefAndCalibrate();
        void setRefp0Refn0AsVefAndCalibrate();
        void setRefp1Refn1AsVefAndCalibrate();
        void setIntVRef();
        
        /* Results */
        float getVoltage_mV();
        float getVoltage_muV();
        int32_t getRawData();
        float getTemperature();

    protected:
        SPIClass *_spi;
        SPISettings mySPISettings;
        int csPin;
        int drdyPin;
        uint8_t regValue;
        float vRef;
        uint8_t gain;
        bool refMeasurement; 
        bool doNotBypassPgaIfPossible;
        ads1220ConvMode convMode;

        void forcedBypassPGA();
        int32_t getData();
        uint32_t readResult();
        uint8_t readRegister(uint8_t reg);
        void writeRegister(uint8_t reg, uint8_t val);
        void command(uint8_t cmd);
};

#endif

