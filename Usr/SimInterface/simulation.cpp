#include "simulation.h"
#include <iostream>
#include "System_DataPool.h"
using namespace std;
TaskHandle_t Sim_Handle;

void Service_CoppeliaSim_Init()
{
    CoppeliaSim->Start("127.0.0.1", 5000, 5);
    xTaskCreate(Task_Sim, "CoppeliaSim service", Huge_Stack_Size, NULL, PriorityHigh, &Sim_Handle);
}


void Task_Sim(void* arg)
{
  /* Cache for Task */
  TickType_t xLastWakeTime_t;
  const TickType_t xBlockTime = pdMS_TO_TICKS(5);
  /* Pre-Load for task */
  xLastWakeTime_t = xTaskGetTickCount();
  (void)arg;
  /* Infinite loop */
  for (;;)
  {
      CoppeliaSim->ComWithServer();

    /* Pass control to the next task */
    vTaskDelayUntil(&xLastWakeTime_t, xBlockTime);
  }
}
