#include "ECSFrame/Notify/Notifier.h"

#include "ECSFrame/Global/GlobalPool.h"
#include "ECSFrame/Global/GlobalQuery.h"

template <typename T>
void IGlobalNotify<T>::SendNotify(EPointer<MsgObject> msg)
{
    if constexpr (std::is_same_v<T, GlobalQuery>)
    {
        ECS::Global::Query()->notify(msg);
    }
    else if constexpr (std::is_same_v<T, GlobalPool>)
    {
        ECS::Global::Pool()->notify(msg);
    }
}

template class IGlobalNotify<GlobalQuery>;
template class IGlobalNotify<GlobalPool>;