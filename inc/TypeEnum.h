//
// Created by Administrator on 2023/4/24 0024.
//

#pragma once

#include <iostream>

enum TypeEnum {

    //Access Modifier
    PUBLIC, PROTECTED, PRIVATE,
    //Table Type
    GLOBAL,
    //Symbol
    OPEN_PARENTHESIS, CLOSE_PARENTHESIS, OPEN_BRACKETS, CLOSE_BRACKETS, OPEN_BRACES, CLOSE_BRACES,
    ADD, SUB, MUL, DIV,
    EQUAL, ADD_EQUAL, SUB_EQUAL,
    INC, DEC,
    GREATER, LESS,
    SEM, COMMA, COLON,
    //AST node type
    ROOT, FUN_BLOCK, EXPR, LEFT_VALUE, DATA, RIGHT_VALUE, CALL, REG, MOV,
    EAX, EBX, ECX, EDX, START,
    //Token type
    ID, NUM, CLASS, MET,
    //Var type
    SHORT, TUPLE,
    //Modifier
    STATIC,
    //Method modifier
    INLINE,
    STACK, OFFSET, TMP, PUSH, IDS, CAL_EXPR, ARROW, INT, DATAS,POINT, END
};