#ifdef __IN_ECLIPSE__
#define CONFIG_FREERTOS_USE_TRACE_FACILITY 1
#undef configUSE_TRACE_FACILITY
#define configUSE_TRACE_FACILITY 1

#undef configUSE_STATS_FORMATTING_FUNCTIONS
#define configUSE_STATS_FORMATTING_FUNCTIONS 1

#define CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS 1
#define CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER 1

// Not required for Arduino IDE
#include <esp32-hal.h>
#include <esp32-hal-gpio.h>
#include <freertos/portable.h>
#include <freertos/task.h>
#include <HardwareSerial.h>
#include <stddef.h>
#endif

// IGNORE EVERYTHING ABOVE THIS LINE - NOT REQUIRED FOR ARDUINO IDE

TaskHandle_t Task0;
TaskHandle_t Task1;
SemaphoreHandle_t Semaphore;

#define LED0 27
#define LED1 25

// Global variables, available to all
static volatile unsigned long count0 = 0;
//static volatile unsigned long count1 = 0;

struct {
	unsigned long countA;
	unsigned long countB;
}volatile myCounters;

void loop0(void * parameter) {
	for (;;) {
		Serial.print("\t\t\t\t\t\t\t\tLoop 0 - Running on core: ");
		Serial.print(xPortGetCoreID());

		Serial.print(" (");

		xSemaphoreTake(Semaphore, portMAX_DELAY);
		Serial.print(++count0);
		xSemaphoreGive(Semaphore);

		Serial.println(")");

		digitalWrite(LED0, HIGH);
		delay(200);
		digitalWrite(LED0, LOW);
		//delay(300);
		// Above is Arduino-speak for:
		vTaskDelay(300 / portTICK_PERIOD_MS);
		// Note it is the TASK that is blocked not the entire thread
	}
}

void loop1(void * parameter) {
	for (;;) {
		Serial.print("Loop 1 - Running on core: ");
		Serial.print(xPortGetCoreID());

		Serial.print(" (");

		xSemaphoreTake(Semaphore, portMAX_DELAY);
		Serial.print(++count0);
		xSemaphoreGive(Semaphore);

		Serial.println(")");

		digitalWrite(LED1, LOW);
		delay(100);
		digitalWrite(LED1, HIGH);
		delay(100);
	}
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Setup started.");

	// Options are: 240 (default), 160, 80, 40, 20 and 10 MHz
	setCpuFrequencyMhz(10);
	int cpuSpeed = getCpuFrequencyMhz();
	Serial.println("Running at " + String(cpuSpeed) + "MHz");

	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);

	// Simple flag, up or down
	Semaphore = xSemaphoreCreateMutex();

	xTaskCreatePinnedToCore(
			loop0, /* Function to implement the task */
			"Task0", /* Name of the task */
			1000, /* Stack size in words */
			NULL, /* Task input parameter */
			10, /* Priority of the task */
			&Task0, /* Task handle. */
			1); /* Core where the task should run */

	xTaskCreatePinnedToCore(
			loop1, /* Function to implement the task */
			"Task1", /* Name of the task */
			1000, /* Stack size in words */
			NULL, /* Task input parameter */
			//configMAX_PRIORITIES - 1, /* Priority of the task */
			1,
			&Task1, /* Task handle. */
			1); /* Core where the task should run */

	//xSemaphoreGive(Semaphore);
	Serial.println("Setup completed.");
}

void loop()
{
//	Suspending and resuming tasks...
//	static TickType_t previousWakeTime = xTaskGetTickCount();
//	static bool taskSuspended = false;
//
//	vTaskDelayUntil(&previousWakeTime, portTICK_PERIOD_MS * 5000);
//
//	if (taskSuspended) {
//		vTaskResume(Task1);
//		taskSuspended = false;
//		Serial.println("Task 1 resumed.");
//	} else {
//		xSemaphoreTake(Semaphore, portMAX_DELAY);
//		vTaskSuspend(Task1);
//		taskSuspended = true;
//		Serial.println("Task 1 suspended.");
//	}
//
//	previousWakeTime = xTaskGetTickCount();

	//delay(1);
	//taskYIELD();
	//vPortYield();
	//yield();
	vTaskDelete(NULL);
}
