/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


TaskHandle_t Button1TaskHandler 							= NULL;
TaskHandle_t Button2TaskHandler 							= NULL;
TaskHandle_t Periodic_Transmitter_TaskHandler = NULL;
TaskHandle_t Uart_Receiver_TaskHandler			  = NULL;
TaskHandle_t Load_1_Simulation_TaskHandler 		= NULL;
TaskHandle_t Load_2_Simulation_TaskHandler 		= NULL;

int i,j;
pinState_t level_0;
pinState_t level_1;
//char runTimeStatsBuffer[190];
char* str;

/*  for calculating: 
	 execution time of task
	total time passed from the start of system execution
	total cpu utlization*/
/********************************************************************************************************/
int task_1_in_time = 0, task_1_out_time = 0, task_1_total_time = 0;
int task_2_in_time = 0, task_2_out_time = 0, task_2_total_time = 0;
int task_3_in_time = 0, task_3_out_time = 0, task_3_total_time = 0;
int task_4_in_time = 0, task_4_out_time = 0, task_4_total_time = 0;
int task_5_in_time = 0, task_5_out_time = 0, task_5_total_time = 0;
int task_6_in_time = 0, task_6_out_time = 0, task_6_total_time = 0;
int system_time = 0;
int cpu_load = 0;
/*******************************************************************************************************/

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */


static void prvSetupHardware( void );
/*-----------------------------------------------------------*/
 pinState_t buttonInput;



void vApplicationTickHook( void )
{
	GPIO_write(PORT_0, PIN1, PIN_IS_HIGH);
	GPIO_write(PORT_0, PIN1, PIN_IS_LOW);
}


/*
void vApplicationIdleHook( void )
{
	GPIO_write(PORT_0, PIN2, PIN_IS_HIGH);
	
}
*/



void Button_1_Monitor( void * pvParameters )
{
		TickType_t xLastWakeTime;
		xLastWakeTime = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag( NULL, ( void * ) 1 ); // tag unique identifier number for Button_1_Monitor task 

    for( ;; )
    {
			
			level_0 = GPIO_read(PORT_1, PIN0);
			
			vTaskDelayUntil( &xLastWakeTime, 100 );
			
    }
}


void Button_2_Monitor( void * pvParameters )
{
		TickType_t xLastWakeTime;
		xLastWakeTime = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag( NULL, ( void * ) 2 ); // tag unique identifier number for Button_2_Monitor task 

    for( ;; )
    {
			
			level_1 = GPIO_read(PORT_1, PIN1);
			
			vTaskDelayUntil( &xLastWakeTime, 2000 );
			
    }
}




void Periodic_Transmitter( void * pvParameters )
{
		TickType_t xLastWakeTime;
		xLastWakeTime = xTaskGetTickCount();
		vTaskSetApplicationTaskTag( NULL, ( void * ) 3 ); // tag unique identifier number for Periodic_Transmitter task 
    for( ;; )
    {
			/*for (int i=0; i<100; i++)
				{
					i = i;
				}
			vTaskGetRunTimeStats( runTimeStatsBuffer );*/
			str = "Periodic_Transmitter task";
			vTaskDelayUntil( &xLastWakeTime, 100 );
    }
}



void Uart_Receiver( void * pvParameters )
{
		TickType_t xLastWakeTime;
		xLastWakeTime = xTaskGetTickCount();
	  vTaskSetApplicationTaskTag( NULL, ( void * ) 4 ); // tag unique identifier number for Uart_Receiver task 
    for( ;; )
    {
			
			if (level_0 == PIN_IS_HIGH)
			{
				vSerialPutString((const signed char*)"Port_1_Pin_0_High", 190);
				xSerialPutChar('\n');
			}

			
			if (level_1 == PIN_IS_HIGH)
			{
				vSerialPutString((const signed char*)"Port_1_Pin_1_High", 190);
				xSerialPutChar('\n');
			}

			//vSerialPutString((const signed char*)str, 190);
			vTaskDelayUntil( &xLastWakeTime, 20 );
    }
}


void Load_1_Simulation( void * pvParameters )
{
		TickType_t xLastWakeTime;
		xLastWakeTime = xTaskGetTickCount();
		vTaskSetApplicationTaskTag( NULL, ( void * ) 5 ); // tag unique identifier number for Load_1_Simulation task 
    for( ;; )
    {
			for ( i=0; i<9990; i++)
			{
				 i = i;
			}
			vTaskDelayUntil( &xLastWakeTime, 10 );
    }
}



void Load_2_Simulation( void * pvParameters )
{
		TickType_t xLastWakeTime;
		xLastWakeTime = xTaskGetTickCount();
		vTaskSetApplicationTaskTag( NULL, ( void * ) 6 ); // tag unique identifier number for Load_2_Simulation task 
    for( ;; )
    {
				for ( j=0; j<23000; j++)
					{
							j = j;
					}
			vTaskDelayUntil( &xLastWakeTime, 100 );
    }
}


/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();


    /* Create Tasks here */
	/* Create the task, storing the handle. */
		  xTaskPeriodicCreate(
                    Button_1_Monitor,       			/* Function that implements the task. */
                    "Button_1_Monitor",       		/* Text name for the task. */
                    100,      				  		/* Stack size in words, not bytes. */
                    ( void * ) 0,       		/* Parameter passed into the task. */
                    1,                  		/* Priority at which the task is created. */
                    &Button1TaskHandler, 50 );   /* Used to pass out the created task's handle. */
			
			
			xTaskPeriodicCreate(
                     Button_2_Monitor,       			/* Function that implements the task. */
                    "Button_2_Monitor",          	/* Text name for the task. */
                    100,      						/* Stack size in words, not bytes. */
                    ( void * ) 0,    			/* Parameter passed into the task. */
                    1,										/* Priority at which the task is created. */
                    &Button2TaskHandler , 50);    /* Used to pass out the created task's handle. */
								
									
			xTaskPeriodicCreate(
                    Periodic_Transmitter,       			/* Function that implements the task. */
                    "Periodic_Transmitter",          	/* Text name for the task. */
                    100,      						/* Stack size in words, not bytes. */
                    ( void * ) 0,    			/* Parameter passed into the task. */
                    1,										/* Priority at which the task is created. */
                    &Periodic_Transmitter_TaskHandler, 100 );    /* Used to pass out the created task's handle. */
										
										
			xTaskPeriodicCreate(
                    Uart_Receiver,       			/* Function that implements the task. */
                    "Uart_Receiver",          	/* Text name for the task. */
                    100,      						/* Stack size in words, not bytes. */
                    ( void * ) 0,    			/* Parameter passed into the task. */
                    1,										/* Priority at which the task is created. */
                    &Uart_Receiver_TaskHandler , 20);    /* Used to pass out the created task's handle. */							
										
										
			xTaskPeriodicCreate(
                    Load_1_Simulation,       			/* Function that implements the task. */
                    "Load_1_Simulation",          	/* Text name for the task. */
                    100,      						/* Stack size in words, not bytes. */
                    ( void * ) 0,    			/* Parameter passed into the task. */
                    1,										/* Priority at which the task is created. */
                    &Load_1_Simulation_TaskHandler, 10 );    /* Used to pass out the created task's handle. */
										
										
										
			xTaskPeriodicCreate(
                    Load_2_Simulation,       			/* Function that implements the task. */
                    "Load_2_Simulation",          	/* Text name for the task. */
                    100,      						/* Stack size in words, not bytes. */
                    ( void * ) 0,    			/* Parameter passed into the task. */
                    1,										/* Priority at which the task is created. */
                    &Load_2_Simulation_TaskHandler, 100 );    /* Used to pass out the created task's handle. */

										
									
										
										
	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


