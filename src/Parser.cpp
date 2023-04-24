#include <algorithm>
#include"parser.h"
#include "TypeEnum.h"

RootTable* Parser::root_table = new RootTable();

std::vector<short> Parser::modifier_first = {STATIC};
std::vector<short> Parser::a_modifier_first = {PUBLIC, PRIVATE, PROTECTED};
std::vector<short> Parser::type_first = {SHORT};

Parser::Parser(std::string file) : l(file), current_table(root_table), current_node(new TreeNode()) {

}

//Root -> Start
TreeNode* Parser::root() {
    //建立根节点
    auto root_node = new TreeNode{ ROOT };

    start(root_node);

    return root_node;
}

//Current token 是否属于传入参数
bool Parser::checkToken(std::vector<std::vector<short>> type_vectors) {
    for (auto type_vector : type_vectors) {
        if (std::find(type_vector.begin(), type_vector.end(), current_token.type) != type_vector.end()) {
            return true;
        }
    }
    return false;
}

void Parser::match(short type) {
    if (current_token.type == type) {
        current_token = l.getToken();
    }
    else {
        throw "e";
    }
}

std::string Parser::matchID() {
    if (current_token.type == ID) {
        std::string value = current_token.value;
        current_token = l.getToken();
        return value;

    }
    else {
        throw "e";
    }
}

std::string Parser::matchNum() {
    if (current_token.type == NUM) {
        std::string value = current_token.value;
        current_token = l.getToken();
        return value;
    }
    else {
        throw "e";
    }
}

//Start -> AccessModifiers Class Start | AccessModifier Modifiers Method Start | AccessModifier Modifiers GlobalStat Start | END
void Parser::start(TreeNode* cn) {
    if (current_token.type != END) {
        short am = accessModifier();

        if (current_token.type == CLASS) {
            disposeClass(cn, am);
        }
        else if (checkToken({modifier_first})) {
            auto ms = modifiers();

            if (current_token.type == MET) {
                method(cn, am, ms);
            }
            else {
                globalStat(cn, am, ms);
            }
        }
    }
}

//AccessModifier -> public | private
short Parser::accessModifier() {
    if (current_token.type == PUBLIC) {
        current_token = l.next();
        return PUBLIC;
    }
    else if (current_token.type == PRIVATE) {
        current_token = l.next();
        return PRIVATE;
    }
    return PRIVATE;
}

//Class -> class ID { ClassAccessBolck }
void Parser::disposeClass(TreeNode* cn, short am) {
    TreeNode* class_node = cn->insertNode(CLASS);
    ClassTable* ct = new ClassTable(am);

    match(CLASS);
    std::string id = matchId();
    //插入类表，并更改current table
    reinterpret_cast<GlobalTable*>(current_table)->insertClassTable(id, ct);
    ct->parent = current_table;
    current_table = ct;
    match(OPEN_BRACES);
    classAccessBlock(class_node);
    match(CLOSE_BRACES);
    current_table = current_table->parent;
}

//ClassAccessBlock -> AccessModifier : ClassBlock ClassAccessBlock | NULL
void Parser::classAccessBlock(TreeNode* cn) {
    if (checkToken({ access_modifier_first })) {
        short am = accessModifier();
        match(COLON);
        classBlock(cn, am);
        classAccessBlock(cn);
    }
}

//ClassBlock -> Modifiers GlobalStat ClassBlock | Modifiers Method ClassBlock | NULL
void Parser::classBlock(TreeNode* cn, short am) {
    if (checkToken({ access_modifier_first, type_first, {MET} })) {
        auto m = modifiers();

        if (current_token.type == MET) {
            method(cn, am, m);
            classBlock(cn, am);
        }
        else {
            globalStat(am, m);
            classBlock(cn, am);
        }
    }
}

//Modifiers -> | static Modifiers | NULL
std::array<bool, 3> Parser::modifiers() {
    std::array<bool, 3> m;

    while (true) {
        if (current_token.type == STATIC) {
            current_token = l.next();
            m[0] = true;
        }
        else {
            return m;
        }
    }
}

//GlobalStat -> Type ID Ids Initialize
void Parser::globalStat(TreeNode* cn, short am, std::array<bool, 3> m) {
    VarType vt = type();
    std::vector<std::string> ids = { matchId() };
    this->statIds(ids);
    initialize(ids);
}

//LocalStat -> Type ID Ids Initialize
void Parser::localStat(TreeNode* cn, std::array<bool, 3> m) {
    VarType vt = type();
    std::vector<std::string> ids = { matchId() };
    statIds(ids);

    for (std::string id : ids) {
        //Insert local var entries
        reinterpret_cast<MethodTable*>(current_table)->insertVarEntry(id, m, vt);
    }

    initialize(ids);
}


//StatIds -> , ID StatIds | NULL
void Parser::statIds(std::vector<std::string>& ids) {
    while (current_token.type == COMMA) {
        current_token = l.next();
        ids.push_back(matchId());
    }
}

//Type -> int | short | ID | tuple < Types >
void Parser::type(TreeNode* cn) {
    if (current_token.type == INT) {
        current_token = l.next();
        cn->insertNode()
    }
    else if (current_token.type == SHORT) {
        current_token = l.next();
        return { SHORT };
    }
    else if (current_token.type == ID) {
        std::string class_id = current_token.value;
    }
    else if (current_token.type == TUPLE) {
        std::vector<VarType>* vts = new std::vector<VarType>();
        types(vts);
    }
    else {
        throw "e";
    }

}

//Types -> Type Types2
//Types2 -> , Types | NULL
void Parser::types(std::vector<VarType>* vts) {
    while (true) {
        vts->push_back(type());

        if (current_token.type == COMMA) {
            current_token = l.next();
        }
        else {
            return;
        }
    }
}

//Initialize -> = RightValue | NULL
void Parser::initialize(std::vector<std::string> ids) {
    if (current_token.type == EQUAL) {
        current_token = l.next();

        TreeNode* cal_expr_node = global_initialize_node->insertNode(CAL_EXPR);

        if (ids.size() == 1) {
            cal_expr_node->insertNode(ID, ids[0]);
        }
        else {
            cal_expr_node->insertNode(IDS, ids);
        }
        cal_expr_node->insertNode(EQUAL);

        rightValue(cal_expr_node);
    }
}

//LeftValue -> LeftObject LeftValueVars
//LeftValueVars -> , LeftValue | NULL
void Parser::leftValue(TreeNode* cn) {
    TreeNode* datas_node = cn->insertNode(DATAS);

    while (true) {
        leftValueObject(datas_node->insertNode(DATA));

        if (current_token.type == COMMA) {
            current_token = l.next();
        }
        else {
            break;
        }
    }
}

//LeftValueObject -> id CallVarObject
//CallVarObject -> . leftValueObject | NULL
void Parser::leftValueObject(TreeNode* cn) {
    while (true) {
        cn->insertNode(ID, matchId());

        if (current_token.type == POINT) {
            current_token = l.next();
        }
        else {
            break;
        }
    }
}

//RightValue -> ( Data Calculate ) | Data Calculate 
void Parser::rightValue(TreeNode* cn) {
    if (current_token.type == OPEN_PARENTHESIS) {
        current_token = l.next();
        data(cn);
        calculate(cn);
        match(CLOSE_PARENTHESIS);
    }
    else {
        data(cn);
        calculate(cn);
    }
}

//Data -> ID | NUM | CALL
void Parser::data(TreeNode* cn) {
    TreeNode* data_node = cn->insertNode(DATAS);

    if (current_token.type == ID) {
        std::string id = matchId();
        if (current_token.type == OPEN_PARENTHESIS) {
            current_token = l.next();
            pushParams(cn);
            match(CLOSE_PARENTHESIS);
        }
        else {
            data_node->insertNode(ID, id);
        }
    }
    else if (current_token.type == NUM) {
        std::string num = matchNum();
        data_node->insertNode(NUM, num);
    }
}

//Calculate -> Opp Data Calculate | NULL
void Parser::calculate(TreeNode* cn) {
    if (checkToken({ {ADD} })) {
        opp(cn);
        data(cn);
        rightValue(cn);
    }
}

//Opp -> +
void Parser::opp(TreeNode* cn) {
    if (current_token.type == ADD) {
        current_token = l.next();
        cn->insertNode(ADD);
    }
    else {
        throw "e";
    }
}

//Method -> met ID { MethodBlock }
void Parser::method(TreeNode* cn, short am, std::array<bool, 3> m) {
    TreeNode* method_node = cn->insertNode(MET);

    match(MET);
    std::string id = matchId();

    //Insert method table
    MethodTable* mt = new MethodTable(am, m);
    if (current_table->table_type == GLOBAL) {
        reinterpret_cast<GlobalTable*>(current_table)->insertMethodTable(id, mt);
    }
    else {
        reinterpret_cast<ClassTable*>(current_table)->insertMethodTable(id, mt);
    }

    params(cn, mt->para_var_entries);

    match(OPEN_BRACES);
    methodBlock(method_node);
    match(CLOSE_BRACES);
}

//MethodParams -> ( Param )
void Parser::params(TreeNode* cn, std::map<std::string, LocalVarEntry>& pes) {
    match(OPEN_PARENTHESIS);
    param(cn, pes);
    match(CLOSE_PARENTHESIS);
}

//Param -> Type ID Params
//Params -> , Param | NULL
void Parser::param(TreeNode* cn, std::map<std::string, LocalVarEntry>& pes) {
    //循环代替递归
    while (true) {
        VarType vt = type();
        std::string id = matchId();
        pes.insert({ id, {vt} });

        if (current_token.type == COMMA) {
            current_token = l.next();
        }
        else {
            return;
        }
    }
}

//ReturnTypes -> -> Types
void Parser::returnTypes(TreeNode* cn, VarType& vt) {
    std::vector<VarType>* vts = new std::vector<VarType>();

    if (current_token.type == ARROW) {
        current_token = l.next();

        types(vts);

    }
}

//MethodBlock -> Expr MethodBlock | NULL
void Parser::methodBlock(TreeNode* cn) {
    if (current_token.type != CLOSE_BRACES) {
        expr(cn);
        methodBlock(cn);
    }
}

//PushParam -> RightValueObject RightValueVars | NULL
void Parser::pushParams(TreeNode* cn) {
    if (current_token.type == ID, current_token.type == NUM) {
        rightValueObject(cn);
    }

    rightValueVars(cn);
}

//RightValueVars -> , PushParam
void Parser::rightValueVars(TreeNode* cn) {
    if (current_token.type == COMMA) {
        current_token = l.next();
        pushParams(cn);
    }
}

//RightValueObject -> ID CallObject | NUM CallObject | CallMethod CallObject
//CallObject -> . RightValueObject | NULL
void Parser::rightValueObject(TreeNode* cn) {
    while (true) {
        if (current_token.type == ID) {
            std::string id = matchId();
            if (current_token.type == OPEN_PARENTHESIS) {
                pushParams(cn->insertNode(CALL, id));
            }
            else {
                cn->insertNode(ID, id);
            }
        }
        else {
            cn->insertNode(NUM, matchNum());
        }

        if (current_token.type == POINT) {
            current_token = l.next();
        }
        else {
            return;
        }
    }
}

//Expr -> LocalStat | Calculate
void Parser::expr(TreeNode* cn) {
    TreeNode* expr_node = cn->insertNode(EXPR);

    if (current_token.type == ID) {
        calculate(cn);
    }
    else {
        auto m = modifiers();
        localStat(cn, m);
    }
}

//CalExpr -> LeftValue = RightValue
void Parser::calExpr(TreeNode* cn) {
    leftValue(cn);
    match(EQUAL);
    cn->insertNode(EQUAL);
    rightValue(cn);
}