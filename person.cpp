#include <iostream>
#include <fstream>
#include <iterator>
#include <exception>
#include "person.h"
#include "helper.h"
#define MAX_GENERATIONS 255

void Person::deserialize(const char* file_name) {
  std::ifstream f(file_name);
  if (!f.is_open()) throw std::exception();
  std::string line;
  std::getline(f,line);
  deserialize0(line.begin(), line.end(), helper::decode());
  std::vector<Person*> v(MAX_GENERATIONS); 
  v.at(0) = this;

  while (std::getline(f, line)) {
    std::pair<int, std::string::iterator> shft = helper::shift(line.begin(), line.end());
    Person* parent = v.at(shft.first-1);
    if (parent == NULL) throw std::exception();  
    (*parent).children_.push_back( Person() );
    Person& p = (*parent).children_.back();
    p.deserialize0(shft.second, line.end(), helper::decode());
    v.at(shft.first) = &p;
  }
}

template <typename I, typename Op>
void Person::deserialize0(I first, I last, Op op) {
  I fst, snd, thd;
  I tmp = first;
  first = std::find_if(first, last, std::bind1st( std::equal_to<char>(), ';'));
  helper::parse_person_info(tmp, first, &fst, &snd, &thd);
  age_ = helper::atoi0(fst+1, snd);
  name_ = op(snd+1, thd);
  sex_type(sex_, helper::atoi0(thd+1, first));
  ++first;

  while (first != last) {
    tmp = first;
    first = std::find_if(first, last, std::bind1st( std::equal_to<char>(), ';')); 
    helper::parse_person_info(tmp, first, &fst, &snd, &thd);
    addrBook_[op(tmp, fst)] =
      PersonInfo( helper::atoi0(fst+1, snd), op(snd+1, thd), helper::atoi0(thd+1, first) );
    ++first;
  }
}

template <typename I, typename Y, typename Op>
Y Person::serialize_addrbook(I first, I last, Y si, Op op) const {
  while (first != last) {
    *si = op(first->first.begin(), first->first.end()) +
          ":" + helper::itos0(first->second.age_) + " " +
          op( first->second.name_.begin(), first->second.name_.end() ) +
          " " + helper::itos0(first->second.sex_) + ";";
    ++first;   
    ++si;
  }
  *si = "\n";
  ++si;
  return si;
}

template <typename Y, typename Op>
void Person::serialize0(Y si, int sh, Op op) const {
  *si = std::string(sh, ' ') + ":" + helper::itos0(age_) + " " +
        op(name_.begin(), name_.end()) + " " + helper::itos0(sex_) + ";";
  ++si;
  si = serialize_addrbook( addrBook_.begin(), addrBook_.end(), si, op);
  serialize_children(children_.begin(), children_.end(), si, sh, op);
}

inline
void Person::serialize(const char* fname) const {
  std::ofstream output(fname);
  serialize0(std::ostream_iterator<std::string>(output), 0, helper::encode());
}

inline
void Person::show() const {
  serialize0(std::ostream_iterator<std::string>(std::cout), 0, helper::identity());
}

template <typename I, typename Y, typename Op>
void Person::serialize_children(I first, I last, Y si, int sh, Op op) const {
  while(first != last) {
    (*first).serialize0(si, sh+1, op);
    ++first;
  }
}

int main() {
  Person p;
  p.deserialize("input.txt");
  p.serialize("output.txt"); 
  p.show();
}

