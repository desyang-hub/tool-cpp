#include <iostream>
#include "toolcpp.h"
#include <iterator>
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

class Range {
private:
    int m_begin;
    int m_end;
public:
    Range(int sta, int end) : m_begin(sta), m_end(end) {}
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

    // auto arr_ptr = toolcpp::make_shared<integer[]>(5);
    // auto p = arr_ptr;
    // cout << p.use_count() << endl;

    // int arr[5] = {1, 2, 3,4};

    // for (auto v : arr) {
    //     cout <<v << endl;
    // }

    // auto lam = []() {
    //     return 1;
    // };

    // cout << __cplusplus << endl;

    // auto arr = {1, 2, 3, 4, 5};

    // auto integers = toolcpp::mapping(arr, [](int item) {
    //     return integer(item);
    // });

    // // for (int i = 0; i < integers.size(); ++i) {
    // //     cout << integers[i].val() << endl;
    // // }

    // for (const auto& item : integers) {
    //     cout << item.val() << endl;
    // }


    // toolcpp::Thread woker([]{
    //     cout << "hhhh" << endl;
    // });


    // {
    //     toolcpp::ThreadPool pool(2);
    //     pool.enqueue([](){
    //         std::this_thread::sleep_for(std::chrono::seconds(1));
    //     });
    // }
    // cout << "program is finish!" << endl;
    

    // toolcpp::IntHeap heap;
    toolcpp::priority_queue<int> heap;

    heap.push(5);
    heap.push(3);
    heap.push(4);
    heap.push(1);
    heap.push(8);

    while (!heap.empty()) {
        cout << heap.pop() << endl;
    }


    return 0;
}
