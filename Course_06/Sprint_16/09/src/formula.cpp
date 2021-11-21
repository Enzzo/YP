#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>

using namespace std::literals;

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
    return output << "#DIV/0!";
}

namespace {
    class Formula : public FormulaInterface {
    public:
        // Реализуйте следующие методы:
        explicit Formula(std::string expression)        
            :ast_(ParseFormulaAST(std::move(expression)))
        {
            
        }
        Value Evaluate(const SheetInterface& sheet) const override {
            const SheetArgs args = [&sheet](const Position p) -> double {
                if (!p.IsValid()) {
                    throw FormulaError(FormulaError::Category::Ref);
                }
                const auto* cell = sheet.GetCell(p);
                if (!cell) {
                    return 0;
                }
                                if (std::holds_alternative<double>(cell->GetValue())) {
                    return std::get<double>(cell->GetValue());
                }

                if (std::holds_alternative<std::string>(cell->GetValue())) {
                    auto value = std::get<std::string>(cell->GetValue());
                    double result = 0;
                    if (!value.empty()) {
                        std::istringstream in(value);
                        if (!(in >> result) || !in.eof()) {
                            throw FormulaError(FormulaError::Category::Value);
                        }
                    }
                    return result;
                }
                throw FormulaError(std::get<FormulaError>(cell->GetValue()));
            };

            try {
                return ast_.Execute(args);
            }
            catch (FormulaError& e) {
                return e;
            }
        }
        std::string GetExpression() const override {
            std::ostringstream result;
            ast_.PrintFormula(result);
            return result.str();
        }

    private:
        const FormulaAST ast_;        
    };
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression) {
    //TODO: если строка expression содержит синтаксически некорректную формулу, то метод должен кинуть исключение FormulaException
    try {
        return std::make_unique<Formula>(std::move(expression));
    }
    catch (...) {
        throw FormulaException("");
    }
}