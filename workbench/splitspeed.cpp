
#define MIB_TAGS main, split_w, split, explode, explode_s
#define MIB_TEST main, split_w, split, explode, explode_s

#include <fsc/profiler.hpp>

#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
// from stdSupport.hpp (copied for no interference)
namespace fsc {

    inline std::vector<std::string> split(std::string /*copy*/ text, std::string const & delimiter = " ") {
        if(delimiter == " ") {
            // istream_iterator version
            std::stringstream iss(text);
            return std::vector<std::string>{std::istream_iterator<std::string>{iss}
                                          , std::istream_iterator<std::string>{}};
        } else {
            // Manual version
            std::vector<std::string> res;
            
            size_t pos = 0;
            std::string token;
            while ((pos = text.find(delimiter)) != std::string::npos) {
                token = text.substr(0, pos);
                res.push_back(std::move(token));
                text.erase(0, pos + delimiter.length());
            }
            res.push_back(text);
            return res;
        }
    }

} // namespace

////////////////////////////////////////////////////////////////////////////////
// from gnuclad

// Fast explode, doesn't support delimiters within strings
inline std::vector<std::string> explode(const std::string str, const char delimiter) {
  std::string buff = "";
  std::vector<std::string> v;
  for(int i = 0; i < (int)str.length(); ++i) {
    if(str[i] == delimiter) {
      v.push_back(buff);
      buff = "";
    } else buff += str[i];
  }
  if(buff != "") v.push_back(buff);
  return v;
}

// Explode, but not within the "safe" sequence
// Attention: Fails if the "safe" sequence contains the toggle itself and
// then the delimiter right behind
// Example 1: explodeSafely("abc|*dexf|ghi*|jkl", '|', '*')
// Example 2: explodeSafely("abc|*de*f|ghi*|jkl", '|', '*')
// Example 3: explodeSafely("abc|*def*|ghi*|jkl", '|', '*')
// Result 1: [abc] [*dexf|ghi*] [jkl]
// Result 2: [abc] [*de*f|ghi*] [jkl]
// Result 3: [abc] [*def*] [ghi*|jkl]
inline std::vector<std::string> explodeSafely(const std::string str,
                                              const char delimiter,
                                              const char toggle) {
  std::string buff = "";
  std::vector<std::string> v;
  bool t = false;
  int len = (int)str.length();
  for(int i = 0; i < len; ++i) {

    if(str[i] == toggle) {  // only toggle off if we have delimiter after toggle
      if(t == false) t = true;
      if(t == true && i < len-1 && str[i+1] == delimiter) t = false;
    }
    if(str[i] == delimiter && t == false) {
      v.push_back(buff);
      buff = "";
    } else buff += str[i];

  }
  if(buff != "") v.push_back(buff);
  return v;
}
////////////////////////////////////////////////////////////////////////////////

int main() {

    const char charset[] =
        " ,,\"\"\"\""
        "abcdefghijklmnopqr";

    std::string str;
    //~ str.resize(200);
    str.resize(2e4);
    std::vector<std::string> v;

    MIB_START(main)
    for(uint i = 0; i < 1000; ++i) {

        std::generate(str.begin(), str.end(),
            [&charset] () { return charset[rand() % (sizeof(charset) - 1)]; }
        );

        MIB_START(split)
        v = fsc::split(str, ",");
        MIB_NEXT(split, split_w)
        v = fsc::split(str, " ");
        MIB_NEXT(split_w, explode)
        v = explode(str, ' ');
        MIB_NEXT(explode, explode_s)
        v = explodeSafely(str, ' ', '"');

        MIB_NEXT(explode_s, split)  // repeat to balance cache heat

        v = fsc::split(str, ",");
        MIB_NEXT(split, split_w)
        v = fsc::split(str, " ");
        MIB_NEXT(split_w, explode)
        v = explode(str, ' ');
        MIB_NEXT(explode, explode_s)
        v = explodeSafely(str, ' ', '"');
        MIB_STOP(explode_s)

    }
    MIB_STOP(main)

    //~ std::cout << str << std::endl;
    MIB_PRINT(cycle);

    return 0;

}
