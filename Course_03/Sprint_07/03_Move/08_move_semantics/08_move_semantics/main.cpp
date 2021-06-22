#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

#include "simple_vector.h"
#include "tests.h"

int main() {
    TestNoncopiableMoveConstructor();
    SimpleVectorFirstPartPraktikumTest();
    SimpleVectorSecondPartPraktikumTest();
    TestReserveConstructor();
    TestReserveMethod();
    TestTemporaryObjConstructor();
    TestTemporaryObjOperator();
    TestNamedMoveConstructor();
    TestNamedMoveOperator();
    TestNoncopiablePushBack();
    TestNoncopiableInsert();
    TestNoncopiableErase();
    return 0;
}