#ifndef _SIGSLOT
#define _SIGSLOT

#include <iostream>
#include <concepts>
#include <type_traits>

namespace
{
    template <class T>
    concept _isMemFuncPtr = requires (T v)
    {
        requires std::is_member_function_pointer<T>::value;
    }

    template<class T>
    requires _isMemFuncPtr
    class Slot
    {

    };

}


#endif //_SIGSLOT