/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_INA3221_h
#define _AUDITORSYSTEMS_INA3221_h

#include <AuditorSystems.h>

#include <Wire.h>
#include <SDL_Arduino_INA3221.h>
   #define INA3221_READ                            (0x01)
/*=========================================================================*/

/*=========================================================================
    CONFIG REGISTER (R/W)
    -----------------------------------------------------------------------*/
    #define INA3221_REG_CONFIG                      (0x00)
    /*---------------------------------------------------------------------*/
    #define INA3221_CONFIG_RESET                    (0x8000)  // Reset Bit
	
    #define INA3221_CONFIG_ENABLE_CHAN1             (0x4000)  // Enable Channel 1
    #define INA3221_CONFIG_ENABLE_CHAN2             (0x2000)  // Enable Channel 2
    #define INA3221_CONFIG_ENABLE_CHAN3             (0x1000)  // Enable Channel 3
	
    #define INA3221_CONFIG_AVG2                     (0x0800)  // AVG Samples Bit 2 - See table 3 spec
    #define INA3221_CONFIG_AVG1                     (0x0400)  // AVG Samples Bit 1 - See table 3 spec
    #define INA3221_CONFIG_AVG0                     (0x0200)  // AVG Samples Bit 0 - See table 3 spec

    #define INA3221_CONFIG_VBUS_CT2                 (0x0100)  // VBUS bit 2 Conversion time - See table 4 spec
    #define INA3221_CONFIG_VBUS_CT1                 (0x0080)  // VBUS bit 1 Conversion time - See table 4 spec
    #define INA3221_CONFIG_VBUS_CT0                 (0x0040)  // VBUS bit 0 Conversion time - See table 4 spec

    #define INA3221_CONFIG_VSH_CT2                  (0x0020)  // Vshunt bit 2 Conversion time - See table 5 spec
    #define INA3221_CONFIG_VSH_CT1                  (0x0010)  // Vshunt bit 1 Conversion time - See table 5 spec
    #define INA3221_CONFIG_VSH_CT0                  (0x0008)  // Vshunt bit 0 Conversion time - See table 5 spec

    #define INA3221_CONFIG_MODE_2                   (0x0004)  // Operating Mode bit 2 - See table 6 spec
    #define INA3221_CONFIG_MODE_1                   (0x0002)  // Operating Mode bit 1 - See table 6 spec
    #define INA3221_CONFIG_MODE_0                   (0x0001)  // Operating Mode bit 0 - See table 6 spec

/*=========================================================================*/

/*=========================================================================
    SHUNT VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA3221_REG_SHUNTVOLTAGE_1                (0x01)
/*=========================================================================*/

/*=========================================================================
    BUS VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA3221_REG_BUSVOLTAGE_1                  (0x02)
/*=========================================================================*/

/*
	INA3221_ADDR40_GND = 0b1000000, // A0 pin -> GND DEC: 64
    INA3221_ADDR41_VCC = 0b1000001, // A0 pin -> VCC DEC: 65
    INA3221_ADDR42_SDA = 0b1000010, // A0 pin -> SDA DEC: 66
    INA3221_ADDR43_SCL = 0b1000011  // A0 pin -> SCL DEC: 67
*/
namespace AuditorSystems
{
		
	template<
		typename T_I2C, T_I2C &C_I2C,
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
		typename T_ShuntResistor







		
	
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
		public T_ShuntResistor

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
		_V_PROP_( ShuntResistor )

	
	public:
	void wireWriteRegister (uint8_t reg, uint16_t value)
{
  C_I2C.beginTransmission(uint8_t(Address().GetValue()));
  #if ARDUINO >= 100
    C_I2C.write(reg);                       // Register
    C_I2C.write((value >> 8) & 0xFF);       // Upper 8-bits
    C_I2C.write(value & 0xFF);              // Lower 8-bits
  #else
    C_I2C.send(reg);                        // Register
    C_I2C.send(value >> 8);                 // Upper 8-bits
    C_I2C.send(value & 0xFF);               // Lower 8-bits
  #endif
  C_I2C.endTransmission();
}

/**************************************************************************/
/*! 
    @brief  Reads a 16 bit values over I2C
*/
/**************************************************************************/
void wireReadRegister(uint8_t reg, uint16_t *value)
{

  C_I2C.beginTransmission(uint8_t(Address().GetValue()));
  #if ARDUINO >= 100
    C_I2C.write(reg);                       // Register
  #else
    C_I2C.send(reg);                        // Register
  #endif
  C_I2C.endTransmission();
  
  delay(1); // Max 12-bit conversion time is 586us per sample

  C_I2C.requestFrom(uint8_t(Address().GetValue()), (uint8_t)2);  
  #if ARDUINO >= 100
    // Shift values to create properly formed integer
    *value = ((C_I2C.read() << 8) | C_I2C.read());
  #else
    // Shift values to create properly formed integer
    *value = ((C_I2C.receive() << 8) | C_I2C.receive());
  #endif
}

//
void INA3221SetConfig(void)
{
 
 
  // Set Config register to take into account the settings above
  uint16_t config = INA3221_CONFIG_ENABLE_CHAN1 |
                    INA3221_CONFIG_ENABLE_CHAN2 |
                    INA3221_CONFIG_ENABLE_CHAN3 |
                    INA3221_CONFIG_AVG1 |
                    INA3221_CONFIG_VBUS_CT2 |
                    INA3221_CONFIG_VSH_CT2 |
                    INA3221_CONFIG_MODE_2 |
                    INA3221_CONFIG_MODE_1 |
                    INA3221_CONFIG_MODE_0;
  wireWriteRegister(INA3221_REG_CONFIG, config);
}

/**************************************************************************/
/*! 
    @brief  Setups the HW (defaults to 32V and 2A for calibration values)
*/
/**************************************************************************/
void begin() {
  C_I2C.begin();    
  // Set chip to known config values to start
  INA3221SetConfig();
    
   // Serial.print("shut resistor="); Serial.println(Shunt_CH1());
       // Serial.print("address="); Serial.println(Address());
    
}

/**************************************************************************/
/*! 
    @brief  Gets the raw bus voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t getBusVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_REG_BUSVOLTAGE_1+(channel -1) *2, &value);
//    Serial.print("BusVoltage_raw=");
//    Serial.println(value,HEX);

  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)(value );
}

/**************************************************************************/
/*! 
    @brief  Gets the raw shunt voltage (16-bit signed integer, so +-32767)
*/
/**************************************************************************/
int16_t getShuntVoltage_raw(int channel) {
  uint16_t value;
  wireReadRegister(INA3221_REG_SHUNTVOLTAGE_1+(channel -1) *2, &value);
   // Serial.print("ShuntVoltage_raw=");
   // Serial.println(value,HEX);
  return (int16_t)value;
}


 
/**************************************************************************/
/*! 
    @brief  Gets the shunt voltage in mV (so +-168.3mV)
*/
/**************************************************************************/
float getShuntVoltage_mV(int channel) {
  int16_t value;
  value = getShuntVoltage_raw(channel);
  return value * 0.005;
}

/**************************************************************************/
/*! 
    @brief  Gets the shunt voltage in volts
*/
/**************************************************************************/
float getBusVoltage_V(int channel) {
  int16_t value = getBusVoltage_raw(channel);
  return value * 0.001;
}

/**************************************************************************/
/*! 
    @brief  Gets the current value in mA, taking into account the
            config settings and current LSB
*/
/**************************************************************************/
float getCurrent_mA(int channel) {
    float valueDec = getShuntVoltage_mV(channel)/ShuntResistor().GetValue();
  
  return valueDec;
}


/**************************************************************************/
/*! 
    @brief  Gets the Manufacturers ID
*/
/**************************************************************************/
int getManufID()
{
  uint16_t value;
  wireReadRegister(0xFE, &value);
  return value;

}
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			
				
				
		}
		
			
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
		
		T_CH1OutputPinsCurrent::SetPinValue(-getCurrent_mA(1));
		T_CH1OutputPinsPower::SetPinValue(getCurrent_mA(1)*getBusVoltage_V(1));
		T_CH1OutputPinsVoltage::SetPinValue(getBusVoltage_V(1));
		T_CH2OutputPinsCurrent::SetPinValue(-getCurrent_mA(2));
		T_CH2OutputPinsPower::SetPinValue(getCurrent_mA(2)*getBusVoltage_V(2));
		T_CH2OutputPinsVoltage::SetPinValue(getBusVoltage_V(2));
		T_CH3OutputPinsCurrent::SetPinValue(-getCurrent_mA(3));
		T_CH3OutputPinsPower::SetPinValue(getCurrent_mA(3)*getBusVoltage_V(3));
		T_CH3OutputPinsVoltage::SetPinValue(getBusVoltage_V(3));
		
		}
	public:
		inline void SystemInit()
		{
		}
		inline void SystemStart()
		{
			  begin();
			
		
		}
	
	};
		
//---------------------------
}
#endif