#include"Logger.h"
using namespace Core::Debug;

void Logger::Begin(){
    
}
void Logger::Log(String text){
    #if DEBUG_LEVEL >= 4
    output(text);
    #endif
}

void Logger::Warn(String text){
    #if DEBUG_LEVEL >= 3
    output(text);
    #endif
}

void Logger::Error(String text){
    #if DEBUG_LEVEL >= 2
    output(text);
    #endif
}
void Logger::Fatal(String text){
    #if DEBUG_LEVEL >= 1
    output(text);
    #endif
}
void Logger::output(String text){
   // bool isStarted=Serial.available();
    //while(!Serial.available())Serial.begin(DEBUG_SPPED);
    Serial.println(text);
    //if(!isStarted)Serial.end();
}

String Logger::str(String text, String text2){
    String tmp=text;
    tmp.concat(text2);
    return tmp;
}