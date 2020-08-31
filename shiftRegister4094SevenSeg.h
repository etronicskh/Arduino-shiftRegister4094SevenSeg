#ifndef shiftRegister4094SevenSeg_h
#define shiftRegister4094SevenSeg_h

#include "Arduino.h"

#define CATHODE 1
#define ANODE 	0

class shiftRegister4094SevenSeg{
public:
	shiftRegister4094SevenSeg(bool type, uint8_t digit_num);
	void begin(uint8_t data_pin, uint8_t clock_pin, uint8_t strobe_pin);
	
	void print(char _ch);
	void print(int _val);
	void print(long _val);
	void print(float _val);
	void print(String _str);

	void print(uint8_t _digit, char _ch);
	void print(uint8_t _from, uint8_t _to, int _val);
	void print(uint8_t _from, uint8_t _to, long _val);
	void print(uint8_t _from, uint8_t _to, float _val);
	void print(uint8_t _from, uint8_t _to, String _str);

	void clear();
	String getArrayString();

private:
	bool 	_seg_type;
	uint8_t _digit_num;
	uint8_t _data_pin;
	uint8_t _clk_pin;
	uint8_t _str_pin;

	char _array_buffer[];
	byte _digit_buffer[];

	void shiftOutData(byte segment_buffer[]);
	void clearBuffer();
	void setBufferArray(uint8_t _from, uint8_t _to, char ch[]);
	void setBufferArray(uint8_t _digit, char _ch);
	void setDigitMapping(char ch[]);
};

#endif
