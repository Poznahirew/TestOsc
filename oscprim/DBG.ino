
//#define DEBUG 
#define DEBUGSERIAL WIFI

void debug_print(char c){
#ifdef DEBUG   
    DEBUGSERIAL.print(c);
#endif
}

void debug_print(String str){
#ifdef DEBUG   
    DEBUGSERIAL.print(str);
#endif
}

void debug_println(String str){
#ifdef DEBUG   
    DEBUGSERIAL.println(str);
#endif
}

