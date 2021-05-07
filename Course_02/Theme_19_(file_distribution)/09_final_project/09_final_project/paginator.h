#pragma once

template<typename It>
class IteratorRange {
    It _begin;
    It _end;

public:
    IteratorRange(const It begin, const It end) : _begin(begin), _end(end) {};
    It begin() const { return _begin; }
    It end() const { return _end; }
};

template<typename It>
std::ostream& operator<<(std::ostream& os, const IteratorRange<It>& ir) {
    for (It it = ir.begin(); it != ir.end(); ++it) {
        os << *it;
    }
    return os;
}

template <typename It>
class Paginator {
    std::vector<IteratorRange<It>> _range;
    It _begin;
    It _end;

public:
    Paginator(const It begin, const It end, const size_t size) :_begin(begin), _end(end) {
        while (_begin != _end) {
            int distance = std::distance(_begin, _end);

            if (distance > static_cast<int>(size)) {
                distance = size;
            }
            _range.push_back({ _begin, _begin + distance });
            std::advance(_begin, distance);
        }
    };

    auto begin()const {
        return _range.begin();
    }
    auto end()const {
        return _range.end();
    }
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}