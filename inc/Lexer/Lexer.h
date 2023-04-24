//
// Created by Administrator on 2023/4/24 0024.
//

#pragma once

#include <iostream>
#include "Token.h"

class Lexer {
public:
    Lexer(std::string file);
    Token getToken();
};