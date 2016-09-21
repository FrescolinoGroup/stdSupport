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
    std::cout << fsc::strip(" foo ") << std::endl; // returns "foo"
    
    std::cout << fsc::split("a b c") << std::endl; 
    // returns a vector with elements {"a", "b", "c"}
    
    std::cout << fsc::split("aCPPbCPPc", "CPP") << std::endl; 
    // returns a vector with elements {"a", "b", "c"}
    
    return 0;
}
