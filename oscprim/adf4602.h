#ifndef ADF4602_H
#define ADF4602_H 
#include <stdint.h>

#define CS_PIN     PC14
#define SCL_PIN    PC15
#define MISO_PIN   PC13

#define TUNER_CS_HI() digitalWrite(CS_PIN,1)
#define TUNER_CS_LO() digitalWrite(CS_PIN,0)

#define TUNER_SDATA_HI() digitalWrite(MISO_PIN,1)
#define TUNER_SDATA_LO() digitalWrite(MISO_PIN,0)

#define TUNER_SCLK_HI() digitalWrite(SCL_PIN,1)
#define TUNER_SCLK_LO() digitalWrite(SCL_PIN,0)

#define WRITE_adf  0x00
#define SET_adf    0x01
#define CLEAR_adf  0x02
#define READ_adf   0x03

#define G1_0    0x0
#define G1_1    0x01
#define G2_0    0x0
#define G2_1    (1<<1)
#define G3_0    0x0
#define G3_1    (1<<2)
#define G4_0    0x0
#define G4_1    (1<<3)

#define F897      G4_1
#define F836      G3_1

#define F1747    (G4_1 | G3_1  | G2_1)
#define F1880     G2_1

//#define ADFDB  1

typedef union _ADF_CMD_t
{
    uint16_t  Data;
    struct {
        uint8_t   subadr : 8;
        uint8_t   cmd : 8;
    }__attribute__((packed));
}
ADF_CMD_t;

typedef union _ADF_t
{
    uint32_t  Data;
    struct  {
        uint32_t      cs : 3;
        uint32_t      op : 2;
        uint32_t      adr : 5;
        uint32_t     cmd : 16;
        uint32_t      nu : 6;
    }__attribute__((packed));
}
ADF_t;

ADF_t AdfCmd;
ADF_CMD_t  AdfSubCmd;
void delayS(int t)
{
    while (t > 1)t--;
}

void TunerWrite(uint32_t C)
{
    static  unsigned char Bit = 0;									// Bit Counter
    int c = 0;


//    debug_println(C);

    TUNER_SDATA_LO();
    TUNER_CS_LO();												// Start CS = Enable
    //TUNER_SCLK_LO(); 											// Standby SCLK 

    for (Bit = 0; Bit < 26; Bit++)								// 8 Bit Write
    {
        TUNER_SCLK_LO();											// Standby SCLK 
        delayS(10);
        c = (C & 0x02000000);
        if (c)
        {
            TUNER_SDATA_HI();
        }
        else
        {
            TUNER_SDATA_LO();
        }
        delayS(10);
        TUNER_SCLK_HI();											// Strobe Signal Bit(SDATA)
        delayS(10);
        C <<= 1;	 										// Next Bit Data
    }

    TUNER_SCLK_LO();  											// Standby SCLK 
    TUNER_CS_HI();
    AdfCmd.adr = 0;
    AdfCmd.cmd = 0;
    AdfCmd.cs = 0x01;
    AdfCmd.op = WRITE_adf;
    AdfCmd.nu = 0;
}

void TunerCmd(uint32_t C)
{
    if (/*Dev.Ary.PA ==*/1) {
        C |= 0x01;
    }
    AdfCmd.adr = 22;
    AdfCmd.cmd = (0x8000 | (C << 11));
#ifdef ADFDB 
    debug_print("TCMD ");
#endif
    TunerWrite(AdfCmd.Data);
}


uint32_t Ch2F(uint32_t arfcn)
{
    uint32_t result = 936600;

    if ((arfcn > 0) && (arfcn <= 124))
    {
        result = arfcn * 200 + 935000;
        //result -= 45000;
    }
    if ((arfcn > 511) && (arfcn <= 885))
    {
        result = ((arfcn - 512) * 200) + 1805200;
        //result -= 95000;
    }

    if (SerialUSB.isConnected()){
        SerialUSB.print("Freq ");
        SerialUSB.println(result);
    }
    return result;

}

void TunerPD(void)
{
    AdfCmd.adr = 1;
    AdfCmd.cmd = 0x1002;
    TunerWrite(AdfCmd.Data);
}


void TunerGain(int g)
{
    AdfCmd.adr = 11;
    AdfCmd.cmd = g;//G;   
#ifdef ADFDB 
    debug_print("Gain ");
#endif
    TunerWrite(AdfCmd.Data);

}

void TunerFreq(int ch) //936600  
{
    uint32_t F = Ch2F(ch);

    if (F < 970000) {
        AdfCmd.adr = 1;
        //AdfCmd.cmd=0x37C3;
        AdfCmd.cmd = 0x3fC3;
#ifdef ADFDB 
        debug_print("adr 1 0x37C3 ");
#endif
        TunerWrite(AdfCmd.Data);
        if (F > 870000) {
            TunerCmd(F897); //Dev.Chn.Band=1;

        }
        else{
            TunerCmd(F836); //Dev.Chn.Band=0;
        }
        AdfCmd.adr = 10;
        AdfCmd.cmd = F / 25u;
#ifdef ADFDB     
        debug_print("F/25 ");
#endif
        TunerWrite(AdfCmd.Data);
    }
    else{
        AdfCmd.adr = 1;
        //AdfCmd.cmd=0x37C7;         
        AdfCmd.cmd = 0x3fC7;
        TunerWrite(AdfCmd.Data);
        if (F > 1820000) {
            TunerCmd(F1880);//Dev.Chn.Band=3;
        }
        else{
            TunerCmd(F1747);//Dev.Chn.Band=2;
        }
        AdfCmd.adr = 10;
        AdfCmd.cmd = (int)(F / 50);
        TunerWrite(AdfCmd.Data);
    }

}

void ADF_init(void)
{
    AdfCmd.cs = 0x01;
    AdfCmd.op = WRITE_adf;
    AdfCmd.nu = 0;

    pinMode(CS_PIN, OUTPUT);
    pinMode(SCL_PIN, OUTPUT);
    pinMode(MISO_PIN, OUTPUT);


    TUNER_CS_HI();
    TUNER_SCLK_LO();
    TUNER_SDATA_LO();
}

void TunerInit(int A)
{
    ADF_init();

    AdfCmd.adr = 2;
    AdfCmd.cmd = 0x0003;
    TunerWrite(AdfCmd.Data);
    delay(50);
    AdfSubCmd.subadr = 151;
    AdfSubCmd.cmd = 0xE0;
    AdfCmd.adr = 0;
    AdfCmd.cmd = AdfSubCmd.Data;
    TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 31;
    AdfCmd.cmd = 0x0010;
    TunerWrite(AdfCmd.Data);
    delay(200);
    AdfCmd.adr = 31;
    AdfCmd.cmd = 0x0000;
    TunerWrite(AdfCmd.Data);
    AdfSubCmd.subadr = 151;
    AdfSubCmd.cmd = 0x6F;
    AdfCmd.adr = 0;
    AdfCmd.cmd = AdfSubCmd.Data;
    TunerWrite(AdfCmd.Data);
    //AdfCmd.adr=1; AdfCmd.cmd=0x2FC5; //0x2FC5-19.2  0x37C5-26  0x27C5

    //11011111000101        1,1,0,11111,0,0,0,10,1
    //11011111000011        1,1,0,11111,0,0,0,01,1   0x37C3  // 900
    //11011111000111        1,1,0,11111,0,0,0,11,1   0x37C7  //1800
    //
    // PD-01000000000010    0,1,0,00000,0,0,0,01,0  0x1002
    AdfCmd.adr = 1;
    AdfCmd.cmd = 0x37C3;           TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 12;
    AdfCmd.cmd = 0x0FA6;           TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 13;
    AdfCmd.cmd = 0x103E;           TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 14;
    AdfCmd.cmd = 0xEE53;           TunerWrite(AdfCmd.Data);
    //AdfCmd.adr=15; AdfCmd.cmd=0x0890;         TunerWrite(AdfCmd.Data);  // 1,0,0,010,01000,0   100111100010      0,0,0,000,00000,0
    AdfCmd.adr = 15;
    AdfCmd.cmd = 0x09E2;           TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 21;
    AdfCmd.cmd = 0x001F;           TunerWrite(AdfCmd.Data);
    // TunerCmd(F897G20);
    TunerCmd(F897);
    AdfCmd.adr = 0;
    AdfSubCmd.subadr = 144;
    AdfSubCmd.cmd = 0x06;
    AdfCmd.cmd = AdfSubCmd.Data;   TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 0;
    AdfSubCmd.subadr = 155;
    AdfSubCmd.cmd = 0x78;
    AdfCmd.cmd = AdfSubCmd.Data;  TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 0;
    AdfSubCmd.subadr = 153;
    AdfSubCmd.cmd = 0x85;
    AdfCmd.cmd = AdfSubCmd.Data;  TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 0;
    AdfSubCmd.subadr = 165;
    AdfSubCmd.cmd = 0x20;
    AdfCmd.cmd = AdfSubCmd.Data;  TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 0;
    AdfSubCmd.subadr = 170;
    AdfSubCmd.cmd = 0xF0;
    AdfCmd.cmd = AdfSubCmd.Data;   TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 0;
    AdfSubCmd.subadr = 171;
    AdfSubCmd.cmd = 0x00;
    AdfCmd.cmd = AdfSubCmd.Data;   TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 0;
    AdfSubCmd.subadr = 174;
    AdfSubCmd.cmd = 0x50;
    AdfCmd.cmd = AdfSubCmd.Data;   TunerWrite(AdfCmd.Data);
    AdfCmd.adr = 0;
    AdfSubCmd.subadr = 175;
    AdfSubCmd.cmd = 0x14;
    AdfCmd.cmd = AdfSubCmd.Data;   TunerWrite(AdfCmd.Data);


    TunerGain(80);
    //TunerFreq(26);//940.2
    TunerFreq(8);
}



#endif 



