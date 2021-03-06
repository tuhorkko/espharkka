/*
 * main.c
 *
 *  Created on: 7 Dec 2019
 *      Author: Jussi M Savola
 */


/*
    FreeRTOS V8.2.1 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?".  Have you defined configASSERT()?  *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    ***************************************************************************
     *                                                                       *
     *   Investing in training allows your team to be as productive as       *
     *   possible as early as possible, lowering your overall development    *
     *   cost, and enabling you to bring a more robust product to market     *
     *   earlier than would otherwise be possible.  Richard Barry is both    *
     *   the architect and key author of FreeRTOS, and so also the world's   *
     *   leading authority on what is the world's most popular real time     *
     *   kernel for deeply embedded MCU designs.  Obtaining your training    *
     *   from Richard ensures your team will gain directly from his in-depth *
     *   product knowledge and years of usage experience.  Contact Real Time *
     *   Engineers Ltd to enquire about the FreeRTOS Masterclass, presented  *
     *   by Richard Barry:  http://www.FreeRTOS.org/contact
     *                                                                       *
    ***************************************************************************

    ***************************************************************************
     *                                                                       *
     *    You are receiving this top quality software for free.  Please play *
     *    fair and reciprocate by reporting any suspected issues and         *
     *    participating in the community forum:                              *
     *    http://www.FreeRTOS.org/support                                    *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!

    Modified by lindh LUT
*/

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Xilinx includes. */
#include "xil_printf.h"
#include "xparameters.h"
#include "xil_types.h"

/* LUT includes. */

#include "rtosharkka.h"

static void printtaa_5();

// static TaskHandle_t xTask4;
xSemaphoreHandle LEDsem;
xSemaphoreHandle Kpsem;
xSemaphoreHandle Kisem;
xSemaphoreHandle Urefsem;
xSemaphoreHandle Uactsem;

// extern float Kp, Ki;
// extern float Uref, Uact;

extern float set_Kp(float value){
	xSemaphoreTake(LEDsem,portMAX_DELAY);
	Kp = value;
	xSemaphoreGive(LEDsem);
	return 0;
};

float set_Ki(float value){
//	xSemaphoreTake(Kisem,portMAX_DELAY);
//	xSemaphoreGive(Kisem);
	Ki = value;
	return 0;
};

float set_Uref(float value){
//	xSemaphoreTake(Urefsem,portMAX_DELAY);
//	xSemaphoreGive(Urefsem);
	Uref = value;
	return 0;
};

float set_Uact(float value) {
//	xSemaphoreTake(Uactsem,portMAX_DELAY);
//	xSemaphoreGive(Uactsem);
	Uact = value;
	return 0;
};

float get_Kp(){
//	xSemaphoreTake(Kpsem,portMAX_DELAY);
//	xSemaphoreGive(Kpsem);
	return Kp;
};

float get_Ki(){
//	xSemaphoreTake(Kisem,portMAX_DELAY);
//	xSemaphoreGive(Kisem);
	return Ki;
};

float get_Uref(){
//	xSemaphoreTake(Urefsem,portMAX_DELAY);
//	xSemaphoreGive(Urefsem);
	return Uref;
};

float get_Uact(){
//	xSemaphoreTake(Uactsem,portMAX_DELAY);
//	xSemaphoreGive(Uactsem);
	return Uact;
};

int main( void ) {

	  AXI_BTN_TRI |= 0xF; // Set direction for buttons 0..3 ,  0 means output, 1 means input
      AXI_LED_TRI = ~0xF;			// Set direction for bits 0-3 to output for the LEDs
       set_Kp(0.5);
       set_Ki(0.5);
       set_Uref(20.0);
       set_Uact(0.0);


	xil_printf( "Hello from Freertos example main\r\n" );

#define  XUART_STATUS (*((volatile u32*)(0xE0001000U + 0x002CU)))
#define RACTIVE    0x00000400U /**< RX active */

// uint32_t  READ_STATUS = 0;

float c = get_Kp();
xil_printf( "Another printti %f\r\n", c );




	/**
	 * Create four tasks t
	 * Each function behaves as if it had full control of the controller.
	 * https://www.freertos.org/a00125.html
	 */
	xTaskCreate( 	ui, 					// The function that implements the task.
					"Led1", 					// Text name for the task, provided to assist debugging only.
					configMINIMAL_STACK_SIZE, 	// The stack allocated to the task.
					NULL, 						// The task parameter is not used, so set to NULL.
					tskIDLE_PRIORITY+1,			// The task runs at the idle priority. Higher number means higher priority.
					NULL );

	xTaskCreate( kontrolli, "Led2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
	xTaskCreate( converter, "Led3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );
	xTaskCreate( ledita_4, "Led4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+4, NULL );			// Higher priority
	xTaskCreate(printtaa_5,"Printti",configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-2, NULL );    // Highest of priority

	// LED Sema just as an example

	vSemaphoreCreateBinary(LEDsem);
	/* vSemaphoreCreateBinary(Kpsem);
	vSemaphoreCreateBinary(Kisem);
	vSemaphoreCreateBinary(Urefsem);
	vSemaphoreCreateBinary(Uactsem); */

	// Start the tasks and timer running.
	// https://www.freertos.org/a00132.html
	vTaskStartScheduler();



	/**
	 * If all is well, the scheduler will now be running, and the following line
	 * will never be reached.  If the following line does execute, then there was
	 * insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	 * to be created.  See the memory management section on the FreeRTOS web site
	 * for more details.
	 */
	for( ;; );
}


/* Implement a function in a C file to generate a periodic interrupt at the
required frequency. */
//void vSetupTickInterrupt( void )
//{
/* FreeRTOS_Tick_Handler() is itself defined in the RTOS port layer.  An extern
declaration is required to allow the following code to compile. */
//extern void FreeRTOS_Tick_Handler( void );

    /* Assume TIMER1_configure() configures a hypothetical timer peripheral called
    TIMER1 to generate a periodic interrupt with a frequency set by its parameter. */
  //  TIMER1_configure( 10);

    /* Next assume Install_Interrupt() installs the function passed as its second
    parameter as the handler for the peripheral passed as its first parameter. */
    //Install_Interrupt( TIMER1, FreeRTOS_Tick_Handler );
//}







static void printtaa_5() {
	static uint8_t s;
	const TickType_t delay = pdMS_TO_TICKS( 1000 );
	for (;;) {
		// xil_printf("Elapsed: %ds\n\n", s++);
		vTaskDelay(delay);  // not strictly synchronous
	}
}
