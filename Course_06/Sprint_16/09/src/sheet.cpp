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
    SetPrintableArea(pos);
    //cells_[pos]->Set(text);
    auto& cell = cells_[pos.row][pos.col];
    if (!cell) {
        cell = std::make_unique<Cell>(*this);
    }
    cell->Set(std::move(text));
}

const CellInterface* Sheet::GetCell(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException("invalid position");
    }
    if (pos.row >= static_cast<int>(cells_.size()) || pos.col >= static_cast<int>(cells_[pos.row].size())) {
        return nullptr;
    }
    return cells_[pos.row][pos.col].get();
}
CellInterface* Sheet::GetCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("invalid position");
    }
    if (pos.row >= static_cast<int>(cells_.size()) || pos.col >= static_cast<int>(cells_[pos.row].size())) {
        return nullptr;
    }
    return cells_[pos.row][pos.col].get();
}

void Sheet::ClearCell(Position pos) {
    Position p{ pos };
    if (!p.IsValid()) {
        throw InvalidPositionException("invalid position");
    }
    if (pos.row < static_cast<int>(cells_.size()) && pos.col < static_cast<int>(cells_[pos.row].size())) {
        if (auto& cell = cells_[pos.row][pos.col]) {
            cell->Clear();
            cell.reset();
        }
    }
}

Size Sheet::GetPrintableSize() const {

    if (cells_.begin() == cells_.end()) return { 0, 0 };

    Size result{ 0, 0 };
    
    for (auto it = cells_.begin(); it != cells_.end(); ++it) {
        //if (it->second->GetText().size() == 0) continue;
        //const int c = it->first.col;
        //const int r = it->first.row;
        
        //if (result.cols <= c) {
        //    result.cols = c + 1;
        //}
        //if (result.rows <= r) {
        //    result.rows = r + 1;
        //}
    }
    return { result.rows, result.cols};
}

void Sheet::PrintValues(std::ostream& output) const {
    Size size = GetPrintableSize();
    for (int r = 0; r < size.rows; ++r) {

        bool first = true;
        //for (int c = 0; c < size.cols; ++c) {
        //    if (!first) {
        //        output << "\t";
        //    }
        //    first = false;
        //    Position pos = { r, c };
        //    auto it = cells_.find(pos);
        //    if (it != cells_.end()) {
        //        auto v = cells_.at(pos)->GetValue();
        //        std::visit([&output](auto&& arg) {output << arg; }, v);
        //    }
        //}
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
            /*auto it = cells_.find(pos);
            if (it != cells_.end()) {
                output << cells_.at(pos)->GetText();
            }*/
        }
        output << "\n";
    }
}

const Cell* Sheet::GetCellPtr(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid pos");
    }

    int row_size = 0;
    for (auto it = cells_.begin(); it != cells_.end(); ++it) {
        //int r = it->first.row;
        //if (row_size < r) row_size = r;
    }

    if (pos.row >= static_cast<int>(cells_.size()) || pos.col >= static_cast<int>(row_size)) {
        return nullptr;
    }

    //return cells_.at({ pos.row, pos.col }).get();
    return {};
}

Cell* Sheet::GetCellPtr(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid pos");
    }

    int row_size = 0;
    for (auto it = cells_.begin(); it != cells_.end(); ++it) {
        //int r = it->first.row;
        //if (row_size < r) row_size = r;
    }

    if (pos.row >= static_cast<int>(cells_.size()) || pos.col >= static_cast<int>(row_size)) {
        return nullptr;
    }

    //return cells_.at({ pos.row, pos.col }).get();
    return {};
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}