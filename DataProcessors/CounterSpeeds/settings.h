#include "types.h"
#include "constants.h"

// TODO: pass directory name as an argument
#include "InData/Servo_0000/posSensorImage.h"
#include "InData/Servo_0000/friction.h"
#include "InData/Servo_0000/characteristics.h"

#pragma once

const s32 maxDelta = 108 * 2;
const s32 deltaIterations = 100;
const s32 printIndents = 2;
const s32 printValuesPerString = 10;

struct PowerModes {
    s32 current_mA[6] = {0, 100, 200, 400, 800, 1600};
    static constexpr size_t count = sizeof(current_mA) / sizeof(current_mA[0]);
    size_t idx = 0;

    void setMode(size_t m) {
      idx = m;
    }

    s32 getCurrent_mA() {
      return current_mA[idx];
    }
};
