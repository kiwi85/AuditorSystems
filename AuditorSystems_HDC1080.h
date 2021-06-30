/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_HDC1080_h
#define _AUDITORSYSTEMS_HDC1080_h
#include <Wire.h>
#include <AuditorSystems.h>

namespace AuditorSystems
{
		template<
		typename T_I2C, T_I2C &C_I2C,
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
	
	protected:
		uint16_t temperatureRaw;
		uint16_t humidityRaw;
	public:
	uint8_t adr=Address().GetValue();
		void config(){

			temperatureRaw=0;
			humidityRaw=0;

			//config the temp sensor to read temp then humidity in one transaction
			//config the resolution to 14 bits for temp & humidity

			writeRegister(0x02,0x10);

		}

		void writeRegister(uint8_t address, uint16_t value){
			C_I2C.beginTransmission(adr);
			C_I2C.write(address);
			C_I2C.write(value);
			C_I2C.endTransmission();
		}



		void readTempHumid(){
			//set pointer register
			C_I2C.beginTransmission(adr);
			C_I2C.write(0x00);
			C_I2C.endTransmission();
			delay(15);
			C_I2C.requestFrom(adr, 4);
			temperatureRaw = temperatureRaw << 8 | C_I2C.read();
			temperatureRaw = temperatureRaw << 8 | C_I2C.read();
			humidityRaw = humidityRaw << 8 | C_I2C.read();
			humidityRaw = humidityRaw << 8 | C_I2C.read();

		}

		//returns temp in celcius
		float getTemp(){

			// (rawTemp/2^16)*165 - 40
			return ( (float)temperatureRaw )*165/65536 - 40;

		}

		float getRelativeHumidity(){

			//(rawHumidity/2^16)*100
			return ( (float)humidityRaw )*100/65536;
		}

		float* getTempHumid(float* tempHumid){

			*tempHumid = getTemp();
			*(tempHumid+1) = getRelativeHumidity();
			return tempHumid;

		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			
				
				
		}
		
			
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
			readTempHumid();
		
		T_HDC1080OutputPinsHumidity::SetPinValue(getRelativeHumidity());
		T_HDC1080OutputPinsTemperature::SetPinValue(getTemp());	 
		
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
	config();

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