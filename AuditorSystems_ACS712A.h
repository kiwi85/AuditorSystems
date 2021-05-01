/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_ACS712A_h
#define _AUDITORSYSTEMS_ACS712A_h

#include <AuditorSystems.h>
#include <ACS712.h>

namespace AuditorSystems
{
	
	template<
		//typename T_I2C, T_I2C &C_I2C,
		typename T_ACS712OutputPinsCurrent,
		typename T_ADC_PIN,	
		typename T_ADC_REF,	
		typename T_ADC_RES,	
		//typename T_Address,
		typename T_autocalibrate,	
		typename T_Enabled,	
		typename T_Sensitivity,
		typename T_ClockInputPin_o_IsConnected,
		typename T_in_mA
	
	> class ACS712A :
		public T_ACS712OutputPinsCurrent,
		public T_ADC_PIN,	
		public T_ADC_REF,	
		public T_ADC_RES,	
		//public T_Address,
		public T_autocalibrate,	
		public T_Enabled,	
		public T_Sensitivity,
		public T_ClockInputPin_o_IsConnected,
		public T_in_mA
	{
		
		
	public:
		_V_PIN_( ACS712OutputPinsCurrent )
	public:
		_V_PROP_( ADC_PIN )
	public:
		_V_PROP_( ADC_REF )
	public:
		_V_PROP_( ADC_RES )
//	public:
	//	_V_PROP_( Address )
	public:
		_V_PROP_( autocalibrate )
	
	public:
		_V_PROP_( Enabled )
	public:
		_V_PROP_( in_mA )
	public:
		_V_PROP_( Sensitivity )
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )
	
	protected:
		float current=0;
		int output=0;
		
	public:
		bool single_shot=false;
		
	protected:
		void ReadData()
		{
			ACS712 ACS(ADC_PIN(), ADC_REF(), ADC_RES(), Sensitivity());
			
			current=ACS.mA_DC();
			if(in_mA()==1)current=current*1;
			else current=current*0.001;
			T_ACS712OutputPinsCurrent::SetPinValue(current);
		}
	
	public:
		void calibrate_o_Receive(void *_Data)
		{
			
			Serial.println("calibration in progress...");
			//Serial.println("accel: "+ACCELERATION_IN_STEPS_PER_SECOND());
			//Serial.println("decel: "+DECELERATION_IN_STEPS_PER_SECOND());
			//Serial.println("speed: "+SPEED_IN_STEPS_PER_SECOND());
			//Serial.println("distance: "+DISTANCE_TO_TRAVEL_IN_STEPS());
			
		}
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			single_shot=true;
				ReadData();
				
		}
		
			
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
			
		if(!single_shot)ReadData();
		
		
		
		
		
		 
			 
		
		}
	public:
		inline void SystemInit()
		{
				
		
			Serial.println("initiating...");
		
				
		}
		inline void SystemStart()
		{
				
		
			Serial.println("Starting up...");
		Serial.println(in_mA());
				
		}
	
	};
		
//---------------------------
}
#endif