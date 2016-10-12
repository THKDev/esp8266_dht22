#ifndef __LED_MANAGER__
#define __LED_MANAGER__

class LedManagerImpl
{
   public:
      void init();
      void powerOn();
      void wlanOn();
      void wlanOff();
      void toggleData();

   private:
      static const int GPIO_LED_POWERUP = 4;
      static const int GPIO_WIFI_CONNTECT = 5;
      static const int GPIO_SEND_DATA = 14;
};

extern LedManagerImpl   LedManager;

#endif /*  __LED_MANAGER__ */
