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
    NullControlPin<true>,
    ControlPin<Pins::POWER_SW, false>,                          // POWER ENABLE
    //PitSleep<WAKE_DELAY>,                                     // Sleep Algo <secs>
    PitSleep<30>,                                               // Sleep Algo <secs>
    //DelaySleep<5>,
    //NullOutputFacet
    RadioRfm69<IOT_BUFFSIZE, Pins::RADIO_CS, Pins::RADIO_IRQ, /*ISRFM69HW*/ false>   // output
>
_station;

//----------------------------------------------------------
static void setLed(bool st)
{
    _station.led(st);
}
//----------------------------------------------------------
void setup()
{
    SPI.swap();
    Serial.swap();
    Serial.begin(115200);
    delay(10);
    Serial.write("\fInit\r\n");
    Serial.printf("Net  =%8s %d\r\n", NETNAME, NETWORKID);
    Serial.printf("Node =%8s %d\r\n", IDSTR, NODE_IDENT);

    // just leave it powered up
    pinMode(Pins::POWER_SW, OUTPUT);
    digitalWriteFast(Pins::POWER_SW, 1);

    ClockControl::enableXtal(true,true);

    //Serial.write("Init2\r\n");
    _station.setup();

    _station.sleep
                 .setupSleepPinStates(_sleepStates);

    _station.battery
                 .setup(3.3/1023, 0.1); // scale, deadband VBATT

    //_station.climate;

    //_station.led(true);

    _station.output
                .setup(
                        IOT_ID_FROM_STR(IDSTR),
                        NODE_IDENT,     //  must be unique, defined in nodeids.h
                        FREQUENCY,      // defined in xxxx-network.h
                        NETWORKID,      // defined in xxxx-network.h
                        setLed          // fp to change led state
                        );

    //_station.powerSwitch(true);
    //digitalWriteFast(Pins::POWER_SWITCH, 1);
    //_station.powerControl();

    wdt_enable(WDT_PERIOD_8KCLK_gc); // 8s - not same codes as basic arduino

    Serial.println("Showtime");
    delay(500);

}
//----------------------------------------------------------
void loop()
{
    //Serial.print('*'); 
    _station.loop();
   //_station.dumpTelemetry();
}
//----------------------------------------------------------
