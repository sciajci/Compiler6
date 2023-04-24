//
// Created by Administrator on 2023/4/24 0024.
//

#pragma once

#include <iostream>
#include <list>
#include "GlobalTable.h"

class RootTable : public Table {
    std::list<GlobalTable> gts;
};