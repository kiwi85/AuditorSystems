/*Created 2-25-2021 - Julian Hüsing Auditor
  AuditorSystems_IOTAPPSTORY.h
  //BETA Visuino ver. 8-0-0-0-10 not yet availble on creation date

  -------------------------------------------------*/

#ifndef _AUDITORSYSTEMS_WIFIMQTT_h
#define _AUDITORSYSTEMS_WIFIMQTT_h



 
#include <AuditorSystems.h>
#include <ArduinoJson.h>
#define WM_NODEBUG
#define SUGGESTED_AREA "Smart Plant Sensors"
#include "version.h"
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <time.h>
#include <stdio.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <MQTT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define USEOTA
// enable OTA
#ifdef USEOTA
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif
#define uS_TO_S_FACTOR 1000000  //Conversion factor for micro seconds to seconds
//#define TIME_TO_SLEEP  60        //Time ESP32 will go to sleep (in seconds)

const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

unsigned long mtime = 0;

RTC_DATA_ATTR int bootCount = 0;

WiFiManager wm;
 // setup some parameters
  WiFiManagerParameter custom_html("<p>This Is Custom HTML</p>"); // only custom html
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", "", 40);
  WiFiManagerParameter custom_mqtt_passw("password", "mqtt password", "", 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "", 6);
  WiFiManagerParameter custom_token("api_token", "api token", "", 16);
 WiFiManagerParameter time_to_sleep("time_to_sleep", "sleep time", "", 4);
  WiFiManagerParameter custom_tokenb("invalid token", "invalid token", "", 0); // id is invalid, cannot contain spaces
  WiFiManagerParameter custom_ipaddress("input_ip", "input IP", "", 15,"pattern='\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}'"); // custom input attrs (ip mask)

  const char _customHtml_checkbox[] = "type=\"checkbox\""; 
  WiFiManagerParameter custom_checkbox("checkbox", "my checkbox", "T", 2, _customHtml_checkbox, WFM_LABEL_AFTER);

namespace AuditorSystems
{


	

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


  > class WIFIMQTT :
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
	
	
	WiFiClient net;
	MQTTClient client;
	IPAddress ip;   
	WiFiUDP ntpUDP;
	NTPClient timeClient(ntpUDP);
	// TEST OPTION FLAGS
	bool TEST_CP         = false; // always start the configportal, even if ap found
	int  TESP_CP_TIMEOUT = 90; // test cp timeout

	bool TEST_NET        = false; // do a network test after connect, (gets ntp time)
	bool ALLOWONDEMAND   = true; // enable on demand
	bool disable_sleep;
	int  ONDDEMANDPIN    = 26; // gpio for button
	String deviceName = "SPS-";
	String json="";
	String config_json="";
	String time_stamp;
	uint32_t chipId=0;
	char topic[150];
	char configtopic[150];
	char controltopic[150];
	char* outTopic;  
	char* controlTopic;
	char* base_topic="homeassistant/sensor/SPS/";
	void saveWifiCallback(){
	  Serial.println("[CALLBACK] saveCallback fired");
	}

	//gets called when WiFiManager enters configuration mode
	void configModeCallback (WiFiManager *myWiFiManager) {
	  Serial.println("[CALLBACK] configModeCallback fired");
	disable_sleep=true;
	}

	void saveParamCallback(){
	  Serial.println("[CALLBACK] saveParamCallback fired");
	   wm.stopConfigPortal();
		delay(100);
		ESP.restart();
	}

	void bindServerCallback(){
	  wm.server->on("/custom",handleRoute);
	  // wm.server->on("/info",handleRoute); // you can override wm!
	}

	void handleRoute(){
	  Serial.println("[HTTP] handle route");
	  wm.server->send(200, "text/plain", "hello from user code");
	}
	void messageReceived(String &topic, String &payload) {
	  Serial.println("incoming: " + topic + " - " + payload);

	  // Note: Do not use the client in the callback to publish, subscribe or
	  // unsubscribe as it may cause deadlocks when other things arrive while
	  // sending and receiving acknowledgments. Instead, change a global variable,
	  // or push to a queue and handle it in the loop after calling `client.loop()`.
	}

	void wifiInfo(){
	  WiFi.printDiag(Serial);
	  Serial.println("SAVED: " + (String)wm.getWiFiIsSaved() ? "YES" : "NO");
	  Serial.println("SSID: " + (String)wm.getWiFiSSID());
	  Serial.println("PASS: " + (String)wm.getWiFiPass());
	}
	void connect() {
	   // Serial.print("\nconnecting...");
	  while (!client.connect("arduino",custom_mqtt_user.getValue() , custom_mqtt_passw.getValue())) {
	  
	   
	  }

	  //Serial.println("\nconnected!");

	  client.subscribe(controlTopic);
	  // client.unsubscribe("/hello");
	}
	void going_to_sleep() {
	  if(disable_sleep!=true){
	  Serial.println("Going to deep sleep now");
	  //Go to sleep now
	  WiFi.disconnect();
	  WiFi.mode(WIFI_OFF);
	  //Wire.end(); // shutdown/power off I2C hardware,
	  // pinMode(21,INPUT); // needed because Wire.end() enables pullups, power Saving
	  //pinMode(22,INPUT);
	  delay(1);
	  digitalWrite(15, LOW);

	  esp_deep_sleep_start();
	}
	}



	void debugchipid(){
	  // WiFi.mode(WIFI_STA);
	  // WiFi.printDiag(Serial);
	  // Serial.println(modes[WiFi.getMode()]);
	  
	  // ESP.eraseConfig();
	  // wm.resetSettings();
	  // wm.erase(true);
	  WiFi.mode(WIFI_AP);
	  // WiFi.softAP();
	  WiFi.enableAP(true);
	  delay(500);
	  // esp_wifi_start();
	  delay(1000);
	  WiFi.printDiag(Serial);
	  delay(60000);
	  ESP.restart();

	 
	}
	
  public:
    void read_variables_json() 
	{
		
		//serializeJson(doc, variables_json);
		//T_NetworkEEPROMVars::SetPinValue(variables_json);
    }
  public:
    inline void SystemStart()
    {
		serializeJson(doc_mqtt, json_mqtt);
		T_MQTTConfigTopics::SetPinValue(json_mqtt);

     

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
  typename T_Device_Class,
  typename T_Icon,
  typename T_Name,
  typename T_Unit_Of_Measurement
  > class ConfigTopic :
	public T_Device_Class,
  public T_Icon,
  public T_Name,
  public T_Unit_Of_Measurement

{
  public:
	_V_PROP_( Device_Class )
    _V_PROP_( Icon )
    _V_PROP_( Name )
	_V_PROP_( Unit_Of_Measurement )
  
   
  public:
 
    String device_class = Device_Class().GetValue();
	String icon = Icon().GetValue();
	String name = Name().GetValue();
	String unit_of_measurement = Unit_Of_Measurement().GetValue();
	
  public:
  inline void SystemInit()
  {
	 StaticJsonDocument<400> doc;
String temp_config="";
String temp="";
if(device_class!=NULL)doc["device_class"] = device_class;
doc["force_update"] = true;
if(unit!=NULL)doc["unit_of_measurement"] = unit;
doc["value_template"] = "{{ value_json."+value_template+" }}";
doc["state_topic"] = String(base_topic)+String(chipId)+"/data/";
doc["name"] = name;
doc["unique_id"] = "sps_"+name+"-"+String(chipId);
if(icon!=NULL)doc["icon"] = icon;
JsonObject device = doc.createNestedObject("device");
device["identifiers"][0] = "sps-"+String(chipId);
device["identifiers"][1] = WiFi.macAddress();
device["name"] = "sps-"+String(chipId);
device["model"] = "SPS-Smart Plant Sensor";
device["manufacturer"] = "JUST3D";
device["suggested_area"] = SUGGESTED_AREA;
device["sw_version"] = SKETCH_VERSION;
serializeJson(doc, temp);
//Serial.println(temp);
 doc.clear();

temp_config = String(base_topic)+String(chipId)+"-"+name+"/config";
 client.publish(temp_config,temp.c_str());
    
	  
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
  > class WifiParameter :
  public T_Length,
  public T_Variable,
  public T_VariableName

{
  public:
	_V_PROP_( Length )
    _V_PROP_( Variable )
    _V_PROP_( VariableName )
  
   
  public:
   
	
  public:
	inline void SystemInit()
	{
		  String value_val=itoa(value, valuestr, 10);
	
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