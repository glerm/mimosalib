/*
 * WiiChuckDemo -- 
 *
 * 2008 Tod E. Kurt, http://thingm.com/
 *
 */

#include <Wire.h>
#include "nunchuck_funcs.h"

#include "WProgram.h"
void setup();
void loop();
int loop_cnt=0;

byte accx,accy, accz, zbut, cbut, joyx, joyy;
int ledPin = 13;


void setup()
{
    Serial.begin(19200);
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
    
}

void loop()
{
    if( loop_cnt > 100 ) { // every 100 msecs get new data
        loop_cnt = 0;

        nunchuck_get_data();

        accx  = nunchuck_accelx(); // ranges from approx 70 - 182
        accy  = nunchuck_accely(); // ranges from approx 65 - 173
        accz  = nunchuck_accelz();
        zbut = nunchuck_zbutton();
        cbut = nunchuck_cbutton();
        joyx = nunchuck_joyx(); 
        joyy = nunchuck_joyy();
            
        Serial.print((byte)accx);
        Serial.print((byte)accy);
        Serial.print((byte)accz);
        Serial.print((byte)zbut);        
        Serial.print((byte)cbut);
        Serial.print((byte)joyx);
        Serial.print((byte)joyy);
    }
    loop_cnt++;
    delay(1);
}


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

