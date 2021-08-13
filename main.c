/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/trace.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#define CCM_RAM __attribute__((section(".ccmram")))

// ----------------------------------------------------------------------------

#include "led.h"

#define BLINK_PORT_NUMBER         (3)
#define BLINK_PIN_NUMBER_GREEN    (12)
#define BLINK_PIN_NUMBER_ORANGE   (13)
#define BLINK_PIN_NUMBER_RED      (14)
#define BLINK_PIN_NUMBER_BLUE     (15)
#define BLINK_ACTIVE_LOW          (false)

struct led blinkLeds[4];

// ----------------------------------------------------------------------------
/*-----------------------------------------------------------*/

/*
 * The LED timer callback function.  This does nothing but switch the red LED
 * off.
 */

static void prvOneShotTimerCallback( TimerHandle_t xTimer );
static void prvAutoReloadTimerCallback( TimerHandle_t xTimer );
static void SendTimerCallback( TimerHandle_t xTimer );
static void ReceieveTimerCallback(TimerHandle_t xTimer);

/*-----------------------------------------------------------*/

/* The LED software timer.  This uses vLEDTimerCallback() as its callback
 * function.
 */
 TimerHandle_t xTimer1 = NULL;
 TimerHandle_t xTimer2 = NULL;
SemaphoreHandle_t xSemA = NULL;
SemaphoreHandle_t xSemB = NULL;
/*TimerHandle_t xTimer3 = NULL;
TimerHandle_t xTimer4 = NULL;*/
BaseType_t xTimer1Started, xTimer2Started;
TaskHandle_t Task1 = NULL;
TaskHandle_t Task2 = NULL;
QueueHandle_t Queue1 ;

/*-----------------------------------------------------------*/
// ----------------------------------------------------------------------------
//
// Semihosting STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace-impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


//-----------------------------------dah my code--------------
int sent=0;
int failed=0;
int Re=0;
int Tsender;
int j=0;
char myTxBuff[100];

int arr[]={100,140,180,220,260,300};
void init(){
	//printf("At Period = %d\n", arr[j] );
	if(j!=0)
	{printf("Total number of successfully sent messages : %d\n",sent);
	printf("Total number of blocked messages : %d\n",failed);}
	//printf("At Period = %d", arr[j] );
	sent=0;
	failed=0;
	Re=0;
	//Tsender=arr[j];
	//j++;
	//printf("%d\n",Tsender);
	//printf("ablha\n");
	xQueueReset(Queue1);
	if(j<6)
	{
		//printf("period %d",arr[j]);
		printf("Next Period = %d\n", arr[j] );
		xTimerChangePeriod(xTimer1,pdMS_TO_TICKS(arr[j]),0);
		xTimerReset(xTimer2,0);
		j++;
	}
	else
	{
		printf("Game Over. \n");
		xTimerDelete(xTimer1,0);
		xTimerDelete(xTimer2,0);
	}

	//printf("In The Queue : %d\n",uxQueueMessagesWaiting(Queue1));
	//printf("b3dha\n");
	//printf("ana gwa\n");
};


void send(void*p){
	//printf("Hellllllo\n");
	//while(1){
	BaseType_t status;
	//if(xSemaphoreTake(xSemA,(TickType_t)10000))
	//{
	//	printf("Hello\n");
	//}
	//}
	int Time;
	//int Tsender=100;
	//vTaskDelay(Tsender);
	//char myTxBuff[100];
	//Queue1= xQueueCreate(2, sizeof(myTxBuff));
	while(1){
		if(xSemaphoreTake(xSemA,(TickType_t)10000))
			{
	//xSemaphoreTake(xSemA,(TickType_t)100000);
	//printf("Hello\n");
		Time=xTaskGetTickCount();
		sprintf(myTxBuff, "%d",Time);
		//printf("the counts is %d\n",Time);
		status=xQueueSend(Queue1, (void*)myTxBuff, (TickType_t)0 );
		if(status==pdPASS)
			sent++;
		else
			failed++;
			}
		//vTaskDelay(3000);
}
}


void receive(void*p){

	//printf("Welcoooooome\n");
		//while(1){

		//if(xSemaphoreTake(xSemB,(TickType_t)10000))
	//{
		//	printf("Welcomeeeeeeeee\n");
		//}
		//}
	char myRxBuff[100];
	while(1){
		if(xSemaphoreTake(xSemB,(TickType_t)10000))
			{
			if(Queue1 != 0){
						if(xQueueReceive(Queue1, (void*) myRxBuff,(TickType_t) 5 ))
							{//printf("data Received : %s \n",myRxBuff);
							Re++;
							//printf("No. : %d\n",Re);
				}

		}

	}
	}

}









/*void my_task1(void*p){
	int count =0;
	vTaskDelay(5000);
	while(1){
		printf("The count is %d\n",count);
		count++;
		vTaskDelay(1000);
	}
}

void my_task2(void*p){
	int count2 =0;
	while(1){
		printf("Hello World %d\n",count2);
		count2++;
		vTaskDelay(1000);
	}
}*/

int
main(int argc, char* argv[])
{
	// Add your code here.
	Queue1= xQueueCreate(2, sizeof(myTxBuff));
	xTaskCreate(send,"S1",2000,(void*)0,1,&Task1);
	//xTaskCreate(my_task2,"T2",200,(void*)0,1,&xTimer4);
	xTaskCreate(receive,"R1",2000,(void*)0,2,&Task2);
	vSemaphoreCreateBinary(xSemA);
	vSemaphoreCreateBinary(xSemB);
	xSemaphoreTake(xSemA,(TickType_t)100000);
	xSemaphoreTake(xSemB,(TickType_t)100000);

	//init();
	xTimer1 = xTimerCreate( "Timer1", ( pdMS_TO_TICKS(100) ), pdTRUE, ( void * ) 0, SendTimerCallback);
	xTimer2 = xTimerCreate( "Timer2", ( pdMS_TO_TICKS(200) ), pdTRUE, ( void * ) 0, ReceieveTimerCallback);
	//printf("hello");
	if (( xTimer1 != NULL) && ( xTimer2 != NULL ) ){
		xTimer1Started = xTimerStart( xTimer1, 0 );
		xTimer2Started = xTimerStart( xTimer2, 0 );
	}
	if( xTimer1Started == pdPASS && xTimer2Started == pdPASS)
	{
		init();
		vTaskStartScheduler();}
	/*blinkLeds[0] = createLed(BLINK_PORT_NUMBER, BLINK_PIN_NUMBER_GREEN, BLINK_ACTIVE_LOW );
	blinkLeds[1] = createLed(BLINK_PORT_NUMBER, BLINK_PIN_NUMBER_ORANGE, BLINK_ACTIVE_LOW );
	blinkLeds[2] = createLed(BLINK_PORT_NUMBER, BLINK_PIN_NUMBER_RED, BLINK_ACTIVE_LOW );
	blinkLeds[3] = createLed(BLINK_PORT_NUMBER, BLINK_PIN_NUMBER_BLUE, BLINK_ACTIVE_LOW );

	for (int i=0; i<4; i++){
		power_up(&blinkLeds[i]);
	}

	xTimer1 = xTimerCreate( "Timer1", ( pdMS_TO_TICKS(2000) ), pdFALSE, ( void * ) 0, prvOneShotTimerCallback);
	xTimer2 = xTimerCreate( "Timer2", ( pdMS_TO_TICKS(1000) ), pdTRUE, ( void * ) 1, prvAutoReloadTimerCallback);

	if( ( xTimer1 != NULL ) && ( xTimer2 != NULL ) )
	{
		xTimer1Started = xTimerStart( xTimer1, 0 );
		xTimer2Started = xTimerStart( xTimer2, 0 );
	}

	if( xTimer1Started == pdPASS && xTimer2Started == pdPASS)
	{
		vTaskStartScheduler();
	}
*/
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

static void SendTimerCallback( TimerHandle_t xTimer ){
	//printf("Helloooooooooo\n");
	//vTaskDelay(3000);
	xSemaphoreGive(xSemA);

}

static void ReceieveTimerCallback(TimerHandle_t xTimer){
	//printf("Welcome\n");
	if(Re<500)
		xSemaphoreGive(xSemB);
	else
		init();
}



//-----------------------------------------------------------------------------
static void prvOneShotTimerCallback( TimerHandle_t xTimer )
{
	trace_puts("One-shot timer callback executing");
	turn_on (&blinkLeds[1]);
}

static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{

	trace_puts("Auto-Reload timer callback executing");

	if(isOn(blinkLeds[0])){
		turn_off(&blinkLeds[0]);
	} else {
		turn_on(&blinkLeds[0]);
	}
}


void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;

	/* This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amout of FreeRTOS heap that
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}

void vApplicationTickHook(void) {
}

StaticTask_t xIdleTaskTCB CCM_RAM;
StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE] CCM_RAM;

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
  /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
  state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

static StaticTask_t xTimerTaskTCB CCM_RAM;
static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH] CCM_RAM;

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

