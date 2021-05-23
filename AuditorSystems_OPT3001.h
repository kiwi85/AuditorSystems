/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_OPT3001_h
#define _AUDITORSYSTEMS_OPT3001_h
#include <Wire.h>
#include <ClosedCube_OPT3001.h>
#include <AuditorSystems.h>

namespace AuditorSystems
{
	ClosedCube_OPT3001 opt3001;
	OPT3001 result;
	template<
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_OutputPinsilluminance
	

		
	
	> class OPT3001 :
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_OutputPinsilluminance
	
	{
		
		
	
		
	
	public:
		_V_PROP_( Address )
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )
	public:
		_V_PROP_( Enabled )
	public:
		_V_PIN_( OutputPinsilluminance )
	public:
		
		float lighting;

void configureSensor() {
	OPT3001_Config newConfig;
	
	newConfig.RangeNumber = B1100;	
	newConfig.ConvertionTime = B0;
	newConfig.Latch = B1;
	newConfig.ModeOfConversionOperation = B11;

	OPT3001_ErrorCode errorConfig = opt3001.writeConfig(newConfig);
	if (errorConfig != NO_ERROR);
		
	else {
		OPT3001_Config sensorConfig = opt3001.readConfig();
	/*
		Serial.println("OPT3001 Current Config:");
		Serial.println("------------------------------");
		
		Serial.print("Conversion ready (R):");
		Serial.println(sensorConfig.ConversionReady,HEX);

		Serial.print("Conversion time (R/W):");
		Serial.println(sensorConfig.ConvertionTime, HEX);

		Serial.print("Fault count field (R/W):");
		Serial.println(sensorConfig.FaultCount, HEX);

		Serial.print("Flag high field (R-only):");
		Serial.println(sensorConfig.FlagHigh, HEX);

		Serial.print("Flag low field (R-only):");
		Serial.println(sensorConfig.FlagLow, HEX);

		Serial.print("Latch field (R/W):");
		Serial.println(sensorConfig.Latch, HEX);

		Serial.print("Mask exponent field (R/W):");
		Serial.println(sensorConfig.MaskExponent, HEX);

		Serial.print("Mode of conversion operation (R/W):");
		Serial.println(sensorConfig.ModeOfConversionOperation, HEX);

		Serial.print("Polarity field (R/W):");
		Serial.println(sensorConfig.Polarity, HEX);

		Serial.print("Overflow flag (R-only):");
		Serial.println(sensorConfig.OverflowFlag, HEX);

		Serial.print("Range number (R/W):");
		Serial.println(sensorConfig.RangeNumber, HEX);

		Serial.println("------------------------------");
*/
	}
	
}
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			
				
				
		}
		
			
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
		result = opt3001.readResult();
		//printResult("OPT3001", result);
		lighting=result.lux;

		T_OutputPinsilluminance::SetPinValue(lighting);
	 
		
		}
	public:
		inline void SystemInit()
		{
		}
		inline void SystemStart()
		{
	opt3001.begin(Address());
/*
	Serial.print("OPT3001 Manufacturer ID");
	Serial.println(opt3001.readManufacturerID());
	Serial.print("OPT3001 Device ID");
	Serial.println(opt3001.readDeviceID());
*/
	configureSensor();
//	printResult("High-Limit", opt3001.readHighLimit());
//	printResult("Low-Limit", opt3001.readLowLimit());
//	Serial.println("----");
 
			//Serial.println("Starting up...OPT3001 at adress"+String(Address()));
		
		}
	
	};
		
//---------------------------
}
#endif