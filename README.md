# Shift-Register 4094 Seven Segment Arduino Library
Use 3 pins of Arduino to control multiple 7-segment display with 4094 8-bit shift register.
 It has been tested on `HEF4094B` `CD4094B` `CD4094BM` and `MC74HCT4094A`.

## Features
- Display upto 255 seven-segment digits
- Display number (Negative, Float)
- Display alphabets (char array)
- Common anode and cathode displays
- Clear the display

## Install Libraray
1. Download the library as ZIP file
2. Arduino IDE > Sketch > Include Library > Add .ZIP Library > Select "Arduino-shiftRegister4094SevenSeg.ZIP"
3. Arduino IDE > File > Examples > Arduino-shiftRegister4094SevenSeg > SimpleDisplay

## Circuit
- **Data pin**: D pin on shift register.
- **Clock pin**: CLK or CP pin on shift register.
- **Strobe pin**: STR on pin shift register.

![input](https://github.com/eTRONICSKH/Arduino-shiftRegister4094SevenSeg/blob/master/circuit.png)

## Reference
### Constructor
- **shiftRegister4094SevenSeg()**
  ```c++
  shiftRegister4094SevenSeg newObject(bool type, uint8_t digit_num);
  ```
  Create shiftRegister4094SevenSeg object (can be more than one), segment type and number of 7-segment digit.
   - **`type`** : segment type, can be `CATHODE` (true) or `ANODE` (false).
   - **`digit_num`** : number of digit, can be between 0 to 255.
   
  Example: 
    ```c++
    shiftRegister4094SevenSeg myDisplay(CATHODE, 4); // Common Cathode with 4 digit display
    ```
### Initializes
- **begin()**
  ```c++
  begin(uint8_t data_pin, uint8_t clock_pin, uint8_t strobe_pin);
  ```
  Initializes the library with communication pins which are connected to Arduino in `setup()`.
  
  Example:
  ```c++
  void setup(){
    myDisplay.begin(2, 3, 4);
  }
  ```
### Functions
- **print()**

  Using `print()` function to print the data to the digits. Print funstion always starts from the `left digit` and `turn all off` when the data is too long to fit the digits.
  - Print one character (A-Z)
  ```c++
  myDisplay.print(char cha); 
  ```
  - Print integer value (positive or negative)
  ```c++
  myDisplay.print(int value);
  ```
  - Print long value (positive or negative)
  ```c++
  myDisplay.print(long value); 
  ```
  - Print float value (Default). Ex 2.34
  ```c++
  myDisplay.print(float value); 
  ```
  - Print float with specific number of decimals. Ex. 234.7
  ```c++
  myDisplay.print(float value, int dec); 
  ```
  - Print string array. Ex. "A45U"
  ```c++
  myDisplay.print(char str[]);
  ```
- **clear()**

  Using `clear()` function to turn off all digit (everything).
  ```c++
  myDisplay.clear();
  ```
- **allOn()**

  Using `allOn()` function to turn on all digit include the dot.
  ```c++
  myDisplay.allOn();
  ```
## TODO
- Alignment, left or right.
- Print at specific digit.
- Clear specific digit, could be multiple digit. Ex. clear(from, to).
- Blink specific digit, could be multiple digit. Ex. blink(from, to).
- Testing to see issues.

## Contacts

Email:  theara729@gmail.com

Web:    [www.etronicskh.com](https://www.etronicskh.com).