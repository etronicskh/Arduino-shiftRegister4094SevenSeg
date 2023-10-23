//4094shiftRegisterSevenSeg.cpp

#include "Arduino.h"
#include "shiftRegister4094SevenSeg.h"
//#include "CharacterEncoding.h"
#include <string.h>

#define SEG_LETTER_INDEX 10
#define SEG_BLANK_ARRAY ' '
#define SEG_BLANK_INDEX 36
#define SEG_DASH_INDEX 37
#define SEG_PERIOD_INDEX 38
#define SEG_DEGREE_INDEX 39

#define IDX_NUMBER 48
#define IDX_UPPER_CASE 65
#define IDX_LOWER_CASE 97


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
  	0b010000000, // 46  '.'  PERIOD
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
	print((String) _val);
}

void shiftRegister4094SevenSeg::print(long _val){
	print((String) _val);
}

void shiftRegister4094SevenSeg::print(float _val){
	print((String) _val);
}

void shiftRegister4094SevenSeg::print(float _val, uint8_t _dec){
	String str_buffer;
	String _str = (String) _val;
	for (int i=0; i<(_str.length()-3) ; i++){ str_buffer+=_str[i];}
	if(_dec<=2){
		uint8_t _last_num = (_str.length()-(3-_dec));
		if(_dec==1)  _last_num++;
		for (int i=(_str.length()-3); i<_last_num; i++){ str_buffer+=_str[i];}
	}else{
		for (int i=(_str.length()-3); i<_str.length(); i++){ str_buffer+=_str[i];}
		for(int i=0; i<_dec-2; i++){str_buffer+='0';}
	}
	print(str_buffer);
}

void shiftRegister4094SevenSeg::print(char _ch){
	print((String) _ch);
}

void shiftRegister4094SevenSeg::print(String _str){
	char cha_buf[_str.length()];
	_str.toCharArray(cha_buf, _str.length()+1);
	setDigitMapping(cha_buf);
}

void shiftRegister4094SevenSeg::clear(){
	char _zero_digit[_digit_num];
	for (int i=0; i<_digit_num; i++) {
		_zero_digit[i]=0;			// Off all segment
	}
	shiftOutData(_zero_digit);
}

void shiftRegister4094SevenSeg::allOn(){
	char _on_digit[_digit_num];
	for (int i=0; i<_digit_num; i++) {
		_on_digit[i]=0b11111111;	// On all segment and dot
	}
	shiftOutData(_on_digit);
}

/*-------------------------------------------- Private Function --------------------------------------------*/

void shiftRegister4094SevenSeg::setDigitMapping(char ch[]){
	uint8_t _cha_len = strlen(ch);

	char digit[_digit_num];
	for (size_t k=0; k<_digit_num; k++) { digit[k] = 0;	} // Clear buffer.

	int _start_idx = (_digit_num-_cha_len);
	for (size_t i = 0; i < _cha_len; ++i) { if(ch[i]=='.') _start_idx++;} // '.' is not a digit count.

	// Debugging
	if(_debug){
		Serial.print("array length: "); Serial.println(_cha_len);
		for (int i=0; i<_cha_len; ++i) {
			Serial.println(ch[i]);
		}
	}
	
	int _cha_idx = 0;
	for(size_t _digit=_start_idx; _digit<_digit_num; _digit++){
		char _char = ch[_cha_idx];
		if(_char>='0' && _char<='9'){
			digit[_digit] = digitCodeMap[_char- '0'];
		}else if(_char>='A' && _char<='Z'){
			digit[_digit] = digitCodeMap[_char - 'A' + SEG_LETTER_INDEX];
		}else if(_char>='a' && _char<='z'){
			digit[_digit] = digitCodeMap[_char - 'a' + SEG_LETTER_INDEX];
		}else if(_char == '-'){
			digit[_digit] = digitCodeMap[SEG_DASH_INDEX];
		}else if(_char == '*'){
			digit[_digit] = digitCodeMap[SEG_DEGREE_INDEX];
		}else if(_char == '.'){
			digit[_digit] = digitCodeMap[SEG_PERIOD_INDEX];
		}else{
			digit[_digit] = digitCodeMap[SEG_BLANK_INDEX];
		}

		if(_cha_idx++ >= _cha_len){ break; }	//Over array size.

		//Add '.' to current digit and move to next.
		if (ch[_cha_idx] == '.'){
			digit[_digit] |= digitCodeMap[SEG_PERIOD_INDEX];
			if(_cha_idx++ >= _cha_len){ break; } //Over array size.
		}
	}

	shiftOutData(digit);	// send data to shift-register.
}

void shiftRegister4094SevenSeg::shiftOutData(char segment_buffer[]){
	digitalWrite(_str_pin, 0);
	for(int i=_digit_num-1; i>=0; i--){
		if(_seg_type) shiftOut(_data_pin, _clk_pin, MSBFIRST, segment_buffer[i]);	//Common cathode
		else shiftOut(_data_pin, _clk_pin, MSBFIRST, ~segment_buffer[i]);			//Common anode
	}
	digitalWrite(_str_pin, 1);
}