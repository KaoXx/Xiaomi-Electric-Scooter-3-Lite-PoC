#pragma once
#include "_base.h"

// Hacked by @KaoXx
// Discovered by @KaoXx


typedef enum {
    ScooterUnlock,
    ScooterLock,
    ScooterStateCOUNT,
} ScooterState;

typedef struct {
    ScooterState state;
    uint32_t mode;
} ScooterCfg;

extern const Protocol protocol_scooter;
