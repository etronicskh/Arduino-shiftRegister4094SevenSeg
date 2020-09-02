#ifndef shiftRegister4094SevenSeg_h
#define shiftRegister4094SevenSeg_h

#include "Arduino.h"

#define CATHODE 	1
#define ANODE 		0


class shiftRegister4094SevenSeg{
public:
	shiftRegister4094SevenSeg(bool type, uint8_t digit_num);
	void begin(uint8_t data_pin, uint8_t clock_pin, uint8_t strobe_pin);
	
	void clear();
	void print(char _ch);
	void print(int _val);
	void print(long _val);
	void print(float _val);
	void print(float _val, uint8_t _dec);
	void print(String _str);
	void allOn();

private:
	bool _debug = false;

	bool 	_seg_type;
	uint8_t _digit_num;
	uint8_t _data_pin;
	uint8_t _clk_pin;
	uint8_t _str_pin;

	char _digit_buffer[];	// Store binary mapping code

	void setDigitMapping(char ch[]);
	void shiftOutData(char segment_buffer[]);
};

#endif