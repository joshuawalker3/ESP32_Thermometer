#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define CLK_PIN 15
#define DISPLAY_1_IN 2
#define DISPLAY_2_IN 4

void start(int, int, int);
void allOn(int, int, int);
void displayNum(int, int, int, int);
void bitEnter(int, int, int, char, char);
void signalSet(int, char);
void signalInput(int);
char* getInput(int);
void clearShiftRegister(int, int, int);
int getTemp(int);


void app_main(void)
{
	char* ourTaskName = pcTaskGetName(NULL);

	ESP_LOGI(ourTaskName, "Hello, starting up\n");

	gpio_reset_pin(CLK_PIN);
	gpio_reset_pin(DISPLAY_1_IN);
	gpio_reset_pin(DISPLAY_2_IN);

	gpio_set_direction(CLK_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(DISPLAY_1_IN, GPIO_MODE_OUTPUT);
	gpio_set_direction(DISPLAY_2_IN, GPIO_MODE_OUTPUT);

	clearShiftRegister(DISPLAY_2_IN, DISPLAY_1_IN, CLK_PIN);
	start(DISPLAY_2_IN, DISPLAY_1_IN, CLK_PIN);
	allOn(DISPLAY_2_IN, DISPLAY_1_IN, CLK_PIN);
	clearShiftRegister(DISPLAY_2_IN, DISPLAY_1_IN, CLK_PIN);

	while (1)
	{
		for(int i = 0; i < 100; i++) {
			displayNum(DISPLAY_2_IN, DISPLAY_1_IN, CLK_PIN, i);
			vTaskDelay(500 / portTICK_PERIOD_MS);
		}
		clearShiftRegister(DISPLAY_2_IN, DISPLAY_1_IN, CLK_PIN);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void start(int inputLeft, int inputRight, int clk)
{
  bitEnter(inputLeft, inputRight, clk, '1', '1');
  vTaskDelay(500 / portTICK_PERIOD_MS);
  bitEnter(inputLeft, inputRight, clk, '0', '0');
  vTaskDelay(500 / portTICK_PERIOD_MS);
  bitEnter(inputLeft, inputRight, clk, '0', '0');
  vTaskDelay(500 / portTICK_PERIOD_MS);
  bitEnter(inputLeft, inputRight, clk, '0', '0');
  vTaskDelay(500 / portTICK_PERIOD_MS);
  bitEnter(inputLeft, inputRight, clk, '0', '0');
  vTaskDelay(500 / portTICK_PERIOD_MS);
  bitEnter(inputLeft, inputRight, clk, '0', '0');
  vTaskDelay(500 / portTICK_PERIOD_MS);
  bitEnter(inputLeft, inputRight, clk, '0', '0');
  vTaskDelay(500 / portTICK_PERIOD_MS);
  bitEnter(inputLeft, inputRight, clk, '0', '0');
  vTaskDelay(500 / portTICK_PERIOD_MS);
  allOn(inputLeft, inputRight, clk);
  vTaskDelay(750 / portTICK_PERIOD_MS);
  clearShiftRegister(inputLeft, inputRight, clk);
}

void allOn(int inputLeft, int inputRight, int clk)
{
  for (int i = 7; i >= 0; i--)
  {
    bitEnter(inputLeft, inputRight, clk, '1', '1');
  }
}

void displayNum(int inputLeft, int inputRight, int clk, int num)
{
  char* digitLeft = getInput(num / 10);
  char* digitRight = getInput(num % 10);
  for (int i = 7; i >= 0; i--)
  {
    bitEnter(inputLeft, inputRight, clk, digitLeft[i], digitRight[i]);
  }
}

void bitEnter(int inputLeft, int inputRight, int clk, char bitLeft, char bitRight)
{
  signalSet(inputLeft, bitLeft);
  signalSet(inputRight, bitRight);
  signalInput(clk);
}

void signalSet(int input, char bit)
{
  if (bit == '1')
  {
    gpio_set_level(input, 1);
  }
  else
  {
    gpio_set_level(input, 0);
  }
}

void signalInput(int clk)
{
  gpio_set_level(clk, 1);
  gpio_set_level(clk, 0);
}

char* getInput(int num)
{
  switch(num)
  {
    case 0: return "11111100";
    case 1: return "01100000";
    case 2: return "11011010";
    case 3: return "11110010";
    case 4: return "01100110";
    case 5: return "10110110";
    case 6: return "10111110";
    case 7: return "11100000";
    case 8: return "11111110";
    case 9: return "11100110";
    default: return "11111111";
  }
}

void clearShiftRegister(int inputLeft, int inputRight, int clk)
{
  gpio_set_level(inputLeft, 0);
  gpio_set_level(inputRight, 0);
  for (int i=0; i < 8; i++)
  {
  	gpio_set_level(clk, 1);
  	gpio_set_level(clk, 0);
  }
}

/*
int getTemp(int readPin)
{
  int pinRead = analogRead(readPin);
  float volts = pinRead * (3.0 / 1024.0);
  float celsius = (volts - 0.5) * 100;
  int farenheit = (celsius * (9.0 / 5.0)) + 32.0;
  return farenheit;
}
*/
