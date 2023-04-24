//
// Created by Administrator on 2023/4/24 0024.
//

#pragma once

#include <iostream>
#include <array>
#include "VarTypes/BasicType.h"

class LocalVarEntry {
public:
    BasicType vt;
    std::array<bool, 3> modifier;
    short segment;
    int offset;
};