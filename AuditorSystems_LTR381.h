/*Created 04-28-2021 - Julian Hüsing Auditor
AuditorSystems_ACS712.h
//BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

-------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_LTR381_h
#define _AUDITORSYSTEMS_LTR381_h
#include <Wire.h>

#include <AuditorSystems.h>
#define LTR381_SLAVE_ADDR                       (0x53)
#define LTR381_I2C_ADDR                         LTR381_ADDR_TRANS(LTR381_SLAVE_ADDR)
#define RES_16BIT                               1
#define RES_8BIT                                2
/* Device info */
#define LTR381_PART_ID_VAL                      0xC2
#define LTR381_MANUFAC_ID_VAL                   0x05

#define LTR381_WAIT_TIME_PER_CHECK              (10)
#define LTR381_WAIT_TIME_TOTAL                  (100)

/*******************************************************************************
*************   Non-Configurable (Unless data sheet is updated)   *************
******************************************************************************/
/* Device register set address */
#define LTR381_MAIN_CTRL_REG                    (0x00)
#define LTR381_ALS_CS_MEAS_RATE_REG             (0x04)
#define LTR381_ALS_CS_GAIN_REG                  (0x05)
#define LTR381_PART_ID_REG                      (0x06)
#define LTR381_MAIN_STATUS_REG                  (0x07)
#define LTR381_CS_DATA_IR_0_REG                 (0x0A)
#define LTR381_CS_DATA_IR_1_REG                 (0x0B)
#define LTR381_CS_DATA_IR_2_REG                 (0x0C)
#define LTR381_CS_DATA_GREEN_0_REG              (0x0D)
#define LTR381_CS_DATA_GREEN_1_REG              (0x0E)
#define LTR381_CS_DATA_GREEN_2_REG              (0x0F)
#define LTR381_CS_DATA_RED_0_REG                (0x10)
#define LTR381_CS_DATA_RED_1_REG                (0x11)
#define LTR381_CS_DATA_RED_2_REG                (0x12)
#define LTR381_CS_DATA_BLUE_0_REG               (0x13)
#define LTR381_CS_DATA_BLUE_1_REG               (0x14)
#define LTR381_CS_DATA_BLUE_2_REG               (0x15)
#define LTR381_INT_CFG_REG                      (0x19)
#define LTR381_INT_PST_REG                      (0x1A)
#define LTR381_ALS_THRES_UP_0_REG               (0x21)
#define LTR381_ALS_THRES_UP_1_REG               (0x22)
#define LTR381_ALS_THRES_UP_2_REG               (0x23)
#define LTR381_ALS_THRES_LOW_0_REG              (0x24)
#define LTR381_ALS_THRES_LOW_1_REG              (0x25)
#define LTR381_ALS_THRES_LOW_2_REG              (0x26)

/* Register MAIN_CTRL field */
#define LTR381_ALS_CS_ENABLE__REG               (LTR381_MAIN_CTRL_REG)
#define LTR381_ALS_CS_ENABLE__POS               (1)
#define LTR381_ALS_CS_ENABLE__MSK               (0x02)
#define LTR381_CS_MODE__REG                     (LTR381_MAIN_CTRL_REG)
#define LTR381_CS_MODE__POS                     (2)
#define LTR381_CS_MODE__MSK                     (4)
#define LTR381_SW_RESET__REG                    (LTR381_MAIN_CTRL_REG)
#define LTR381_SW_RESET__POS                    (4)
#define LTR381_SW_RESET__MSK                    (0x10)

/* Register ALS_CS_MEAS_RATE field */
#define LTR381_ALS_CS_MEAS_RATE__REG            (LTR381_ALS_CS_MEAS_RATE_REG)
#define LTR381_ALS_CS_MEAS_RATE__POS            (0)
#define LTR381_ALS_CS_MEAS_RATE__MSK            (0x07)
#define LTR381_ALS_CS_RESOLUTION__REG           (LTR381_ALS_CS_MEAS_RATE_REG)
#define LTR381_ALS_CS_RESOLUTION__POS           (4)
#define LTR381_ALS_CS_RESOLUTION__MSK           (0x70)

/* Register ALS_CS_GAIN field */
#define LTR381_ALS_CS_GAIN_RANGE__REG           (LTR381_ALS_CS_GAIN_REG)
#define LTR381_ALS_CS_GAIN_RANGE__POS           (0)
#define LTR381_ALS_CS_GAIN_RANGE__MSK           (0x07)

/* Register PART_ID field */
#define LTR381_PART_NUMBER_ID__REG              (LTR381_PART_ID_REG)
#define LTR381_PART_NUMBER_ID__POS              (4)
#define LTR381_PART_NUMBER_ID__MSK              (0xF0)

/* Register MAIN_STATUS field */
#define LTR381_ALS_CS_DATA_STATUS__REG          (LTR381_MAIN_STATUS_REG)
#define LTR381_ALS_CS_DATA_STATUS__POS          (3)
#define LTR381_ALS_CS_DATA_STATUS__MSK          (0x08)
#define LTR381_ALS_CS_INT_STATUS__REG           (LTR381_MAIN_STATUS_REG)
#define LTR381_ALS_CS_INT_STATUS__POS           (4)
#define LTR381_ALS_CS_INT_STATUS__MSK           (0x10)
#define LTR381_POWER_ON_STATUS__REG             (LTR381_MAIN_STATUS_REG)
#define LTR381_POWER_ON_STATUS__POS             (5)
#define LTR381_POWER_ON_STATUS__MSK             (0x20)

/* Register INT_CFG field */
#define LTR381_ALS_INT_PIN_EN__REG              (LTR381_INT_CFG_REG)
#define LTR381_ALS_INT_PIN_EN__POS              (2)
#define LTR381_ALS_INT_PIN_EN__MSK              (0x04)
#define LTR381_ALS_INT_SEL__REG                 (LTR381_INT_CFG_REG)
#define LTR381_ALS_INT_SEL__POS                 (4)
#define LTR381_ALS_INT_SEL__MSK                 (0x30)

/* Register INT_PST field */
#define LTR381_ALS_CS_PERSIST__REG              (LTR381_INT_PST_REG)
#define LTR381_ALS_CS_PERSIST__POS              (4)
#define LTR381_ALS_CS_PERSIST__MSK              (0xF0)
namespace AuditorSystems
{
	const uint8_t gamma_table[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,
    3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   6,   6,
    6,   7,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  11,  11,  11,
    12,  12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
    19,  20,  20,  21,  22,  22,  23,  23,  24,  25,  25,  26,  26,  27,  28,  28,
    29,  30,  30,  31,  32,  33,  33,  34,  35,  35,  36,  37,  38,  39,  39,  40,
    41,  42,  43,  43,  44,  45,  46,  47,  48,  49,  50,  50,  51,  52,  53,  54,
    55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  71,
    72,  73,  74,  75,  76,  77,  78,  80,  81,  82,  83,  84,  86,  87,  88,  89,
    91,  92,  93,  94,  96,  97,  98,  100, 101, 102, 104, 105, 106, 108, 109, 110,
    112, 113, 115, 116, 118, 119, 121, 122, 123, 125, 126, 128, 130, 131, 133, 134,
    136, 137, 139, 140, 142, 144, 145, 147, 149, 150, 152, 154, 155, 157, 159, 160,
    162, 164, 166, 167, 169, 171, 173, 175, 176, 178, 180, 182, 184, 186, 187, 189,
    191, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
    223, 225, 227, 229, 231, 233, 235, 238, 240, 242, 244, 246, 248, 251, 253, 255
};
	template<
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_ClockInputPin_o_IsConnected,
		typename T_ColorOutputPin,
		typename T_Enabled,
		typename T_UVOutputPin
	

		
	
	> class LTR381 :
		public T_Address,
		public T_ClockInputPin_o_IsConnected,
		public T_ColorOutputPin,
		public T_Enabled,		
		public T_UVOutputPin
	
	{
		
		
	
		
	
	public:
		_V_PROP_( Address )
	public:
		_V_PROP_( ClockInputPin_o_IsConnected )
	public:
		_V_PIN_( ColorOutputPin )
	public:
		_V_PROP_( Enabled )
	public:
		_V_PIN_( UVOutputPin )
	
	private:
		
		bool clocked=false;

	public:
	uint8_t addr=(uint8_t)Address().GetValue();
		
	//const int Address = 0x53; // obtained by i2c scanner; all default address of tlv sensor - 0x1F, 0x5E, or 0x3E

	byte i2c_readRegisterByte (uint8_t deviceAddress, uint8_t registerAddress) {
	  byte registerData;
	  C_I2C.beginTransmission(deviceAddress);              // set sensor target
	  C_I2C.write(registerAddress);                                     // set memory pointer
	  C_I2C.endTransmission();
	  // delete this comment – it was only needed for blog layout.
	  C_I2C.requestFrom( deviceAddress,  1);     // request one byte
	  registerData = C_I2C.read();
	  // you could add more data reads here if you request more than one byte
	  return registerData;           // the returned byte from this function is the content from registerAddress
	}
	// delete this comment – it was only needed to maintain blog layout
	byte i2c_writeRegisterByte (uint8_t deviceAddress, uint8_t registerAddress, uint8_t newRegisterByte)  {
	  byte result;
	  C_I2C.beginTransmission(deviceAddress);
	  C_I2C.write(registerAddress);
	  C_I2C.write(newRegisterByte);
	  result = C_I2C.endTransmission(); // Wire.endTransmission(); returns 0 if write operation was successful
	  // delete this comment – it was only needed for blog layout.
	  //delay(5);  // optional:  some sensors need time to write the new data, but most do not. Check Datasheet.
	  if (result > 0)
	  {
		Serial.print(F("FAIL in I2C register write! Error code: "));
		Serial.println(result);
	  }
	  // delete this comment – it was only needed for blog layout.
	  return result;    // the returned value from this function could be tested as shown above
	  //it’s a good idea to check the return from Wire.endTransmission() the first time you write to a sensor
	  //if the first test is okay (result is 0), then I2C sensor coms are working and you don’t have to do extra tests
	}

	uint32_t get_green(byte RES_){
	byte high,middle,low;

	high=i2c_readRegisterByte(addr,LTR381_CS_DATA_GREEN_0_REG);
	middle=i2c_readRegisterByte(addr,LTR381_CS_DATA_GREEN_1_REG);
	low=i2c_readRegisterByte(addr,LTR381_CS_DATA_GREEN_2_REG) & 0x0F;

	if(RES_==RES_16BIT)return  (((low * 256 * 256) + (middle * 256) + high));
	if(RES_==RES_8BIT)return  gamma_table[high];


	}
	uint32_t get_ir(byte RES_){
	byte high,middle,low;

	high=i2c_readRegisterByte(addr,LTR381_CS_DATA_IR_0_REG);
	middle=i2c_readRegisterByte(addr,LTR381_CS_DATA_IR_1_REG);
	low=i2c_readRegisterByte(addr,LTR381_CS_DATA_IR_2_REG) & 0x0F;

	if(RES_==RES_16BIT)return  (((low * 256 * 256) + (middle * 256) + high));
	if(RES_==RES_8BIT)return gamma_table[high];


	}
	uint32_t get_red(byte RES_){
	byte high,middle,low;

	high=i2c_readRegisterByte(addr,LTR381_CS_DATA_RED_0_REG);
	middle=i2c_readRegisterByte(addr,LTR381_CS_DATA_RED_1_REG);
	low=i2c_readRegisterByte(addr,LTR381_CS_DATA_RED_2_REG) & 0x0F;

	if(RES_==RES_16BIT)return  (((low * 256 * 256) + (middle * 256) + high));
	if(RES_==RES_8BIT)return  gamma_table[high];


	}
	uint32_t get_blue(byte RES_){
	byte high,middle,low;

	high=i2c_readRegisterByte(addr,LTR381_CS_DATA_BLUE_0_REG);
	middle=i2c_readRegisterByte(addr,LTR381_CS_DATA_BLUE_1_REG);
	low=i2c_readRegisterByte(addr,LTR381_CS_DATA_BLUE_2_REG) & 0x0F;

	if(RES_==RES_16BIT)return  (((low * 256 * 256) + (middle * 256) + high));
	if(RES_==RES_8BIT)return  gamma_table[high];
	}
	void ltr381_init(){
	 
		 
			i2c_writeRegisterByte(addr, LTR381_ALS_CS_MEAS_RATE_REG, 0x42);
			i2c_writeRegisterByte(addr, LTR381_ALS_CS_GAIN_REG, 0x01);
			i2c_writeRegisterByte(addr, LTR381_MAIN_CTRL_REG, 0x06);
			
		
	}
		void read_sensor(){
				Mitov::TColor AColor;
				AColor.Red=get_red(RES_8BIT);
				AColor.Green=get_green(RES_8BIT);
				AColor.Blue=get_blue(RES_8BIT);
				if(T_ColorOutputPin::GetPinIsConnected())T_ColorOutputPin::SetPinValue(AColor);
				if(T_UVOutputPin::GetPinIsConnected())T_UVOutputPin::SetPinValue(get_ir(RES_8BIT));
				}
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			clocked=true;
			read_sensor();	
				
		}
		
			
	public:
		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) return;
			
			if(clocked==false){
			read_sensor();
			}
		
				
		
		}
	public:
		inline void SystemInit()
		{
			 
		}
		inline void SystemStart()
		{
			
			C_I2C.begin();   //join I2C bus
			ltr381_init();
			read_sensor();	
		}
	};
		
//---------------------------
}
#endif