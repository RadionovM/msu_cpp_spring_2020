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

const Matrix& Matrix::operator *=(int mult)
{
    for(auto& row : rows)
    {
        for(auto& cell : row.row)
            cell *= mult;
    }
    return *this;
}

Matrix::Row& Matrix::operator [](size_t i)
{
    return rows.at(i);
}

Matrix::Row Matrix::operator [](size_t i) const
{
    return rows.at(i);
}

int& Matrix::Row::operator [](size_t i)
{
    return row.at(i);
}

int Matrix::Row::operator [](size_t i) const
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
    for(size_t i = 0; i < a.getRows(); ++i)
        if(a[i] != b[i])
            return true;
    return false;
}
