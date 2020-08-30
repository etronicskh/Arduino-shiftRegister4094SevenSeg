#ifndef shiftRegister4094SevenSeg_h
#define shiftRegister4094SevenSeg_h

#include "Arduino.h"

#define CATHODE 1
#define ANODE 	0

class shiftRegister4094SevenSeg{
public:
	shiftRegister4094SevenSeg(uint8_t data_pin, uint8_t clock_pin, uint8_t strobe_pin);
	void begin(bool type, uint8_t digit);

private:
	bool 	_seg_type;
	uint8_t _digit_num;
	uint8_t _data_pin;
	uint8_t _clk_pin;
	uint8_t _str_pin;

	char _main_buffer[];

	void shiftOutData(int segment_buffer[]);
	void clearBuffer();
};

#endif
