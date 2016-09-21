/** ****************************************************************************
 * \file    test_convert.cpp
 * \brief   Testing convert functions
 * \author
 * Year      | Name
 * --------: | :------------
 * 2016      | Mario S. Koenz
 * 2016      | C.Frescolino
 * \copyright  see LICENSE
 ******************************************************************************/

#include <catch.hpp>
#include <fsc/stdSupport.hpp>

TEST_CASE("testing sto<T>", "[fsc, sto<T>]") { 
    
    
    
    //------------------- vector -------------------
    std::string text1 = "[1, 2, 3]";
    std::vector<int> cmp1{1,2,3};
    
    auto vec1 = fsc::sto<std::vector<int>>(text1);
    
    CHECK(vec1 == cmp1);
    
    // more than 1D vectors in vectors don't work yet
    std::vector<std::vector<int>> cmp2{{1},{2},{3}};
    std::string text2 = "[[1], [2], [3]]";
    auto vec2 = fsc::sto<std::vector<std::vector<int>>>(text2);
    
    CHECK(vec2 == cmp2);
    
    //------------------- split -------------------
    std::vector<std::string> cmp3{"a","b","cdef","1231","ewr"};
    auto vec3 = fsc::split("a b cdef 1231 ewr");
    CHECK(vec3 == cmp3);
    
    auto vec4 = fsc::split("axbxcdefx1231xewr", "x");
    CHECK(vec4 == cmp3);
    
    auto vec5 = fsc::split("afoobarbfoobarcdeffoobar1231foobarewr", "foobar");
    CHECK(vec5 == cmp3);
    
    //------------------- strip -------------------
    std::string cmp6 = "res";
    
    CHECK(fsc::strip(" res ") == cmp6);
    CHECK(fsc::strip("res  ") == cmp6);
    CHECK(fsc::strip("  res") == cmp6);
    
    //------------------- int -------------------
    int cmp9 = 123;
    
    CHECK(fsc::sto<int>("  123") == cmp9);
    CHECK_THROWS_AS(fsc::sto<int>("123  "), std::runtime_error);
    CHECK(fsc::sto<int>("123") == cmp9);
    CHECK_THROWS_AS(fsc::sto<int>("300000000000"), std::out_of_range);
    CHECK(fsc::sto<long>("300000000000") == 300000000000l);
    CHECK_THROWS_AS(fsc::sto<uint8_t>("300"), std::out_of_range);
    
    
}
