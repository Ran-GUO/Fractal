#include <complex>
#include <QImage>
#include <vector>
#include <chrono>
#include <sstream>
#include <string>
#ifndef ELEC4_UTIL_H
#define ELEC4_UTIL_H

namespace ELEC4 {
class Commify {
private:
 long long int value_;
public:
 explicit Commify(long long int value) :value_(value){}

 const std::string toString() const{
 std::ostringstream ss;
 ss<<value_;
 return ss.str();
 }

 friend std::ostream& operator<<(std::ostream &os, const Commify &c) {
 std::string s=c.toString();
// std::ostringstream ss;
 size_t len=s.length();
 for(long int i=(int)len-3;i>0;i-=3)
     s.insert(i, ",");
     os<<s;

 return os;
 }
};
}


#endif // ELEC4_UTIL_H
