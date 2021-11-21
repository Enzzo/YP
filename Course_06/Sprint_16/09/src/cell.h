#pragma once

#include "common.h"
#include "formula.h"

#include <functional>
#include <unordered_set>
#include <optional>

class Sheet;

class Cell : public CellInterface {
public:
    Cell(Sheet& sheet);
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;

private:
    class Impl {
    public:
        virtual Value GetValue() const = 0;
        virtual std::string GetText() const = 0;

    protected:
        Value value_;
        std::string text_;
    };

    class EmptyImpl : public Impl {
    public:
        EmptyImpl() {
            value_ = text_ = "";
        }
        Value GetValue() const override {
            return value_;
        }

        std::string GetText() const override {
            return text_;
        }
    };
    class TextImpl : public Impl {
    public:
        TextImpl(std::string_view text) {
            text_ = text;
            if (text[0] == '\'') {
                text = text.substr(1);
            }
            value_ = std::string(text);
        }

        Value GetValue() const override {
            return value_;
        }
        std::string GetText() const override {
            return text_;
        }
    };
    class FormulaImpl : public Impl {
        std::unique_ptr<FormulaInterface> formula_ptr_;
        const SheetInterface& sheet_;
        mutable std::optional<FormulaInterface::Value> cache_;

    public:
        FormulaImpl(std::string_view expression, const SheetInterface& sheet)
        : sheet_(sheet){
            if (expression.empty() || expression[0] != FORMULA_SIGN) {
                throw std::logic_error("");
            }
            expression = expression.substr(1);
            value_ = std::string(expression);
            formula_ptr_ = ParseFormula(std::move(std::string(expression)));
            //text_ = "=" + formula_ptr_->GetExpression();
        }
        Value GetValue() const override {
            auto value = formula_ptr_->Evaluate(sheet_);
            if (std::holds_alternative<double>(value)) {
                return std::get<double>(value);
            }
            return std::get<FormulaError>(value);
        }
        std::string GetText() const override {
            return text_;
        }
    };
    std::unique_ptr<Impl> impl_;
    Sheet& sheet_;
    std::unordered_set<Cell*> l_nodes_;
    std::unordered_set<Cell*> r_nodes_;
    // Добавьте поля и методы для связи с таблицей, проверки циклических 
    // зависимостей, графа зависимостей и т. д.
};