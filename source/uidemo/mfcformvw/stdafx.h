#pragma once

#include <module/plugininc.h>

class CLocalResource
{
    HINSTANCE m_old;
public:
    CLocalResource();
    ~CLocalResource();
};
