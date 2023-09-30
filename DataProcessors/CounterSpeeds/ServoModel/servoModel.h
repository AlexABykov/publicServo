#include "../settings.h"

#pragma once

struct ServoState {
    d64 time = 0;
    d64 speed = 0;
};

struct ServoModel {
    Friction friction;
    PosSensorSettings posSensorSettings;
    ServoCharacteristics characteristics;
    PowerModes powerModes;
    ServoState buffer[maxDelta + 1];
    static constexpr size_t bufferSize = sizeof(buffer) / sizeof(buffer[0]);
    s32 iterations = deltaIterations;
    d64 dt = 0;
    d64 totalTime = 0;
    d64 speed = 0;
    d64 dsIdeal = piTimes2 / ((s32) posSensorSettings.gaps * posSensorSettings.gapDivisions); // Build ideal curves
    d64 ds = dsIdeal / iterations / 2; // divide by 2 for the ds/2 quad step
    char const *pDebugMessageName = "SERVO_MODEL DEBUG: ";
    char const *pMessageHeader = "SERVO_MODEL MESSAGE: ";
    char const *pTestHeader = "SERVO_MODEL TEST: ";

    d64 getAcceleration();

    void updateState();

    void buildCurve();

    void printInput() const;
};

