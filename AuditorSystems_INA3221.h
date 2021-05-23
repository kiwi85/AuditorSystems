/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_INA3221_h
#define _AUDITORSYSTEMS_INA3221_h

#include <AuditorSystems.h>

#include <Wire.h>
#include <Beastdevices_INA3221.h>
/*
	INA3221_ADDR40_GND = 0b1000000, // A0 pin -> GND DEC: 64
    INA3221_ADDR41_VCC = 0b1000001, // A0 pin -> VCC DEC: 65
    INA3221_ADDR42_SDA = 0b1000010, // A0 pin -> SDA DEC: 66
    INA3221_ADDR43_SCL = 0b1000011  // A0 pin -> SCL DEC: 67
*/
namespace AuditorSystems
{
	Beastdevices_INA3221 ina3221(INA3221_ADDR41_VCC);
	template<
		typename T_Address,
		typename T_CH1OutputPinsVoltage,
		typename T_CH1OutputPinsCurrent,
		typename T_CH1OutputPinsPower,
		typename T_CH2OutputPinsVoltage,
		typename T_CH2OutputPinsCurrent,
		typename T_CH2OutputPinsPower,
		typename T_CH3OutputPinsVoltage,
		typename T_CH3OutputPinsCurrent,
		typename T_CH3OutputPinsPower,
		
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_Shunt_CH1,
		typename T_Shunt_CH2,
		typename T_Shunt_CH3






		
	
	> class INA3221 :
		public T_Address,
		public T_CH1OutputPinsVoltage,
		public T_CH1OutputPinsCurrent,
		public T_CH1OutputPinsPower,
		public T_CH2OutputPinsVoltage,
		public T_CH2OutputPinsCurrent,
		public T_CH2OutputPinsPower,
		public T_CH3OutputPinsVoltage,
		public T_CH3OutputPinsCurrent,
		public T_CH3OutputPinsPower,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_Shunt_CH1,
		public T_Shunt_CH2,
		public T_Shunt_CH3
	{
		
		
	
	public:
		_V_PROP_( Address )
	public:
		_V_PIN_( CH1OutputPinsCurrent )
		_V_PIN_( CH1OutputPinsPower )
		_V_PIN_( CH1OutputPinsVoltage )
		_V_PIN_( CH2OutputPinsCurrent )
		_V_PIN_( CH2OutputPinsPower )
		_V_PIN_( CH2OutputPinsVoltage )
		_V_PIN_( CH3OutputPinsCurrent )
		_V_PIN_( CH3OutputPinsPower )
		_V_PIN_( CH3OutputPinsVoltage )
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )
	public:
		_V_PROP_( Enabled )
	
	public:
		_V_PROP_( Shunt_CH1 )
		_V_PROP_( Shunt_CH2 )
		_V_PROP_( Shunt_CH3 )
	public:
		uint8_t i2c_adr=Address().GetValue();
	

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			
				
				
		}
		
			
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
		
		T_CH1OutputPinsCurrent::SetPinValue(ina3221.getCurrent(INA3221_CH1)/1000);
		T_CH1OutputPinsPower::SetPinValue(ina3221.getCurrent(INA3221_CH1)/1000*ina3221.getVoltage(INA3221_CH1));
		T_CH1OutputPinsVoltage::SetPinValue(ina3221.getVoltage(INA3221_CH1));
		T_CH2OutputPinsCurrent::SetPinValue(ina3221.getCurrent(INA3221_CH2)/1000);
		T_CH2OutputPinsPower::SetPinValue(ina3221.getCurrent(INA3221_CH2)/1000*ina3221.getVoltage(INA3221_CH2));
		T_CH2OutputPinsVoltage::SetPinValue(ina3221.getVoltage(INA3221_CH2));
		T_CH3OutputPinsCurrent::SetPinValue(ina3221.getCurrent(INA3221_CH3)/1000);
		T_CH3OutputPinsPower::SetPinValue(ina3221.getCurrent(INA3221_CH3)/1000*ina3221.getVoltage(INA3221_CH3));
		T_CH3OutputPinsVoltage::SetPinValue(ina3221.getVoltage(INA3221_CH3));
		
		}
	public:
		inline void SystemInit()
		{
		}
		inline void SystemStart()
		{
			  ina3221.begin();
				ina3221.reset();
			
			// Set shunt resistors to 10 mOhm for all channels
			  ina3221.setShuntRes(Shunt_CH1(), Shunt_CH2(), Shunt_CH3());
			//Serial.println("Starting up...INA3221 at address"+String(Address()));
		
		}
	
	};
		
//---------------------------
}
#endif