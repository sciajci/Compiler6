//
// Created by Administrator on 2023/4/23 0023.
//

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "BasicType.h"

class Tuple : public BasicType {
    std::shared_ptr<std::vector<BasicType>> tuple_types;
};