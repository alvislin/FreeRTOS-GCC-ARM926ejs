/*
Copyright 2013, Jernej Kovacic

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


/**
 * @file
 * A simple demo application.
 *
 * @author Jernej Kovacic
 */


#include <stddef.h>
#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>

#include "app.h"

/* Struct with settings for each task */
typedef struct _paramStruct
{
    char* text;        /* text to be printed by the task */
    uint32_t delay;    /* delay in miliseconds */
} paramStruct;

/* Default parameters if no parameter struct is available */
static const char defaultText[] = "<NO TEXT>\r\n";
static const uint32_t defaultDelay = 1000;


/* Task function that will be instantiated in multiple tasks */
void vTaskFunction( void *pvParameters )
{
    const char* taskName;
    uint32_t delay;
    paramStruct* params = (paramStruct*) pvParameters;

    taskName = ( NULL==params || NULL==params->text ? defaultText : params->text );
    delay = ( NULL==params ? defaultDelay : params->delay);

    for( ; ; )
    {
        /* Print out the name of this task. */

        vPrintMsg(taskName);

        vTaskDelay( delay / portTICK_RATE_MS );
    }

    /*
     * If the task implementation ever manages to break out of the
     * infinite loop above, it must be deleted before reaching the
     * end of the function!
     */
    vTaskDelete(NULL);
}

/* Parameters for two tasks */
static const paramStruct tParam[2] =
{
    (paramStruct) { .text="Task1\r\n", .delay=2000 },
    (paramStruct) { .text="Task2\r\n", .delay=3000 }
};



/* Startup function that creates and runs two FreeRTOS tasks */
void main(void)
{
    /*
     * Make sure (instartup.s) that main is entered in Supervisor mode.
     * When vTaskStartScheduler launches the first task, it will switch
     * to System mode and enable interrupt exceptions.
     */
    vPrintMsg("= = = T E S T   S T A R T E D = = =\r\n\r\n");

    assertTaskCreate(vTaskFunction, "task1", 128, (void*) &tParam[0], 3, NULL);
    assertTaskCreate(vTaskFunction, "task2", 128, (void*) &tParam[1], 2, NULL);

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    /*
     * If all goes well, vTaskStartScheduler should never return
     * but just in case "end up" in an infinite loop
     */

    vPrintMsg("Could not start the scheduler!!!\r\n");
    for ( ; ; );
}
