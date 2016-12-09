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

#include <iostream>

#include <assert.h>
#include <iterator>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/// \brief support functions for the std containers
///
/// We define functions to help with IO of std containers
namespace fsc {
//=================== to_string ===================
/// \cond IMPLEMENTATION_DETAIL_DOC
namespace detail {
    template <typename T>
    std::string array_to_string_impl(T const &arr) {
        std::stringstream ss;
        ss << "[";
        for(size_t i = 0; i < arr.size(); ++i) {
            if(i != 0) ss << ", ";
            ss << arr[i];
        }
        ss << "]";
        return ss.str();
    }

    template <typename T>
    std::string map_to_string_impl(T const &m) {
        std::stringstream ss;
        ss << "{";
        for(auto it = m.begin(); it != m.end(); ++it) {
            if(it != m.begin()) ss << ", ";
            ss << it->first << ": " << it->second;
        }
        ss << "}";
        return ss.str();
    }
}  // end namespace detail
/// \endcond

/// \brief Generic version that can be specialized or overloaded
/// (since partial function specialization is not possible).
/// Just forwards to std::to_string
/// \param arg: T must support the syntax `std::cout << t`
///
/// Example:
/// ~~~{.cpp}
/// std::vector<int> vec{1,2,3};
/// auto str = to_string(vec);   // str == "[1, 2, 3]"
/// ~~~
template <typename T>
std::string to_string(T const &arg) {
    return std::to_string(arg);
}
/// \brief Overloaded version for std::string
/// \param arg: a std::string that will be returned as a const refernece
///
/// Example:
/// ~~~{.cpp}
/// auto str = to_string(std::string("hello"));   // str == "hello"
/// ~~~
inline std::string const &to_string(std::string const &arg) { return arg; }

/// \brief Overloaded version for char*
/// \param arg: a char* that will be returned as a std::string
///
/// Example:
/// ~~~{.cpp}
/// auto str = to_string(std::string("hello"));   // str == "hello"
/// ~~~
inline std::string to_string(char const *const arg) { return std::string(arg); }

/// \brief Converts a `std::vector<T>` to a `std::string`
/// \param vec: a `std::vector<T>` where T must support the syntax `std::cout <<
/// t`
///
/// Example:
/// ~~~{.cpp}
/// std::vector<int> vec{1,2,3};
/// auto str = to_string(vec);   // str == "[1, 2, 3]"
/// ~~~
template <typename T>
std::string to_string(std::vector<T> const &vec) {
    return detail::array_to_string_impl(vec);
}
/// \brief Converts a `std::array<T, N>` to a `std::string`
/// \param arr: a `std::array<T, N>` where T must support the syntax `std::cout
/// << t`
///
/// Example:
/// ~~~{.cpp}
/// std::array<int, 3> arr{1,2,3};
/// auto str = to_string(arr);   // str == "[1, 2, 3]"
/// ~~~
template <typename T, size_t N>
std::string to_string(std::array<T, N> const &arr) {
    return detail::array_to_string_impl(arr);
}
/// \brief Converts a `std::map<K, V>` to a `std::string`
/// \param m: a `std::map<K, V>` where K and V must support the syntax
/// `std::cout << k` and `std::cout << v`
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
std::string to_string(std::map<K, V> const &m) {
    return detail::map_to_string_impl(m);
}

//=================== sto<T> ===================
// forward declaration
template <typename T>
inline T sto(std::string const &text);

/// \brief Splits a string on a delimiter
/// \param text: The input string
/// \param delimiter: the delimiter string (not char)
/// \returns a `std::vector<std::string>` with the spilt parts ot the input
///
/// It uses a faster version if the delimiter is " " with istringstream
/// iterators
inline std::vector<std::string> split(std::string /*copy*/ text,
                                      std::string const &delimiter = " ") {
    if(delimiter == " ") {
        // istream_iterator version
        std::stringstream iss(text);
        return std::vector<std::string>{std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>{}};
    } else {
        // Manual version
        std::vector<std::string> res;

        size_t pos = 0;
        std::string token;
        while((pos = text.find(delimiter)) != std::string::npos) {
            token = text.substr(0, pos);
            res.push_back(std::move(token));
            text.erase(0, pos + delimiter.length());
        }
        res.push_back(text);
        return res;
    }
}

/// \brief Strips whitespace from the begin and end of the string
/// \param text: The input string
/// \returns The input with removed whitespace
inline std::string strip(std::string /*copy*/ text) {
    size_t start = 0;
    size_t end = text.size();
    while(text[start] == ' ') ++start;
    while(text[end - 1] == ' ') --end;

    text.erase(end, text.size() - 1);
    text.erase(0, start);
    return text;
}

/// \cond IMPLEMENTATION_DETAIL_DOC
namespace detail {
#define FSC_STO_CHECK_ERROR(T)                                    \
    if(len < text.size()) {                                       \
        std::string type = #T;                                    \
        throw std::runtime_error("fsc::sto<" + type +             \
                                 ">: could not convert " + text + \
                                 " fully to " + type);            \
    }  //

#define FSC_STO_IMPL(T, fct)                           \
    template <>                                        \
    struct sto_impl<T> {                               \
        inline static T sto(std::string const &text) { \
            size_t len = 0;                            \
            T res = std::fct(text, &len);              \
            FSC_STO_CHECK_ERROR(T)                     \
            return res;                                \
        }                                              \
    };  //

// types that don't have a native std::stoX function
#define FSC_STO_SPEC_IMPL(T, T_conv, fct_conv)                          \
    template <>                                                         \
    struct sto_impl<T> {                                                \
        inline static T sto(std::string const &text) {                  \
            size_t len = 0;                                             \
            T_conv res = std::fct_conv(text, &len);                     \
            /* Boundary check */                                        \
            if(res > std::numeric_limits<T>::max() or                   \
               res < std::numeric_limits<T>::min()) {                   \
                throw std::out_of_range("fsc::sto<" + std::string(#T) + \
                                        "> is out of range");           \
            }                                                           \
            FSC_STO_CHECK_ERROR(T)                                      \
            return static_cast<T>(res);                                 \
        }                                                               \
    };  //

    // generic sto_impl
    template <typename T>
    struct sto_impl {
        static_assert(std::is_same<T, T>::value,
                      "fsc::sto<T>: type not supported");
    };

    //------------------- basic types -------------------
    FSC_STO_IMPL(double, stod)
    FSC_STO_IMPL(float, stof)
    FSC_STO_IMPL(int, stoi)
    FSC_STO_IMPL(long double, stold)
    FSC_STO_IMPL(long, stol)
    FSC_STO_IMPL(long long, stoll)
    FSC_STO_IMPL(unsigned long, stoul)
    FSC_STO_IMPL(unsigned long long, stoull)

    FSC_STO_SPEC_IMPL(int8_t, int, stoi)
    FSC_STO_SPEC_IMPL(uint8_t, unsigned long, stoul)
    FSC_STO_SPEC_IMPL(int16_t, int, stoi)
    FSC_STO_SPEC_IMPL(uint16_t, unsigned long, stoul)
    FSC_STO_SPEC_IMPL(unsigned int, unsigned long, stoul)

    template <>
    struct sto_impl<std::string> {
        inline static std::string sto(std::string const &text) { return text; }
    };

    template <typename T>
    struct sto_impl<std::vector<T>> {
        inline static std::vector<T> sto(std::string /*copy*/ text) {
            text = strip(text);
            auto N = text.size() - 1;

            if((text[0] != '[') or (text[N] != ']')) {
                throw std::runtime_error(
                    "fsc::sto<std::vector<T>>: could not convert " + text +
                    " fully to std::vector<T>");
            }

            auto restxt = split(text.substr(1, N - 1), ",");
            std::vector<T> res;

            for(auto &a : restxt) {
                using fsc::sto;  // to allow external overloads
                res.push_back(sto<T>(a));
            }
            return res;
        }
    };

#undef FSC_STO_IMPL
#undef FSC_STO_SPEC_IMPL
#undef FSC_STO_CHECK_ERROR

#undef FSC_STO_IMPL
#undef FSC_STO_SPEC_IMPL
#undef FSC_STO_CHECK_ERROR

}  // end namespace detail
/// \endcond

/// \brief Converts a string to the requested type
/// \param text: The input
/// \returns The converted type
/// \exception std::out_of_range: If the correctly converted value does not
/// fit the type
/// \exception std::runtime_error: Opposite to the std::stoX functions, this
/// conversion throws an error if it can not parse the whole input
///
/// It is implemented for: `int8_t`, `int16_t`, `int`, `long`, `long long`
/// as well as all unsigned counterparts, for `float`, `double`, `long
/// double`, `std::string` and to `std::vector<T>` where `T` has to have a
/// `sto<T>(std::string)` functionality.
template <typename T>
T sto(std::string const &text) {
    return detail::sto_impl<T>::sto(text);
}

/// \brief Tries to get an element from a map and falls back to a default if
/// it does not exist.
/// \returns Eighter the value to the key if it exists, and the default
/// value otherwise.
template <typename Key, typename Value>
Value const &get(
    std::map<Key, Value> const &m  ///< the map we want to get the element from
    ,
    typename std::map<Key, Value>::key_type const &key  ///< the key in question
    ,
    typename std::map<Key, Value>::mapped_type const
        &value  ///< return this value if the map does not contain the key
    ) noexcept {
    // TODO: check if this is actually faster than asking for permission
    try {
        return m.at(key);
    } catch(std::out_of_range) {
        return value;
    }
}

/// \example io_example.cpp

/// \example convert_example.cpp

/// \example diverse_example.cpp
}  // end namespace fsc

//=================== global stream ops ===================
/// \brief prints a `std::vector<T>`
/// \param os: the destination stream
/// \param arg: the vector to print
///
/// Just uses std::to_string internally.
template <typename T>
inline std::ostream &operator<<(std::ostream &os, std::vector<T> const &arg) {
    os << fsc::to_string(arg);
    return os;
}
/// \brief prints a `std::array<T, N>`
/// \param os: the destination stream
/// \param arg: the array to print
///
/// Just uses std::to_string internally.
template <typename T, size_t N>
inline std::ostream &operator<<(std::ostream &os, std::array<T, N> const &arg) {
    os << fsc::to_string(arg);
    return os;
}
/// \brief prints a `std::map<K, V>`
/// \param os: the destination stream
/// \param arg: the map to print
///
/// Just uses std::to_string internally.
template <typename K, typename V>
inline std::ostream &operator<<(std::ostream &os, std::map<K, V> const &arg) {
    os << fsc::to_string(arg);
    return os;
}

#endif  // FSC_STDSUPPORT_HPP_GUARD
