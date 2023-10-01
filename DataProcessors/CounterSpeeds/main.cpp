// Program for generating points values of the approximation of idealSpeed = f(deltaAngle)
//   with given servo characteristics (InData/# directory) and a given set of PowerModes.
//   The result is called "CounterCurve" and is stored as a file which is used by servo angle non-linear P-regulator.

#include "ServoModel/servoModel.h"
#include "FilePrinter/filePrinter.h"
#include <iostream>

int main() {
  ServoModel servo;
  string_t m = servo.pMessageHeader;
  std::cout << m << "Counter-Speeds image file generator started." << std::endl;
  std::cout << m << "size of void pointer: " << sizeof(void *) * 8 << " bits" << std::endl;
  std::cout << m << "loaded settings." << std::endl;

  s32 divisionsCount = (s32) servo.posSensorSettings.gapDivisions;

  if (divisionsCount != 4) {
    std::cout << "INPUT DATA FATAL ERROR: only quad encoders are supported for now. Given divisions count \""
              << divisionsCount << "\" is unacceptable for implemented simulation." << std::endl;
    return -4;
  }

  servo.printInput();

  // friction current test example:
  f32 testSpeed = 0.1f;
  f32 testFriction = servo.friction.getFrictionCurrent(testSpeed);
  std::cout << servo.pTestHeader << "speed = " << testSpeed << " rad/s returned friction current = "
            << testFriction * 1000 << " mA" << std::endl;

  FilePrinter filePrinter;
  std::cout << endl;
  s32 printModelNumber = servo.characteristics.modelNumber;
  filePrinter.startFile(printModelNumber);
  auto powerModesCount = PowerModes::count;

  for (auto modeIdx = 0; modeIdx < powerModesCount; modeIdx++) {
    PowerModes *pM = &servo.powerModes;
    pM->setMode(modeIdx);
    std::cout << "\n" << m << "power mode idx = " << modeIdx << ", generating curves for current: "
              << pM->getCurrent_mA() << " mA" << std::endl;

    servo.buildCurve();
    bool isLastBuffer = (modeIdx == powerModesCount - 1);
    filePrinter.printBuffer(servo.buffer, isLastBuffer);
  }

  std::cout << endl;
  // TODO: implement tests - check the results,
  //  finish the out file or restore it's original state in case of getting of an unacceptable look.
  filePrinter.finishFile();
  return 0;
}
