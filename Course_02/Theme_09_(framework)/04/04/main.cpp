#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& [Key, Value] : m) {
        if (!first) {
            os << ", ";
        }
        else {
            first = false;
        }
        os << Key <<": "<<Value;
    }
    os << "}";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    bool first = true;
    for(const T& t : v){
        if (!first) {
            os << ", ";
        }
        else {
            first = false;
        }
        os << t;
    }
    os << "]";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    os << "{";
    bool first = true;
    for (const T& t : s) {
        if (!first) {
            os << ", ";
        }
        else {
            first = false;
        }
        os << t;
    }
    os << "}";
    return os;
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file,
    const std::string& func, unsigned line, const std::string& hint) {
    if (t != u) {
        std::cout << std::boolalpha;
        std::cout << file << "(" << line << "): " << func << ": ";
        std::cout << "ASSERT_EQUAL(" << t_str << ", " << u_str << ") failed: ";
        std::cout << t << " != " << u << ".";
        if (!hint.empty()) {
            std::cout << " Hint: " << hint;
        }
        std::cout << std::endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, "")

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const std::string& expr_str, const std::string& file, const std::string& func, unsigned line,
    const std::string& hint) {
    if (!value) {
        std::cout << file << "(" << line << "): " << func << ": ";
        std::cout << "ASSERT(" << expr_str << ") failed.";
        if (!hint.empty()) {
            std::cout << " Hint: " << hint;
        }
        std::cout << std::endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

std::vector<int> TakeEvens(const std::vector<int>& numbers) {
    std::vector<int> evens;
    for (int x : numbers) {
        if (x % 2 == 0) {
            evens.push_back(x);
        }
    }
    return evens;
}

std::map<std::string, int> TakeAdults(const std::map<std::string, int>& people) {
    std::map<std::string, int> adults;
    for (const auto& [name, age] : people) {
        if (age >= 18) {
            adults[name] = age;
        }
    }
    return adults;
}

bool IsPrime(int n) {
    if (n < 2) {
        return false;
    }
    int i = 2;
    while (i * i <= n) {
        if (n % i == 0) {
            return false;
        }
        ++i;
    }
    return true;
}

std::set<int> TakePrimes(const std::set<int>& numbers) {
    std::set<int> primes;
    for (int number : numbers) {
        if (IsPrime(number)) {
            primes.insert(number);
        }
    }
    return primes;
}

int main() {
    {
        const std::set<int> numbers = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        const std::set<int> expected_primes = { 2, 3, 5, 7, 11, 13 };
        ASSERT_EQUAL(TakePrimes(numbers), expected_primes);
    }

    {
        const std::map<std::string, int> people = { {"Ivan", 19}, {"Sergey", 16}, {"Alexey", 18} };
        const std::map<std::string, int> expected_adults = { {"Alexey", 18}, {"Iva", 19} };
        ASSERT_EQUAL(TakeAdults(people), expected_adults);
    }

    {
        const std::vector<int> numbers = { 3, 2, 1, 0, 3, 6 };
        const std::vector<int> expected_evens = { 2, 0, 6};
        ASSERT_EQUAL(TakeEvens(numbers), expected_evens);
    }
}