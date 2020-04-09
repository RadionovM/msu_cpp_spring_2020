#include "bigint.hpp"
#include  <cstring>

BigInt::BigInt(): size(1), sign(1)
{
  data = new int8_t[1]();
}

BigInt::BigInt(int64_t x) : sign(1), size(0)
{
    if(!x)
    {
        size = 1;
        data = new int8_t[1]();
        return;
    }
    if(x < 0)
    {
        sign = -1;
        x *= -1;
    }

    int64_t tmp = x;
    for(; tmp != 0; ++size)
        tmp = tmp / 10;

    data = new int8_t[size]();

    for (int i = size-1; i >= 0; --i)
    {
        data[i] = x % 10;
        x /= 10;
    }
}

BigInt::~BigInt()
{
    delete[] data;
}

BigInt::BigInt(const BigInt& other): size(other.size), sign(other.sign) 
{
    data = new int8_t[size];
    memcpy(data, other.data, size);
}

const BigInt& BigInt::operator=(const BigInt& other) 
{
    if (this == &other)
        return *this;
    delete[] data;
    int8_t* data= new int8_t[other.size];
    size = other.size;
    sign = other.sign;
    memcpy(data, other.data, size);
}

const BigInt& BigInt::operator=(int64_t x) 
{
    sign = 1;
    if(!x)
    {
        size = 1;
        data = new int8_t[1]();
        return *this;
    }

    if(x < 0)
    {
        sign = -1;
        x *= -1;
    }

    int64_t tmp = x;
    size = 0;
    for(; tmp != 0; ++size)
        tmp = tmp / 10;
    delete[] data;
    data = new int8_t[size]();

    for (int i = size-1; i >= 0; --i)
    {
        data[i] = x % 10;
        x /= 10;
    }
}

BigInt BigInt::operator+(const BigInt& other) const 
{
    if(sign * other.sign < 0)
    {
        if(sign < 0)
        {
            BigInt tmp(*this);
            tmp.sign = 1;
            return other - tmp;
        }
        else
        {
            BigInt tmp(other);
            tmp.sign = 1;
            return *this - tmp;
        }
    }
    int max_size = (size >= other.size ? size : other.size);
    int min_size = (size >= other.size ? other.size : size);
    int8_t* max_num= (size >= other.size ? data : other.data);
    int sum;
    BigInt tmp;
    tmp.size = max_size + 1;
    tmp.data = new int8_t[tmp.size + 1]();
    tmp.sign = sign;

    for(int i=0; i < min_size; ++i)
    {
        sum = data[size-1-i] + other.data[other.size-1-i]+ tmp.data[max_size-i];
        tmp.data[max_size-i] = sum % 10;
        tmp.data[max_size-1-i] += sum / 10;
    }
    for(int i=min_size; i < max_size; ++i)
    {
        sum = tmp.data[max_size-i] + max_num[max_size-1-i];
        tmp.data[max_size-i] = sum % 10;
        tmp.data[max_size-1-i] = sum / 10;
    }
    if(tmp.data[0] == 0)
    {
        int8_t *res = new int8_t[max_size];
        memcpy(res, tmp.data + 1, max_size);
        delete[] tmp.data;
        tmp.data = res;
        tmp.size = max_size;
    }
    return tmp;
}

BigInt BigInt::operator-(const BigInt& other) const 
{
    if(sign * other.sign < 0)
    {
        if(sign < 0)
        {
            BigInt tmp(other);
            tmp.sign = -1;
            return *this + tmp;
        }
        else
        {
            BigInt tmp(other);
            tmp.sign = 1;
            return *this + tmp;
        }
    }
    int max_size = (size > other.size ? size : other.size);
    int min_size = (size > other.size ? other.size : size);
    int8_t* max_num= (size > other.size ? data : other.data);
    int8_t* min_num= (size > other.size ? other.data : data);
    int sum;
    int equal {1};
    BigInt tmp;
    tmp.size = max_size + 1;
    tmp.data = new int8_t[tmp.size + 1]();
    tmp.sign = (size > other.size ? sign : -1*other.sign);

    if(size == other.size)
    {
        max_size = min_size = size;
        for (int i=0; i < size; ++i)
        {
            if (data[i] > other.data[i])
            {
                max_num = data;
                min_num = other.data;
                tmp.sign = sign;
                equal = 0;
                break;
            }
            else if (data[i] < other.data[i])
            {
                max_num = other.data;
                min_num = data;
                tmp.sign = -1 * other.sign;
                equal = 0;
                break;
            }
        }
        if(equal == 1)
        {
            tmp.size = 1;
            tmp.sign = 1;
            tmp.data = new int8_t[1];
            tmp.data[0] = 0;
            return tmp;
        }
    }
    tmp.size = max_size;
    tmp.data = new int8_t[max_size]();

    for (int i=0; i < min_size; ++i) 
    {
        sum = max_num[max_size-1-i] - min_num[min_size-1-i] + 10;
        tmp.data[max_size-1-i] += sum % 10;
        if (sum / 10 == 0) 
            tmp.data[max_size-2-i] = -1;
        if (tmp.data[max_size-1-i] == -1) 
        {
            tmp.data[max_size-1-i] = 9;
            tmp.data[max_size-2-i] = -1;
        }
    }
    for (int i=min_size; i < max_size; ++i) 
    {
        tmp.data[max_size-1-i] += max_num[max_size-1-i];
        if (tmp.data[max_size-i-1] == -1) 
        {
            tmp.data[max_size-i-1] = 9;
            tmp.data[max_size-i-2] = -1;
        }
    }
    int size_ = tmp.size;
    for (int i=0; i < tmp.size; ++i) 
    {
        if(tmp.data[i] == 0)
            size_--;
        else
            break;
    }
    int8_t *ptr = new int8_t[size_];
    for (int i = 0; i < size_; ++i)
        ptr[size_-1-i] = tmp.data[tmp.size-1-i];
    delete[] tmp.data;
    tmp.size = size_;
    tmp.data = ptr;
    return tmp;
}

BigInt BigInt::operator-() const
{
    BigInt tmp(*this);
    tmp.sign = -1;
    return tmp;
}

bool BigInt::operator==(const BigInt& other) const 
{
    if (this == &other)
        return true;
    if (size != other.size) 
        return false;
    for (int i=0; i < size; ++i) 
        if (data[i] != other.data[i]) 
            return false;
    return true;
}

bool BigInt::operator!=(const BigInt& other) const 
{
    return !(*this == other);
}

bool BigInt::operator>(const BigInt& other) const 
{
    if (this == &other) 
        return false;
    if (sign * size > other.sign * other.size) 
        return true;
    else if (sign * size < other.sign * other.size) 
        return false;
    if (sign == 1 && other.sign == -1) 
        return true;
    else if (sign == -1 && other.sign == 1) 
        return false;
    for (int i=0; i < size; ++i) 
    {
        if (data[i] > other.data[i]) 
        {
            if (sign == 1)
                return true;
            else
                return false;
        }
        else if (data[i] < other.data[i]) 
        {
            if (sign == 1)
                return false;
            else
                return true;
        }
    }
    return false;
}

bool BigInt::operator<(const BigInt& other) const 
{
    if(*this == other)
        return false;
    return !(*this > other);
}

bool BigInt::operator>=(const BigInt& other) const 
{
    return !(*this < other);
}

bool BigInt::operator<=(const BigInt& other) const 
{
    return !(*this > other);
}


std::ostream& operator<<(std::ostream& out, const BigInt& value) 
{
    if (value.sign == -1)
        out << '-';
    for (int i=0; i < value.size; ++i)
        out << int(value.data[i]);
    return out;
}
