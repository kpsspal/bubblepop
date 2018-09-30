#ifdef _MSC_VER
#define INTTOSTRING(__int) (std::to_string (static_cast<long long>(__int)))
#define FLOATTOSTRING(__int) (std::to_string (static_cast<float>(__int)))
#else
#include <sstream>
template <typename T>
std::string ToString(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}
#define INTTOSTRING(__int) ToString(__int)
#define FLOATTOSTRING(__int) ToString(__int)
#endif
