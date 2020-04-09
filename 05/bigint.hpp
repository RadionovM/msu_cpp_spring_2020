#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <limits>
#include <sstream>
class BigInt 
{
    int size;
    int sign;
    int8_t* data;
 public:
    BigInt();
    BigInt(int64_t x);
    ~BigInt();
    BigInt(const BigInt& other);
    const BigInt& operator=(const BigInt& other);
    const BigInt& operator=(int64_t x);

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator-() const;

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
   
    friend std::ostream& operator<<(std::ostream& out, const BigInt& value);

};


