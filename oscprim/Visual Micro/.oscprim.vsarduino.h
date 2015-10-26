/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: STM32 Discovery F407, Platform=STM32F4, Package=Arduino_STM32
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define BOARD_discovery_f407
#define VECT_TAB_BASE
#define ERROR_LED_PORT GPIOD
#define ERROR_LED_PIN 14
#define F_CPU 168000000L
#define ARDUINO 165
#define ARDUINO_STM32DiscoveryF407
#define ARDUINO_ARCH_STM32F4
#define MCU_STM32F406VG
#define STM32_HIGH_DENSITY
#define STM32F2
#define __cplusplus
#define GCC_VERSION 40803

#include <arduino.h>
#include <discovery_f4.h> 
#include <variant.h> 
#undef F
#define F(string_literal) ((const PROGMEM char *)(string_literal))
#undef PSTR
#define PSTR(string_literal) ((const PROGMEM char *)(string_literal))
#undef cli
#define cli()
#define pgm_read_byte(address_short)
#define pgm_read_word(address_short)
#define pgm_read_word2(address_short)
#define digitalPinToPort(P)
#define digitalPinToBitMask(P) 
#define digitalPinToTimer(P)
#define analogInPinToBit(P)
#define portOutputRegister(P)
#define portInputRegister(P)
#define portModeRegister(P)
#include <oscprim.ino>
#include <CMDcgmi.ino>
#include <CMDstart.ino>
#include <DBG.ino>
#include <adc.ino>
#include <adf.ino>
#include <adf4602.h>
#include <comand.ino>
#endif
