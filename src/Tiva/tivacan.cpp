// OpenLCB Adaptation of FlexCAN library
// copyright DPH 2017
#if 0
//#pragma message("!!! In tivacan.cpp ")
//#if defined(TARGET_IS_BLIZZARD_RB1)
//#if defined(TARGET_IS_TM4C123_RA1) || defined(TARGET_IS_TM4C123_RA3) || defined(TARGET_IS_TM4C123_RB1)
#ifdef ENERGIA_ARCH_TIVAC
#pragma message("!!! compiling tivacan.cpp ")

#include <Energia.h>
#include "Arduino.h"

//#include "OlcbCan.h"
#include "tivacan.h"
//#include "TivaCANv0.h"

class CANClass;
CANClass canbus(0);

// ----------------------------------------------------------------------------
/**
 * \ingroup	can_interface
 * \brief	Initialisierung des CAN Interfaces
 *
 * \param	bitrate	Gewuenschte Geschwindigkeit des CAN Interfaces
 *
 * \return	false falls das CAN Interface nicht initialisiert werden konnte,
 *			true ansonsten.
 */
void OlcbCanClass::init() {
    //Serial.print("\nIn tivacan can_init");
    canbus.begin();
    //return true;
    return;
}

uint8_t OlcbCanClass::avail() {
    //return 0!=tivaCAN.available();
    return 0!=canbus.available();
}

uint8_t OlcbCanClass::read() {
    //Serial.print("\nIn tivacan::read()");
    //CAN_message_t m;
    CAN_message_t m;
    if(!canbus.available()) return 0;
    canbus.read(&m,1);
    if(m.err!=0) return 0;
    this->id = m.id;
    this->flags.extended = m.eff;
    this->flags.rtr = m.rtr;
    this->length = m.dlc;
    for(int i=0;i<m.dlc;i++) this->data[i] = m.buf[i];
    return 1;
}

uint8_t OlcbCanClass::txReady() {
            //Serial.print("\n tivacan::txReady()#A");
    bool b = canbus.tx_idle();
            //Serial.print("\n     canbus->tx_idle()=");
            //Serial.print(b );
    return b;
}

uint8_t OlcbCanClass::write(long timeout) {
    CAN_message_t m;
            //Serial.print("\n     TivaCan::write()#A");
    m.id = this->id;
    m.dlc = this->length;
    m.eff = 1;
    m.rtr=0;
    memcpy(m.buf,this->data,m.dlc);
            //Serial.print(m.id, HEX);
            //Serial.print("]("); Serial.print(m.length);
            //Serial.print(") ");
            //for(int i=0;i<m.length;i++)
            //    { Serial.print(m.data[i],HEX); Serial.print(" "); }
    if(timeout==0 && this->txReady()) {
        active = true;
        return canbus.write(&m,1);
    }
            //Serial.print("\n     TivaCan::write()#C");
    long to = millis() + timeout;
    while(millis()<to) {
        if(this->txReady()) {
            active = true;
            return canbus.write(&m,1);
        }
    }
    return false;
}
uint8_t OlcbCanClass::write() { return this->write(0); }

#endif // blizzard

#endif // 0
