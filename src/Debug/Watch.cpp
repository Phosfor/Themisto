/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

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
