#pragma once

namespace MadSword
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f):m_Time(time)
		{
			m_Time = time;
		}

		operator float() const { return m_Time * 0.001f; }

		float GetSeconds() const { return m_Time*0.001f; }
		float GetMilliseconds() const { return  m_Time; }
	private:
		float m_Time=0.0f;
	};

	class Timer
	{
	public:
		Timer()
		{
			m_LasTime = std::chrono::high_resolution_clock::now();
		}

		float GetLasTime() const
		{
			auto tp = std::chrono::high_resolution_clock::now() - m_LasTime;
			return std::chrono::duration_cast<std::chrono::nanoseconds>(tp).count()*0.000001f;
		}

		~Timer()
		{
			auto delta = std::chrono::high_resolution_clock::now() - m_LasTime;
			auto a = std::chrono::duration_cast<std::chrono::nanoseconds>(delta);

			std::cout << "t " << a.count() * 0.000001f << "ms" << "\n";
		}

		std::chrono::time_point<std::chrono::steady_clock> m_LasTime;
	};
}