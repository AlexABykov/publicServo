#include "../settings.h"
#include "../ServoModel/servoModel.h"
#include <fstream>
#include <iostream>

using namespace std;

struct FilePrinter {
    string_t pFileName = "OutData/counterCurve.h";
    ofstream rOutFile;
    bool fileOpened = false;

    void startFile(s32 modelNumber);

    void printBuffer(ServoState *pBuffer, bool lastBuffer);

    void finishFile();

    void printSpaces(s32 spacesCount);
};
