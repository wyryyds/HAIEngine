#pragma once

namespace HAIEngine
{
	class TimeStep
	{
	public:
		TimeStep(double time = 0.0f) : m_time(time) {}

		operator double() const { return m_time; }

		float GetSeconds() const { return m_time; }
		float GetMilliSeconds() const { return m_time * 1000.0f; }
	private:
		double m_time;
	};
}