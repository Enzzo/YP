#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <unordered_map>
//
//class CellHasher {
//public:
//    size_t operator()(const Position p) const {
//        return std::hash<std::string>()(p.ToString());
//    }
//};
//
//class CellComparator {
//public:
//    bool operator()(const Position& lhs, const Position& rhs) const {
//        return lhs == rhs;
//    }
//};

class Sheet : public SheetInterface {
public:
    //using Table = std::unordered_map<Position, std::unique_ptr<Cell>, CellHasher, CellComparator>;

    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

    const Cell* GetCellPtr(Position pos) const;
    Cell* GetCellPtr(Position pos);

	// Можете дополнить ваш класс нужными полями и методами
private:
    void SetPrintableArea(Position pos) {
        cells_.resize(std::max(pos.row + 1, static_cast<int>(cells_.size())));
        cells_[pos.row].resize(std::max(pos.col + 1, static_cast<int>(cells_[pos.row].size())));
    }
    void Print(std::ostream& output,
        const std::function<void(const CellInterface&)>& PrintCellFunction) const;
private:
    //Table cells_;
    std::vector<std::vector<std::unique_ptr<Cell>>> cells_;
};