#include <array>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std::literals;

class VehiclePlate {
    std::array<char, 3> letters_;
    int digits_;
    int region_;

    auto AsTuple() const {
        return std::tie(letters_, digits_, region_);
    }

public:
    bool operator==(const VehiclePlate& other) const {
        return AsTuple() == other.AsTuple();
    }

    bool operator<(const VehiclePlate& other) const {
        return AsTuple() < other.AsTuple();
    }
    VehiclePlate(char l0, char l1, int digits, char l2, int region)
        : letters_{ l0, l1, l2 }
        , digits_(digits)
        , region_(region) {
    }

    std::string ToString() const {
        std::ostringstream out;
        out << letters_[0] << letters_[1];

        // ����� ��������� �������� ����� ������ ����� ������
        // �� ��� ����, ���������� �������� ������������:
        // setfill ����� ������ ��� ����������,
        // right ����� ������������ �� ������� ����,
        // setw ����� ����������� �������� ���������� ������
        out << std::setfill('0') << std::right << std::setw(3) << digits_;
        out << letters_[2] << std::setw(2) << region_;

        return out.str();
    }

    int Hash() const {
        return digits_;
    }
};

std::ostream& operator<<(std::ostream& out, VehiclePlate plate) {
    out << plate.ToString();
    return out;
}

class VehiclePlateHasher {
    std::hash<std::string> hasher_;
public:
    size_t operator()(const VehiclePlate& plate) const {
        return static_cast<size_t>(hasher_(plate.ToString()));
    }
};

struct ParkingException {};

template <typename Clock>
class Parking {
    // ��� ��������� � ���� ������ ���������� ��������� �� ������� ������������ 
    // typename; ����� ����� ��������, ������� ���������� ��� ������ �����
    using Duration = typename Clock::duration;
    using TimePoint = typename Clock::time_point;

public:
    Parking(int cost_per_second) : cost_per_second_(cost_per_second) {}

    // ����������� ������ � ��������� �������
    void Park(VehiclePlate car) {
        if (now_parked_.count(car)) {
            throw ParkingException();
        }
        now_parked_[car] = Clock::now();
    }

    // ������� ������ � ��������� �������
    void Withdraw(const VehiclePlate& car) {
        if (!now_parked_.count(car)) {
            throw ParkingException();
        }
        
        complete_parks_[car] = Clock::now() - now_parked_.at(car);
        now_parked_.erase(car);
    }

    // �������� ���� �� ���������� ����������
    int64_t GetCurrentBill(const VehiclePlate& car) const {
        Duration duration = {};
        const auto& parked = now_parked_.find(car);
        if (parked != now_parked_.end()) {
            duration += Clock::now() - parked->second;
        }
        const auto& completed = complete_parks_.find(car);
        if(completed != complete_parks_.end()){
            duration += completed->second;
        }
        
        return std::chrono::duration_cast<std::chrono::seconds>(duration).count() * cost_per_second_;;
    }

    // ��������� ��������� ������
    // �� ������, ������� ��������� �� �������� �� ������ ������, ������ 
    // �������� �� ��������, �� ������ ������� ��� ��� ���������� � ����
    std::unordered_map<VehiclePlate, int64_t, VehiclePlateHasher> EndPeriodAndGetBills() {
        std::unordered_map<VehiclePlate, int64_t, VehiclePlateHasher> end_period_map;

        std::for_each(now_parked_.begin(), now_parked_.end(), [&end_period_map, this](auto& p) {
            auto& [car, elapsed] = p;
            end_period_map[car] = std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - elapsed).count() * this->cost_per_second_;
            elapsed = Clock::now();
            });

        std::for_each(complete_parks_.begin(), complete_parks_.end(), [&end_period_map, this](auto& p) {
            auto& [car, duration] = p;
            if (duration.count() > 0) {
                end_period_map[car] += std::chrono::duration_cast<std::chrono::seconds>(duration).count() * this->cost_per_second_;
                duration = {};
            }
            });
        return end_period_map;
    }

    // �� ������� ���� �����
    auto& GetNowParked() const {
        return now_parked_;
    }

    // �� ������� ���� �����
    auto& GetCompleteParks() const {
        return complete_parks_;
    }

private:
    int cost_per_second_;
    std::unordered_map<VehiclePlate, TimePoint, VehiclePlateHasher> now_parked_;
    std::unordered_map<VehiclePlate, Duration, VehiclePlateHasher> complete_parks_;
};

// ��� ���� ������ ������������ ��� ������������
// ��� ������� ������� �������, ������� �� ������ ����
class TestClock {
public:
    using time_point = std::chrono::system_clock::time_point;
    using duration = std::chrono::system_clock::duration;

    static void SetNow(int seconds) {
        current_time_ = seconds;
    }

    static time_point now() {
        return start_point_ + std::chrono::seconds(current_time_);
    }

private:
    inline static time_point start_point_ = std::chrono::system_clock::now();
    inline static int current_time_ = 0;
};

int main() {
    Parking<TestClock> parking(10);

    TestClock::SetNow(10);
    parking.Park({ 'A', 'A', 111, 'A', 99 });

    TestClock::SetNow(20);
    parking.Withdraw({ 'A', 'A', 111, 'A', 99 });
    parking.Park({ 'B', 'B', 222, 'B', 99 });

    TestClock::SetNow(40);
    assert(parking.GetCurrentBill({ 'A', 'A', 111, 'A', 99 }) == 100);
    assert(parking.GetCurrentBill({ 'B', 'B', 222, 'B', 99 }) == 200);
    parking.Park({ 'A', 'A', 111, 'A', 99 });

    TestClock::SetNow(50);
    assert(parking.GetCurrentBill({ 'A', 'A', 111, 'A', 99 }) == 200);
    assert(parking.GetCurrentBill({ 'B', 'B', 222, 'B', 99 }) == 300);
    assert(parking.GetCurrentBill({ 'C', 'C', 333, 'C', 99 }) == 0);
    parking.Withdraw({ 'B', 'B', 222, 'B', 99 });

    TestClock::SetNow(70);
    {
        // �������� ����
        auto bill = parking.EndPeriodAndGetBills();

        // ��� ��� ������ ������� ������������ �������,
        // ����� ��������� ��� �������� � �������������� ������
        assert((bill
            == std::unordered_map<VehiclePlate, int64_t, VehiclePlateHasher>{
                { {'A', 'A', 111, 'A', 99}, 400},
                { {'B', 'B', 222, 'B', 99}, 300 },
        }));
    }

    TestClock::SetNow(80);
    {
        // �������� ����
        auto bill = parking.EndPeriodAndGetBills();

        // ��� ��� ������ ������� ������������ �������,
        // ����� ��������� ��� �������� � �������������� ������
        assert((bill
            == std::unordered_map<VehiclePlate, int64_t, VehiclePlateHasher>{
                { {'A', 'A', 111, 'A', 99}, 100},
        }));
    }

    try {
        parking.Park({ 'A', 'A', 111, 'A', 99 });
        assert(false);
    }
    catch (ParkingException) {
    }

    try {
        parking.Withdraw({ 'B', 'B', 222, 'B', 99 });
        assert(false);
    }
    catch (ParkingException) {
    }

    std::cout << "Success!"s << std::endl;
}