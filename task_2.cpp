#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <fstream>
#include <cassert>

using namespace std;

class Domain {
public:
    Domain() = default;
    Domain(string_view name):domain_name_(name) {
       
        std::reverse(domain_name_.begin(), domain_name_.end());
        domain_name_.push_back('.');
    }

    bool operator==(const Domain& another) const {
        return this->domain_name_ == another.domain_name_;
    }
    bool operator<(const Domain& another) const {
        return this->domain_name_ < another.domain_name_;
    }

   
    // принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& another_domain)const {
        if (another_domain.domain_name_.size() >domain_name_.size()) {
            return false;
        }
        return std::equal(domain_name_.begin(), domain_name_.begin() + another_domain.domain_name_.size(), another_domain.domain_name_.begin());
    }
private:
    string domain_name_;
    friend class DomainChecker;
};

class DomainChecker {
public:
    template <typename Iterator>
    DomainChecker(Iterator it1, Iterator it2) :banned_domains({it1,it2}) {
        sort(banned_domains.begin(), banned_domains.end());
        auto it = unique(banned_domains.begin(), banned_domains.end(),
            [](const auto& lhs, const auto& rhs) {return rhs.IsSubdomain(lhs); });
        banned_domains.erase(it, banned_domains.end());
    }
    // конструктор должен принимать список запрещённых доменов через пару итераторов

    bool IsForbidden(const Domain& domain) const {
        auto it = lower_bound(banned_domains.begin(), banned_domains.end(), domain);
        if (it != banned_domains.end() && *it == domain) {
            return true;
        }
        if(it==banned_domains.begin()) return false;
        --it;
        return domain.IsSubdomain(*it);
    }
    //, возвращающий true, если домен запрещён
private:
    vector<Domain> banned_domains;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
std::vector<Domain> ReadDomains(std::istream& cin, size_t num_domen) {
    vector<Domain> result(num_domen);
    string domain_str;
    for (size_t i = 0; i < num_domen; ++i) {
        getline(cin, domain_str);
        result[i] = Domain(domain_str);
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

namespace tests {

    void TestDomain() {
        // Проверяем оператор сравнения

        assert(!(Domain("example.com") == Domain("anotherdomain.com")));

        // Проверяем IsSubdomain
        assert(Domain("google.com").IsSubdomain(Domain("com")));
        assert(!Domain("example.com").IsSubdomain(Domain("google.com")));

        // Проверяем с поддоменами
        assert(Domain("mail.google.com").IsSubdomain(Domain("google.com")));
        assert(Domain("sub.example.com").IsSubdomain(Domain("example.com")));
        assert(!Domain("example.com").IsSubdomain(Domain("sub.example.com")));
    }

    // Определяем пользовательскую функцию для тестирования класса DomainChecker
    void TestDomainChecker() {
        {
            // Создаем список запрещенных доменов для тестирования
            std::vector<Domain> forbidden_domains = {
                Domain("example.com"),
                Domain("google.com"),
                Domain("test.com"),
                Domain("sub.example.com"),
                Domain("mail.google.com")
            };

            // Создаем экземпляр DomainChecker для тестирования
            DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

            // Проверяем IsForbidden
            assert(checker.IsForbidden(Domain("example.com")));
            assert(!checker.IsForbidden(Domain("subexample.com")));
            assert(checker.IsForbidden(Domain("google.com")));
            assert(checker.IsForbidden(Domain("sub.example.com")));
            assert(checker.IsForbidden(Domain("mail.google.com")));
            assert(!checker.IsForbidden(Domain("example.org")));
            assert(!checker.IsForbidden(Domain("m.example.org")));
            assert(!checker.IsForbidden(Domain("google.org")));
            assert(!checker.IsForbidden(Domain("test.org")));
        }
        {
            // Тестируем пустой список запрещенных доменов
            std::vector<Domain> forbidden_domains;
            DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
            assert(!checker.IsForbidden(Domain("example.com")));
            assert(!checker.IsForbidden(Domain("")));
            assert(!checker.IsForbidden(Domain("subexample.com")));
            assert(!checker.IsForbidden(Domain("google.com")));
            assert(!checker.IsForbidden(Domain("sub.example.com")));
            assert(!checker.IsForbidden(Domain("mail.google.com")));
            assert(!checker.IsForbidden(Domain("example.org")));
            assert(!checker.IsForbidden(Domain("m.example.org")));
            assert(!checker.IsForbidden(Domain("google.org")));
            assert(!checker.IsForbidden(Domain("test.org")));
        }
    }
}//!namespace tests

int main() {
    tests::TestDomain();
    tests::TestDomainChecker();

   //ifstream in("in_test_10.txt");
   
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}