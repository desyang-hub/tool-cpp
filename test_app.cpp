#include <toolcpp.h>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
    
    vector<int> arr{1, 2, 3, 45};

    auto arr_strs = toolcpp::mapping(arr, [](int item) {
        return item + 1;
    });

    toolcpp::for_each(arr_strs.begin(), arr_strs.end(), [](int item){
        cout << item << endl;
    });

    return 0;
}
