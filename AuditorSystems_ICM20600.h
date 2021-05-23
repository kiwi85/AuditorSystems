/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_ICM20600_h
#define _AUDITORSYSTEMS_ICM20600_h

#include <AuditorSystems.h>

#include <Wire.h>
#include "ICM20600.h"




namespace AuditorSystems
{
ICM20600 icm20600(true);	
	template<
		typename T_AccelOutputPinsX,
		typename T_AccelOutputPinsY,
		typename T_AccelOutputPinsZ,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_GyroOutputPinsX,
		typename T_GyroOutputPinsY,
		typename T_GyroOutputPinsZ,
		
		typename T_OutputPinsTemperature






		
	
	> class ICM20600 :
		
		public T_AccelOutputPinsX,
		public T_AccelOutputPinsY,
		public T_AccelOutputPinsZ,
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_GyroOutputPinsX,
		public T_GyroOutputPinsY,
		public T_GyroOutputPinsZ,
		
		public T_OutputPinsTemperature
		
		
	{
		
		
	
	public:
		_V_PROP_( Address )
	public:
		_V_PIN_( GyroOutputPinsX )
		_V_PIN_( GyroOutputPinsY )
		_V_PIN_( GyroOutputPinsZ )
		_V_PIN_( AccelOutputPinsX )
		_V_PIN_( AccelOutputPinsY )
		_V_PIN_( AccelOutputPinsZ )
		_V_PIN_( OutputPinsTemperature )
			public:
		_V_PROP_( ClockInputPin_o_IsConnected )
	public:
		_V_PROP_( Enabled )
	
	
	public:
		uint8_t i2c_adr=Address().GetValue();
	

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			
				
				
		}
		
	public:
		
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
		
		if(T_GyroOutputPinsX::GetPinIsConnected())T_GyroOutputPinsX::SetPinValue(icm20600.getGyroscopeX());
		if(T_GyroOutputPinsY::GetPinIsConnected())T_GyroOutputPinsY::SetPinValue(icm20600.getGyroscopeY());
		if(T_GyroOutputPinsZ::GetPinIsConnected())T_GyroOutputPinsZ::SetPinValue(icm20600.getGyroscopeZ());
		if(T_AccelOutputPinsX::GetPinIsConnected())T_AccelOutputPinsX::SetPinValue(icm20600.getAccelerationX());
		if(T_AccelOutputPinsY::GetPinIsConnected())T_AccelOutputPinsY::SetPinValue(icm20600.getAccelerationY());
		if(T_AccelOutputPinsZ::GetPinIsConnected())T_AccelOutputPinsZ::SetPinValue(icm20600.getAccelerationZ());
		if(T_OutputPinsTemperature::GetPinIsConnected())T_OutputPinsTemperature::SetPinValue(icm20600.getTemperature());
		
		
		}
	public:
		inline void SystemInit()
		{
		}
		inline void SystemStart()
		{
				   Wire.begin();
			     icm20600.initialize();
			
		
		
		}
	
	};
		
//---------------------------
}
#endif