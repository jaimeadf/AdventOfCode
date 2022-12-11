#pragma once

#include <string>
#include <deque>


struct Operation
{
    char Operator;
    std::string A;
    std::string B;

    Operation(char op = '+', const std::string& a = "old", const std::string& b = "0")
        : Operator(op), A(a), B(b)
    {

    }

    unsigned long long Evaluate(unsigned long long x)
    {
        unsigned long long a = ResolveMember(x, A);
        unsigned long long b = ResolveMember(x, B);

        switch (Operator)
        {
            case '+':
                return a + b;
            case '*':
                return a * b;
            default:
                return 0;
        }
    }

    static Operation Parse(const std::string& expression)
    {
        size_t equalIndex = expression.find('=');
        size_t operatorIndex = FindOperatorIndex(expression, equalIndex);

        return Operation(
            expression[operatorIndex],
            expression.substr(equalIndex + 1, operatorIndex - equalIndex - 1),
            expression.substr(operatorIndex + 1)
        );
    }

private:
    unsigned long long ResolveMember(unsigned long long x, const std::string& value)
    {
        return value.find("old") != std::string::npos ? x : std::atoi(value.c_str());
    }

    static size_t FindOperatorIndex(const std::string& expression, size_t position)
    {
        char operators[] = { '+', '*' };

        for (char op : operators)
        {
            size_t index = expression.find_first_of(op);

            if (index != std::string::npos)
            {
                return index;
            }
        }

        return std::string::npos;
    }
};

struct Test
{
    unsigned long long DivideBy;
    size_t IfTrueMonkey;
    size_t IfFalseMonkey;

    size_t Check(unsigned long long worryLevel)
    {
        return worryLevel % DivideBy == 0 ? IfTrueMonkey : IfFalseMonkey;
    }
};

struct Monkey
{
    std::deque<unsigned long long> Items;
    Operation ItemOperation;
    Test ItemTest;
};