/** ****************************************************************************
 * \file        stdSupport.hpp
 * \brief       adds operator<< to some std::containers as well as to_string
 * \author
 * Year      | Name
 * --------: | :------------
 * 2015-2016 | Mario S. Koenz
 * 2016      | C. Frescolino
 * \copyright  see LICENSE
 ******************************************************************************/

#ifndef FSC_STDSUPPORT_HPP_GUARD
#define FSC_STDSUPPORT_HPP_GUARD

#include <vector>
#include <map>
#include <sstream>

// yes, I know what I'm doing and what namespace this is :P
/// \brief support functions for the std containers
/// 
/// We define functions to help with IO of std containers
namespace std {
    /// \cond IMPLEMENTATION_DETAIL_DOC
    namespace detail {
        template<typename T>
        std::string array_to_string_impl(T const & arr) {
            std::stringstream ss;
            ss << "[";
            for(size_t i = 0; i < arr.size(); ++i) {
                if(i != 0) ss << ", ";
                ss << arr[i];
            }
            ss << "]";
            return ss.str();
        }
        
        template<typename T>
        std::string map_to_string_impl(T const & m) {
            std::stringstream ss;
            ss << "{";
            for(auto it = m.begin(); it != m.end(); ++it) {
                if(it != m.begin()) ss << ", ";
                ss << it->first << ": " << it->second;
            }
            ss << "}";
            return ss.str();
        }
    }//end namespace detail
    /// \endcond
    
    /// \brief Converts a `std::vector<T>` to a `std::string`
    /// \param vec: a `std::vector<T>` where T must support the syntax `std::cout << t`
    ///  
    /// Example:
    /// ~~~{.cpp}
    /// std::vector<int> vec{1,2,3};
    /// auto str = to_string(vec);   // str == "[1, 2, 3]"
    /// ~~~
    template <typename T>
    std::string to_string(std::vector<T> const &vec) noexcept {
        return detail::array_to_string_impl(vec);
    }
    /// \brief Converts a `std::array<T, N>` to a `std::string`
    /// \param arr: a `std::array<T, N>` where T must support the syntax `std::cout << t`
    ///  
    /// Example:
    /// ~~~{.cpp}
    /// std::array<int, 3> arr{1,2,3};
    /// auto str = to_string(arr);   // str == "[1, 2, 3]"
    /// ~~~
    template <typename T, size_t N>
    std::string to_string(std::array<T, N> const &arr) noexcept {
        return detail::array_to_string_impl(arr);
    }
    /// \brief Converts a `std::map<K, V>` to a `std::string`
    /// \param m: a `std::map<K, V>` where K and V must support the syntax `std::cout << k` and `std::cout << v`
    ///  
    /// Example:
    /// ~~~{.cpp}
    /// std::map<std::string, int> m;
    /// m["c"] = 1;
    /// m["a"] = 1;
    /// m["b"] = 1;
    /// auto str = to_string(m);   // str == "{a: 1, b: 1, c: 1}"
    /// ~~~
    template <typename K, typename V>
    std::string to_string(std::map<K, V> const &m) noexcept {
        return detail::map_to_string_impl(m);
    }
    
    /// \brief prints a `std::vector<T>`
    /// \param os: the destination stream
    /// \param arg: the vector to print
    ///
    /// Just uses std::to_string internally.
    template<typename T>
    std::ostream & operator<<(std::ostream & os, std::vector<T> const & arg) {
        os << to_string(arg);
        return os;
    }
    /// \brief prints a `std::array<T, N>`
    /// \param os: the destination stream
    /// \param arg: the array to print
    ///
    /// Just uses std::to_string internally.
    template<typename T, size_t N>
    std::ostream & operator<<(std::ostream & os, std::array<T, N> const & arg) {
        os << to_string(arg);
        return os;
    }
    /// \brief prints a `std::map<K, V>`
    /// \param os: the destination stream
    /// \param arg: the map to print
    ///
    /// Just uses std::to_string internally.
    template<typename K, typename V>
    std::ostream & operator<<(std::ostream & os, std::map<K, V> const & arg) {
        os << to_string(arg);
        return os;
    }
    
}  // end namespace std

#endif //FSC_STDSUPPORT_HPP_GUARD
