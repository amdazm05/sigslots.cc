#include <sigslot.tpp>
#include <iostream>

void non_lamb(int x, int y, int z)
{
    std::cout<<x<<y<<z<<std::endl;
}

class data_t
{
    int i=0;
public:
    data_t(int i): i(i){}
    void funct(int x,int y,int z)
    {
        std::cout<<i<<" -- "<<x<<y<<z<<std::endl;
    }
    static void conceptfail(int x,int y,int z)
    {
        
    }
};

int main()
{
    data_t dat(9);
    std::function<void(int, int, int)> 
        lamb = 
            [](int x, int y, int z) 
            { std::cout << x << y << z << std::endl; };
    Slot_t sig(non_lamb);
    Signal_t<void,int,int,int> sigi;
    Signalm_t<data_t,void,int,int,int> sigi2;
    sigi.connect(sig);
    Slot_t sig2(lamb);
    Slotm_t sig3(&data_t::funct);
    // Case of failure
    // Slotm_t sig4(&data_t::conceptfail);
    sigi2.connect(dat,sig3);
    sigi.emit(1,2,3);
    return 0;
}
