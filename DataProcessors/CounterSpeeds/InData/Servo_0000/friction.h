// TODO: generate this file content outside
#include "cmath"

#pragma once

struct Friction {
    const f32 frictionStallCurrent = 0.125f;
    const f32 frictionWudMaxCurrent = 0.130f;
    const f32 frictionWudAvgCurrent = 0.07f;
    const f32 frictionWudMinCurrent = 0.05f;
    const f32 frictionMaxCurrent = 0.1f;
    const f32 frictionSlideCurrent = 0.05f;
    const f32 frictionStopCurrent = 0.096f;

    static f32 getLoadFriction(f32 speed) {
      return speed * 0;
    }

    f32 getFrictionCurrent(f32 speed) const {
      if (speed == 0) {
        return frictionStallCurrent;
      }

      f32 frictionCurrent;
      f32 dir = 0;

      if (speed > 0) {
        dir = 1;
      } else {
        if (speed < 0) {
          dir = -1;
        }
      }

      const f32 minimalSpeed = 0.00001f;

      if (fabsf(speed) <= minimalSpeed) {
        return frictionMaxCurrent * dir;
      }

      // https://https://www.desmos.com/calculator/a9qv9feseg
      f32 x = fabsf(speed);
      f32 s = 100;
      f32 f1 = s / pow(x + 0.125, 0.25) / 1.681;
      f32 k = 50;
      f32 f2 = pow(x, 1.1) / k;
      f32 f3 = (f1 + f2) / s;
      frictionCurrent = f3 * frictionMaxCurrent;
      frictionCurrent += getLoadFriction(speed);

      return fabsf(frictionCurrent) * dir;
    }
};

