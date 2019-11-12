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
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }

private:
    // process uses variadic templates
    template <class T>
    Error process(T&& val)
    {
        if (std::is_same<T, uint64_t&>::value) {

            out_ << val << Separator;
        }
        else if (std::is_same<T, bool&>::value) {

            out_ << (val ? "true" : "false") << Separator;
        }
        else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;

    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (std::is_same<T, uint64_t&>::value) {

            out_ << val << Separator;
        }
        else if (std::is_same<T, bool&>::value) {

            out_ << (val ? "true" : "false") << Separator;
        }
        else {
            return Error::CorruptedArchive;
        }

        process(std::forward<Args>(args)...);
        return Error::NoError;
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
        return load(val);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&... args)
    {
        if (load(val) == Error::CorruptedArchive) return Error::CorruptedArchive;
        process(std::forward<Args&>(args)...);
        return Error::NoError;
    }

    Error load(uint64_t& value)
    {
        in_stream_ >> value;

        if (in_stream_.good()) {
            return Error::NoError;
        }
        else {
            return Error::CorruptedArchive;
        }
    }

    Error load(bool& value)
    {
        std::string text;
        in_stream_ >> text;

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
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
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};
