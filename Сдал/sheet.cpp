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
    return GetCellPtr(pos);
}

CellInterface* Sheet::GetCell(Position pos) {
    return GetCellPtr(pos);
}

void Sheet::ClearCell(Position pos) {
    if (!pos.IsValid()) {
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
    Size size{ 0, 0 };
    for (int row = 0; row < static_cast<int>(cells_.size()); ++row) {
        for (int col = static_cast<int>(cells_[row].size()) - 1; col >= 0; --col) {
            if (const auto& cell = cells_[row][col]) {
                if (!cell->GetText().empty()) {
                    size.rows = std::max(size.rows, row + 1);
                    size.cols = std::max(size.cols, col + 1);
                    break;
                }
            }
        }
    }
    return size;
    /*if (cells_.begin() == cells_.end()) return { 0, 0 };

    Size result{ 0, 0 };
    
    for (auto it = cells_.begin(); it != cells_.end(); ++it) {
        if (it->second->GetText().size() == 0) continue;
        const int c = it->first.col;
        const int r = it->first.row;
        
        if (result.cols <= c) {
            result.cols = c + 1;
        }
        if (result.rows <= r) {
            result.rows = r + 1;
        }
    }
    return { result.rows, result.cols};*/
}

void Sheet::Print(std::ostream& output,
    const std::function<void(const CellInterface&)>& PrintCellFunction) const {
    auto size = GetPrintableSize();

    for (int row = 0; row < size.rows; ++row) {
        int last_non_empty_cell = static_cast<int>(cells_[row].size());

        for (int col = 0; col < size.cols; ++col) {
            if (col > 0) {
                output << '\t';
            }

            if (col < last_non_empty_cell) {
                if (const auto& cell = cells_[row][col]) {
                    PrintCellFunction(*cell);
                }
            }
        }
        output << '\n';
    }
}

void Sheet::PrintValues(std::ostream& output) const {
    Print(output, [&](const CellInterface& cell) {
        std::visit([&](const auto& value) { output << value; }, cell.GetValue());
        });
}

void Sheet::PrintTexts(std::ostream& output) const {
    Print(output, [&output](const CellInterface& cell) { output << cell.GetText(); });
}

const Cell* Sheet::GetCellPtr(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }

    if (pos.row >= static_cast<int>(cells_.size()) || pos.col >= static_cast<int>(cells_[pos.row].size())) {
        return nullptr;
    }

    return cells_[pos.row][pos.col].get();
}

Cell* Sheet::GetCellPtr(Position pos) {
    return const_cast<Cell*>(
        static_cast<const Sheet&>(*this).GetCellPtr(pos));
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}