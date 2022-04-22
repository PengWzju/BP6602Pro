#include "tc_water_pump.h"
#include "arduino.h"

byte status_inquire_instruction[5] = {0xFD, 0x01, 0xCF, (0x01 + 0xCF) & 0x7F, 0xFE};
byte open_pump_instruction[10] = {0xFD, 0x01, 0xC0, 0XCE, 0x00, 0x00, 0x0F, 0x00, (0x01 + 0xC0 + 0xCE + 0x00 + 0x00 + 0x0F + 0x00) & 0x7F, 0xFE};
//byte open_pump_instruction[10] = {0xFD, 0x01, 0xC0, 0XCE, 0x00, 0x00, 0x0F, 0x00, (0x01 + 0xC0 + 0xCE + 0x01 + 0x00 + 0x0F + 0x00) & 0x7F, 0xFE};
byte close_pump_instruction[10] = {0xFD, 0x01, 0xC0, 0XCD, 0x00, 0x00, 0x0F, 0x00, (0x01 + 0xC0 + 0xCD + 0x00 + 0x00 + 0x0F + 0x00) & 0x7F, 0xFE};
byte rx_data[20];

int rx_num = 20;

unsigned long pump_former_time = 0;
bool data_receive = false;

byte pump_status;
byte pump_err;
float pump_temp;

bool open_pump_cmd = false;
bool close_pump_cmd = false;

byte former_pump_status;
byte former_pump_err;
float former_pump_temp;
byte former_rx_5;
byte former_rx_6;
byte former_rx_7;


unsigned long cmd_former_time = 0;
//unsigned long process_time = 0;

#define INQUIRE 0
#define OPEN_PUMP 1
#define CLOSE_PUMP 2

byte cmd_type = 3;

void pump_init()
{
  Serial2.begin(9600);
}

void manage_pump() {
  if ((millis() - cmd_former_time) > 2000) {
    cmd_former_time = millis();

    if (cmd_type == INQUIRE) {
      rx_num = 0;

      //      process_time = millis();
      //      while (true) {
      //        if (Serial2.available() > 0) {
      //          Serial.print("time:");
      //          Serial.println(millis() - process_time);
      //          break;
      //        }
      //        if ((millis() - process_time) > 20) {
      //          break;
      //        }
      //      }

      while (Serial2.available() > 0 || (rx_num > 5 && rx_num < 20))
      {
        if (Serial2.available() <= 0) {
          continue;
        }
        byte input_char = byte(Serial2.read());

        if (input_char != 0xFE && rx_num < 19)
        {
          rx_data[rx_num++] = input_char;
        }
        else
        {
          rx_data[rx_num] = input_char;
          break;
        }
      }
      byte check_num = 0;
      for (int i = 1; i < 18; i++)
        check_num += rx_data[i];
      check_num = check_num & 0X7F;
      if (check_num != rx_data[18] || rx_num != 19) {
        if (rx_num > 0) {
          //          Serial.print("inquire: ");
          //          for (int i = 0; i <= rx_num; i++) {
          //            Serial.print(rx_data[i], HEX);
          //            Serial.print(' ');
          //          }
          //          Serial.println();
          rx_data[3] = former_pump_status;
          rx_data[16] = former_pump_err;
          rx_data[5] = former_rx_5;
          rx_data[6] = former_rx_6;
          rx_data[7] = former_rx_7;
        } else {
          rx_data[3] = 0;
          rx_data[5] = 0;
          rx_data[6] = 0;
          rx_data[7] = 0;
          rx_data[16] = 0;
          former_pump_status = 0;
          former_pump_err = 0;
          former_rx_5 = 0;
          former_rx_6 = 0;
          former_rx_7 = 0;
          //          Serial.println("inquire:none");
        }
        pump_status = rx_data[3];
        pump_err = rx_data[16];
        pump_temp = rx_data[5] * 100 + rx_data[6] + float(rx_data[7]) / 100;

        if (abs(pump_temp - former_pump_temp) > 35) {
          pump_temp = former_pump_temp;
        }
        if (rx_data[3] > 0 && pump_temp < 1) {
          pump_temp = former_pump_temp;
        }
        former_pump_temp = pump_temp;

      } else {
        //        Serial.print("inquire:");
        //        for (int i = 0; i <= rx_num; i++) {
        //          Serial.print(rx_data[i], HEX);
        //          Serial.print(' ');
        //        }
        //        Serial.println();
        former_pump_status = rx_data[3];
        former_pump_err = rx_data[16];
        former_rx_5 = rx_data[5];
        former_rx_6 = rx_data[6];
        former_rx_7 = rx_data[7];

        pump_status = rx_data[3];
        pump_err = rx_data[16];
        pump_temp = rx_data[5] * 100 + rx_data[6] + float(rx_data[7]) / 100;

        if (abs(pump_temp - former_pump_temp) > 35) {
          pump_temp = former_pump_temp;
        }
        if (rx_data[3] > 0 && pump_temp < 1) {
          pump_temp = former_pump_temp;
        }
        former_pump_temp = pump_temp;
      }
    } else if (cmd_type == OPEN_PUMP) {
      int byte_num = 0;
      while (Serial2.available() > 0 ) {
        byte input_char = byte(Serial2.read());
        //        Serial.print(input_char, HEX);
        //        Serial.print(' ');
        byte_num++;
      }
      if (byte_num > 0) {
        //        Serial.println();
        //        Serial.print("open_bytes:");
        //        Serial.println(byte_num);
        open_pump_cmd = true;
      } else {
        //        Serial.println("open false");
        open_pump_cmd = false;
      }
    } else if (cmd_type == CLOSE_PUMP) {
      int byte_num = 0;
      while (Serial2.available() > 0) {
        byte input_char = byte(Serial2.read());
        //        Serial.print(input_char, HEX);
        //        Serial.print(' ');
        byte_num++;
      }
      if (byte_num > 0) {
        //        Serial.println();
        //        Serial.print("close_bytes:");
        //        Serial.println(byte_num);
        close_pump_cmd = true;
      } else {
        close_pump_cmd = false;
      }
    }

    if (open_pump_cmd) {
      for (int i = 0; i < 10; i++)
        Serial2.write(open_pump_instruction[i]);
      cmd_type = OPEN_PUMP;
      //      Serial.println("open send");
    } else if (close_pump_cmd) {
      for (int i = 0; i < 10; i++)
        Serial2.write(close_pump_instruction[i]);
      cmd_type = CLOSE_PUMP;
      //      Serial.println("close send");
    } else {
      for (int i = 0; i < 5; i++)
        Serial2.write(status_inquire_instruction[i]);
      cmd_type = INQUIRE;
      //      Serial.println("inquire send");
    }
  }
}

void open_pump()
{
  open_pump_cmd = true;
}

void close_pump()
{
  close_pump_cmd = true;
}
