#include <vector>
class Matrix;
bool operator ==(const Matrix& a, const Matrix& b);
bool operator !=(const Matrix& a, const Matrix& b);
class Matrix
{
    public:
    Matrix(size_t row, size_t col);
    size_t getRows() const;
    size_t getColumns() const;
    class Row
    {
        Row(size_t col);
        size_t size() const;
        std::vector<int> row;
        friend Matrix;
        friend bool operator !=(const Matrix::Row& a, const Matrix::Row& b);
        public:
            int& operator [](size_t i);
    };
    void operator *=(int mult);
    Matrix::Row& operator [](size_t i);
    private:
        std::vector<Row> rows;
    size_t row;
    size_t col;
    friend bool operator ==(const Matrix& a, const Matrix& b);
    friend bool operator !=(const Matrix& a, const Matrix& b);
};
bool operator !=(const Matrix::Row& a, const Matrix::Row& b);
