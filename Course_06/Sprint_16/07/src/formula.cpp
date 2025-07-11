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
        Value Evaluate() const override {
            try {
                return ast_.Execute();
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
    try {

        std::unique_ptr<FormulaInterface> test = std::make_unique<Formula>(std::move(expression));

        return test;
    }
    catch (...) {
        throw FormulaException("");
    }
}