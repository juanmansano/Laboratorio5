#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

osThreadId_t thread1_id, thread2_id;

osMutexId_t mutex1_id, mutex2_id;

void thread1(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    osMutexAcquire(mutex1_id, osWaitForever);
    tick = osKernelGetTickCount();
    
    state ^= LED1;
    osMutexAcquire(mutex2_id, osWaitForever);
    LEDWrite(LED1, state);
    osMutexRelease(mutex2_id);
    
    osDelayUntil(tick + 100);
    osMutexRelease(mutex1_id);
  } // while
} // thread1

void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    osMutexAcquire(mutex2_id, osWaitForever);
    tick = osKernelGetTickCount();
    
    state ^= LED2;
    osMutexAcquire(mutex1_id, osWaitForever);
    LEDWrite(LED2, state);
    osMutexRelease(mutex1_id);
    
    osDelayUntil(tick + 100);
    osMutexRelease(mutex2_id);
  } // while
} // thread2

void main(void){
  LEDInit(LED2 | LED1);

  osKernelInitialize();
  
  mutex1_id = osMutexNew(NULL);
  mutex2_id = osMutexNew(NULL);

  thread1_id = osThreadNew(thread1, NULL, NULL);
  thread2_id = osThreadNew(thread2, NULL, NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
