#include <vector>
#include <ostream>
#include <istream>
#include <iostream>
#include <ctype.h>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char separator = ' ';

public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }

private:
    template <class T>
    Error process(T&& val)// по сути для uint64 только
    {
        out_ << val << separator;
        return Error::NoError;
    }

    Error process(bool val)//??? не специализация а просто перегрузка 
    {
        out_ << (val ? "true" : "false") << separator;
        return Error::NoError;
    }

    template <class Arg, class... Args>
    Error process(Arg&& val, Args&&... args)
    {
        process(val);
        return process(std::forward<Args>(args)...);
    }


    std::ostream& out_;
};

class Deserializer
{
    static constexpr char separator = ' ';

public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }

private:
    template <class T>
    Error process(T& val)
    {
        std::string text;
        in_ >> text;
        try
        {
            if(!text.empty() && text[0] == '-')
                throw std::out_of_range("");
            val = std::stoull(text);
        }
        catch(std::invalid_argument)
        {
            return Error::CorruptedArchive;
        }
        catch(std::out_of_range)
        {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(bool& val)//??? не специализация а просто перегрузка 
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            val = true;
        else if (text == "false")
            val = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    template <class Arg, class... Args>
    Error process(Arg& val, Args&&... args)
    {
        if(process(val) != Error::NoError)
            return Error::CorruptedArchive;
        return process(std::forward<Args>(args)...);
    }

    std::istream& in_;
};
