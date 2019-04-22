#pragma once
#include <iostream>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
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
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::ostream &out_;
    // process использует variadic templates
    Error save(const bool val) {
        if (val)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;

        return Error::NoError;
    }

    Error save(const uint64_t val) {
        out_ << val << Separator;
        return Error::NoError;
    }

    template<class T>
    Error process(T&& val) {
        return save(std::forward<T>(val));
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args) {
        Error error = save(std::forward<T>(val));
        if (error == Error::NoError)
            return process(std::forward<Args>(args)...);
        else
            return error;
    }
};

class Deserializer {
    static constexpr char Separator = ' ';
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
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::istream &in_;
    // process использует variadic templates
    Error load(bool& val) {
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

    Error load(uint64_t& val) {
        std::string text;
        in_ >> text;

        if (text.size() == 0)
            return Error::CorruptedArchive;
        uint64_t res = 0;
        for (size_t i = 0; i < text.size(); i++) {
            if (isdigit(text[i]))
                res = res * 10 + (text[i] - '0');
            else
                return Error::CorruptedArchive;
        }
        val = res;
        return Error::NoError;
    }

    template<class T>
    Error process(T&& val) {
        return load(std::forward<T>(val));
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args) {
        Error error = load(std::forward<T>(val));
        if (error == Error::NoError)
            return process(std::forward<Args>(args)...);
        else
            return error;
    }
};
