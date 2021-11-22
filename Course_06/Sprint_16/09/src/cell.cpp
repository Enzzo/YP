#include "cell.h"

#include <cassert>
#include <iostream>
#include <string>
#include <optional>
#include <stack>

// Реализуйте следующие методы
Cell::Cell(Sheet& sheet)
	:impl_(std::make_unique<EmptyImpl>())
	,sheet_(sheet)
{
	
}

Cell::~Cell() {

}

std::vector<Position> Cell::GetReferencedCells() const { return impl_->GetReferencedCells(); }

void Cell::Set(std::string text) {
    std::unique_ptr<Impl> impl;

    if (text.empty()) {
        impl = std::make_unique<EmptyImpl>();
    }
    else if (text.size() > 1 && text[0] == FORMULA_SIGN) {
        impl = std::make_unique<FormulaImpl>(std::move(text), sheet_);
    }
    else {
        impl = std::make_unique<TextImpl>(std::move(text));
    }

    if (WouldIntroduceCircularDependency(*impl)) {
        throw CircularDependencyException("Setting this formula would introduce circular dependency!");
    }

    impl_ = std::move(impl);

    for (Cell* outgoing : r_nodes_) {
        outgoing->l_nodes_.erase(this);
    }

    r_nodes_.clear();
    
    for (const auto& pos : impl_->GetReferencedCells()) {
        Cell* outgoing = sheet_.GetCellPtr(pos);
        
        if (!outgoing) {
            sheet_.SetCell(pos, "");
            outgoing = sheet_.GetCellPtr(pos);
        }

        r_nodes_.insert(outgoing);
        outgoing->l_nodes_.insert(this);
    }

    InvalidateCacheRecursive(true);
}

void Cell::InvalidateCacheRecursive(bool force) {
    impl_->InvalidateCache();

    for (Cell* incoming : l_nodes_) {
        incoming->InvalidateCacheRecursive();
    }
}

bool Cell::WouldIntroduceCircularDependency(const Impl& new_impl) const {
    if (new_impl.GetReferencedCells().empty()) {
        return false;
    }

    std::unordered_set<const Cell*> referenced;
    for (const auto& pos : new_impl.GetReferencedCells()) {
        referenced.insert(sheet_.GetCellPtr(pos));
    }

    std::unordered_set<const Cell*> visited;
    std::stack<const Cell*> to_visit;
    to_visit.push(this);
    while (!to_visit.empty()) {
        const Cell* current = to_visit.top();
        to_visit.pop();
        visited.insert(current);

        if (referenced.find(current) != referenced.end()) {
            return true;
        }

        for (const Cell* incoming : current->l_nodes_) {
            if (visited.find(incoming) == visited.end()) {
                to_visit.push(incoming);
            }
        }
    }

    return false;
}

void Cell::Clear() {
	impl_ = std::make_unique<EmptyImpl>();
}

Cell::Value Cell::GetValue() const {
	return impl_->GetValue();
}
std::string Cell::GetText() const {
	return impl_->GetText();
}