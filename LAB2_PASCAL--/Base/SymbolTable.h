#pragma once

#include <unordered_map>
#include <optional>
#include "SymbolTableRecord.h"

class SymbolTable {
private:
    unordered_map<string, SymbolTableRecord> symbols;

public:
    void insert(const SymbolTableRecord& record);
    optional<SymbolTableRecord> find(const string& name) const;
    bool update(const string& name, variant<double, int> newValue);
    bool delete_rec(const string& name);
};