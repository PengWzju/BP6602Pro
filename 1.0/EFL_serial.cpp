#include "efl_serial.h"
#include <Arduino.h>
#include "message.h"
#include "cppQueue.h"
#include "tc_main.h"
#include "usart.h"


#define IMPLEMENTATION FIFO
#define OVERWRITE false
#define INSTRUCTION_LENGTH 60
#define INSTRUCTION_BUFFER 10

#define BUFSIZE 4
#define MAX_CMD_SIZE 64


struct Instruction {
  int length = 0;
  char content[INSTRUCTION_LENGTH];
};

// ========================= private varibles ========================
static char cmdbuffer[BUFSIZE][MAX_CMD_SIZE];
static int buflen = 0;
static char serial_char;
static int serial_count = 0;
static boolean comment_mdoe = false;

/*建立结构体*/
Instruction instruction;
#define BUFF_SIZE 20
char input_ins[BUFF_SIZE];
/*建立缓冲区*/
Queue instruction_queue(sizeof(Instruction), INSTRUCTION_BUFFER, IMPLEMENTATION, OVERWRITE);
char input_char = ' ';

void read_serial()
{
  boolean full = instruction_queue.isFull();
  while (Serial.available() > 0 && !full)                 //串口有数据且缓冲区没满，逐字节读取串口接收到的数据
  {
    input_char = char(Serial.read());

    if (input_char != '\n' && input_char != '\r' && instruction.length < (INSTRUCTION_LENGTH - 1))
    {
      if (input_char >= 'a' && input_char <= 'z') // 指令不区分大小写
      {
        input_char = input_char - 32;
      }
      instruction.content[instruction.length++] = input_char; //只要没碰到换行或分号，并且结构体没满，将读取的字节存入结构体
    }
    else if (instruction.length >= 1 )
    {
      instruction_queue.push(&instruction);
      serial_init();                                            //清空结构体变量的值
      return;
    }
    delayMicroseconds(50);
  }
  //delayMicroseconds(100);
  if (!instruction_queue.isEmpty())
  {
    Instruction ins;
    instruction_queue.pop(&ins); //从缓冲区弹出到(&ins)
    process_string(ins.content, ins.length);
    //strcpy(cur_command,ins.content);
    Serial.println(ins.content);
    Serial.println(MSG_OK);
    //print_txt(USART_COMMAND, ins.content);
    //Serial.println();
  }
}


/*初始化字符串，全部置0*/
void serial_init() {
  for (byte i = 0; i < INSTRUCTION_LENGTH; i++)
  {
    instruction.content[i] = 0;
  }
  instruction.length = 0;
}
