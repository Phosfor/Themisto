#include "Debug/Watch.hpp"

string nope(Watch* watch)
{
    return "";
}

Watch::Watch()
{
    Active = true;
    Object = (void*)NULL;
    MemberName = "";
    OutFile = NULL;
    Parent = NULL;
    Type = NotAWatch;
    ID = "";
    Name = "";
    Expression = nope;
}

Watch::~Watch()
{
}
