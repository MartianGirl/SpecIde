#pragma once

enum class JoystickType {
    KEMPSTON_OLD,
    KEMPSTON_NEW,
    FULLER,
    CURSOR,
    SINCLAIR
};

enum class SnapshotModel {
    ZX_48K_ISSUE2,
    ZX_48K_ISSUE3,
    ZX_128K,
    ZX_PLUS2,
    ZX_PLUS2A,
    ZX_PLUS3,
    PENTAGON,
    UNSUPPORTED
};

struct SaveState {

    uint16_t af = 0x0000;
    uint16_t bc = 0x0000;
    uint16_t de = 0x0000;
    uint16_t hl = 0x0000;

    uint16_t af_ = 0x0000;
    uint16_t bc_ = 0x0000;
    uint16_t de_ = 0x0000;
    uint16_t hl_ = 0x0000;

    uint16_t pc = 0x0000;
    uint16_t sp = 0x0000;
    uint16_t ir = 0x0000;

    uint16_t ix = 0x0000;
    uint16_t iy = 0x0000;

    uint16_t iff = 0x0000;
    uint8_t im = 0x00;

    uint8_t border = 0x00;
    JoystickType joystick = JoystickType::SINCLAIR;
    bool issue2 = false;
    bool samrom = false;

    SnapshotModel model = SnapshotModel::UNSUPPORTED;

    uint8_t port_0x1ffd = 0x00;
    uint8_t port_0x7ffd = 0x00;
    uint8_t port_0xfffd = 0x00;
    uint8_t ayRegs[16];

    bool emuAy8912 = false;
    bool emuFuller = false;
    bool emuRefresh = false;

    uint32_t tStates = UINT32_MAX;

    bool dataIsCompressed = false;
    std::vector<uint8_t> memory[16];
};

// vim: et:sw=4:ts=4:
