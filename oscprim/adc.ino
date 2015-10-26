
#define ADC_LED_PIN BOARD_LED_PIN+2
#define ADC_DBG_PIN BOARD_LED_PIN+1
#define MAX_RELOAD ((1 << 16) - 1)
HardwareTimer timer(2);
int Value = 0;
int buffer[512] = { 0 };


static void vADCTask(void *pvParameters) {
    
    pinMode(PA7, INPUT_ANALOG);

    pinMode(ADC_LED_PIN, OUTPUT);
    pinMode(ADC_DBG_PIN, OUTPUT);
    

    adc_init(ADC1);
    adc_set_sample_rate(ADC1, ADC_SMPR_1_5); // Sample rate?
    adc_set_extsel(ADC1, ADC_ADC12_TIM2_CC2);
    adc_set_exttrig(ADC1, 1); // Does something useful, I think?
    adc_set_reg_seqlen(ADC1, 1); // 1 Channel per sequence?
    ADC1->regs->SQR3 = 7; // Channel 1
    //DMA, Enable, SW Start, Cont. Mode, SWSTART Ext. Sel
    ADC1->regs->CR2 |= ( ADC_CR2_ADON | ADC_CR2_SWSTART | ADC_CR2_CONT );
  

    debug_println("ADC init");
    ADCQueue = xQueueCreate(2, sizeof(int));
    debug_println("ADC query init");


    for (;;) {
        int element = 0;
        if (!xQueueReceive(ADCQueue, &element, portMAX_DELAY))
            continue;
        if (element == 1){

            digitalWrite(ADC_LED_PIN, 1);
            Value = analogRead(PA7);
            digitalWrite(ADC_LED_PIN, 0);

            debug_println("ADC get result");
            WIFI.print("+start: ");
            WIFI.println(Value);
            WIFI.println("OK");
        }
        else if (element > 1)
        {
            debug_println("Start conversation");

            digitalWrite(ADC_LED_PIN, 1);
            for (int i = 0; i < element; ++i)
            {
                togglePin(ADC_DBG_PIN);
                buffer[i]=analogRead(PA7);
                delay_us(2);

            }
            digitalWrite(ADC_LED_PIN, 0);

            debug_println("ADC get result");
            WIFI.print("+adc;");
            WIFI.print(element);
            WIFI.print(":");
            for (int i = 0; i < element-1; ++i)
            {
                WIFI.print(buffer[i]);
                WIFI.print(",");

            }
            WIFI.println(buffer[element-1]);
            WIFI.println("OK");
        }
    }
}
