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
	

namespace AuditorSystems
{

RTC_DATA_ATTR int bootCount = 0;

const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

WiFiManager wm;
// setup some parameters

WiFiClient net;
MQTTClient client;
IPAddress ip;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
WiFiManagerParameter custom_mqtt_user("user", "mqtt user", "", 40);
WiFiManagerParameter custom_mqtt_passw("password", "mqtt password", "", 40);
WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "", 6);
WiFiManagerParameter time_to_sleep("time_to_sleep", "sleep time", "", 4);
const char _customHtml_checkbox[] = "type=\"checkbox\"";
WiFiManagerParameter custom_checkbox("checkbox", "my checkbox", "T", 2, _customHtml_checkbox, WFM_LABEL_AFTER);
//--------------------------
template <  
  typename T_deepsleep,
  typename T_DeviceName,
  typename T_Enabled,
  typename T_Modebutton,
  typename T_MQTTConfigTopics,
  typename T_MQTTStatus,
  typename T_MQTTSubscribedPayload,
  typename T_NetworkDeviceName,
  typename T_NetworkEEPROMVars,
  typename T_NetworkLocalIP,
  typename T_NetworkMAC,
  typename T_NetworkOTAProgress,
  typename T_NetworkRSSI,
  typename T_NetworkStatus

  > class WIFIMQTT :
  public T_deepsleep,
  public T_DeviceName,
  public T_Enabled,
  public T_Modebutton,
  public T_MQTTConfigTopics,
  public T_MQTTStatus,
  public T_MQTTSubscribedPayload,
  public T_NetworkDeviceName,
  public T_NetworkEEPROMVars,
  public T_NetworkLocalIP,
  public T_NetworkMAC,
  public T_NetworkOTAProgress,
  public T_NetworkRSSI,
  public T_NetworkStatus










{
  public:   
	_V_PROP_( deepsleep )
    _V_PROP_( DeviceName )
    _V_PROP_( Enabled )
    _V_PROP_( Modebutton )
 


  public:
	_V_PIN_( NetworkDeviceName )
    _V_PIN_( NetworkEEPROMVars )
    _V_PIN_( NetworkMAC )
    _V_PIN_( NetworkLocalIP )
    _V_PIN_( NetworkOTAProgress )
    _V_PIN_( NetworkRSSI )
    _V_PIN_( NetworkStatus )
	_V_PIN_( MQTTStatus )
	_V_PIN_( MQTTConfigTopics )
	_V_PIN_( MQTTSubscribedPayload )

  public:

	unsigned long mtime = 0;
    // TEST OPTION FLAGS
    bool TEST_CP         = false; // always start the configportal, even if ap found
    int  TESP_CP_TIMEOUT = 90; // test cp timeout

    bool TEST_NET        = false; // do a network test after connect, (gets ntp time)
    bool ALLOWONDEMAND   = true; // enable on demand
    bool disable_sleep;
    int  ONDDEMANDPIN    = Modebutton(); // gpio for button
    StaticJsonDocument<1024> doc_mqtt_subpayload;
	StaticJsonDocument<1024> doc_mqtt_config;
    String deviceName = DeviceName().GetValue();
    String json_mqtt_config = "";
	String json_mqtt_subpayload = "";
    String wifi_config_json = "";
    String time_stamp;
    uint32_t chipId = 0;
	byte status=0;
    char topic[150];
    char configtopic[150];
    char controltopic[150];
    char* outTopic;
    char* controlTopic;
    char* base_topic = "homeassistant/sensor/SPS/";
	int printEntry =0;
    void saveWifiCallback() {
      Serial.println("[CALLBACK] saveCallback fired");
    }

    //gets called when WiFiManager enters configuration mode
    void configModeCallback (WiFiManager *myWiFiManager) {
      Serial.println("[CALLBACK] configModeCallback fired");
      disable_sleep = true;
    }

    void saveParamCallback() {
      Serial.println("[CALLBACK] saveParamCallback fired");
      wm.stopConfigPortal();
      delay(100);
      ESP.restart();
    }

    void bindServerCallback() {
      wm.server->on("/custom", handleRoute);
      // wm.server->on("/info",handleRoute); // you can override wm!
    }

    void handleRoute() {
      Serial.println("[HTTP] handle route");
      wm.server->send(200, "text/plain", "hello from user code");
    }
    void messageReceived(String &topic, String &payload) {
      Serial.println("incoming: " + topic + " - " + payload);
		//doc_mqtt_subpayload[topic] = payload;
		  serializeJson(doc_mqtt_subpayload, json_mqtt_subpayload);
		   serializeJson(doc_mqtt_subpayload, Serial);
      //T_MQTTConfigTopics::SetPinValue(json_mqtt_subpayload);
    
    }

    
    void connect() {
      // Serial.print("\nconnecting...");
      while (!client.connect("arduino", custom_mqtt_user.getValue() , custom_mqtt_passw.getValue())) {


      }

      //Serial.println("\nconnected!");

      client.subscribe(controlTopic);
      
    }
    void going_to_sleep() {
      if (disable_sleep != true) {
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



    void debugchipid() {
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
    inline void SystemStart()
    {
     
	  wm.addParameter(&custom_mqtt_server);
	  wm.addParameter(&custom_mqtt_user);
	  wm.addParameter(&custom_mqtt_passw);
	  wm.addParameter(&custom_mqtt_port);
      wm.addParameter(&time_to_sleep);
      wm.addParameter(&custom_checkbox);
 for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  deviceName += String(chipId);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  ++bootCount;
	Serial.println("Boot number: " + String(bootCount));
wm.setDebugOutput(false);
   // callbacks
  wm.setAPCallback(configModeCallback);
  wm.setWebServerCallback(bindServerCallback);
  wm.setSaveConfigCallback(saveWifiCallback);
  wm.setSaveParamsCallback(saveParamCallback);

  // add all your parameters here
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_user);
  wm.addParameter(&custom_mqtt_passw);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&time_to_sleep);
  wm.addParameter(&custom_checkbox);

  // set values later if you want
  custom_mqtt_server.setValue("broker.hivemq.com",17);
  custom_mqtt_user.setValue("kiwi85",6);
  custom_mqtt_passw.setValue("#Mauritius2018",14);
  custom_mqtt_port.setValue("1883",4);
  char buf[12];
  //time_to_sleep.setValue(itoa(deepsleep(),buf,10),4);
  std::vector<const char *> menu = {"wifi","wifinoscan","info","param","close","sep","erase","update","restart","exit"};
  wm.setMenu(menu); // custom menu, pass vector
  wm.setHostname(deviceName.c_str());
  wm.setConfigPortalTimeout(30);
  wm.setConnectTimeout(20);
  wm.setCleanConnect(true); // disconnect before connect, clean connect
  wm.setBreakAfterConfig(true); // needed to use saveWifiCallback
  if(!wm.autoConnect(deviceName.c_str(),"")) {
    Serial.println("failed to connect and hit timeout, restarting in 3s");
    delay(3000);
    ESP.restart();
  }
  else if(TEST_CP) {
    // start configportal always
    delay(1000);
    Serial.println("TEST_CP ENABLED");
    wm.setConfigPortalTimeout(TESP_CP_TIMEOUT);
    wm.startConfigPortal(deviceName.c_str());
  }
  else {
    //if you get here you have connected to the WiFi
    // Serial.println("connected...yeey :)");
  }
  
  //wifiInfo();
  pinMode(ONDDEMANDPIN, INPUT_PULLUP);

  #ifdef USEOTA
    ArduinoOTA.begin();
  #endif
  
  outTopic    = strcat(topic,base_topic);   
  outTopic    = strcat(topic,String(chipId).c_str());
  outTopic    = strcat(topic,"/data/");
  controlTopic  = strcat(controltopic,base_topic);
  controlTopic  = strcat(controltopic,"/");
  controlTopic  = strcat(controltopic,String(chipId).c_str());
  controlTopic  = strcat(controltopic,"/control");
  client.begin(custom_mqtt_server.getValue(), net);

   client.onMessage(messageReceived);

  connect();
 timeClient.begin();
// Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
timeClient.setTimeOffset(3600*2);
 ip = WiFi.localIP();
esp_sleep_enable_timer_wakeup(atoi(time_to_sleep.getValue()) * uS_TO_S_FACTOR);
esp_sleep_enable_ext0_wakeup(GPIO_NUM_26, 0); //1 = High, 0 = Low
//Print the wakeup reason for ESP32 and touchpad too
esp_sleep_enable_ext1_wakeup(0x100000000,ESP_EXT1_WAKEUP_ANY_HIGH);
//esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 1); //1 = High, 0 = Low
 serializeJson(doc_mqtt_config, json_mqtt_config);
    
      //T_NetworkEEPROMVars::SetPinValue(wifi_config_json);
	 
    }

    inline void SystemLoopBegin()
    {
     
		client.loop();
		timeClient.update();
		if (!client.connected()) {
		connect();
		}
 
  
  #ifdef USEOTA
  ArduinoOTA.handle();
  #endif
  // is configuration portal requested?
  if (ALLOWONDEMAND && digitalRead(ONDDEMANDPIN) == LOW ) {
    delay(10);
    if ( digitalRead(ONDDEMANDPIN) == LOW ){
      Serial.println("BUTTON PRESSED");
      wm.setConfigPortalTimeout(140);
      wm.setParamsPage(false); // move params to seperate page, not wifi, do not combine with setmenu!

      // disable captive portal redirection
      // wm.setCaptivePortalEnable(false);
      
      if (!wm.startConfigPortal(deviceName.c_str(),"")) {
        Serial.println("failed to connect and hit timeout, restarting in 3s");
        delay(3000);
	ESP.restart();
      }
    }
    else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
      //getTime();
    }
  }

  
  // put your main code here, to run repeatedly:
time_stamp=timeClient.getFormattedDate();
 if (millis() - printEntry > 3000) {
        status = random(0, 100);
        Mitov::String ip = WiFi.localIP().toString();
        Mitov::String mac = WiFi.macAddress();
        T_NetworkLocalIP::SetPinValue(ip);
        T_NetworkDeviceName::SetPinValue(deviceName);

        T_NetworkMAC::SetPinValue(mac);
        T_NetworkRSSI::SetPinValue(WiFi.RSSI());
        
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
  typename T_Unit_Of_Measurement,
  typename T_ValueJson
  
  > class ConfigTopic :
  public T_Device_Class,
  public T_Icon,
  public T_Name,
  public T_Unit_Of_Measurement,
  public T_ValueJson

{
  public:
    _V_PROP_( Device_Class )
    _V_PROP_( Icon )
    _V_PROP_( Name )
    _V_PROP_( Unit_Of_Measurement )
	_V_PROP_( ValueJson )


  public:

    String device_class = Device_Class();
    String icon = Icon();
    String name = Name();
    String unit_of_measurement = Unit_Of_Measurement();
	String value_template = ValueJson();

  public:
    inline void SystemInit()
    {


    }

    inline void SystemStart()
    {
      StaticJsonDocument<400> doc;
      String temp_config = "";
      String temp = "";
      if (device_class != NULL)doc["device_class"] = device_class;
      doc["force_update"] = true;
      if (unit_of_measurement != NULL)doc["unit_of_measurement"] = unit_of_measurement;
      doc["value_template"] = "{{ value_json." + value_template + " }}";
      doc["state_topic"] = String(C_OWNER.base_topic) + String(C_OWNER.chipId) + "/data/";
      doc["name"] = name;
      doc["unique_id"] = "sps_" + name + "-" + String(C_OWNER.chipId);
      if (icon != NULL)doc["icon"] = icon;
      JsonObject device = doc.createNestedObject("device");
      device["identifiers"][0] = "sps-" + String(C_OWNER.chipId);
      device["identifiers"][1] = WiFi.macAddress();
      device["name"] = "sps-" + String(C_OWNER.chipId);
      device["model"] = "SPS-Smart Plant Sensor";
      device["manufacturer"] = "JUST3D";
      device["suggested_area"] = SUGGESTED_AREA;
      device["sw_version"] = SKETCH_VERSION;
      serializeJson(doc, temp);
      //Serial.println(temp);
      doc.clear();

      temp_config = String(C_OWNER.base_topic) + String(C_OWNER.chipId) + "-" + name + "/config";
      client.publish(temp_config, temp.c_str());




    }
    inline void SystemLoopBegin()
    {
    }


};

//---------------------------

template <
  typename T_OWNER, T_OWNER &C_OWNER,
  typename T_Subscribe_topic,
  typename T_TopicName
  

  > class SubscriptionTopic :
  public T_Subscribe_topic,
  public T_TopicName


{
  public:
    _V_PROP_( Subscribe_topic )
    _V_PROP_( TopicName )

  
  public:
    inline void SystemInit()
    {
		
    
    }
    inline void SystemStart()
    {
     
     

     
	client.subscribe(Subscribe_topic());

    }
    inline void SystemLoopBegin()
    {
		
    }


};
}
#endif