//#include "rtc.h"
//#include "evsys.h"
//#include "tca0.h"

#include <SPI.h>
#include "defs.h"
#include "Outstation.h"
#include <psiiot.h>
#include <iotcrc.h>
#include <SleepControl.h>
#include <PitSleep.h>
#include <AnalogPin.h>
#include <RFM69.h>         //get it here: https://www.github.com/lowpowerlab/rfm69
//#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RadioRFM69.h>
#include <BME280Unit.h>
#include <psiutil.h>

const SleepStates _sleepStates[] =
{
//   Pin            Normal      Sleep
    {Pins::MOSI,    OUTPUT,     INPUT_PULLUP },
    {Pins::SCK,     OUTPUT,     INPUT_PULLUP },
    {255} // end mark
};

/**
 * The codebase is a monstrous chunk of template 
 * Wizardry. Simpler to maintain than far too many 
 * #ifdefs.
 */
IotStation<
    //NullSensorFacet,
    BME280Unit<Pins::BME280_CS>,
    AnalogPin<Pins::VBATT, telemetry::TelemIndex::BattVolts>,
    //NullAnalog<Pins::VBATT, telemetry::TelemIndex::BattVolts>,
    NullAnalog<0,telemetry::TelemIndex::NOTIMPL>,               // No LDR (params are ignored)
    //ControlPin<Pins::LED, true>,                              // LED (pulldown)
    NullControlPin<0,true>,
    ControlPin<Pins::POWER_SW, false>,                          // POWER ENABLE
    //PitSleep<WAKE_DELAY>,                                     // Sleep Algo <secs>
    PitSleep<20+UNIT>,                                          // Sleep Algo <secs>
    //DelaySleep<5>,
    //NullOutputFacet
    RadioRfm69<IOT_BUFFSIZE, Pins::RADIO_CS, Pins::RADIO_IRQ, /*ISRFM69HW*/ false>   // output
>
_station;

//----------------------------------------------------------
static void commonSetup()
{
#if SWAP_SPI    
    SPI.swap();
#endif
#if SWAP_SERIAL
    Serial.swap();
#endif
    delay(500);
    Serial.begin(115200);
    //XTRACEF("RST=%02x\r\n", RSTCTRL.RSTFR);
    delay(1000);
    //XTRACE("\fInit");
    XTRACE("\f");
}
//----------------------------------------------------------

#if 0
// TEST SHITE
void setup()
{
    commonSetup();
    //_station.powerpin.setup();
    pinModeFast(Pins::POWER_SW, OUTPUT);
}
//----------------------------------------------------------
void loop()
{
    Serial.print('*');
    delay(500);
    //_station.powerpin.setLogical(true);
    digitalWriteFast(Pins::POWER_SW, true);
    Serial.print('1');
    delay(500);
    //_station.powerpin.setLogical(false);
    digitalWriteFast(Pins::POWER_SW, false);
    Serial.print('0');
    delay(500);
}
#else
//----------------------------------------------------------
static void setLed(bool st)
{
    _station.led(st);
}
//----------------------------------------------------------
void setup()
{
    commonSetup();
    //Serial.printf("Net  =%8s %d\r\n", NETNAME, NETWORKID);
    //Serial.printf("Node =%8s %d\r\n", IDSTR, NODE_IDENT);

    _station.setup();

    _station.sleep
                 .setupSleepPinStates(_sleepStates);

    _station.battery
                 .setup(3.3/1023, 0.1); // scale, deadband VBATT

    _station.climate.setTempOffset(TEMP_OFFSET);

    //_station.led(true);

    _station.output
                .setup(
                        IOT_ID_FROM_STR(IDSTR),
                        NODE_IDENT,     //  must be unique, defined in nodeids.h
                        FREQUENCY,      // defined in xxxx-network.h
                        NETWORKID,      // defined in xxxx-network.h
                        setLed          // fp to change led state
                        );

    wdt_enable(WDT_PERIOD_8KCLK_gc); // 8s - not same codes as basic arduino

    //XTRACE("Showtime");
    delay(500);

}
//----------------------------------------------------------
void loop()
{
    //Serial.print('*'); 
    _station.loop();
    //delay(1000);
   //_station.dumpTelemetry();
}
//----------------------------------------------------------
#endif