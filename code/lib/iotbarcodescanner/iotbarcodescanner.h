#ifdef INCLUDE_IOTBARCODESCANNER
#ifndef _PLUGIN_H
#define _PLUGIN_H


//*---------------------------------------------------------------------------
//* Espurna overrides
//* -------------------------------------------------------------------------
//*  Here put espurna plugin specific overrides
#define RELAY_PROVIDER          RELAY_PROVIDER_HW655
#define SERIAL_BAUDRATE         9600 

//---------------------------------------------------------------------------
//* Plugin defines
//----------------------------------------------------------------------------
//* plese refer to plugin1.ino for more help and inline documentaion
//* Plugin enabled flag (enabling or disabling execution)
#define IOTBARCODESCANNER_ENABLE             1
//* sample plugin MTQQ topic
#define MQTT_TOPIC_PLUGIN1          "PLUGIN1"
//* Sample plugin reporting interval (0 no reporting)
#define PLUGIN_REPORT_EVERY        30
//* Sample plugin parameter values
#define PLUGIN_PARAMETER_1         0

//------------------------------------------------------------
//* Plugin public interface
//------------------------------------------------------------
//* declare the plugin setup function (used by custom.h)
void iotBarcodeScannerSetup();
//* get plugin enabled state
bool plugin1Enabled();

#endif
#endif
