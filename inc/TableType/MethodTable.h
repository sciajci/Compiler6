//
// Created by Administrator on 2023/4/23 0023.
//

#pragma once

#include <iostream>
#include <list>
#include "Table.h"
#include "EntryType/LocalVarEntry.h"

class MethodClass : public Table {
public:
    std::list<LocalVarEntry> lves;
};