#pragma once

#include <string>
#include <variant>

using namespace std;

enum class SymbolKind {
    CONSTANT,
    VARIABLE
};

enum class SymbolType {
    INTEGER,
    DOUBLE
};

enum class SymbolScope {
    LOCAL,
    GLOBAL
};

struct SymbolTableRecord {
    string name;
    SymbolType type;
    SymbolKind kind;
    variant<double, int> value;
    SymbolScope scope;

    SymbolTableRecord(string n, SymbolType t, SymbolKind k, variant<double, int> v, SymbolScope s = SymbolScope::GLOBAL)
        : name(n), type(t), kind(k), value(v), scope(s) {}
};