/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_HDC2080_h
#define _AUDITORSYSTEMS_HDC2080_h
#include <Wire.h>
#include <AuditorSystems.h>
//  Constants for setting measurement resolution
#define FOURTEEN_BIT 0
#define ELEVEN_BIT 1
#define NINE_BIT 2

//  Constants for setting sensor mode
#define TEMP_AND_HUMID 0
#define TEMP_ONLY 1
#define HUMID_ONLY 2
#define ACTIVE_LOW 0
#define ACTIVE_HIGH 1
#define LEVEL_MODE 0
#define COMPARATOR_MODE 1

//  Constants for setting sample rate
#define MANUAL 0
#define TWO_MINS 1
#define ONE_MINS 2
#define TEN_SECONDS 3
#define FIVE_SECONDS 4
#define ONE_HZ 5
#define TWO_HZ 6
#define FIVE_HZ 7
//Define Register Map
#define TEMP_LOW 0x00
#define TEMP_HIGH 0x01
#define HUMID_LOW 0x02
#define HUMID_HIGH 0x03
#define INTERRUPT_DRDY 0x04
#define TEMP_MAX 0x05
#define HUMID_MAX 0x06
#define INTERRUPT_CONFIG 0x07
#define TEMP_OFFSET_ADJUST 0x08
#define HUM_OFFSET_ADJUST 0x09
#define TEMP_THR_L 0x0A
#define TEMP_THR_H 0x0B
#define HUMID_THR_L 0x0C
#define HUMID_THR_H 0x0D
#define CONFIG 0x0E
#define MEASUREMENT_CONFIG 0x0F
#define MID_L 0xFC
#define MID_H 0xFD
#define DEVICE_ID_L 0xFE
#define DEVICE_ID_H 0xFF
namespace AuditorSystems
{
		template<
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_HDC2080OutputPinsHumidity,
		typename T_HDC2080OutputPinsTemperature

		
	
	> class HDC2080 :
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_HDC2080OutputPinsHumidity,
		public T_HDC2080OutputPinsTemperature
	{
		
		
	public:
		_V_PIN_( HDC2080OutputPinsHumidity )
		_V_PIN_( HDC2080OutputPinsTemperature )
	
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
	byte _addr=Address().GetValue();
	unsigned long lastmillis=0;
	float readTemp(void)
{
	uint8_t byte[2];
	uint16_t temp;
	byte[0] = readReg(TEMP_LOW);
	byte[1] = readReg(TEMP_HIGH);
	temp = byte[1];
	temp = (temp << 8) | byte[0];
	float f = temp;
	f = ((f * 165.0f) / 65536.0f) - 40.0f;
	return f;
}

uint8_t readTempOffsetAdjust(void)
{
	return readReg(TEMP_OFFSET_ADJUST);
}

// Página 22 do datasheet
uint8_t setTempOffsetAdjust(uint8_t value)
{
	writeReg(TEMP_OFFSET_ADJUST, value);
	return this->readTempOffsetAdjust();
}

float readHumidity(void)
{
	uint8_t byte[2];
	uint16_t humidity;
	byte[0] = readReg(HUMID_LOW);
	byte[1] = readReg(HUMID_HIGH);
	humidity = byte[1];
	humidity = (humidity << 8) | byte[0];
	float f = humidity;
	f = (f / 65536.0f) * 100.0f;

	return f;
}

uint8_t readHumidityOffsetAdjust(void)
{
	return readReg(HUM_OFFSET_ADJUST);
}

// Página 23 do datasheet
uint8_t setHumidityOffsetAdjust(uint8_t value)
{
	writeReg(HUM_OFFSET_ADJUST, value);
	return this->readHumidityOffsetAdjust();
}

void enableHeater(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = readReg(CONFIG);

	//set bit 3 to 1 to enable heater
	configContents = (configContents | 0x08);

	writeReg(CONFIG, configContents);
}

void disableHeater(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = readReg(CONFIG);

	//set bit 3 to 0 to disable heater (all other bits 1)
	configContents = (configContents & 0xF7);
	writeReg(CONFIG, configContents);
}

void openReg(uint8_t reg)
{
	C_I2C.beginTransmission(_addr); // Connect to HDC2080
	C_I2C.write(reg);			   // point to specified register
	C_I2C.endTransmission();		   // Relinquish bus control
}

uint8_t readReg(uint8_t reg)
{
	openReg(reg);
	uint8_t reading;			// holds byte of read data
	C_I2C.requestFrom(_addr, 1); // Request 1 byte from open register
	if (C_I2C.available() == 0)
	{
		reading = 0;
	}
	else
	{
		reading = C_I2C.read();
	}

	return reading;
}

void writeReg(uint8_t reg, uint8_t data)
{

	C_I2C.beginTransmission(_addr); // Open Device
	C_I2C.write(reg);			   // Point to register
	C_I2C.write(data);			   // Write data to register
	C_I2C.endTransmission();		   // Relinquish bus control
}

void setLowTemp(float temp)
{
	uint8_t temp_thresh_low;

	// Verify user is not trying to set value outside bounds
	if (temp < -40.0f)
	{
		temp = -40.0f;
	}
	else if (temp > 125.0f)
	{
		temp = 125.0f;
	}

	// Calculate value to load into register
	temp_thresh_low = (uint8_t)(256.0f * (temp + 40.0f) / 165.0f);

	writeReg(TEMP_THR_L, temp_thresh_low);
}

void setHighTemp(float temp)
{
	uint8_t temp_thresh_high;

	// Verify user is not trying to set value outside bounds
	if (temp < -40.0f)
	{
		temp = -40.0f;
	}
	else if (temp > 125.0f)
	{
		temp = 125.0f;
	}

	// Calculate value to load into register
	temp_thresh_high = (uint8_t)(256.0f * (temp + 40.0f) / 165.0f);

	writeReg(TEMP_THR_H, temp_thresh_high);
}

void setHighHumidity(float humid)
{
	uint8_t humid_thresh;

	// Verify user is not trying to set value outside bounds
	if (humid < 0.0f)
	{
		humid = 0.0f;
	}
	else if (humid > 100.0f)
	{
		humid = 100.0f;
	}

	// Calculate value to load into register
	humid_thresh = (uint8_t)(256.0f * humid / 100.0f);

	writeReg(HUMID_THR_H, humid_thresh);
}

void setLowHumidity(float humid)
{
	uint8_t humid_thresh;

	// Verify user is not trying to set value outside bounds
	if (humid < 0.0f)
	{
		humid = 0.0f;
	}
	else if (humid > 100.0f)
	{
		humid = 100.0f;
	}

	// Calculate value to load into register
	humid_thresh = (uint8_t)(256.0f * humid / 100.0f);

	writeReg(HUMID_THR_L, humid_thresh);
}

//  Return humidity from the low threshold register
float readLowHumidityThreshold(void)
{
	uint8_t regContents;

	regContents = readReg(HUMID_THR_L);
	float f = regContents;
	f = f * 100.0f / 256.0f;
	return f;
}

//  Return humidity from the high threshold register
float readHighHumidityThreshold(void)
{
	uint8_t regContents;

	regContents = readReg(HUMID_THR_H);
	float f = regContents;
	f = f * 100.0f / 256.0f;
	return f;
}

//  Return temperature from the low threshold register
float readLowTempThreshold(void)
{
	uint8_t regContents;

	regContents = readReg(TEMP_THR_L);
	float f = regContents;
	f = (f * 165.0f / 256.0f) - 40.0f;
	return f;
}

//  Return temperature from the high threshold register
float readHighTempThreshold(void)
{
	uint8_t regContents;

	regContents = readReg(TEMP_THR_H);
	float f = regContents;
	f = (f * 165.0f / 256.0f) - 40.0f;

	return f;
}

/* Upper two bits of the MEASUREMENT_CONFIG register controls
   the temperature resolution*/
void setTempRes(int resolution)
{
	uint8_t configContents;
	configContents = readReg(MEASUREMENT_CONFIG);

	switch (resolution)
	{
	case FOURTEEN_BIT:
		configContents = (configContents & 0x3F);
		break;

	case ELEVEN_BIT:
		configContents = (configContents & 0x7F);
		configContents = (configContents | 0x40);
		break;

	case NINE_BIT:
		configContents = (configContents & 0xBF);
		configContents = (configContents | 0x80);
		break;

	default:
		configContents = (configContents & 0x3F);
	}

	writeReg(MEASUREMENT_CONFIG, configContents);
}
/*  Bits 5 and 6 of the MEASUREMENT_CONFIG register controls
    the humidity resolution*/
void setHumidRes(int resolution)
{
	uint8_t configContents;
	configContents = readReg(MEASUREMENT_CONFIG);

	switch (resolution)
	{
	case FOURTEEN_BIT:
		configContents = (configContents & 0xCF);
		break;

	case ELEVEN_BIT:
		configContents = (configContents & 0xDF);
		configContents = (configContents | 0x10);
		break;

	case NINE_BIT:
		configContents = (configContents & 0xEF);
		configContents = (configContents | 0x20);
		break;

	default:
		configContents = (configContents & 0xCF);
	}

	writeReg(MEASUREMENT_CONFIG, configContents);
}

/*  Bits 2 and 1 of the MEASUREMENT_CONFIG register controls
    the measurement mode  */
void setMeasurementMode(int mode)
{
	uint8_t configContents;
	configContents = readReg(MEASUREMENT_CONFIG);

	switch (mode)
	{
	case TEMP_AND_HUMID:
		configContents = (configContents & 0xF9);
		break;

	case TEMP_ONLY:
		configContents = (configContents & 0xFC);
		configContents = (configContents | 0x02);
		break;

	case HUMID_ONLY:
		configContents = (configContents & 0xFD);
		configContents = (configContents | 0x04);
		break;

	default:
		configContents = (configContents & 0xF9);
	}

	writeReg(MEASUREMENT_CONFIG, configContents);
}

/*  Bit 0 of the MEASUREMENT_CONFIG register can be used
    to trigger measurements  */
void triggerMeasurement(void)
{
	uint8_t configContents;
	configContents = readReg(MEASUREMENT_CONFIG);

	configContents = (configContents | 0x01);
	writeReg(MEASUREMENT_CONFIG, configContents);
}

/*  Bit 7 of the CONFIG register can be used to trigger a 
    soft reset  */
void reset(void)
{
	uint8_t configContents;
	configContents = readReg(CONFIG);

	configContents = (configContents | 0x80);
	writeReg(CONFIG, configContents);
	delay(50);
}

/*  Bit 2 of the CONFIG register can be used to enable/disable 
    the interrupt pin  */
void enableInterrupt(void)
{
	uint8_t configContents;
	configContents = readReg(CONFIG);

	configContents = (configContents | 0x04);
	writeReg(CONFIG, configContents);
}

/*  Bit 2 of the CONFIG register can be used to enable/disable 
    the interrupt pin  */
void disableInterrupt(void)
{
	uint8_t configContents;
	configContents = readReg(CONFIG);

	configContents = (configContents & 0xFB);
	writeReg(CONFIG, configContents);
}

/*  Bits 6-4  of the CONFIG register controls the measurement 
    rate  */
void setRate(int rate)
{
	uint8_t configContents;
	configContents = readReg(CONFIG);

	switch (rate)
	{
	case MANUAL:
		configContents = (configContents & 0x8F);
		break;

	case TWO_MINS:
		configContents = (configContents & 0x9F);
		configContents = (configContents | 0x10);
		break;

	case ONE_MINS:
		configContents = (configContents & 0xAF);
		configContents = (configContents | 0x20);
		break;

	case TEN_SECONDS:
		configContents = (configContents & 0xBF);
		configContents = (configContents | 0x30);
		break;

	case FIVE_SECONDS:
		configContents = (configContents & 0xCF);
		configContents = (configContents | 0x40);
		break;

	case ONE_HZ:
		configContents = (configContents & 0xDF);
		configContents = (configContents | 0x50);
		break;

	case TWO_HZ:
		configContents = (configContents & 0xEF);
		configContents = (configContents | 0x60);
		break;

	case FIVE_HZ:
		configContents = (configContents | 0x70);
		break;

	default:
		configContents = (configContents & 0x8F);
	}

	writeReg(CONFIG, configContents);
}

/*  Bit 1 of the CONFIG register can be used to control the  
    the interrupt pins polarity */
void setInterruptPolarity(int polarity)
{
	uint8_t configContents;
	configContents = readReg(CONFIG);

	switch (polarity)
	{
	case ACTIVE_LOW:
		configContents = (configContents & 0xFD);
		break;

	case ACTIVE_HIGH:
		configContents = (configContents | 0x02);
		break;

	default:
		configContents = (configContents & 0xFD);
	}

	writeReg(CONFIG, configContents);
}

/*  Bit 0 of the CONFIG register can be used to control the  
    the interrupt pin's mode */
void setInterruptMode(int mode)
{
	uint8_t configContents;
	configContents = readReg(CONFIG);

	switch (mode)
	{
	case LEVEL_MODE:
		configContents = (configContents & 0xFE);
		break;

	case COMPARATOR_MODE:
		configContents = (configContents | 0x01);
		break;

	default:
		configContents = (configContents & 0xFE);
	}

	writeReg(CONFIG, configContents);
}

uint8_t readInterruptStatus(void)
{
	uint8_t regContents;
	regContents = readReg(INTERRUPT_DRDY);
	return regContents;
}

//  Clears the maximum temperature register
void clearMaxTemp(void)
{
	writeReg(TEMP_MAX, 0x00);
}

//  Clears the maximum humidity register
void clearMaxHumidity(void)
{
	writeReg(HUMID_MAX, 0x00);
}

//  Reads the maximum temperature register
float readMaxTemp(void)
{
	uint8_t regContents;

	regContents = readReg(TEMP_MAX);
	float f = regContents;
	f = (f * 165.0f / 256.0f) - 40.0f;

	return f;
}

//  Reads the maximum humidity register
float readMaxHumidity(void)
{
	uint8_t regContents;

	regContents = readReg(HUMID_MAX);
	float f = regContents;
	f = (f / 256.0f) * 100.0f;

	return f;
}

// Enables the interrupt pin for comfort zone operation
void enableThresholdInterrupt(void)
{

	uint8_t regContents;
	regContents = readReg(INTERRUPT_CONFIG);

	regContents = (regContents | 0x78);

	writeReg(INTERRUPT_CONFIG, regContents);
}

// Disables the interrupt pin for comfort zone operation
void disableThresholdInterrupt(void)
{
	uint8_t regContents;
	regContents = readReg(INTERRUPT_CONFIG);

	regContents = (regContents & 0x87);

	writeReg(INTERRUPT_CONFIG, regContents);
}

// enables the interrupt pin for DRDY operation
void enableDRDYInterrupt(void)
{
	uint8_t regContents;
	regContents = readReg(INTERRUPT_CONFIG);

	regContents = (regContents | 0x80);

	writeReg(INTERRUPT_CONFIG, regContents);
}

// disables the interrupt pin for DRDY operation
void disableDRDYInterrupt(void)
{
	uint8_t regContents;
	regContents = readReg(INTERRUPT_CONFIG);

	regContents = (regContents & 0x7F);

	writeReg(INTERRUPT_CONFIG, regContents);
}



	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			
				
				
		}
		
			
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
			if (millis() - lastmillis >= 500)
{  
lastmillis = millis();     
		
		T_HDC2080OutputPinsHumidity::SetPinValue(readHumidity());
		T_HDC2080OutputPinsTemperature::SetPinValue(readTemp());	 
}
		}
	public:
		inline void SystemInit()
		{
			
		}
		inline void SystemStart()
		{
	C_I2C.begin();
			reset();
			 setMeasurementMode(TEMP_AND_HUMID);  // Set measurements to temperature and humidity
			setRate(FIVE_HZ);                     // Set measurement frequency to 1 Hz
			setTempRes(FOURTEEN_BIT);
			setHumidRes(FOURTEEN_BIT);
			triggerMeasurement();
		}
	
	};
		
//---------------------------
}
#endif