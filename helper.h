#include <sstream>
#include <algorithm>
#include <stdlib.h>
#define MAX_DECODE_SIZE 125
#define MAX_DECODE_SYMBOL_SIZE 16
#define MAX_ENCODE_SIZE 525

namespace helper {

  template <typename I>
  void parse_person_info(I first, I last, I* fst, I* snd, I* thd) {
    *fst = std::find_if(first,  last, std::bind1st( std::equal_to<char>(), ':'));
    *snd = std::find_if(*fst+1, last, std::bind1st( std::equal_to<char>(), ' '));
    *thd = std::find_if(*snd+1, last, std::bind1st( std::equal_to<char>(), ' '));
  }

  template <typename I>
  int atoi0(I first, I last) { return atoi( std::string(first, last).c_str() ); }

  std::string itos0(int d) {
    std::stringstream ss;
    ss << d;
    return ss.str();
  }

  template <typename I>
  std::pair<int, I> shift(I first, I last) {
    I tmp = first;
    first = std::find_if(first, last, std::bind1st( std::not_equal_to<char>(),' '));
    return std::make_pair( std::distance(tmp, first), first);
  }

  struct decode {
    template <typename I>
    std::string operator()(I first, I last) {
      std::string str;
      std::string buf;
      str.reserve(MAX_DECODE_SIZE); 
      buf.reserve(MAX_DECODE_SYMBOL_SIZE);
      while(first != last) {
        if (*first == '.') {
          str.push_back( (char)atoi0( buf.begin(), buf.end() ));
          buf.clear();
        } else {
          buf.push_back(*first);
        }
        ++first;
      }
      return str;
    }
  };

  struct encode {
    template <typename I>
    std::string operator()(I first, I last) {
      std::string str;
      str.reserve(MAX_ENCODE_SIZE);
      while(first != last) {
        str.append( itos0((int)*first) );
        str.push_back('.');
        ++first;
      }
      return str;
    }
  };

  struct identity {
    template <typename I>
    std::string operator()(I first, I last) {
      return std::string(first, last);    
    }
  };
}

