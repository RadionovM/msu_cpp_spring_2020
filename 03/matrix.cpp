#include "matrix.hpp"
#include <cstdlib>

Matrix::Matrix(size_t row, size_t col): rows(row, {col})
{}

Matrix::Row::Row(size_t col):row(col, 0)
{}

size_t Matrix::Row::size() const
{
    return row.size();
}

size_t Matrix::getRows() const
{
    return rows.size();
}

size_t Matrix::getColumns() const
{
    return (rows.size() ? rows.back().size() : 0);
}

void Matrix::operator *=(int mult)
{
    for(auto& row : rows)
    {
        for(auto& cell : row.row)
            cell *= mult;
    }
}

Matrix::Row& Matrix::operator [](size_t i)
{
    return rows.at(i);
}

int& Matrix::Row::operator [](size_t i)
{
    return row.at(i);
}
bool operator !=(const Matrix::Row& a, const Matrix::Row& b)
{
    return a.row != b.row;
}

bool operator ==(const Matrix& a, const Matrix& b)
{
    return !(a != b);
}

bool operator !=(const Matrix& a, const Matrix& b)
{
    if(a.getRows() != b.getRows() || a.getColumns() != b.getColumns())
        return true;
    auto it_a = a.rows.begin();
    auto it_b = b.rows.begin();
    while(it_a != a.rows.end() && it_b != b.rows.end())
    {
        if(*it_a != *it_b)
            return true;
        ++it_a;
        ++it_b;
    }
    if(it_a != a.rows.end() || it_b != b.rows.end())
        return true;
    return false;
}
