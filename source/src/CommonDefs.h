#pragma once

#include <cstdint>

enum class FileTypes {
    FILETYPE_TAP,
    FILETYPE_TZX,
    FILETYPE_CDT,
    FILETYPE_PZX,
    FILETYPE_DSK,
    FILETYPE_TRD,
    FILETYPE_Z80,
    FILETYPE_SNA,
    FILETYPE_CSW,
    FILETYPE_ERR
};

enum class RomVariant {
    ROM_48_EN,
    ROM_48_ES,
    ROM_128_EN,
    ROM_128_ES,
    ROM_PLUS2_EN,
    ROM_PLUS2_ES,
    ROM_PLUS3_EN,
    ROM_PLUS3_ES,
    ROM_PENTAGON,
    ROM_CPC464_EN,
    ROM_CPC464_ES,
    ROM_CPC464_FR,
    ROM_CPC664_EN,
    ROM_CPC6128_EN,
    ROM_CPC6128_ES,
    ROM_CPC6128_FR
};

struct InputMatrixPosition {

    uint8_t row;
    uint8_t key;
};

