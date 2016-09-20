/** ****************************************************************************
 * \file        print_example.cpp
 * \brief       Shows the print functions for some containers 
 * \author
 * Year      | Name
 * --------: | :------------
 * 2015-2016 | Mario S. Koenz
 * 2016      | C.Frescolino
 * \copyright  see LICENSE
 ******************************************************************************/

#include <iostream>
#include <fsc/stdSupport.hpp>

int main() {
    
    std::vector<int> vec{1,2,3};
    auto str1 = to_string(vec);
    std::cout << str1 << std::endl; // "[1, 2, 3]"
    
    std::array<int, 3> arr{1,2,3};
    std::cout << arr << std::endl; // "[1, 2, 3]"
    
    std::map<std::string, int> m;
    m["c"] = 1;
    m["a"] = 1;
    m["b"] = 1;
    std::cout << map << std::endl; // "{a: 1, b: 1, c: 1}"

    return 0;
}
