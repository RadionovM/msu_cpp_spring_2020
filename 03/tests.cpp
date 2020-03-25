#include <assert.h>
#include <stdio.h>
#include <stdexcept>
#include "matrix.hpp"


int main()
{
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m(rows, cols);

    assert(m.getRows() == 5);
    assert(m.getColumns() == 3);

    m[1][2] = 5; // строка 1, колонка 2
    double x = m[4][1];

    m *= 3; // умножение на число

    Matrix m1(rows, cols);
    assert(m1 != m);

    Matrix m2(rows+1, cols);
    assert(m2 != m1);

    bool out_of_range_catched {false};
    try
    {
        m[6][2] = 5; // строка 1, колонка 2
    }
    catch(const std::out_of_range& e)
    {
        out_of_range_catched = true;
    }
    assert(out_of_range_catched);

    const Matrix m_c(rows, cols);
    m_c[1][2] =5;
    assert(m_c != m); 
    return 0;
}
