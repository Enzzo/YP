#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <unordered_map>

class CellHasher {
public:
    size_t operator()(const Position p) const {
        return std::hash<std::string>()(p.ToString());
    }
};

class CellComparator {
public:
    bool operator()(const Position& lhs, const Position& rhs) const {
        return lhs == rhs;
    }
};

class Sheet : public SheetInterface {
public:
    using Table = std::unordered_map<Position, std::unique_ptr<Cell>, CellHasher, CellComparator>;

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
    void PrintableArea(Position pos) {
        int r = 0; 
        int c = 0; 
        for (auto it = cells_.begin(); it != cells_.end(); ++it) {
            int rm = it->first.row + 1;
            int cm = it->first.col + 1;
            if (r < rm) r = rm;
            if (c < cm) c = cm;
        }
        cells_[{r, c}];
    }

private:
    Table cells_;
};