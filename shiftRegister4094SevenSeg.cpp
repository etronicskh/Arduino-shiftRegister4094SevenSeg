//4094shiftRegisterSevenSeg.cpp

#include "Arduino.h"
#include "shiftRegister4094SevenSeg.h"
#include "CharacterEncoding.h"

const byte SegmentChar[] = {
	0b00111111,
  	0b00000110,
  	0b01011011,
  	0b01001111,
  	0b01100110,
  	0b01101101,
  	0b01111101,
	0b00000111,
  	0b01111111,
  	0b01101111,
	0b00000000
};

shiftRegister4094SevenSeg::shiftRegister4094SevenSeg(uint8_t data_pin, uint8_t clock_pin, uint8_t strobe_pin):_data_pin(data_pin), _clk_pin(clock_pin), _str_pin(strobe_pin){
  // Default Constructor of EmSevenSegment class
}

void shiftRegister4094SevenSeg::begin(bool type, uint8_t digit){
	_seg_type 	= type;
	_digit_num 	= digit;
	pinMode(_data_pin, OUTPUT);
	pinMode(_clk_pin, OUTPUT);
	pinMode(_str_pin, OUTPUT);
}

void shiftRegister4094SevenSeg::shiftOutData(int segment_buffer[]){
	/* - shift data to shift register.
	 * - digit count from left to right.
	 * - write last digit first.
	 */
	digitalWrite(_str_pin, 0);
	for(int i=_digit_num-1; i>=0; i--){
		shiftOut(_data_pin, _clk_pin, MSBFIRST, SegmentChar[segment_buffer[i]]);
	}
	digitalWrite(_str_pin, 1);
}

void shiftRegister4094SevenSeg::clearBuffer(){
	for(int i=0; i<_digit_num; i++){
		_main_buffer[i] = 0; // set each element to 0.
	}
}