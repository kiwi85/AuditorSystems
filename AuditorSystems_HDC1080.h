/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_HDC1080_h
#define _AUDITORSYSTEMS_HDC1080_h
#include "ClosedCube_HDC1080.h"
#include <AuditorSystems.h>

namespace AuditorSystems
{
	ClosedCube_HDC1080 hdc1080;
	template<
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_HDC1080OutputPinsHumidity,
		typename T_HDC1080OutputPinsTemperature

		
	
	> class HDC1080 :
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_HDC1080OutputPinsHumidity,
		public T_HDC1080OutputPinsTemperature
	{
		
		
	public:
		_V_PIN_( HDC1080OutputPinsHumidity )
		_V_PIN_( HDC1080OutputPinsTemperature )
	
	public:
		_V_PROP_( Address )
	
	public:
		_V_PROP_( Enabled )
	
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )
	
	
	

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			
				
				
		}
		
			
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
		
		T_HDC1080OutputPinsHumidity::SetPinValue(hdc1080.readHumidity());
		T_HDC1080OutputPinsTemperature::SetPinValue(hdc1080.readTemperature());	 
		
		}
	public:
		inline void SystemInit()
		{
		}
		inline void SystemStart()
		{
					// Default settings: 
	//  - Heater off
	//  - 14 bit Temperature and Humidity Measurement Resolutions
	hdc1080.begin(Address());

	//Serial.print("Manufacturer ID=0x");
	//Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
	//Serial.print("Device ID=0x");
	//Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device
	
 
			//Serial.println("Starting up...HDC1080 at adress"+String(Address()));
		
		}
	
	};
		
//---------------------------
}
#endif