#ifndef __settingsMenu_h__
#define __settingsMenu_h__

#include "ubloxextensions.h"

const int CONNECTIONSMENUID=10;
const int NMEAMENUID=20;
const int NMEAMSGMENUID = NMEAMENUID + 100; //NOTE This goes to 120..132
const int GNSSMENUID=50;
const int ABOUTMENUID=80;
const int USBSETTINGSMENUID = CONNECTIONSMENUID+100;
const int BLUETOOTHSETTINGSMENUID = CONNECTIONSMENUID+200;
  MenuItem *gnssMenuItems[] =
  {
    new MenuItem(GNSSMENUID + 1, "GPS", "Enabled"),
    new MenuItem(GNSSMENUID + 2, "GLONASS", "Enabled"),
    new MenuItem(GNSSMENUID + 3, "Galileo", "Enabled"),
    new MenuItem(GNSSMENUID + 4, "Beidou", "Enabled"),
    new MenuItem(GNSSMENUID + 5, "High Precision Mode"),
    new MenuItem(GNSSMENUID + 6, "Rate", "---")
  };
  MenuItem *aboutMenuItems[] =
  {
    new MenuItem(ABOUTMENUID + 1, "Version", "1.0"),
    new MenuItem(ABOUTMENUID + 2, "GPS Info"),
  };
  MenuItem *USBSettingsMenuItems[] =
  {
    new MenuItem(USBSETTINGSMENUID + 1, "Output", "---"),
  };
  MenuItem *BluetoothSettingsMenuItems[] =
  {
    new MenuItem(BLUETOOTHSETTINGSMENUID + 1, "Output", "---"),
  };
  MenuItem *connectionsMenuItems[] =
  {
    new MenuItem(CONNECTIONSMENUID + 1, "USB", USBSettingsMenuItems, 1),
    new MenuItem(CONNECTIONSMENUID + 2, "Bluetooth", BluetoothSettingsMenuItems, 1),
  };
  MenuItem *enabledNmeaMessagesItems[] = {    
    new MenuItem(NMEAMSGMENUID, "GGA", "--", CFG_MSGOUT_NMEA_ID_GGA_USB),
    new MenuItem(NMEAMSGMENUID, "GLL", "--", CFG_MSGOUT_NMEA_ID_GLL_USB),
    new MenuItem(NMEAMSGMENUID, "GSA", "--", CFG_MSGOUT_NMEA_ID_GSA_USB),
    new MenuItem(NMEAMSGMENUID, "GSV", "--", CFG_MSGOUT_NMEA_ID_GSV_USB),
    new MenuItem(NMEAMSGMENUID, "RMC", "--", CFG_MSGOUT_NMEA_ID_RMC_USB),
    new MenuItem(NMEAMSGMENUID, "VTG", "--", CFG_MSGOUT_NMEA_ID_VTG_USB),
    new MenuItem(NMEAMSGMENUID, "GRS", "--", CFG_MSGOUT_NMEA_ID_GRS_USB),
    new MenuItem(NMEAMSGMENUID, "GST", "--", CFG_MSGOUT_NMEA_ID_GST_USB),
    new MenuItem(NMEAMSGMENUID, "ZDA", "--", CFG_MSGOUT_NMEA_ID_ZDA_USB),
    new MenuItem(NMEAMSGMENUID, "GBS", "--", CFG_MSGOUT_NMEA_ID_GBS_USB),
    new MenuItem(NMEAMSGMENUID, "DTM", "--", CFG_MSGOUT_NMEA_ID_DTM_USB),
    new MenuItem(NMEAMSGMENUID, "GNS", "--", CFG_MSGOUT_NMEA_ID_GNS_USB),
    new MenuItem(NMEAMSGMENUID, "VLW", "--", CFG_MSGOUT_NMEA_ID_VLW_USB),
  };
    MenuItem *nmeaSettingsMenuItems[] =
  {
    new MenuItem(NMEAMENUID + 1, "Version", "---"),
    new MenuItem(NMEAMENUID + 2, "Messages", enabledNmeaMessagesItems, 13)
  };

  MenuItem *mainMenuItems[] =
  {
    new MenuItem(1, "Connections", connectionsMenuItems, 2),
    new MenuItem(2, "NMEA", nmeaSettingsMenuItems, 2),
    new MenuItem(3, "RTCM", "TODO"),
    new MenuItem(4, "SBAS", "---"),
    new MenuItem(5, "GNSS", gnssMenuItems, 6),
    new MenuItem(6, "Reset"),
    new MenuItem(7, "Info/About", aboutMenuItems, 2)
  };
Menu *menu = new Menu(new MenuItem(0, "Settings", mainMenuItems, 7));
MenuItem *gpsInfoMenu[] =
    {
      new MenuItem(0, "Hardware Version"),
      new MenuItem(0, "---"),
      new MenuItem(0, "Software Version"),
      new MenuItem(0, "---"),
      new MenuItem(0, ""),
      new MenuItem(0, ""),
      new MenuItem(0, ""),
      new MenuItem(0, ""),
      new MenuItem(0, ""),
      new MenuItem(0, ""),
      new MenuItem(0, ""),
      new MenuItem(0, ""),
      new MenuItem(0, ""),
    };
int initSettingsMenu(SFE_UBLOX_GPS *gps)
{
  auto sbas = gps->getVal8(CFG_SBAS_USE_DIFFCORR);
  mainMenuItems[3]->setValue(sbas == 0 ? "Disabled" : "Enabled");
  //auto frequency = gps->getNavigationFrequency();
  auto frequency = gps->getVal16(UBLOX_CFG_RATE_MEAS);
  gnssMenuItems[5]->setValue((String(frequency) + " ms").c_str());
  for(uint8_t i = 0; i<13; i++) {
    auto child = enabledNmeaMessagesItems[i];
    child->setValue(gps->getVal8(child->getTag()) > 0 ? "Enabled" : "Disabled");
  }
  //Get NMEA version
  uint8_t nmeaVersion = gps->getVal8(CFG_NMEA_PROTVER);
  if(nmeaVersion == CFG_NMEA_PROTVER_V21)
    nmeaSettingsMenuItems[0]->setValue("v2.1");
  else if(nmeaVersion == CFG_NMEA_PROTVER_V23)
    nmeaSettingsMenuItems[0]->setValue("v2.3");
  else if(nmeaVersion == CFG_NMEA_PROTVER_V40)
    nmeaSettingsMenuItems[0]->setValue("v4.0");
  else if(nmeaVersion == CFG_NMEA_PROTVER_V41)
    nmeaSettingsMenuItems[0]->setValue("v4.1");
  else if(nmeaVersion == CFG_NMEA_PROTVER_V411)
    nmeaSettingsMenuItems[0]->setValue("v4.11");
  else 
    nmeaSettingsMenuItems[0]->setValue("v?.?");
  
  // Get USB Port settings:
  uint8_t payload[] = { COM_PORT_USB };
  sendCommand(gps, UBX_CLASS_CFG, UBX_CFG_PRT, 1, payload);
  //gps->getVal8(
  /*auto outtype = getval8(14);
  if(outtype == COM_TYPE_NMEA)
    USBSettingsMenuItems[0]->setValue("NMEA");
  else if(outtype == COM_TYPE_RTCM3)
    USBSettingsMenuItems[0]->setValue("RTCM");
*/
  int menuCount = 4;
  if(getModuleInfo(gps, 1100))
  {
    gpsInfoMenu[3]->setTitle(minfo.swVersion);
    gpsInfoMenu[1]->setTitle(minfo.hwVersion);
    for (int i = 0; i < minfo.extensionNo; i++)
    {
      gpsInfoMenu[4+i]->setTitle(minfo.extension[i]);
    }
    menuCount+=minfo.extensionNo;
  }
  else
  {
    gpsInfoMenu[3]->setTitle("N/A");
    gpsInfoMenu[1]->setTitle("N/A");
  }
  aboutMenuItems[1]->setChildren(gpsInfoMenu, menuCount);
};
int resetGps(SFE_UBLOX_GPS *gps)
{
  gps->factoryReset();
  gps->setAutoPVT(true, true); //Tell the GPS to "send" each solution
  gps->setAutoHPPOSLLH(true); //Tell the GPS to "send" each high-accuracy solution, accuracy etc
  gps->setSerialRate(115200, COM_PORT_UART2); // Configure speed on bluetooth port
  //gps->setNavigationFrequency(1);
  gps->saveConfiguration(); //Save the current settings to flash and BBR
}
int processMenu(Menu *currentMenu, SFE_UBLOX_GPS *gps)
{
  // Process menu if active
    auto result = currentMenu->processMenu();
    if(result == MENU_RESULT_EXIT)
    {
      return result;
    }
    else if(result == 1) // Connections
    {
      auto item = currentMenu->selectedMenuItem();
      if(item->getValue() == "On")
        item->setValue("Off");
      else
        item->setValue("On");
      currentMenu->refresh();
    }
    else if(result == 2) // Connections
    {
      auto item = currentMenu->selectedMenuItem();
      if(item->getValue() == "Enabled")
        item->setValue("Disabled");
      else
        item->setValue("Enabled");
      currentMenu->refresh();
    }
    else if(result == 3) // Connections
    {
      auto item = currentMenu->selectedMenuItem();
      if(item->getValue() == "Enabled")
        item->setValue("Disabled");
      else
        item->setValue("Enabled");
      currentMenu->refresh();
    }
    else if(result == 4) // SBAS
    {
      //auto item = currentMenu->selectedMenuItem();

      uint8_t sbas = gps->getVal8(CFG_SBAS_USE_DIFFCORR);
      if(sbas == 0)
        sbas = 1;
      else sbas = 0;
      bool isok = gps->setVal8(CFG_SBAS_USE_DIFFCORR, sbas, VAL_LAYER_FLASH + VAL_LAYER_RAM + VAL_LAYER_BBR);
      mainMenuItems[3]->setValue(sbas == 0 ? "Disabled" : "Enabled");
      
      /*if(item->getValue() == "Enabled")
        item->setValue("Disabled");
      else
        item->setValue("Enabled");*/
      currentMenu->refresh();
    }
    else if(result == USBSETTINGSMENUID + 1) // USB Output
    {
      auto item = currentMenu->selectedMenuItem();
      if(item->getValue() == "NMEA") {
        item->setValue("RTCM");
        gps->setUSBOutput(COM_TYPE_RTCM3);
        gps->saveConfiguration(); 
      }
      else {
        item->setValue("NMEA");
        gps->setUSBOutput(COM_TYPE_NMEA);
        gps->saveConfiguration(); 
      }
      currentMenu->refresh();
    }
    else if(result == BLUETOOTHSETTINGSMENUID + 1) // Bluetooth Output
    {
      auto item = currentMenu->selectedMenuItem();
      if(item->getValue() == "NMEA") {
        item->setValue("RTCM");
        gps->setUART2Output(COM_TYPE_RTCM3);
        gps->saveConfiguration(); 
      }
      else {
        item->setValue("NMEA");
        gps->setUART2Output(COM_TYPE_NMEA);
        gps->saveConfiguration(); 
      }
      currentMenu->refresh();
    }
    else if(result == GNSSMENUID + 6) // Navigation rate
    {
       auto item = currentMenu->selectedMenuItem();
       uint16_t value = 1000;
       if(item->getValue() == "1000 ms")   
         value = 500;
       else if(item->getValue() == "500 ms")
         value = 250;         
       else if(item->getValue() == "250 ms")
         value = 100;        
       else if(item->getValue() == "100 ms")
         value = 1000;
       gps->setVal16(UBLOX_CFG_RATE_MEAS, value, VAL_LAYER_FLASH + VAL_LAYER_RAM + VAL_LAYER_BBR);
       item->setValue((String(value) + " ms").c_str());
       currentMenu->refresh();
    }
    else if(result == NMEAMENUID + 1) // NMEA Version
    {
      auto value = nmeaSettingsMenuItems[0]->getValue();
      uint8_t nmeaVersion = gps->getVal8(CFG_NMEA_PROTVER);      
      if(value == "v2.1")
        nmeaVersion = CFG_NMEA_PROTVER_V23;
      else if(value == "v2.3")
        nmeaVersion = CFG_NMEA_PROTVER_V40;
      else if(value == "v4.0")
        nmeaVersion = CFG_NMEA_PROTVER_V41;
      else if(value == "v4.1")
        nmeaVersion = CFG_NMEA_PROTVER_V411;
      else if(value == "v4.11")
        nmeaVersion = CFG_NMEA_PROTVER_V21;
      /*
      
      if(nmeaVersion == (uint8_t)CFG_NMEA_PROTVER_V21)
        nmeaVersion = CFG_NMEA_PROTVER_V23;
      else if(nmeaVersion == (uint8_t)CFG_NMEA_PROTVER_V23)
        nmeaVersion = CFG_NMEA_PROTVER_V40;
      else if(nmeaVersion == (uint8_t)CFG_NMEA_PROTVER_V40)
        nmeaVersion = CFG_NMEA_PROTVER_V41;
      else if(nmeaVersion == (uint8_t)CFG_NMEA_PROTVER_V41)
        nmeaVersion = CFG_NMEA_PROTVER_V411;
      else if(nmeaVersion == (uint8_t)CFG_NMEA_PROTVER_V411)
        nmeaVersion = CFG_NMEA_PROTVER_V21;
      else return -1;*/
      if(gps->setVal(CFG_NMEA_PROTVER, nmeaVersion, VAL_LAYER_FLASH + VAL_LAYER_RAM + VAL_LAYER_BBR))
      {
        if(nmeaVersion == CFG_NMEA_PROTVER_V21)
          nmeaSettingsMenuItems[0]->setValue("v2.1");
        else if(nmeaVersion == CFG_NMEA_PROTVER_V23)
          nmeaSettingsMenuItems[0]->setValue("v2.3");
        else if(nmeaVersion == CFG_NMEA_PROTVER_V40)
          nmeaSettingsMenuItems[0]->setValue("v4.0");
        else if(nmeaVersion == CFG_NMEA_PROTVER_V41)
          nmeaSettingsMenuItems[0]->setValue("v4.1");
        else if(nmeaVersion == CFG_NMEA_PROTVER_V411)
          nmeaSettingsMenuItems[0]->setValue("v4.11");
        currentMenu->refresh();
      }
    }
    else if(result == NMEAMSGMENUID) // NMEA message toggles
    {
      auto item = currentMenu->selectedMenuItem();
      auto title = item->getTitle();
      uint32_t messageid = item->getTag();
      bool enabled =  (item->getValue() == "Enabled");
      //uint8_t id = result - NMEAMSGMENUID;
      bool isok = gps->setVal8(messageid, enabled ? 0 : 1, VAL_LAYER_FLASH + VAL_LAYER_RAM + VAL_LAYER_BBR);
      /*bool isok = false;
      if(enabled)
        isok = gps->disableNMEAMessage(id, COM_PORT_UART2) && gps->disableNMEAMessage(id, COM_PORT_USB);
      else
        isok = gps->enableNMEAMessage(id, COM_PORT_UART2) && gps->enableNMEAMessage(id, COM_PORT_USB);*/
      if(isok)
      {
        if(enabled) item->setValue("Disabled");
        else item->setValue("Enabled");
        currentMenu->refresh();
      }
    }
  
    else if(result == 6) // Reset
    {
      resetGps(gps);
    }
    else if(result > 0) {
      Serial.println("UNKNOWN MENU ID: " + String(result));
    }
  }

#endif