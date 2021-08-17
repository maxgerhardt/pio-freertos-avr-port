# pio-freertos-avr-port

## How was this project created?

* Created lib/FreeRTOS folder
* downloaded current version of https://github.com/FreeRTOS/FreeRTOS-Kernel
* put all *.c files in the top-level directory of FreeRTOS-Kernel (croutine.c, tasks.c, ..) in lib/FreeRTOS
* out all *.h files from FreeRTOS-Kernel/include in lib/FreeRTOS
* put FreeRTOS-Kernel/portable/MemMang/heap_3.c in lib/FreeRTOS
* copy files from generic ATMega port (https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/main/portable/ThirdParty/GCC/ATmega, or FreeRTOS-Kernel/portable/ThirdParty/GCC/ATmega) into lib/FreeRTOS
* carefully read readme.md of aforementioned port to learn about port-specific quirks (such as the needed statement to be able to use malloc)
* took a look at ATMega328PB demo code https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS/Demo/AVR_ATmega328PB_Xplained_mini_GCC/RTOSDemo
* copied FreeRTOSConfig.h from there into `src/` with the modifications:
    * instead of `#define configCPU_CLOCK_HZ			( ( unsigned long ) 8000000 )`, uses the `F_CPU` macro to be correct all the time
* took a look at ATMega323 demo code https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS/Demo/AVR_ATMega323_IAR (*just* regarding the application, this uses a different FreeRTOS port)
* copied select code from demo to `src/main.c`
    * regarding needed application hooks `vApplicationStackOverflowHook()`, `vApplicationMallocFailedHook()` (optional), `vApplicationIdleHook()`, `vApplicationGetIdleTaskMemory()` and `vApplicationGetTimerTaskMemory()`
    * regarding simple application for blinking an LED from `main.c` and `ParTest/ParTest.c`

## Does this work?

The project was verified to be working on an Arduino Uno.

## Can this interwork with the Arduino framework?

In theory yes, if the FreeRTOS implementation does not require any of the timers that are also used by the Arduino code (such as a timer for `millis()` etc.). This is not tested.