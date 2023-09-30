#include "filePrinter.h"

void FilePrinter::startFile(s32 modelNumber) {
  rOutFile.open(pFileName);
  PowerModes powerModes;

  if (!rOutFile.fail()) {
    fileOpened = true;
    auto powerModesCount = PowerModes::count;
    std::cout << "FILE MESSAGE: successfully opened file \"" << pFileName << "\"" << " for writing" << std::endl;
    rOutFile << "// Generated File. DON'T EDIT\n// fileFormat: \"*.h, C++\"\n";
    rOutFile << "// max described distance: " << maxDelta << "\n";
    rOutFile << "// used " << powerModesCount << " power modes: ";

    for (auto i = 0; i < powerModesCount; i++) {
      powerModes.idx = i;
      rOutFile << powerModes.getCurrent_mA();

      if (i < powerModesCount - 1) {
        rOutFile << ", ";
      }
    }

    rOutFile << " mA\n";

    rOutFile << "\nstruct CounterSpeeds {";
    rOutFile << "\n";
    printSpaces(1);
    rOutFile << "const s32 modelNumber = " << modelNumber << ";\n";
    printSpaces(1);
    rOutFile << "const s8 powerModesCount = " << powerModesCount << ";\n";
    printSpaces(1);
    rOutFile << "const s16 counterSpeedsSize = " << ServoModel::bufferSize - 1 << ";\n";

    rOutFile << "\n";
    printSpaces(1);
    rOutFile << "const s32 absCounterSpeed_mRad[" << powerModesCount << "][" << ServoModel::bufferSize - 1 << "] = {";
    // "bufferSize - 1" as the first element is omitted - it always contains zeroes
  } else {
    fileOpened = false;
    std::cout << "FILE FATAL ERROR: file \"" << pFileName << "\" open or creation failed" << std::endl;
  }
}

void FilePrinter::printBuffer(ServoState *pBuffer, bool lastBuffer) {
  if (fileOpened) {
    auto bufSize = ServoModel::bufferSize;
    std::cout << "FILE DEBUG: buffer[1].speed = " << pBuffer[1].speed << " rad/s" << std::endl;
    rOutFile << "\n";
    printSpaces(2);
    rOutFile << "{\n";

    for (auto i = 1; i < bufSize; i++) {
      if (i > 1) {
        rOutFile << ", ";

        if ((i - 2) % printValuesPerString == printValuesPerString - 1) {
          rOutFile << "\n";
          printSpaces(3);
        }
      } else {
        printSpaces(3);
      }

      auto r = lround(abs(pBuffer[i].speed) * 1000.0);
      rOutFile << r;
    }

    rOutFile << "\n";
    printSpaces(2);
    rOutFile << "}";

    if (!lastBuffer) {
      rOutFile << ",";
    }

    rOutFile << "\n";
  }
}

void FilePrinter::finishFile() {
  if (fileOpened) {
    printSpaces(1);
    rOutFile << "};\n};\n";
    auto bufSize = ServoModel::bufferSize;
    d64 totalBytes = ((d64) bufSize - 1) * PowerModes::count * sizeof(s32);
    // "bufferSize - 1" as the first element is omitted - it always contains zeroes
    bool kilo = false;

    if (totalBytes >= 1000) {
      kilo = true;
      totalBytes /= 1000;
    }

    rOutFile << "\n// total data size: " << totalBytes << " ";

    if (kilo) {
      rOutFile << "kilo";
    }

    rOutFile << "Bytes\n";
    rOutFile << "// End of Generated File.\n";
    rOutFile.close();
  }

  std::cout << "FILE MESSAGE: successfully saved file \"" << pFileName << "\"" << std::endl;
}

void FilePrinter::printSpaces(s32 spacesCount) {
  for (s32 i = 0; i < spacesCount * printIndents; i++) {
    rOutFile << " ";
  }
}
