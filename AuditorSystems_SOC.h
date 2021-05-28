/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_SOC_h
#define _AUDITORSYSTEMS_SOC_h

#include <AuditorSystems.h>


namespace AuditorSystems
{
	/*enum TArduinoSOCApproximation
	{
		asigmoidal, 
		sigmoidal, 
		linear
	};*/
	
	template<
	typename T_Approximation,
	typename T_Bat_SOC,
	typename T_Bat_SOH,
	typename T_Capacity,
	typename T_Capacity_remain,
	typename T_Capacity_used,
	typename T_Discharge_voltage,
	typename T_Enabled,
	typename T_Max_voltage
	


	> class SOC :
	public T_Approximation,
	public T_Bat_SOC,
	public T_Bat_SOH,
	public T_Capacity,
	public T_Capacity_remain,
	public T_Capacity_used,
	public T_Discharge_voltage,
	public T_Enabled,
	public T_Max_voltage
	









		
	{
	
	public:	
		_V_PROP_( Approximation )
		_V_PROP_( Capacity )
		_V_PIN_( Capacity_remain )
		_V_PIN_( Capacity_used )
		_V_PROP_( Discharge_voltage )
		_V_PROP_( Enabled )
		_V_PROP_( Max_voltage )
		_V_PIN_( Bat_SOC )
		_V_PIN_( Bat_SOH )
	
		
	protected:
		float old_val_soc;
		float old_val_capacity,old_val_capacity_remain;
		float old_val_soh;
	
	public:
		
		float voltage=0;
		float current=0;
		uint8_t soc,start_up_soc;
		float soh;
		float current_capacity,soh_capacity=0,current_capacity_remain;
		int capacity=Capacity();
		
		byte init_soc=0;
		bool ready;
		bool charging=false;
		
		unsigned long previousMillis = 0; 

		const long interval = 1000; 

	
    public: 
		 uint8_t initial_soc(){
			 uint8_t result;
			if(Approximation()==0)result = 101 - (101 / pow(1 + pow(1.33 * (voltage - Discharge_voltage())/(Max_voltage() - Discharge_voltage()) ,4.5), 3)); //asigmoidal
		
			if(Approximation()==1)result = 105 - (105 / (1 + pow(1.724 * (voltage - Discharge_voltage())/(Max_voltage() - Discharge_voltage()), 5.5)));//sigmoidal normal
			//result = 102 - (102 / (1 + pow(1.621 * (voltage - Discharge_voltage().GetValue())/(Max_voltage().GetValue() - Discharge_voltage().GetValue()), 8.1)));//sigmoidal fast
			
		
		//else result = (voltage - Discharge_voltage()) * 100 / (Max_voltage() - Discharge_voltage());//linear mapping
		//Serial.println("SOC: "+String(result));
		
			return result;
		
	
		}
	
	
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
			
			
			if(init_soc<=2){
			soc=initial_soc();
		
				
			
			}
			unsigned long currentMillis = millis();
			if (currentMillis - previousMillis >= interval) {
		
			previousMillis = currentMillis;
			if(charging==false&&current>500){
					soc=soc-(current*0.000277778/capacity)*100;
					
			}
			if(charging==false&&current<500){
					soc=initial_soc();
					
			}
			if(charging==true&&current>500)soc=soc+(current*0.000277778/capacity)*100;
			else soc=initial_soc();
			current_capacity+=current*0.000277778;
			soh_capacity+=current*0.000277778;

		current_capacity_remain=capacity*(soc/100);
			}
		

	
			
			if(old_val_soc!=soc){
		  old_val_soc=soc;
			T_Bat_SOC::SetPinValue(soc);
			
			}
					
					
				
			if( old_val_capacity!=current_capacity &&current_capacity!=0){
					T_Capacity_used::SetPinValue(current_capacity);
					old_val_capacity=current_capacity;
				}
			if( old_val_capacity_remain!=current_capacity_remain ){
				old_val_capacity_remain=current_capacity_remain;
					T_Capacity_remain::SetPinValue(current_capacity_remain);
			}
			if( old_val_soh!=soh ){
					T_Bat_SOH::SetPinValue(soh);
					old_val_soh=soh;
					
				}
			
		
		}
	public:
		
		inline void SystemStart()
		{
				
		 
			
				
		}
		inline void Voltage_o_Receive( void *_Data )
		{
			
			voltage = *(float * )_Data;
			//soc=initial_soc();
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