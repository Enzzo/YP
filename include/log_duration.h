#pragma once

#include <chrono>
#include <string>
#include <iostream>

#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)

using namespace std::literals;

class LogDuration {
	using Clock = std::chrono::steady_clock;

	Clock::time_point begin_time_ = Clock::now();
	const std::string operation_;

public:	

	LogDuration() : operation_(""s){};
	
	LogDuration(const std::string& operation) : operation_(operation + ": "s) {};
	
	~LogDuration() {
		std::cerr << this->Duration() << std::endl;
	}

	inline const std::string Duration()const;
};

inline const std::string LogDuration::Duration()const {
	Clock::time_point end_time_ = Clock::now();
	Clock::duration dur_ = end_time_ - begin_time_;

	return operation_ + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(dur_).count()) + " ms"s;
}