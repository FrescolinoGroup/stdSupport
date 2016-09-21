/** ****************************************************************************
 * \file        convert_example.cpp
 * \brief       Shows the fsc::sto<T> capabilities
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
    
    std::cout << fsc::sto<int>("42") << std::endl;
    
    std::cout << fsc::sto<double>("3.14") << std::endl;
    
    std::cout << fsc::sto<std::vector<int>>("[1, 2, 5, 7]")  << std::endl;
    
    // for now, std::vector<std::vector<int>> does not yet work
    
    // exceptions
    fsc::sto<int>("24.12"); // will throw std::runtime_error;
    fsc::sto<int>("24foo"); // will throw std::runtime_error;
    fsc::sto<uint8_t>("256");   // will throw std::out_of_bound
    
    return 0;
}
