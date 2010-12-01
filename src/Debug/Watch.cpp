#include "Debug/Watch.hpp"

string nope_evalute(Watch* watch)
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
    Expression = nope_evalute;
    UpdateInterval = DEFAULT_TIMEOUT;
    LeftFromLastUpdate = 0;
}

Watch::~Watch()
{
}
