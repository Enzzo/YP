/*
#include <cstdlib>
#include <future>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "log_duration.h"
#include "test_runner_p.h"

using namespace std::string_literals;

template<typename K, typename V>
class ConcurrentMap {
public:
    static_assert(std::is_integral_v<K>, "ConcurrentMap supports only integer keys");

    struct Access {
        V& ref_to_value;
        std::lock_guard<std::mutex> guard;
    };

    explicit ConcurrentMap(size_t bucket_count)
        : data(bucket_count),
        SIZE_MAP(bucket_count) {
    }

    Access operator[](const K& key) {
        std::lock_guard<std::mutex> lock(guard_);
        Submap& item = data[GetIndex(key)];
        return { item.submap[key], std::lock_guard(item.m) };
    }

    std::map<K, V>BuildOrdinaryMap() {
        std::map<K, V> result;
        for (auto& i : data) {
            for (auto& d : i.submap) {
                result[d.first] = d.second;
            }
        }
        return result;
    }

private:
    struct Submap {
        std::map<K, V> submap;
        std::mutex m;
    };
    std::vector<Submap> data;
    std::mutex guard_;
    int SIZE_MAP;

    int GetIndex(K key) {
        int index = key;
        return abs(index) % data.size();
    }
};


template <typename K, typename V>
class ConcurrentMap {
public:
	static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

	struct Access {
		V& ref_to_value;
		lock_guard<mutex> guard;
	};
	struct Base {
		map<K, V> Map;
		mutex m;
	};

	explicit ConcurrentMap(size_t bucket_count) : OrdinaryMap(bucket_count), SIZE_MAP(bucket_count) {}

	Access operator[](const K& key)
	{
		lock_guard<mutex> s(guard_base);

		Base& item = OrdinaryMap[key_pos(key)];
		return { item.Map[key], lock_guard(item.m) };
	}

	map<K, V> BuildOrdinaryMap()
	{
		map<K, V> result;
		for (auto& it : OrdinaryMap)
		{
			for (auto& it_2 : it.Map)
			{
				result[it_2.first] = it_2.second;
			}
		}
		return result;
	}
private:
	vector<Base> OrdinaryMap;
	mutex guard_base;
	int SIZE_MAP;

	int64_t key_pos(K key)
	{
		int64_t num = key;
		return abs(num) % SIZE_MAP;
	}
};
*/