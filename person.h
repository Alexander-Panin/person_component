#include <string>
#include <vector>
#include <map>

class IPerson
{
public:
  virtual void deserialize(const char*) = 0;
  virtual void show() const = 0;
  virtual void serialize(const char*) const = 0;
};

template <typename T>
void sex_type(T& t, int c) { t = T(c); }

class PersonInfo
{
public:
  unsigned age_;
  std::string name_;
  enum {undef, man, woman} sex_;

  PersonInfo(unsigned age, std::string name, int sex) : age_(age), name_(name)
  { sex_type(sex_, sex); }
  PersonInfo(const PersonInfo& p) : age_(p.age_), name_(p.name_), sex_(p.sex_) {}
  PersonInfo() : age_(0), name_("") { sex_type(sex_, undef); }
};

class Person : PersonInfo, IPerson
{
public:
  std::vector<Person> children_;
  std::map<std::string, PersonInfo> addrBook_;

  void deserialize(const char*);
  void show() const;
  void serialize(const char*) const;

private:
  template <typename I, typename Op>
  void deserialize0(I first, I last, Op);
  template <typename Y, typename Op>
  void serialize0(Y si, int, Op) const;
  template <typename I, typename Y, typename Op>
  void serialize_children(I first , I last, Y si, int, Op) const;
  template <typename I, typename Y, typename Op>
  Y serialize_addrbook(I first , I last, Y si, Op) const;
};
