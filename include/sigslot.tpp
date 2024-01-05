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
        void call(Arg... params)
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
        void call(T& member, Arg... params)
        {
            if (funcmem) funcmem(member, params...);
        }
    };

    //CTAD
    template <typename Callback>
    Slot_t(Callback) -> Slot_t<Callback>;
    template<typename Ret,typename ...Arg>
    class Signal_t
    {
    private:    
        std::vector<Slot_t<Ret,Arg...>*> _slots;
    public:
        Signal_t() = default;
        ~Signal_t() = default;
        void connect(Slot_t<Ret,Arg...> & slot)
        {
            _slots.push_back(&slot);
        }
        void emit(Arg... params)
        {
            for(auto & slot:_slots)
                (*slot).call(params...);
        }
    };

    template<typename T,typename Ret,typename ...Arg>
    class Signalm_t
    {
    private:    
        std::vector<Slotm_t<T,Ret,Arg...>*> _slots;
        std::vector<T*> _members;
    public:
        Signalm_t() = default;
        ~Signalm_t() = default;
        void connect(T & mem,Slotm_t<T,Ret,Arg...> & slot)
        {
            _slots.push_back(&slot);
            _members.push_back(&mem);
        }
        void emit(Arg... params)
        {
            if(_slots.size()!=_members.size())
                throw std::runtime_error("Signalm_t : slots have unjust members");
            for(std::size_t i;i<_members.size();i++)
            {
                (*_slots[i]).call(*_members[i],params...);
            }
        }
    };
}


#endif //_SIGSLOT