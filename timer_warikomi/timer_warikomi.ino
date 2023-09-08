#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


volatile unsigned int count; //ISR-main間の共有変数

  // 0b00000000 = (DP)GFEDCBA の順になるように配線したと想定する。
unsigned char seg[10] = {0b00111111,0b00000110,
                         0b01011011,0b01001111,
                         0b01100110,0b01101101,
                         0b01111101,0b00100111,
                         0b01111111,0b01101111};


ISR ( TIMER0_COMPA_vect ){
  static unsigned int num, count,digit;
  display(num,seg,digit);
  count++;
  digit++;
  if (count == 100) {
    num++;
    count = 0;
  }
  if (digit == 4){
    digit = 0;
  }
}

void display(unsigned int a,unsigned char* pseg,char digit){
  switch(digit){
    case 0:
      PORTB = 0b00000001;
      PORTD = pseg[(a / 1000)]; //1000のくらいの表示  
      break;
    case 1:
      PORTB = 0b00000010;
      PORTD = pseg[(a % 1000) / 100]; // 100のくらいの表示
      break;
    case 2:
      PORTB = 0b00000100;
      PORTD = pseg[(a % 100) / 10];
      break;
    case 3:
      PORTB = 0b00001000;
      PORTD = pseg[(a % 10)];
      break;
  }
}



int main(void) {

  DDRB = 0xFF;
  DDRD = 0xFF;
  PORTD = 0x00;

  //timer関係の変数宣言
  TCCR0A = 0b00000010;
  TCCR0B = 0b00000101;
  TIMSK0 = 0b00000010;

  OCR0A = 50; //TCCR0Bの下三桁に依存するが、大体１ms?

  sei();

  while (true) {
  }

  return 0;
}