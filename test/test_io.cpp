/** ****************************************************************************
 * \file    demo_test.cpp
 * \brief   Demonstrates testing with catch
 * \author
 * Year      | Name
 * --------: | :------------
 * 2016      | C.Frescolino
 * \copyright  see LICENSE
 ******************************************************************************/

#include <catch.hpp>
#include <fsc/stdSupport.hpp>

TEST_CASE("testing to_string", "[std, to_string]") { 
    
    //------------------- vector -------------------
    
    std::string cmp1 = "[1, 2, 3]";
    std::vector<int> vec{1,2,3};
    
    auto str1 = to_string(vec);
    
    CHECK(str1 == cmp1);
    
    //------------------- array -------------------
    std::string cmp2 = "[1, 2, 3]";
    std::array<int, 3> arr{1,2,3};
    
    auto str2 = to_string(arr);
    
    CHECK(str2 == cmp2);
    
    //------------------- map -------------------
    std::string cmp3 = "{a: 1, b: 1, c: 1}";
    std::map<std::string, int> m;
    m["c"] = 1;
    m["a"] = 1;
    m["b"] = 1;
    auto str3 = to_string(m);
    
    CHECK(str3 == cmp3);
    
    //------------------- operator<< -------------------
    std::stringstream ss;
    
    ss << vec << arr << m;
    
    CHECK(ss.str() == cmp1 + cmp2 + cmp3);
}
