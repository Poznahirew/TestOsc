#include <MapleFreeRTOS.h>

#define WIFI Serial1
xQueueHandle ADCQueue;

String Comand = "";

static void vCPUTask(void *pvParameters) {
    //xTaskCreate(vCPUDeb, (signed portCHAR *)"CPUDebug",  configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY + 5, NULL);
    for (;;) {
      vTaskDelay(250);
      togglePin(PD5);
    }
}

static void vWIFITask(void *pvParameters) {
   
    for (;;) {
        // Read while we have data
        if (WIFI.available()>0)
        {
            char inChar = WIFI.read();// Read a character
            debug_print(inChar);
            togglePin(BOARD_LED_PIN);

            if (inChar == '\r'){// Check for termination character
                debug_println(Comand);
                PasreComand(Comand);
                Comand = "";
            }
            else{
                Comand += inChar;
            }
         
        }
    }
}

void setup() {
  // put your setup code here, to run once:
pinMode(BOARD_LED_PIN, OUTPUT);
pinMode(PD5, OUTPUT);
WIFI.begin(115200);

   xTaskCreate(vCPUTask,  (signed portCHAR *)"CPU", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
   xTaskCreate(vWIFITask, (signed portCHAR *)"CMD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
   xTaskCreate(vADCTask,  (signed portCHAR *)"ADC", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
   xTaskCreate(vADFTask, (signed portCHAR *)"ADF", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
   vTaskStartScheduler();
}

void loop() {
}
