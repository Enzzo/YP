#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <unordered_set>

using namespace std;

class Domain {
    std::string domain_;
    // разработайте класс домена

    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами

    // разработайте operator==

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
public:
    void SetDomain(std::istream& in) {
        in >> domain_;
    }
private:
};

std::istream& operator>>(std::istream& in, Domain& domain) {
    domain.SetDomain(in);
    return in;
}

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template<typename It, typename domain = Domain>
    DomainChecker(It first, It last) {
        
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& checked_domain) {
        return true;
    }
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа

std::vector<Domain> ReadDomains(std::istream& in, const size_t domains_count) {
    std::vector<Domain> domains(domains_count);
    for (size_t i = 0; i < domains_count; ++i) {
        in >> domains[i];
    }    
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    
    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}