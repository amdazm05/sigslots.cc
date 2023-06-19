#ifndef _SIGSLOT
#define _SIGSLOT

#include <iostream>
#include <concepts>
#include <type_traits>
#include <functional>
#include <vector>

// Tenative design, still considering things
namespace 
{
    // Make this functionality with member function pointers too 
    template <class T>
    concept _isMemFuncPtr = requires (T v)
    {
        requires std::is_member_function_pointer<T>::value;
    };

    template <class T>
    concept _isFuncPtr = requires (T v)
    {
        requires std::is_pointer<T>::value && std::is_function<std::remove_pointer<T>>::value;
    };

    template<typename ...Args>
    class Slot
    {
        public:
            slot(std::function<void(Args...)> && params)
            {
                _callback = params;
            }
            void emit(Args ... params)
            {
                _callback(params...);
            }
            
        private:
            std::function<void(Args ...)> _callback;
    };

    template <typename ...Args>
    class Signal
    {
        public:
            Signal()
            {

            }
            ~Signal()
            {

            }
            void emit(Args... params)
            {
                for(auto & emitters: _slots)
                    emitters.emit(params ...);
            }
            void connect(Slot<Args...> && slot)
            {
                _slots.emplace_back(slot);
            }
        private:
            std::vector<Slot<Args...>> _slots;
    };
}


#endif //_SIGSLOT