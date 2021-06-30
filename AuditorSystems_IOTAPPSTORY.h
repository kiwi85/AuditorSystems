/*Created 2-25-2021 - Julian Hüsing Auditor
  AuditorSystems_IOTAPPSTORY.h
  //BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

  -------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_IOTAPPSTORY_h
#define _AUDITORSYSTEMS_IOTAPPSTORY_h
#define COMPDATE __DATE__ __TIME__
#define MODEBUTTON 26

#include <IOTAppStory.h>
 
#include <AuditorSystems.h>
#include <ArduinoJson.h>

char* string2char(String command){
		if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
		}
	}
namespace AuditorSystems
{
 IOTAppStory IAS(COMPDATE, MODEBUTTON); // Initialize IotAppStory


	

//--------------------------
template <
	typename T_DeviceName,
	typename T_Enabled,
	typename T_Modebutton,
	typename T_NetworkLocalIP,
	typename T_NetworkMAC,
	typename T_NetworkDeviceName,
	typename T_NetworkRSSI,	
	typename T_NetworkStatus,	
	typename T_NetworkEEPROMVars,
	typename T_NetworkOTAProgress,
	typename T_updInt


  > class IOTAPPSTORY :
	public T_DeviceName,
	public T_Enabled,
	public T_Modebutton,
	public T_NetworkLocalIP,
	public T_NetworkMAC,
	public T_NetworkDeviceName,
	public T_NetworkRSSI,	
	public T_NetworkStatus,	
	public T_NetworkEEPROMVars,
	public T_NetworkOTAProgress,
	public T_updInt





 

{
  public:
    _V_PROP_( DeviceName )
    _V_PROP_( Enabled )
	_V_PROP_( Modebutton )
    _V_PROP_( updInt )
	

   public:
   	_V_PIN_( NetworkDeviceName ) 
   	_V_PIN_( NetworkEEPROMVars )
	_V_PIN_( NetworkMAC )
	_V_PIN_( NetworkLocalIP )
   	_V_PIN_( NetworkOTAProgress )
    _V_PIN_( NetworkRSSI )
	_V_PIN_( NetworkStatus )






  
  public:
	
	Mitov::String deviceName = DeviceName();
	String chipId="";
    long interval=(long)updInt().GetValue();
    String txtval="";
	Mitov::String variables_json="";
    bool sendvalues = false;
	int status=0;
	StaticJsonDocument<1024> doc;
	unsigned long printEntry;	
	
  public:
    void read_variables_json() 
	{
		variables_json="";
		serializeJson(doc, variables_json);
		T_NetworkEEPROMVars::SetPinValue(variables_json);
    }
  public:
    inline void SystemStart()
    {
	

      chipId      = String(ESP_GETCHIPID);
      chipId      = "-" + chipId.substring(chipId.length() - 3);
      deviceName += chipId;

	
      IAS.preSetDeviceName(deviceName);



      // You can configure callback functions that can give feedback to the app user about the current state of the application.
      // In this example we use serial print to demonstrate the call backs. But you could use leds etc.
/*
      IAS.onModeButtonShortPress([this]() {
        Serial.println(F(" If mode button is released, I will enter in firmware update mode."));
        Serial.println(F("*-------------------------------------------------------------------------*"));
		T_NetworkStatus::SetPinValue(1);
      });

      IAS.onModeButtonLongPress([this]() {
        Serial.println(F(" If mode button is released, I will enter in configuration mode."));
        Serial.println(F("*-------------------------------------------------------------------------*"));
		T_NetworkStatus::SetPinValue(2);
      });

      IAS.onModeButtonVeryLongPress([this]() {
        Serial.println(F(" If mode button is released, I won't do anything unless you program me to."));
        Serial.println(F("*-------------------------------------------------------------------------*"));
   
		T_NetworkStatus::SetPinValue(3);
      });

      IAS.onFirmwareUpdateProgress([this](int written, int total) {
        Serial.print(".");

        
          if(written%5==0){
          Serial.print(F("\n Written "));
          Serial.print(written);
          Serial.print(F(" of "));
          Serial.print(total);
		  T_NetworkOTAProgress::SetPinValue(written);
          }
        
      });

      
        IAS.onModeButtonNoPress([this]() {
        Serial.println(F(" Mode Button is not pressed."));
        Serial.println(F("*-------------------------------------------------------------------------*"));
		T_NetworkStatus::SetPinValue(4);
        });

        IAS.onFirstBoot([this]() {
        Serial.println(F(" Run or display something on the first time this app boots"));
        Serial.println(F("*-------------------------------------------------------------------------*"));
		T_NetworkStatus::SetPinValue(5);
        });

        IAS.onFirmwareUpdateCheck([this]() {
        Serial.println(F(" Checking if there is a firmware update available."));
        Serial.println(F("*-------------------------------------------------------------------------*"));
		T_NetworkStatus::SetPinValue(6);
        });

        IAS.onFirmwareUpdateDownload([this]() {
        Serial.println(F(" Downloading and Installing firmware update."));
        Serial.println(F("*-------------------------------------------------------------------------*"));
		T_NetworkStatus::SetPinValue(7);
        });

        IAS.onFirmwareUpdateError([this](String statusMessage) {
        Serial.println(F(" Update failed...Check your logs"));
        Serial.println(F("*-------------------------------------------------------------------------*"));
		T_NetworkStatus::SetPinValue(8);
        });

        IAS.onConfigMode([this]() {
        Serial.println(F(" Starting configuration mode. Search for my WiFi and connect to 192.168.4.1."));
        Serial.println(F("*-------------------------------------------------------------------------*"));
		T_NetworkStatus::SetPinValue(9);
        });

        IAS.onFirstBoot([]() {
			 
	
			IAS.eraseEEPROM('L');

	         });
      
*/
      /* TIP! delete the lines above when not used */

      IAS.begin();                                            // Run IOTAppStory
      IAS.setCallHomeInterval(interval);                  // Call home interval in seconds(disabled by default), 0 = off, use 60s only for development. Please change it to at least 2 hours in production
	
	read_variables_json();

    }

    inline void SystemLoopBegin()
    {
		if (millis() - printEntry > 3000) {  
		status=random(0,100); 
		Mitov::String ip=WiFi.localIP().toString();
		Mitov::String mac=WiFi.macAddress();
		T_NetworkLocalIP::SetPinValue(ip);
		T_NetworkDeviceName::SetPinValue(deviceName);
		
		T_NetworkMAC::SetPinValue(mac);
		T_NetworkRSSI::SetPinValue(WiFi.RSSI());
      	read_variables_json();
		printEntry = millis();
		}
    }
};


//---------------------------
template <
  typename T_OWNER, T_OWNER &C_OWNER,
  typename T_Length,
  typename T_Variable,
  typename T_VariableName
  > class TxtVar :
  public T_Length,
  public T_Variable,
  public T_VariableName

{
  public:
	_V_PROP_( Length )
    _V_PROP_( Variable )
    _V_PROP_( VariableName )
  
   
  public:
 
    String name = VariableName();
	String value = Variable();
	unsigned int length=Length()+1;
	
  public:
  inline void SystemInit()
  {
	   char* val=string2char(value);
     const char* label=string2char(name);

      IAS.addField(val, label, 10);  
	  C_OWNER.doc[name] = value;
  }
	
    inline void SystemStart()
    {
      //Serial.println(Variable());
    
	
	
 
    }
    inline void SystemLoopBegin()
    {
    }


};

//---------------------------
template <
  typename T_OWNER, T_OWNER &C_OWNER,
  typename T_Length,
  typename T_Variable,
  typename T_VariableName
  > class IntVar :
  public T_Length,
  public T_Variable,
  public T_VariableName

{
  public:
	_V_PROP_( Length )
    _V_PROP_( Variable )
    _V_PROP_( VariableName )
  
   
  public:
    String name = VariableName();
	unsigned int length=Length()+1;
	int value = Variable().GetValue();
	char valuestr[50];
	
  public:
	inline void SystemInit()
	{
		  String value_val=itoa(value, valuestr, 10);
	
	char* val=string2char(value_val);
     const char* label=string2char(name);
	
     IAS.addField(val, label, length,'N');
	  C_OWNER.doc[name] = value_val;	
	}
    inline void SystemStart()
    {
      //Serial.println(Variable());
      
   
	
 
    }
    inline void SystemLoopBegin()
    {
    }


};
//---------------------------
}
#endif