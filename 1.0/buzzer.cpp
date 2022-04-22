#include "buzzer.h"
#include "pins.h"
#include "arduino.h"
#include <avr/pgmspace.h>
/*
// 爱拼才会赢
#define NTC0 -1
#define NTC1 262
#define NTC2 294
#define NTC3 330
#define NTC4 350
#define NTC5 393
#define NTC6 441
#define NTC7 495

#define NTCL1 131
#define NTCL2 147
#define NTCL3 165
#define NTCL4 175
#define NTCL5 196
#define NTCL6 221
#define NTCL7 248

#define NTCH1 525
#define NTCH2 589
#define NTCH3 661
#define NTCH4 700
#define NTCH5 786
#define NTCH6 882
#define NTCH7 990

// 葫芦娃


//列出全部C调的频率
#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625
//列出所有节拍
const int tune_apchy[] PROGMEM =               //根据简谱列出各频率
{
  NTC5, NTC5, NTC6,
  NTCH1, NTC6, NTC5, NTC6, NTCH1, NTC6, NTC5,
  NTC3, NTC3, NTC3, NTC5,
  NTC6, NTC6, NTC5, NTCH3, NTCH3, NTCH2, NTCH1,
  NTCH2, NTCH1, NTCH2,
  NTCH3, NTCH3, NTCH2, NTCH3, NTCH2, NTCH1, NTCH2, NTCH1, NTC6,

  NTCH2, NTCH2, NTCH2, NTCH1, NTC6, NTC5,
  NTC6, NTC5, NTC5, NTCH1, NTC6, NTC5, NTC1, NTC3,
  NTC2, NTC1, NTC2,
  NTC3, NTC5, NTC5, NTC3, NTCH1, NTC7,
  NTC6, NTC5, NTC6, NTCH1, NTCH2, NTCH3,

  NTCH3, NTCH2, NTCH1, NTC5, NTCH1, NTCH2, NTCH3,

  NTCH2, NTC0, NTCH3, NTCH2,
  NTCH1, NTC0, NTCH2, NTCH1, NTC6, NTC0,

  NTCH2, NTC6, NTCH1, NTCH1, NTCH1, NTC6, NTC5, NTC3,
  NTC5,
  NTC5, NTC6, NTCH1, NTC7, NTC6,
  NTCH3, NTCH3, NTCH3, NTCH3, NTCH2, NTCH2, NTCH1,
  NTC6, NTCH3, NTCH2, NTCH1, NTCH2, NTCH1, NTC6,
  NTCH1,
};
const float duration_apchy[] PROGMEM =                 //根据简谱列出各节拍
{
  0.5, 0.25, 0.25,
  1.5, 0.5, 0.5, 0.25, 0.25, 0.5, 0.5,
  1 + 1 + 1, 0.5, 0.25, 0.25,
  1.5, 0.5, 0.5, 0.5, 0.25, 0.25, 0.5,
  1 + 1 + 1, 0.5, 0.5,
  0.5, 0.5, 0.5, 0.25, 0.25, 0.5, 0.25, 0.25, 0.5,
  0.5, 0.5, 0.5, 0.25, 0.25, 1 + 1,
  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  1 + 1 + 1, 0.5, 0.5,


  1.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  1.5, 0.5, 1, 0.5, 0.25, 0.25,
  1.5, 0.5, 0.5, 0.5, 0.5, 0.25, 0.25,
  1 + 1 + 1, 0.5, 0.25, 0.25,
  1, 0.5, 0.25, 0.25, 1, 1,
  0.5, 0.5, 0.5, 0.5, 1, 0.25, 0.25, 0.5,
  1 + 1 + 1 + 1,
  0.5, 0.5, 0.5, 0.5, 1 + 1,
  0.5, 0.5, 0.5, 0.5, 1.5, 0.25, 0.25,
  1.5, 0.5, 1, 0.25, 0.25, 0.25, 0.25, 1 + 1 + 1 + 1,

};


// 天空之城
#define NOTE_D0 -1
#define NOTE_D1 294
#define NOTE_D2 330
#define NOTE_D3 350
#define NOTE_D4 393
#define NOTE_D5 441
#define NOTE_D6 495
#define NOTE_D7 556

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278

#define NOTE_DH1 589
#define NOTE_DH2 661
#define NOTE_DH3 700
#define NOTE_DH4 786
#define NOTE_DH5 882
#define NOTE_DH6 990
#define NOTE_DH7 112
//以上部分是定义是把每个音符和频率值对应起来，其实不用打这么多，但是都打上了，后面可以随意编写

#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625
//这部分是用英文对应了拍子，这样后面也比较好看

const int tune_air_city[] PROGMEM =
{
  NOTE_D0, NOTE_D0, NOTE_D0, NOTE_D6, NOTE_D7, NOTE_DH1, NOTE_D7, NOTE_DH1, NOTE_DH3, NOTE_D7, NOTE_D7, NOTE_D7, NOTE_D3, NOTE_D3,
  NOTE_D6, NOTE_D5, NOTE_D6, NOTE_DH1, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D3, NOTE_D4, NOTE_D3, NOTE_D4, NOTE_DH1,
  NOTE_D3, NOTE_D3, NOTE_D0, NOTE_DH1, NOTE_DH1, NOTE_DH1, NOTE_D7, NOTE_D4, NOTE_D4, NOTE_D7, NOTE_D7, NOTE_D7, NOTE_D0, NOTE_D6, NOTE_D7,
  NOTE_DH1, NOTE_D7, NOTE_DH1, NOTE_DH3, NOTE_D7, NOTE_D7, NOTE_D7, NOTE_D3, NOTE_D3, NOTE_D6, NOTE_D5, NOTE_D6, NOTE_DH1,
  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D2, NOTE_D3, NOTE_D4, NOTE_DH1, NOTE_D7, NOTE_D7, NOTE_DH1, NOTE_DH1, NOTE_DH2, NOTE_DH2, NOTE_DH3, NOTE_DH1, NOTE_DH1, NOTE_DH1,
  NOTE_DH1, NOTE_D7, NOTE_D6, NOTE_D6, NOTE_D7, NOTE_D5, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_DH1, NOTE_DH2, NOTE_DH3, NOTE_DH2, NOTE_DH3, NOTE_DH5,
  NOTE_DH2, NOTE_DH2, NOTE_DH2, NOTE_D5, NOTE_D5, NOTE_DH1, NOTE_D7, NOTE_DH1, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH3,
  NOTE_D6, NOTE_D7, NOTE_DH1, NOTE_D7, NOTE_DH2, NOTE_DH2, NOTE_DH1, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_DH4, NOTE_DH3, NOTE_DH2, NOTE_DH1,
  NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH6, NOTE_DH6, NOTE_DH5, NOTE_DH5, NOTE_DH3, NOTE_DH2, NOTE_DH1, NOTE_DH1, NOTE_D0, NOTE_DH1,
  NOTE_DH2, NOTE_DH1, NOTE_DH2, NOTE_DH2, NOTE_DH5, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH3, NOTE_DH6, NOTE_DH6, NOTE_DH5, NOTE_DH5,
  NOTE_DH3, NOTE_DH2, NOTE_DH1, NOTE_DH1, NOTE_D0, NOTE_DH1, NOTE_DH2, NOTE_DH1, NOTE_DH2, NOTE_DH2, NOTE_D7, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D7
};//这部分就是整首曲子的音符部分，用了一个序列定义为tune，整数

const float duration_air_city[] PROGMEM =
{
  1, 1, 1, 0.5, 0.5,     1 + 0.5, 0.5, 1, 1,     1, 1, 1, 0.5, 0.5,
  1 + 0.5, 0.5, 1, 1,     1, 1, 1, 1,          1 + 0.5, 0.5, 1, 1,
  1, 1, 0.5, 0.5, 0.5, 0.5,    1 + 0.5, 0.5, 1, 1,     1, 1, 1, 0.5, 0.5,
  1 + 0.5, 0.5, 1, 1,    1, 1, 1, 0.5, 0.5,     1 + 0.5, 0.5, 1, 1,
  1, 1, 1, 0.5, 0.5,    1, 0.5, 0.25, 0.25, 0.25, 0.5,    0.5, 0.5, 0.5, 0.25, 0.5, 1,
  0.5, 0.5, 0.5, 0.5, 1, 1,    1, 1, 1, 0.5, 0.5,    1 + 0.5, 0.5, 1, 1,
  1, 1, 1, 0.5, 0.5,    1.5, 0.5, 1, 1,    1, 1, 1, 1,
  0.5, 0.5, 1, 1, 0.5, 0.5,    1.5, 0.25, 0.5, 1,    1, 1, 1, 1,
  1, 1, 1, 1,    1, 1, 1, 1,    0.5, 0.5, 1, 1, 0.5, 0.5,
  1, 0.5, 0.5, 1, 1,    1, 1, 1, 1,    1, 1, 1, 1,
  0.5, 0.5, 1, 1, 0.5, 0.5,    1, 0.5, 0.25, 0.5, 1,    1, 1, 1, 0.5, 0.5
};//这部分是整首曲子的节拍部分，也定义个序列duration，浮点（数组的个数和前面音符的个数是一样的，一一对应么）

void apchy_play();
void air_city_play();

*/
void didi_play();
void alarm_play();

void buzzer_init()
{
  pinMode(BUZZER_PIN, OUTPUT);
}
void play(byte type, int times = 1)
{
  switch (type)
  {
/*
    case APCHY:
      apchy_play();
      break;
    case AIR_CITY:
      air_city_play();
      break;
*/
    case DIDI:
      didi_play();
      break;
    case ALARM:
      alarm_play();
      break;
  }
}
/*
void apchy_play()
{
  for (int x = 0; x < sizeof(tune_apchy) / sizeof(tune_apchy[0]); x++) //循环音符的次数
  {
    tone(BUZZER_PIN, pgm_read_word_near((unsigned short)(&tune_apchy[x]))); //此函数依次播放tune序列里的数组，即每个 音符

    delay(400 * pgm_read_float(&duration_apchy[x])); //每个音符持续的时间，即节拍duration，是调整时间的越大，曲子速度越慢，越小曲子速度越快，自己掌握吧

    noTone(BUZZER_PIN);//停止当前音符，进入下一音符
  }
}

void air_city_play()
{
  for (int x = 0; x < sizeof(tune_air_city) / sizeof(tune_air_city[0]); x++) //循环音符的次数
  {
    tone(BUZZER_PIN, pgm_read_word_near((unsigned short)(&tune_air_city[x]))); //此函数依次播放tune序列里的数组，即每个 音符

    delay(400 * pgm_read_float(&duration_air_city[x])); //每个音符持续的时间，即节拍duration，是调整时间的越大，曲子速度越慢，越小曲子速度越快，自己掌握吧

    noTone(BUZZER_PIN);//停止当前音符，进入下一音符
  }
}
*/
void didi_play()
{
  for (byte i = 0; i < 200; i++)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1000);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(100);
  }
}


void alarm_play()
{
  for (int i = 700; i <= 800; i++) //用循环的方式将频率从200HZ 增加到800HZ
  {
    tone(BUZZER_PIN, i);                   //在四号端口输出频率
    delay(5);                     //该频率维持5毫秒
  }
  noTone(BUZZER_PIN);//停止当前音符，进入下一音符
//  delay(1000);                    //最高频率下维持4秒钟
//  for (int i = 800; i >= 700; i--)
//  {
//    tone(BUZZER_PIN, i);
//    delay(5);
//  }
}
