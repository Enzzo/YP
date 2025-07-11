#include "FormulaAST.h"
#include "common.h"
#include "test_runner_p.h"

namespace {
double ExecuteASTFormula(const std::string& expression) {
    FormulaAST f = ParseFormulaAST(expression);
    std::cout << std::endl;
    f.PrintFormula(std::cout);
    double result = f.Execute();
    std::cout << " = " << result;
    return result;
}
}  // namespace

int main() {
    ASSERT_EQUAL(ExecuteASTFormula("1--"), 1.0);
    ASSERT_EQUAL(ExecuteASTFormula("1+2*((3-4))/5"), 0.6);
    try {
        ASSERT_EQUAL(ExecuteASTFormula("1+--20"), 21.0);
    } catch (const FormulaError& fe) {
        std::cout << fe.what() << std::endl;
    }

    std::cout << "\nTests Passed" << std::endl;
    return 0;
}