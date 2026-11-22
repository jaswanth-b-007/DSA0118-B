#include <iostream>
#include <cstring>
class Text {
    char* s;
    size_t len;
public:
    Text(const char* t = "") : len(std::strlen(t)) { s = new char[len + 1]; std::strcpy(s, t); }
    Text(const Text& o) : len(o.len) { s = new char[len + 1]; std::strcpy(s, o.s); }
    Text& operator=(const Text& o)
    {
        if (this == &o) return *this;
        delete[] s;
        len = o.len;
        s = new char[len + 1];
        std::strcpy(s, o.s);
        return *this;
    }
    ~Text() { delete[] s; }
    Text operator+(const Text& o) const
    {
        Text r;
        r.len = len + o.len;
        delete[] r.s;
        r.s = new char[r.len + 1];
        std::strcpy(r.s, s);
        std::strcat(r.s, o.s);
        return r;
    }
    bool operator==(const Text& o) const { return std::strcmp(s, o.s) == 0; }
    bool operator!=(const Text& o) const { return !(*this == o); }
    friend std::ostream& operator<<(std::ostream& os, const Text& t) { return os << t.s; }
};
int main()
{
    Text a("Hello"), b(" World"), c("!");
    Text r = a + b + c;
    std::cout << r << '\n';
    std::cout << std::boolalpha << (a == Text("Hello")) << ' ' << (a != b) << '\n';
    a = a;
    std::cout << a << '\n';
}