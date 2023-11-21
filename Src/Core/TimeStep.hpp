#pragma once

namespace HAIEngine
{
	class TimeStep
	{
	public:
		TimeStep(float time = 0.0f) : m_time(time) {}

		operator float() { return m_time; }

		float GetSeconds() const { return m_time; }
		float GetMilliSeconds() const { return m_time * 1000.0f; }
	private:
		float m_time;
	};
}