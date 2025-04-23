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

struct SymbolTableRecord {
    string name;
    SymbolType type;
    SymbolKind kind;
    variant<double, int> value;

    SymbolTableRecord(string n, SymbolType t, SymbolKind k, variant<double, int> v)
        : name(n), type(t), kind(k), value(v) {}
};