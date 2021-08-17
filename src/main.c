#include <FreeRTOS.h>
#include <task.h>

/* The period to toggle LED. */
#define mainBLINK_LED_OK_HALF_PERIOD	( ( TickType_t ) (500 / portTICK_PERIOD_MS))
#define mainLED_BLINK_PRIORITY			( tskIDLE_PRIORITY + 2 )

/* defines for which LED to use. PB7 on an Arduino Mega2560. PB5 (D13) on an uno. */
#ifdef __AVR_ATmega2560__
#define partestLED_PORTB_DATA_REG_BIT			( (uint8_t) 0x01 << 7 )
#define partestLED_PORTB_DIR_REG_BIT			( (uint8_t) 0x01 << 7 )
#define partestLED_ON							partestLED_PORTB_DIR_REG_BIT
#else
#define partestLED_PORTB_DATA_REG_BIT			( (uint8_t) 0x01 << 5 )
#define partestLED_PORTB_DIR_REG_BIT			( (uint8_t) 0x01 << 5 )
#define partestLED_ON							partestLED_PORTB_DIR_REG_BIT
#endif
static volatile uint8_t uCurrentLedOutputVal = (uint8_t) ~partestLED_ON;
void vParTestToggleLED( UBaseType_t uxLED );
void vParTestInitialise( void );
static void vBlinkOnboardUserLED( void *pvParameters );

extern void* __malloc_heap_end;
extern unsigned int __malloc_margin;

int main() {

  //as required by portable/ThirdParty/GCC/ATmega/README
  if( __malloc_heap_end == 0 )
    __malloc_heap_end = (char *)(RAMEND - __malloc_margin);

  vParTestInitialise();
  xTaskCreate( vBlinkOnboardUserLED, "LED", 50, NULL, mainLED_BLINK_PRIORITY, NULL );

	vTaskStartScheduler();
  //not reached
  return 0;
}

static void vBlinkOnboardUserLED( void *pvParameters )
{
	/* The parameters are not used. */
	( void ) pvParameters;

	/* Cycle forever, blink onboard user LED at a certain frequency. */
	for( ;; )
	{
		vParTestToggleLED( 0 );

		vTaskDelay( mainBLINK_LED_OK_HALF_PERIOD );
	}

}

void vParTestInitialise( void )
{
	/* Turn on user LED. This function is not thread safe. */
	DDRB |= partestLED_PORTB_DIR_REG_BIT;
	PORTB |= partestLED_PORTB_DIR_REG_BIT;
	uCurrentLedOutputVal = (uint8_t) ~partestLED_ON;
}


void vParTestToggleLED( UBaseType_t uxLED )
{
	/* There's only one LED on this board. */
	( void ) uxLED;
	
	/* Toggle user LED. 
	The compound action is guaranteed to be not interrupted by other tasks. */
	vTaskSuspendAll();
	
	if ( uCurrentLedOutputVal == partestLED_ON )
	{
		/* Turn off. */
		DDRB |= partestLED_PORTB_DIR_REG_BIT;
		PORTB &= ~partestLED_PORTB_DIR_REG_BIT;
		uCurrentLedOutputVal = (uint8_t) ~partestLED_ON;
	}
	else
	{
		/* Turn on. */
		DDRB |= partestLED_PORTB_DIR_REG_BIT;
		PORTB |= partestLED_PORTB_DIR_REG_BIT;
		uCurrentLedOutputVal = partestLED_ON;
	}
		
	xTaskResumeAll();
}

/* needed FreeRTOS hook functions */

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	/* When stack overflow happens, trap instead of attempting to recover.
	Read input arguments to learn about the offending task. */
	for( ;; )
	{
		/* Doesn't do anything yet. */
	}
}


void vApplicationMallocFailedHook() {
	for( ;; )
	{
		/* Doesn't do anything yet. */
	}
}

void vApplicationIdleHook( void )
{
	/* Doesn't do anything yet. */
}


/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static -- otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

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
/*-----------------------------------------------------------*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static -- otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}