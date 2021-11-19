#pragma once
#include <map>
#include <string>

using namespace std::string_literals;

template <typename Key, typename Value>
class ConcurrentMap {
    struct Bucket {
        std::map<Key, Value> data;
        std::mutex bucket_access;
    };
    std::vector<Bucket> buckets_;
    std::mutex map_access_;

public:
    static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys"s);

    struct Access {
        Value& ref_to_value;
        std::lock_guard<std::mutex> access;
    };

    explicit ConcurrentMap(size_t bucket_count) : buckets_(bucket_count) {};

    Access operator[](const Key& key) {
        std::lock_guard<std::mutex> guard(map_access_);
        int n = GetBucket(key);
        auto& item = buckets_[n];

        return { item.data[key], std::lock_guard<std::mutex>(item.bucket_access) };
    }

    void Erase(const Key& key) {
        std::lock_guard<std::mutex> guard(map_access_);
        int n = GetBucket(key);
        auto& item = buckets_[n];
        item.data.erase(key);
    }

    size_t Size() {
        size_t size = 0;
        for (const Bucket& b : buckets_) {
            size += b.data.size();
        }
        return size;
    }

    std::map<Key, Value> BuildOrdinaryMap() {
        std::lock_guard<std::mutex> guard(map_access_);
        std::map<Key, Value> result;
        for (Bucket& bucket : buckets_) {
            std::lock_guard<std::mutex>guard(bucket.bucket_access);
            for (const auto& [key, value] : bucket.data) {
                result[key] = value;
            }
        }
        return result;
    };

private:
    uint64_t GetBucket(const Key& key) {
        return std::abs(static_cast<int>(key)) % buckets_.size();
    }
};