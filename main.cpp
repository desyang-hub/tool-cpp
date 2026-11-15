#include <iostream>
#include "toolcpp.h"
using namespace std;

class test {
    
public:
    test() {
        cout << "construct" << endl;
    }

    ~test() {
        cout << "destroy" << endl;
    }
};

class integer {
    int m_val;
public:
    integer(int val = 0) : m_val(val) {
        cout << "integer construct" << endl;
    }

    ~integer() {
        cout << "integer destroy" << endl;
    }

    int val() const {
        return m_val;
    }
};

int main(int argc, char const *argv[])
{

    // auto t = toolcpp::make_unique<test>();

    // auto t_arr = toolcpp::make_unique<test[]>(2);

    // constexpr int total = sizeof(test) + sizeof(integer);

    // auto addr = toolcpp::make_unique<char[]>(total);

    // auto test_obj = new (addr.get()) test();
    // auto integer_obj = new (addr.get() + sizeof(test)) integer(4);

    // cout << integer_obj->val() << endl;

    // auto s_p = toolcpp::make_shared<integer>(2);

    // auto ptr = new integer(2);
    // auto s_p = toolcpp::shared_ptr<integer>(ptr);

    // {
    //     auto s_p1 = s_p;
    //     auto tmp = std::move(s_p1);
    
    //     cout << s_p1.use_count() << endl;
    // }
    
    // cout << s_p.use_count() << endl;

    auto arr_ptr = toolcpp::make_shared<integer[]>(5);
    auto p = arr_ptr;
    cout << p.use_count() << endl;

    return 0;
}
