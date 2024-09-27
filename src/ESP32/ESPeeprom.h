#include <EEPROM.h>

class ESPeeprom : public EEPROMClass {

public:
   void update(int a, uint8_t d) {
      if(read(a)==d) return;
      write(a,d);
   }
  
};

