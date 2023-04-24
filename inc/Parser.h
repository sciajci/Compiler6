//
// Created by Administrator on 2023/4/23 0023.
//

#pragma once

#include <string>
#include <vector>
#include "TreeNode.h"
#include "TableType/GlobalTable.h"
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "TableType/RootTable.h"

class Parser {
public:
    Parser(std::string file);

    TreeNode* root();

private:
    static RootTable* root_table;

    static std::vector<short> modifier_first;
    static std::vector<short> a_modifier_first;
    static std::vector<short> type_first;

    Lexer l;
    Token current_token;
    TreeNode* current_node;
    Table* current_table;

    bool checkToken(std::vector<std::vector<short>> type_vectors);
    void match(const short type);
    std::string matchID();
    std::string matchNum();

    void start(TreeNode* cn);

    short accessModifier();
    std::array<bool, 3> modifiers();

    void globalStat(TreeNode* cn, short am, std::array<bool, 3> m);
    void localStat(TreeNode* cn, std::array<bool, 3> m);
    void statIds(std::vector<std::string>& ids);
    void initialize(std::vector<std::string> id);

    void disposeClass(TreeNode* cn, short am);
    void classAccessBlock(TreeNode* cn);
    void classBlock(TreeNode* cn, short am);

    void method(TreeNode* cn, short am, std::array<bool, 3> m);
    void params(TreeNode* cn, std::map<std::string, LocalVarEntry>& pes);
    void param(TreeNode* cn, std::map<std::string, LocalVarEntry>& pes);
    void methodBlock(TreeNode* cn);
    void returnTypes(TreeNode* cn, VarType& vt);
    void pushParams(TreeNode* cn);

    void expr(TreeNode* cn);
    void calExpr(TreeNode* cn);

    void leftValue(TreeNode* cn);
    void leftValueObject(TreeNode* cn);
    void rightValue(TreeNode* cn);
    void rightValueObject(TreeNode* cn);
    void rightValueVars(TreeNode* cn);
    void calculate(TreeNode* cn);
    void data(TreeNode* cn);
    void opp(TreeNode* cn);
    void type(TreeNode* cn);
    void types(std::vector<VarType>* vts);
};