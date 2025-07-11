#include "log_duration.h"

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <cassert>

using namespace std::literals;

std::vector<float> ComputeAvgTemp(const std::vector<std::vector<float>>& vs) {
    // место для вашего решения

    std::vector<float> res;

    if (vs.empty()) return res;

    for (size_t i = 0; i < vs[0].size(); ++i) {
        
        float sum = .0f;
        int cnt = 0;
        for (size_t j = 0; j < vs.size(); ++j) {
            const float t = vs[j][i];
            sum += (t > 0) ? t: 0;
            cnt += (t > 0) ? 1 : 0;
        }
        res.push_back((cnt > 0)?sum / cnt:0);
    }
    return res;
}

std::vector<float> GetRandomVector(int size) {
    static std::mt19937 engine;
    std::uniform_real_distribution<float> d(-100, 100);

    std::vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

void test() {
    // 4 дня по 3 измерения
    std::vector<std::vector<float>> v = {
        {0, -1, -1},
        {1, -2, -2},
        {2, 3, -3},
        {3, 4, -4}
    };

    // среднее для 0-го измерения (1+2+3) / 3 = 2 (не учитывам 0)
    // среднее для 1-го измерения (3+4) / 2 = 3.5 (не учитывам -1, -2)
    // среднее для 2-го не определено (все температуры отрицательны), поэтому должен быть 0

    assert(ComputeAvgTemp(v) == std::vector<float>({ 2, 3.5f, 0 }));
}

int main() {
    std::vector<std::vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
    }

    std::vector<float> avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        avg = ComputeAvgTemp(data);
    }

    std::cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << std::endl;
    
    test();
    return 0;
}