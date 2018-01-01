#include <iostream>
#include <bitset>

template <uint32_t v>
struct binary{
    typedef decltype(v) Type;
    static const Type value =
        binary<v / 10>::value * 2 + v % 10;
};

template <>
struct binary<0>{
    static const uint32_t value = 0;
};

int main() {
    using namespace std;
    cout << binary<1010>::value << endl;
    cout << bitset<sizeof(uint32_t) * 8>(binary<1010>::value) << endl;
    return 0;
}
/*
10
00000000000000000000000000001010
*/
/*
由于binary在编译器做工作，其::value是一个编译器常数，编译器可以将其直接编码到目的码中，因而当需要使用它时可以节省内存查找时间
*/

#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

template <typename Iterator>
struct iterator_traits;

//局部特化
template <typename Iterator>
struct iterator_traits<Iterator*>{
    typedef Iterator value_type;
};

using namespace std;
int main() {

    cout << abi::__cxa_demangle(typeid(::iterator_traits<int*>::value_type).name(), nullptr, nullptr, nullptr) << endl;
    return 0;
}
/*
int
*/


#include <iostream>
struct A;
template <typename T> struct X {
    int v;
    void convertTo(A& a) ;
};

struct A { int v; };

template <typename T>
void X<T>::convertTo(A& a) {
    a.v = v; // 这里需要A的实现
}

int main() {
    X<int> x;
    return 0;
}
/*
这里模板定义时需要struct A的实现，非依赖名称寻找，否则会有warning
*/


// enable_if example: two ways of using enable_if
#include <iostream>
#include <type_traits>

// 1. the return type (bool) is only valid if T is an integral type:
template <class T>
typename std::enable_if<std::is_integral<T>::value,bool>::type
  is_odd (T i) {return bool(i%2);}

// 2. the second template argument is only valid if T is an integral type:
template < class T,
           class = typename std::enable_if<std::is_integral<T>::value>::type>
bool is_even (T i) {return !bool(i%2);}

int main() {

  short int i = 1;    // code does not compile if type of i is not integral

  std::cout << std::boolalpha;
  std::cout << "i is odd: " << is_odd(i) << std::endl;
  std::cout << "i is even: " << is_even(i) << std::endl;

  return 0;
}
/*
enable_if 的主要作用就是当某个 condition 成立时，enable_if可以提供某种类型。enable_if在标准库中通过结构体模板实现的，声明如下：

template<bool Cond, class T = void> struct enable_if;

通过原型得到模板实参，然后计算片特化形式的匹配，如果无法匹配则使用原型
*/
