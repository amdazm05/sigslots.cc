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
    template <class T>
    concept _isMemFuncPtr = requires (T v)
    {
        requires std::is_member_function_pointer<T>::value;
    }

    template<class T,typename ...Args>
    requires _isMemFuncPtr<T>
    class Slot
    {
        public:
            slot(T callback) 
            {
                //Since the callback is a member function here
                //Need to encapsulate it somehow
            }
        private:
            std::function<void()> _callback;
    };

    template <class T>
    class Signal
    {
        public:
            Signal();
            ~Signal();
            void connect(Slot<T> && slot);
        private:
            std::vector<Slot<T>> _slots;
    };
}


#endif //_SIGSLOT