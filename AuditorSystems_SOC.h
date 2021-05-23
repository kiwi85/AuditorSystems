/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_SOC_h
#define _AUDITORSYSTEMS_SOC_h

#include <AuditorSystems.h>
#define VOLTAGE_6 voltage*voltage*voltage*voltage*voltage*voltage
#define VOLTAGE_5 voltage*voltage*voltage*voltage*voltage
#define VOLTAGE_4 voltage*voltage*voltage*voltage
#define VOLTAGE_3 voltage*voltage*voltage
#define VOLTAGE_2 voltage*voltage

namespace AuditorSystems
{
	
	template<
		typename T_Battery_Type,
		typename T_Capacity,
		typename T_Enabled,
		typename T_OutputPins_SOC,		
		typename T_OutputPins_Capacity_used,
		typename T_OutputPins_Capacity_remain,
		typename T_OutputPins_SOH


		
		
	
	> class SOC :
		public T_Battery_Type,
		public T_Capacity,
		public T_Enabled,
		public T_OutputPins_SOC,
		public T_OutputPins_Capacity_used,
		public T_OutputPins_Capacity_remain,
		public T_OutputPins_SOH
		
	{
		
	public:
		_V_PROP_( Battery_Type )	
	public:
		_V_PROP_( Capacity )
	public:
		_V_PROP_( Enabled )

	public:	
		_V_PIN_( OutputPins_SOC )
	public:
		_V_PIN_( OutputPins_Capacity_used )
	public:
		_V_PIN_( OutputPins_Capacity_remain )
	public:
		_V_PIN_( OutputPins_SOH )
	
		
	protected:
		float old_val_soc;
		float old_val_capacity,old_val_capacity_remain;
		float old_val_soh;
		bool ASOCConnected = T_OutputPins_SOC::GetPinIsConnected();
		bool ACapacity_usedConnected = T_OutputPins_Capacity_used::GetPinIsConnected();
		bool ACapacity_remainConnected = T_OutputPins_Capacity_remain::GetPinIsConnected();
		bool ASOHConnected = T_OutputPins_SOH::GetPinIsConnected();
		
	public:
		
		float voltage=0;
		float current=0;
		float soc,start_up_soc;
		float soh;
		float current_capacity,soh_capacity=0,current_capacity_remain;
		int capacity=Capacity();
		String bat=Battery_Type().GetValue();
		byte init_soc=0;
		bool ready;
		bool charging=false;
		unsigned long previousMillis = 0; 

		const long interval = 1000; 

	
	public:
	
		inline bool initial_soc(byte selection){
		
		switch(selection){
		case 0:
				soc=constrain((223483.7 - 292861.8*voltage + 153179.3*VOLTAGE_2 - 40005.46*VOLTAGE_3 + 5220.717*VOLTAGE_4 - 272.4359*VOLTAGE_5),0,100);
				break;
		case 1: 		
				soc=constrain(( 373150.6 - 494536.1*voltage + 261651.2*VOLTAGE_2 - 69115.53*VOLTAGE_3 + 9118.59*VOLTAGE_4 - 480.7692*VOLTAGE_5),0,100);
				break;
		case 2:
				soc=constrain(( -851986.4 + 1181858*voltage - 653229.7*VOLTAGE_2 + 179806.2*VOLTAGE_3 - 24647.44*VOLTAGE_4 + 1346.154*VOLTAGE_5),0,100);
				break;
		case 3:
				soc=constrain(( -861712.3 + 1151443*voltage - 612614.8*VOLTAGE_2 + 162212.3*VOLTAGE_3 - 21376.75*VOLTAGE_4 + 1121.795*VOLTAGE_5),0,100);
				break; 
		default:
				soc=0;
				break;
		}
		return true;
		
		}
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
			
			
			if(init_soc<=2){
			
		
			if(bat=="Li-Ion")ready=initial_soc(0);	
			if(bat=="NiCd")ready=initial_soc(1);	
			if(bat=="CGR18650CH")ready=initial_soc(2);
			if(bat=="AW-18650")ready=initial_soc(3);		
			start_up_soc=soc;
			}
			unsigned long currentMillis = millis();
			if (currentMillis - previousMillis >= interval) {
		
			previousMillis = currentMillis;
			soc=soc-(current*0.000277778/capacity)*100;
			current_capacity+=current*0.000277778;
			soh_capacity+=current*0.000277778;
//if(voltage>=4.11&&charging==false)soh_capacity=0;
		current_capacity_remain=capacity*(soc/100);
			}
		

		//if(voltage<=3.45&&voltage>=3.4&&charging==false)soh=100*soh_capacity/capacity;
			
			
		  
			
			
			if( ASOCConnected &&old_val_soc!=soc){
					T_OutputPins_SOC::SetPinValue(soc);
					old_val_soc=soc;
				}
			if( ACapacity_usedConnected&&old_val_capacity!=current_capacity &&current_capacity!=0){
					T_OutputPins_Capacity_used::SetPinValue(current_capacity);
					old_val_capacity=current_capacity;
				}
			if( ACapacity_remainConnected&&old_val_capacity_remain!=current_capacity_remain){
					T_OutputPins_Capacity_remain::SetPinValue(current_capacity_remain);
					old_val_capacity_remain=current_capacity_remain;
				}
			if( ASOHConnected&&old_val_soh!=soh ){
					T_OutputPins_SOH::SetPinValue(soh);
					old_val_soh=soh;
					
				}
			
		
		}
	public:
		
		inline void SystemStart()
		{
				
		 //T_OutputPins_SOC::SetPinValue();
		 //T_OutputPins_Capacity::SetPinValue();
		 //T_OutputPins_SOH::SetPinValue();
		
			//Serial.println(bat);
			//Serial.println(capacity);
			
			
		
				
		}
		inline void Voltage_o_Receive( void *_Data )
		{
			voltage = *(float * )_Data;
			init_soc++;
			if(init_soc>3)init_soc=3;
						
		}
		inline void Current_o_Receive( void *_Data )
		{
			current = *(float * )_Data;
			
		}
		inline void Charging_o_Receive( void *_Data )
		{
			charging = *(bool * )_Data;
			
			
		}
	};
		
//---------------------------
}
#endif