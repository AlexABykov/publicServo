#include "servoModel.h"
#include <iostream>

d64 ServoModel::getAcceleration() {
  d64 inert = characteristics.inertia;
  d64 kT = characteristics.kt;
  d64 current = (d64) powerModes.getCurrent_mA() / 1000;
  d64 frictionCurrent = friction.getFrictionCurrent(speed);
  d64 currentEffective = current + frictionCurrent; // "+" because of negative time direction
  d64 torque = currentEffective * kT;
  d64 accel = torque / inert; // a = F / m
  return accel;
}

void ServoModel::buildCurve() {
  d64 inert = characteristics.inertia;
  d64 kT = characteristics.kt;
  f32 current = (f32) powerModes.getCurrent_mA() / 1000;

  buffer[0].time = 0;         // real time range ends with zero, but buffer is used in other direction
  buffer[0].speed = 0;        // target speed is set to zero

  // calculate landing time and last observed speed
  // (SSH in other positive time model), first iteration of the first segment
  f32 frictionCurrent = friction.getFrictionCurrent(0);
  f32 currentEffective = current + frictionCurrent; // "+" because of negative time direction
  d64 torque = currentEffective * kT;
  d64 accel = torque / inert; // F=ma -> a=F/m
  d64 halfDs = ds / 2;
  speed = sqrt(2 * accel * halfDs); // v = sqrt(2as)
  dt = sqrt(2 * halfDs / accel); // t = sqrt(2s/a)
  totalTime = -dt; // because of negative time direction

  // SSD, other iterations of the first segment
  for (s32 i = 1; i < iterations / 2; i++) {
    updateState();
  }

  buffer[1].time = totalTime;
  buffer[1].speed = speed; // just an unsigned speed for now

  // RUN, all iterations, first 2 points are already processed
  for (s32 p = 2; p < bufferSize; p++) {
    for (s32 i = 0; i < iterations; i++) {
      updateState();
    }

    buffer[p].time = totalTime; // negative time direction is already applied by using "totalTime -= dt"
    buffer[p].speed = speed;    // NOTE: calculated speed is always positive or zero for now

    if (p > (bufferSize - 5)) {
      std::cout << pDebugMessageName << "speed: " << speed << " rad/s" << std::endl;
      std::cout << pDebugMessageName << "minimal sustain time (expectation): " << abs(totalTime) * 1000 << " ms"
                << std::endl;
    }
  }
}

void ServoModel::updateState() {
  d64 accel = getAcceleration();
  d64 speed1 = sqrt(speed * speed + 2 * accel * ds); // v1^2 = v0^2 + 2as, speed1 is always positive
  d64 speedDelta = speed1 - speed; // never mind the direction changes through all of this model implementation
  dt = abs(speedDelta / accel); // dv = at -> t = dv/a
  totalTime -= dt; // minus because of negative time direction used as main in the "counter-speeds" project
  speed = speed1;  // speed is always positive too
}

void ServoModel::printInput() const {
  string_t m = pMessageHeader;
  std::cout << m << "model number: #" << characteristics.modelNumber << std::endl;
  std::cout << m << "gaps count: " << (s32) posSensorSettings.gaps << ", quad encoder" << std::endl;
  std::cout << m << "one turn: " << (s32) posSensorSettings.gaps * (s32) posSensorSettings.gapDivisions << " quads"
            << std::endl;
  std::cout << m << "max pos delta: |" << ServoModel::bufferSize - 1 << "|" << std::endl;
  std::cout << m << "Kt: " << characteristics.kt * 1000000 << " µ" << std::endl;
  std::cout << m << "Mj: " << characteristics.inertia * 1000000000 << "·10ˉ⁹ kg·m²" << std::endl;
}
