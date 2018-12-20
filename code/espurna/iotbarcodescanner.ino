
#ifdef _PLUGIN_H
/*

Plugin Module description
Plugin Copyright

*/

//*for better readability and usage of this template file the writing style is:
//* (//*) for comments
//* (//) for commented code (just delete the // to activate)
//* This plugin templat allows web interface provisioning the fronand
//* HTML, JS, CSS needs to be coded into the original core (on current design)

//*---------------------------------------------------------------------------
//* Plugin includes
//*---------------------------------------------------------------------------
#include <EEPROM.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <vector>
#include <functional>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
//#include <otherPluginIncludeFiles>

//*----------------------------------------------------------------------------
//* Private
//*----------------------------------------------------------------------------
//*----------------------------------------------------------------------------
//* Plugin globals
//*----------------------------------------------------------------------------
bool _plugin1_enabled = false;
;
unsigned long _pluginReportInterval = 1000 * PLUGIN_REPORT_EVERY;
unsigned long _pluginLastUpdate = 0;
unsigned long _pluginCounter = PLUGIN_PARAMETER_1;

char instanceURL[512];
char token[128];
bool sfdcAuthorized = false;

#include <WiFiClient.h>
//*----------------------------------------------------------------------------
//* Plugin helper functions
//*----------------------------------------------------------------------------

int loginAndGetToken(String user, String password, char *instanceURL, char *token)
{
    StaticJsonBuffer<512> jsonBuffer;
    String loginPayload = "grant_type=password&client_id=3MVG9I5UQ_0k_hTkAPIld6q7UhbaLvBiFjDZ67.bGTeJjH24COcQztfqVPS.Tv5JLjpuiZjvHgGZpPWzSvhGc&client_secret=492835174303365953&username=" + user + "&password=" + password;
    //String sslSalesforceCertificate;

    if (wifiConnected())
    {
        //DEBUG_MSG_P(PSTR("[PLUGIN] WiFi connected...\n"));
        //std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        //DEBUG_MSG_P(PSTR("[PLUGIN] WiFiClientSecure Constructor called...\n"));
        //sslSalesforceCertificate = "0B 33 19 AC 6D 9E C1 5F 08 AB 93 17 2A FE F9 E0 90 69 C7 9A";

        //client->setFingerprint(fingerprint);
        //client->setInsecure();

        HTTPClient http;

        //DEBUG_MSG_P(PSTR("[PLUGIN] [HTTP] begin...\n"));

        //new

        //‎ login.salesforce.com: 0b 33 19 ac 6d 9e c1 5f 08 ab 93 17 2a fe f9 e0 90 69 c7 9a
        //0B 33 19 AC 6D 9E C1 5F 08 AB 93 17 2A FE F9 E0 90 69 C7 9A

        //if (http.begin(*client, "https://login.salesforce.com/services/oauth2/token")) {  // HTTPS
        if (http.begin("https://login.salesforce.com/services/oauth2/token", "0B 33 19 AC 6D 9E C1 5F 08 AB 93 17 2A FE F9 E0 90 69 C7 9A"))
        {

            http.addHeader("Content-Type", "application/x-www-form-urlencoded");

            int httpCode = http.POST(loginPayload);

            //USE_SERIAL.print("[HTTP] GET...\n");
            // start connection and send HTTP header
            //int httpCode = http.GET();

            // httpCode will be negative on error
            if (httpCode > 0)
            {
                // HTTP header has been send and Server response header has been handled
                //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
                //DEBUG_MSG_P(PSTR("[PLUGIN] [HTTP] GET... code: %d\n\n"), httpCode);

                // file found at server
                if (httpCode == HTTP_CODE_OK)
                {
                    String payload = http.getString();
                    //DEBUG_MSG_P(PSTR("[PLUGIN] Payload length: %d\n\n"), payload.length());

                    //DEBUG_MSG_P(PSTR("Payload:\n%s\n"), payload.c_str());

                    JsonObject &root = jsonBuffer.parseObject(payload.c_str());
                    const char *tokenJSON = root["access_token"];
                    const char *instanceURLJSON = root["instance_url"];

                    if (tokenJSON && instanceURLJSON)
                    {
                        //DEBUG_MSG_P(PSTR("[PLUGIN] Token length: %d\n\n"), strlen(tokenJSON));
                        strcpy(token, tokenJSON);
                        strcpy(instanceURL, instanceURLJSON);
                        //DEBUG_MSG_P(PSTR("[PLUGIN] Token 2 length: %d\n\n"), strlen(token));
                    }
                    else
                    {
                        DEBUG_MSG_P(PSTR("Token or instanceURL is null !!!\n"));
                        return -1;
                    }
                    //DEBUG_MSG_P(PSTR("Instance URL:%s\n"), instanceURL);
                }
            }
            else
            {
                //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                DEBUG_MSG_P(PSTR("[PLUGIN] [HTTP] GET... failed, error: %s\n"), http.errorToString(httpCode).c_str());
                return -2;
            }
            http.end();
        }
        else
        {

            DEBUG_MSG_P(PSTR("[PLUGIN] Connection failed\n"));
            return -3;
        }
    }
    else
    {
        DEBUG_MSG_P(PSTR("[HTTP} Unable to connect\n"));
        return -4;
    }

    return 0;
}

//* Plugin loop helper function(s)
//*------------------------------
void _plugin1Function1()
{
int result;
//* Debug messages (anywere in your code)
//*--------------------------------------
#if PLUGIN_REPORT_EVERY != 0
    //DEBUG_MSG_P(PSTR("[PLUGIN] Plugin debug message c format: {%.}\n"), {.});
    if (millis() - _pluginLastUpdate > _pluginReportInterval)
    {
        _pluginLastUpdate = millis();
        DEBUG_MSG_P(PSTR("[PLUGIN] Loop counter: %d\n"), _pluginCounter);
        _pluginCounter++;

        if (wifiConnected())
        {
            DEBUG_MSG_P(PSTR("WiFi Connected!\n"));
            if(!sfdcAuthorized){
                DEBUG_MSG_P(PSTR("SFDC Authorization in progress...\n"));
                result = loginAndGetToken("iot@pwc.com.iot", "Esp8266012019bustHJp4OZBgXH5flNxAwReer", instanceURL, token);
                if(result == 0){
                    sfdcAuthorized = true;
                } else {
                    DEBUG_MSG_P(PSTR("Authorization error: %d\n"),result);
                }
            }
        }
        else
        {
            DEBUG_MSG_P(PSTR("WiFi Not Connected!\n"));
        }

        //wifiDebug(WIFI_STA);
        
        DEBUG_MSG_P(PSTR("Token:\n%s\n---\n"), token);
        DEBUG_MSG_P(PSTR("Instance URL: %s\n---\n"), instanceURL);
        //testHttpClient();
    }
#endif
}

void testHttpClient()
{
    if (wifiConnected())
    {

        //WiFiClient client;

        HTTPClient http;

        DEBUG_MSG_P(PSTR("[HTTP] begin...\n"));
        if (http.begin("http://jigsaw.w3.org/HTTP/connection.html"))
        { // HTTP

            DEBUG_MSG_P(PSTR("[HTTP] GET...\n"));
            // start connection and send HTTP header
            int httpCode = http.GET();

            // httpCode will be negative on error
            if (httpCode > 0)
            {
                // HTTP header has been send and Server response header has been handled
                DEBUG_MSG_P(PSTR("[HTTP] GET... code: %d\n"), httpCode);

                // file found at server
                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
                {
                    String payload = http.getString();
                    DEBUG_MSG_P(PSTR("%s"), payload.c_str());
                }
            }
            else
            {
                DEBUG_MSG_P(PSTR("[HTTP] GET... failed, error: %s\n"), http.errorToString(httpCode).c_str());
            }

            http.end();
        }
        else
        {
            DEBUG_MSG_P(PSTR("[HTTP} Unable to connect\n"));
        }
    }
}

//* If frontend support needed
//* Websocket support helper functions
//*-----------------------------------
//#if WEB_SUPPORT
//* parse ws values
//      unsigned char pluginParsePayload(const char * payload) {
//          char * p = ltrim((char *)payload);
//            if (strcmp(p, "query") == 0) {
//                return 0;
//           }
//      }
//      void _pluginWebSocketUpdate(JsonObject& root) {
//          JsonArray& plugin = root.createNestedArray("pluginStatus");
//          plugin.add("OK");
//      }
//      void _pluginWebSocketOnAction(uint32_t client_id, const char * action, JsonObject& data) {
//* on action register event action code here
//          if (strcmp(action, "plugin") != 0) return;
//          if (data.containsKey("status")) {
//              unsigned char value = pluginParsePayload(data["status"]);
//              if (value == 0) {
//                  wsSend(_pluginWebSocketUpdate);
//              }
//	        }
//      }
//      bool _pluginWebSocketOnReceive(const char * key, JsonVariant& value) {
//* on recieve event code here
//          return (strncmp(key, "plugin", 6) == 0);
//	    }
//        void _pluginWebSocketOnParse(JsonObject& root) {
//* socket parse event code here
//	    }
//      void _pluginWebSocketOnStart(JsonObject& root) {
//* socket start event code here
//        _relayWebSocketUpdate(root);

//* Configuration params
//        JsonArray& config = root.createNestedArray("pluginConfig");
//        JsonObject& line = config.createNestedObject();
//        line["pluginParam1"] = "Param1";
//        line["pluginParam2"] = "Param2";
//        root["pluginRoot"]   = "pluginRoot";
//	    }
//* set websocket handlers
//      void _pluginSetupWS() {
//          wsOnSendRegister(_pluginWebSocketOnStart);
//          wsOnReceiveRegister(_pluginWebSocketOnReceive);
//          wsOnActionRegister(_pluginWebSocketOnAction);
//          wsOnAfterParseRegister(_pluginWebSocketOnParse);
//	    }
//#endif

//* If API support needed
//* API register helper function
//*-----------------------------
#if WEB_SUPPORT
//* register api functions
//* apiRegister(key, _getFunction, _putFunction);
//* this is API registraion to enable disable the plugin
//* use this as template to create additional API calls for the plugin
void _pluginSetupAPI()
{
    char key[15];
    snprintf_P(key, sizeof(key), PSTR("%s"), MQTT_TOPIC_PLUGIN1);
    apiRegister(key, [](char *buffer, size_t len) {
               //snprintf_P(buffer, sizeof(buffer), PSTR("Ok - %d"), _pluginCounter);
               snprintf_P(buffer, len, PSTR("%d"), _plugin1_enabled ? "ON" : "OFF");
               DEBUG_MSG_P(PSTR("[PLUGIN1] API call return: (%s)\n"), buffer); },
                [](const char *payload) {
                    unsigned char value = relayParsePayload(payload);
                    if (value == 0xFF)
                    {
                        DEBUG_MSG_P(PSTR("[PLUGIN1] Wrong payload (%s)\n"), payload);
                        return;
                    }
                    _plugin1_enabled = (value == 1);
                    setSetting("PLG1_EN", _plugin1_enabled);
                    //snprintf_P(buffer, len, PSTR("%d"), _plugin1_enabled ? "ON" : "OFF");
                    //DEBUG_MSG_P(PSTR("[PLUGIN1] API call return: (%s)\n"), buffer);
                });
}
#endif

//* If terminal Terminal commands needed
//* Plugin terminal commands helper function
//*-----------------------------------------
#if TERMINAL_SUPPORT
void _pluginInitCommands()
{
    //* Register Terminal commad to turn on/off the plugin
    //* use this as template to create additional plugin terminal commands
    settingsRegisterCommand(F("PLUGIN1"), [](Embedis *e) {
        if (e->argc == 0)
        {
            DEBUG_MSG_P(PSTR("Pluin1 Status: %s\n"), _plugin1_enabled ? "ON" : "OFF");
            DEBUG_MSG_P(PSTR("Send 0/1 to enable/disable\n"));
            DEBUG_MSG_P(PSTR("+OK\n"));
            return;
        }
        _plugin1_enabled = (String(e->argv[1]).toInt() == 1);
        setSetting("PLG1_EN", _plugin1_enabled);
        DEBUG_MSG_P(PSTR("Pluin1 Set Status: %s\n"), _plugin1_enabled ? "ON" : "OFF");
        DEBUG_MSG_P(PSTR("+OK\n"));
    });
}
#endif

//* end of helper functions
//*----------------------------------------------------------------------------

//*----------------------------------------------------------------------------
//* Plugin setup
//*----------------------------------------------------------------------------
void iotBarcodeScannerSetup()
{
    //*    plugin setup code
    //* myPluginSetup() is called by custom.h - espurna plugin entry point

    _plugin1_enabled = getSetting("PLG1_EN", IOTBARCODESCANNER_ENABLE).toInt() == 1;

    

    //* Register plugin loop to espurna main loop
    //*------------------------------------------
    espurnaRegisterLoop(_pluginLoop);

//* If API used set up Api
//*-----------------------
#if WEB_SUPPORT
    _pluginSetupAPI();
#endif

    //* If Web Sockets set up WS
    //*-------------------------
    //    #if WEB_SUPPORT
    //        _pluginSetupWS();
    //    #endif

    //* If MQTT used set up MQTT
    //*-------------------------
    //    #if MQTT_SUPPORT
    //        _pluginSetupMQTT();
    //    #endif

    //* If trminal used set up terminal
    //*-------------------------------
#if TERMINAL_SUPPORT
    _pluginInitCommands();
#endif

    if(wifiConnected()){
        loginAndGetToken("iot@pwc.com.iot", "Esp8266012019bustHJp4OZBgXH5flNxAwReer", instanceURL, token);
        DEBUG_MSG_P(PSTR("Token:\n%s\n---\n"), token);
        DEBUG_MSG_P(PSTR("Instance URL: %s\n---\n"), instanceURL);
    } else {
        DEBUG_MSG_P(PSTR("!!! WiFi not connected!\n"));
    }
    DEBUG_MSG_P(PSTR("[PLUGIN] Plugin setup code finished \n"));
}
//* end of plugin setup
//*----------------------------------------------------------------------------

//*----------------------------------------------------------------------------
//* PLUGIN loop
//*----------------------------------------------------------------------------
void _pluginLoop()
{
    //*   plugin loop code
    //* _pluginLoop regitered (espurnaRegisterLoop) by myPluginSetup()
    //* myPluginSetup() is called by custom.h - espurna plugin entry point

    //* if plugin disabled dont run the coded
    if (_plugin1_enabled != 1)
        return;

    //* here call all plugin loop functions (copy sumple function as needed)
    _plugin1Function1();
}

//*----------------------------------------------------------------------------
//* Public - Plugin API
//*----------------------------------------------------------------------------
//* Here goes ublic plugin API definitions and coded
//* currently plugin enabled query implemented
//* use this as template to create additionl API calls
//* to Set/Get internal plugin data or use plugin functions
bool plugin1Enabled()
{
    return _plugin1_enabled;
}
#endif //#ifdef _PLUGIN_H
