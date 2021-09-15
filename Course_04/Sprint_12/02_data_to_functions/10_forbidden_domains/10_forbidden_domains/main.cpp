#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <set>

using namespace std;

class Domain {
    std::string domain_;

public:
    // разработайте класс домена
    Domain() = delete;

    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(std::string domain) : domain_(domain) {};    

    // разработайте operator==
    bool operator==(const Domain& other) {
        return this->domain_ == other.domain_;
    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& other) const {
        std::string_view l_name = other.GetName();
        std::string_view r_name = this->GetName();        
        for (size_t i = 0; i < l_name.size() && i < r_name.size(); i++) {
            if (l_name[i] != r_name[i]) {
                return false;
            }
        }
        return true;
    }
    
    std::string_view GetName() const {
        return domain_;
    }

private:
};

bool operator<(const Domain& lhs, const Domain& rhs) {
    std::string_view l_name = lhs.GetName();
    std::string_view r_name = rhs.GetName();
    return std::lexicographical_compare(l_name.begin(), l_name.end(), r_name.begin(), r_name.end());
}

class DomainChecker {
    std::vector<Domain> forbidden_domains_;

public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template<typename It, typename domain = Domain>
    DomainChecker(It first, It last) {
        while (first != last) {
            forbidden_domains_.push_back(*first++);
        }
        //sort vector
        std::sort(forbidden_domains_.begin(), forbidden_domains_.end());
        
        //очистим вектор запрещённых доменов от дублирующих аналогов с поддоменами
        //создадим компаратор, который посимвольно сверяет наименьший домен с наибольшим:
        auto domain_comp = [](const Domain& lhs, const Domain& rhs) {
            //если левый домен полностью находится в правом с начала, то правый удаляем
            return lhs.IsSubdomain(rhs);
        };

        auto it = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(), domain_comp);
        forbidden_domains_.erase(it, forbidden_domains_.end());
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& checked_domain) {
        //std::string_view domain_name = checked_domain.GetName();
        auto it = std::lower_bound(forbidden_domains_.begin(), forbidden_domains_.end(), checked_domain);
        bool result = false;
        if (it != forbidden_domains_.end() && it != forbidden_domains_.begin()) {
            result = checked_domain.IsSubdomain(*it);
        }
        return result;
    }
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа

std::vector<Domain> ReadDomains(std::istream& in, const size_t domains_count) {
    std::vector<Domain> domains;
    domains.reserve(domains_count);
    std::string domain_name;

    for (size_t i = 0; i < domains_count; ++i) {
        in >> domain_name;
        std::string reverse(domain_name.rbegin(), domain_name.rend());
        domains.push_back({ reverse + '.'});
    }    
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    Number num;
    input >> num;
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