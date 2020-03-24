#include <vector>

class Matrix
{
    public:
        Matrix(size_t row, size_t col);
        ~Matrix() = default;

        size_t getRows() const;
        size_t getColumns() const;

        class Row
        {
            Row(size_t col);
            size_t size() const;
            std::vector<int> row;

            public:
                int& operator [](size_t i);

            friend Matrix;
            friend bool operator !=(const Matrix::Row& a, const Matrix::Row& b);
        };

        Matrix::Row& operator [](size_t i);
        void operator *=(int mult);

    private:
        std::vector<Row> rows;
    friend bool operator ==(const Matrix& a, const Matrix& b);
    friend bool operator !=(const Matrix& a, const Matrix& b);
};

bool operator !=(const Matrix::Row& a, const Matrix::Row& b);
bool operator ==(const Matrix& a, const Matrix& b);
bool operator !=(const Matrix& a, const Matrix& b);
