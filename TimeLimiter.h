#pragma once

#include <chrono>
#include <thread>
#include <atomic>

class TimeLimiter {

public:
	TimeLimiter(int tps)
		: m_frame_time(std::chrono::nanoseconds(static_cast<long>(1000000000.0 / tps)))
		, m_previous_time(std::chrono::steady_clock::now())
		, m_time_taken(std::chrono::nanoseconds(0))
	{
		sleep();
	}

	void sleep() {
		m_then = std::chrono::steady_clock::now();
		m_time_taken = std::chrono::nanoseconds((m_then - m_previous_time).count());
		if (m_time_taken < m_frame_time) {
			std::this_thread::sleep_for(m_frame_time - m_time_taken);
		}
		m_previous_time = std::chrono::steady_clock::now();
		m_dt = std::chrono::nanoseconds(m_previous_time - m_then);
	}

	const double dt() const {
		return static_cast<double>(m_dt.count() / 1000000000.0);
	}

private:
	std::chrono::nanoseconds m_frame_time;
	std::chrono::time_point<std::chrono::steady_clock> m_previous_time;
	std::chrono::time_point<std::chrono::steady_clock> m_then;
	std::chrono::nanoseconds m_time_taken;
	std::chrono::nanoseconds m_dt;
};