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

    // https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
    template <typename Ret, typename... Arg> 
    class Slot_t
    {
    private:
        std::function<Ret(Arg...)> fun;
        Ret(*func)(Arg...);
    public:
        Slot_t() = default;
        ~Slot_t() = default;
        Slot_t(std::function<Ret(Arg...)> callback) : fun(callback) {}
        Slot_t(Ret(*callback)(Arg...)) : func(callback) {}
        void emit(Arg... params)
        {
            if (fun) fun(params...);
            if (func) func(params...);
        }
    };

    template<typename T,typename Ret,typename ...Arg>
    class Slotm_t
    {
        private:
        std::function<Ret(T&, Arg...)>  funcmem;
        public:
        Slotm_t(Ret (T::*callback)(Arg...))
        {
            funcmem = [callback](T& instance, Arg... args) {
                return (instance.*callback)(args...);
            };
        }
        void emit(T & member,Arg... params)
        {
            if (funcmem) funcmem(member,params...);
        }
    };

    //CTAD
    template <typename Callback>
    Slot_t(Callback) -> Slot_t<Callback>;

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