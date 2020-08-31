//4094shiftRegisterSevenSeg.cpp

#include "Arduino.h"
#include "shiftRegister4094SevenSeg.h"
#include "CharacterEncoding.h"

#define SEG_LETTER_INDEX 10
#define SEG_BLANK_ARRAY ' '
#define SEG_BLANK_INDEX 36
#define SEG_DASH_INDEX 37
#define SEG_PERIOD_INDEX 38
#define SEG_DEGREE_INDEX 39


const byte digitCodeMap[] = {
	// 0bGFEDCBA
	0b00111111, // 0   "0"          AAAA
  	0b00000110, // 1   "1"         F    B
  	0b01011011, // 2   "2"         F    B
  	0b01001111, // 3   "3"          GGGG
  	0b01100110, // 4   "4"         E    C
  	0b01101101, // 5   "5"         E    C
  	0b01111101, // 6   "6"          DDDD
  	0b00000111, // 7   "7"
  	0b01111111, // 8   "8"
  	0b01101111, // 9   "9"
  	0b01110111, // 65  'A'
  	0b01111100, // 66  'b'
  	0b00111001, // 67  'C'
  	0b01011110, // 68  'd'
  	0b01111001, // 69  'E'
  	0b01110001, // 70  'F'
  	0b00111101, // 71  'G'
  	0b01110110, // 72  'H'
  	0b00000110, // 73  'I'
  	0b00001110, // 74  'J'
  	0b01110110, // 75  'K'  Same as 'H'
  	0b00111000, // 76  'L'
  	0b00000000, // 77  'M'  NO DISPLAY
  	0b01010100, // 78  'n'
  	0b00111111, // 79  'O'
  	0b01110011, // 80  'P'
  	0b01100111, // 81  'q'
  	0b01010000, // 82  'r'
  	0b01101101, // 83  'S'
  	0b01111000, // 84  't'
  	0b00111110, // 85  'U'
  	0b00111110, // 86  'V'  Same as 'U'
  	0b00000000, // 87  'W'  NO DISPLAY
  	0b01110110, // 88  'X'  Same as 'H'
  	0b01101110, // 89  'y'
  	0b01011011, // 90  'Z'  Same as '2'
  	0b00000000, // 32  ' '  BLANK
  	0b01000000, // 45  '-'  DASH
  	0b10000000, // 46  '.'  PERIOD
  	0b01100011, // 42  '*'  DEGREE ..
};

shiftRegister4094SevenSeg::shiftRegister4094SevenSeg(bool type, uint8_t digit_num):_seg_type(type), _digit_num(digit_num){
  // Default Constructor of EmSevenSegment class
}

void shiftRegister4094SevenSeg::begin(uint8_t data_pin, uint8_t clock_pin, uint8_t strobe_pin){
	_data_pin = data_pin;
	_clk_pin  = clock_pin;
	_str_pin  = strobe_pin;
	pinMode(_data_pin, OUTPUT);
	pinMode(_clk_pin, OUTPUT);
	pinMode(_str_pin, OUTPUT);
	clear();
}

void shiftRegister4094SevenSeg::print(int _val){
	String _str = (String) _val;
	print(_str);
}

void shiftRegister4094SevenSeg::print(long _val){
	String _str = (String) _val;
	print(_str);
}

void shiftRegister4094SevenSeg::print(float _val){
	String _str = (String) _val;
	print(_str);
}

void shiftRegister4094SevenSeg::print(char _ch){
	clearBuffer();
	setBufferArray(_digit_num-1, _ch);
}

void shiftRegister4094SevenSeg::print(String _str){
	clearBuffer();
	char cha_buf[_str.length()];
	_str.toCharArray(cha_buf, _str.length());
	setBufferArray(_digit_num-_str.length(), _digit_num-1, cha_buf);
}

void shiftRegister4094SevenSeg::print(uint8_t _from, uint8_t _to, int _val){
	String _str = (String) _val;
	print(_from, _to, _str);
}

void shiftRegister4094SevenSeg::print(uint8_t _from, uint8_t _to, long _val){
	String _str = (String) _val;
	print(_from, _to, _str);
}

void shiftRegister4094SevenSeg::print(uint8_t _from, uint8_t _to, float _val){
	String _str = (String) _val;
	print(_from, _to, _str);
}

void shiftRegister4094SevenSeg::print(uint8_t _digit, char _ch){
	setBufferArray(_digit, _ch);
}

void shiftRegister4094SevenSeg::print(uint8_t _from, uint8_t _to, String _str){
	char cha_buf[_str.length()];
	_str.toCharArray(cha_buf, _str.length());
	setBufferArray(_from, _to, cha_buf);
}

void shiftRegister4094SevenSeg::clear(){
	clearBuffer();
	shiftOutData(_digit_buffer);
}

String shiftRegister4094SevenSeg::getArrayString(){
	String _str;
	for(size_t i=0; i<_digit_num; i++){
		_str+=_array_buffer[i];
	}
	return _str;
}

/*-------------------------------------------- Private Function --------------------------------------------*/

void shiftRegister4094SevenSeg::shiftOutData(byte segment_buffer[]){
	/* - shift data to shift register.
	 * - digit count from left to right.
	 * - write last digit first.
	 */
	digitalWrite(_str_pin, 0);
	for(size_t i=_digit_num-1; i>=0; i--){
		if(_seg_type) shiftOut(_data_pin, _clk_pin, MSBFIRST, segment_buffer[i]);	//Common cathode
		else shiftOut(_data_pin, _clk_pin, MSBFIRST, ~segment_buffer[i]);			//Common anode
	}
	digitalWrite(_str_pin, 1);
}

void shiftRegister4094SevenSeg::clearBuffer(){
	for(size_t i=0; i<_digit_num; i++){
		_array_buffer[i] = SEG_BLANK_ARRAY; // set each element to 0.
		_digit_buffer[i] = digitCodeMap[SEG_BLANK_INDEX];
	}
}

void shiftRegister4094SevenSeg::setBufferArray(uint8_t _from, uint8_t _to, char ch[]){
	int _ch_last_index = sizeof(ch) - 2;
	for(size_t i=_to; i<=_from; i--){
		if (_ch_last_index >= 0) {
			_array_buffer[i] = ch[_ch_last_index];
			_ch_last_index--;
		}else{
			_array_buffer[i] = SEG_BLANK_ARRAY;
		}
	}
	shiftOutData(_digit_buffer);
}

void shiftRegister4094SevenSeg::setBufferArray(uint8_t _digit, char _ch){
	_array_buffer[_digit] = _ch;
	shiftOutData(_digit_buffer);
}

void shiftRegister4094SevenSeg::setDigitMapping(char ch[]){
	uint8_t _cha_index = 0;
	byte _seg = digitCodeMap[SEG_BLANK_INDEX];
	for(size_t _digit_index=0; _digit_index<_digit_num; _digit_index++){
		const char _char = ch[_cha_index];
		if(_char>='0' && _char<='9'){
			_seg = digitCodeMap[_char-'A'];
		}else if(_char>='A' && _char<='Z'){
			_seg = digitCodeMap[_char - 'A' + SEG_LETTER_INDEX];
		}else if(_char>='a' && _char<='z'){
			_seg = digitCodeMap[_char - 'a' + SEG_LETTER_INDEX];
		}else if(_char == '-'){
			_seg = digitCodeMap[SEG_DASH_INDEX];
		}else if(_char == '*'){
			_seg = digitCodeMap[SEG_DEGREE_INDEX];
		}else if(_char == '.'){
			_seg = digitCodeMap[SEG_PERIOD_INDEX];
		}else{
			_seg = digitCodeMap[SEG_BLANK_INDEX];
		}
		_digit_buffer[_digit_index] = _seg; //
		_cha_index++;

		//Add '.' to current digit and move to next.
		if (_char == '.'){
			_digit_buffer[_digit_index] |= digitCodeMap[SEG_PERIOD_INDEX];
			_cha_index++;
		}
	}
}