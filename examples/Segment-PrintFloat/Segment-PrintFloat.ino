#include <shiftRegister4094SevenSeg.h>

#define clk_pin 2
#define data_pin 4
#define str_pin 3
#define seg_num 4

float fl = 34.52;

shiftRegister4094SevenSeg disp(CATHODE, seg_num); // CATHODE or ANODE, Number of segment.

void setup() {
  Serial.begin(9600);
  disp.begin(data_pin, clk_pin, str_pin); // DATA_PIN, CLOCK_PIN, STROBE_PIN
  
  Serial.println("Started..");
}

void loop() {
  disp.print(fl);  //print 34.52 (defualt)
  delay(1000);
  
  disp.print(fl, 0);  //print 34
  delay(1000);

  disp.print(fl, 1);  //print 34.5
  delay(1000);

  disp.print(fl, 4);  //print 34.5000
  delay(1000);

  disp.clear(); // turn all digit off
  delay(1000);
}
