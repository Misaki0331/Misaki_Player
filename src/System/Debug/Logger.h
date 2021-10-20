#include<M5Stack.h>
#include"../Build.h"
namespace Core{
    namespace Debug{
        class Logger{
            private:
            static void output(String text);
            public:
            static void Begin();
            static void Log(String text);
            static void Warn(String text);
            static void Error(String text);
            static void Fatal(String text);
            static String str(String text, String text2);
        };
    }
}