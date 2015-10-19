
//AT+start=1
int Start = 0;

void start_get()              
{
    WIFI.println("OK");


}

void start_set(const String& value)
{
    int isr = value.toInt();
    xQueueSend(ADCQueue, &isr, 0);
    //WIFI.println("OK");
}

