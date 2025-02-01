# ADS1220_WE

This is an Arduino library for the 24-bit, 4 channel ADS1220 ADC. I have tried to make it convenient and easy to use. All features of the ADS1220 have been implemented. The examples should help to understand how to use the functions.

If you like the library, it would be great if you could give a star. If you find bugs, please inform me.

On my website you find:

1) An article about the ADS1220 and the use of this library

https://wolles-elektronikkiste.de/ads1220-4-kanal-24-bit-a-d-wandler       (German) 

https://wolles-elektronikkiste.de/en/4-channel-24-bit-adc-ads1220          (English)

2) An article how to use the ADS1220 for typical applications, such as thermocouples, NTCs, RTDs or Wheatstone bridges:

https://wolles-elektronikkiste.de/ads1220-teil-2-anwendungen                (German)

https://wolles-elektronikkiste.de/en/ads1220-part-2-applications            (English)

<h3>Using several SPI devices</h3>

If you use several ADS1220 and the Arduino UNO R4 WIFI or Minima, have a look at ADS1220_WE_two_devices_one_spi_interface.ino. There is an issue if SPI.begin() is called more than once and the sketch shows how to avoid this problem by using an alternative constructor. Please also choose this alternative constructor if you face issues when using other SPI devices in parallel to an ADS1220. There is no need to use the alternative constructor when using an Arduino UNO R3 (or other AVR based boards) or ESP32 - these are the ones I have tested. 

If you compare the SPI begin functions of the Arduino Renesas Core (UNO R4 boards):

https://github.com/arduino/ArduinoCore-renesas/blob/99b34548c5952e7e9b687267f076d24ae782e9dc/libraries/SPI/SPI.cpp#L62

and the Arduino AVR Core:

https://github.com/arduino/ArduinoCore-avr/blob/c8c514c9a19602542bc32c7033f48fecbbda4401/libraries/SPI/src/SPI.cpp#L26

then you will see that the latter checks whether it has been initialized using the variable ``initialized``. Also the the Renesas version has a similar variable called ``_is_initialized`` (see line 163) but it's not used to avoid executing begin() a second time. I am not sure if there is a reason for this. If anyone knows please tell me! 
