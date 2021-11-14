#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

Sheet::~Sheet() {}

void Sheet::SetCell(Position pos, std::string text) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("invalid position");
    }
    cells_[pos].Set(text);
}

const CellInterface* Sheet::GetCell(Position pos) const {
    Position p{ pos };
    if (!p.IsValid()) {
        throw InvalidPositionException("invalid position");
    }
    auto it = cells_.find(p);
    if (it == cells_.end()) {
        return nullptr;
    }

    const Cell* c = &(cells_.at(p));
    if (c->GetText().size() == 0) {
        return nullptr;
    }
    return c;
}
CellInterface* Sheet::GetCell(Position pos) {
    Position p{ pos };
    if (!p.IsValid()) {
        throw InvalidPositionException("invalid position");
    }
    auto it = cells_.find(p);
    if (it == cells_.end()) {
        return nullptr;
    }
    Cell* c = &(cells_.at(p));
    if (c->GetText().size() == 0) {
        return nullptr;
    }
    return c;
}

void Sheet::ClearCell(Position pos) {
    Position p{ pos };
    if (!p.IsValid()) {
        throw InvalidPositionException("invalid position");
    }
    cells_[pos].Clear();
}

Size Sheet::GetPrintableSize() const {

    if (cells_.begin() == cells_.end()) return { 0, 0 };

    Size result{ 0, 0 };
    
    for (auto it = cells_.begin(); it != cells_.end(); ++it) {
        if (it->second.GetText().size() == 0) continue;
        const int c = it->first.col;
        const int r = it->first.row;
        
        if (result.cols <= c) {
            result.cols = c + 1;
        }
        if (result.rows <= r) {
            result.rows = r + 1;
        }
    }
    return { result.rows, result.cols};
}

void Sheet::PrintValues(std::ostream& output) const {
    Size size = GetPrintableSize();
    for (int r = 0; r < size.rows; ++r) {

        bool first = true;
        for (int c = 0; c < size.cols; ++c) {
            if (!first) {
                output << "\t";
            }
            first = false;
            Position pos = { r, c };
            auto it = cells_.find(pos);
            if (it != cells_.end()) {
                auto v = cells_.at(pos).GetValue();
                std::visit([&output](auto&& arg) {output << arg; }, v);
            }
        }
        output << "\n";
    }
}
void Sheet::PrintTexts(std::ostream& output) const {
    Size size = GetPrintableSize();
    for (int r = 0; r < size.rows; ++r) {
        
        bool first = true;
        for (int c = 0; c < size.cols; ++c) {
            if (!first) {
                output << "\t";                
            }
            first = false;
            Position pos = { r, c };
            auto it = cells_.find(pos);
            if (it != cells_.end()) {
                output << cells_.at(pos).GetText();
            }
        }
        output << "\n";
    }
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}