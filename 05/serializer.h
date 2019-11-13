#pragma once
#include <iostream>
#include <string>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
private:
    std::ostream& out_;
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out_a) : out_(out_a) {}

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(const ArgsT... args)
    {
        return process(args...);
    }


private:
    // process uses variadic templates
    template <class T>
    Error process(T&& val)
    {
        return print_stream(val);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if ( process(val) == Error::CorruptedArchive ) return Error::CorruptedArchive;
        process(std::forward<Args>(args)...);
        return Error::NoError;
    }
    
    Error print_stream(uint64_t val)
    {
        out_ << val << Separator;
        return ( out_ ) ? Error::NoError : Error::CorruptedArchive;
    }

    Error print_stream(bool val)
    {
        out_ << std::boolalpha << val << Separator;
        return ( out_ ) ? Error::NoError : Error::CorruptedArchive;
    }
};



class Deserializer
{
private:
    std::istream& in_stream_;
    static constexpr char Separator = ' ';

public:
    explicit Deserializer(std::istream& in_a) : in_stream_(in_a) {}

    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }

private:
    // process uses variadic templates
    template <class T>
    Error process(T&& val)
    {
        return load_value(val);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if ( process(val) == Error::CorruptedArchive ) return Error::CorruptedArchive;
        process(std::forward<Args>(args)...);
        return Error::NoError;
    }

    Error load_value(uint64_t& value)
    {
        in_stream_ >> value;
        return ( in_stream_ ) ? Error::NoError : Error::CorruptedArchive;
    }

    Error load_value(bool& value)
    {
        in_stream_ >> std::boolalpha >> value;
        return ( in_stream_ ) ? Error::NoError : Error::CorruptedArchive;
    }
};

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};