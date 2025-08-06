#include "System.h"

static GlobleSystem *GetSystem()
{
    static GlobleSystem systemInstance;
    return &systemInstance;
}
