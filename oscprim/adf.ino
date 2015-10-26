#include <MapleFreeRTOS.h>
#include "adf4602.h"

static void vADFTask(void *pvParameters) {

    TunerInit(0);
    for (;;) {
        vTaskDelay(10000);
    }

}
