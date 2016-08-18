#include "Shaker.h"
#include <random>
#include "Log\Log.h"
#include "Engine\Engine.h"
#include "Input\FileReader.h"
#include <sstream>
/*
Generates a random shake.
*/
CShaker::CShaker()
:m_Timer(0)
{
}

//void CShaker::AddRandoms(const std::string &RandomValuesFile)
//{
//	std::ifstream source;
//	source.open(RandomValuesFile, std::ios_base::in);
//	if (!source)
//		assert(false);
//	else
//	{
//		unsigned int i = 0;
//		float l_Random;
//		for (std::string line; std::getline(source, line);)
//		{
//			std::istringstream in(line);
//
//			if (i == 0)
//			{
//				in >> m_Duration;
//				++i;
//			}
//			else if (i == 1)
//			{
//				in >> m_Frequency;
//				++i;
//			}
//			else
//			{
//				in >> l_Random;
//				m_Randoms.push_back(l_Random);
//			}
//		}
//	}
//}

/*
Computes some random values to be used in the shaker.
*/
void CShaker::Start(int Duration, int Frequency, float MinRandom, float MaxRandom, unsigned int SameSignMaxConsecutiveNumbers)
{
	m_Duration = Duration;
	m_Frequency = Frequency;

	std::random_device l_RandomDevice;
	std::mt19937 l_Mt(l_RandomDevice());

	std::uniform_real_distribution<double> dist(MinRandom, MaxRandom);

	int l_Samples = (int)(((float)m_Duration / 1000.0f) * m_Frequency);
	float l_Random = 0.0f;

	unsigned int l_Consecutives = 0;
	float l_LastRandom = 0.0f;

	for (int i = 0; i < l_Samples; ++i)
	{
		l_Random = float(dist(l_Mt));

		if (l_LastRandom != 0.0f)
		{
			if (l_Random > 0.0f && (l_Random*l_LastRandom > 0.0f))
				l_Consecutives++;
			else if ((l_Random < 0.0f && (l_Random*l_LastRandom > 0.0f)))
				l_Consecutives++;
			else l_Consecutives = 0;

			if (l_Consecutives > SameSignMaxConsecutiveNumbers)
			{
				l_Random *= -1.0f;
				l_Consecutives = 0;
			}
		}

		m_Randoms.push_back(l_Random);
		l_LastRandom = l_Random;
	}
}

/*  
Computes some random values to be used in the shaker and writes them to a file.
Duration [in]: Shake duration in milliseconds.
Frequency [in]: Shake Frecuency in milliseconds.
MinRandom [in]: Minimum random value that will be generated.
MaxRandom [in]: Maximum random value that will be generated.
*/
void CShaker::Start(int Duration, int Frequency, float MinRandom, float MaxRandom, const std::string &FileName, unsigned int SameSignMaxConsecutiveNumbers)
{
	std::ofstream out(FileName,std::ios::out);
	if (!out.good())
	{
		out.close();
		assert(false);
	}

	m_Duration = Duration;
	m_Frequency = Frequency;
	out << Duration;
	out << "\n";
	out << m_Frequency;

	//out.write((char*)&m_Duration, sizeof(int));
	//out.write((char*)&m_Frequency, sizeof(int));

	std::random_device l_RandomDevice;
	std::mt19937 l_Mt(l_RandomDevice());

	std::uniform_real_distribution<double> dist(MinRandom, MaxRandom);

	int l_Samples = (int) (((float)m_Duration / 1000.0f) * m_Frequency);
	float l_Random = 0.0f;

	unsigned int l_Consecutives = 0;
	float l_LastRandom = 0.0f;

	for (int i = 0; i < l_Samples; ++i)
	{
		l_Random = float(dist(l_Mt));
		
		if (l_LastRandom != 0.0f)
		{
			if (l_Random > 0.0f && (l_Random*l_LastRandom > 0.0f))
				l_Consecutives++;
			else if ((l_Random < 0.0f && (l_Random*l_LastRandom > 0.0f)))
				l_Consecutives++;
			else l_Consecutives = 0;

			if (l_Consecutives > SameSignMaxConsecutiveNumbers)
			{
				l_Random *= -1.0f;
				l_Consecutives = 0;
			}
		}

		m_Randoms.push_back(l_Random); 
		out << "\n";
		out << l_Random;
		l_LastRandom = l_Random;
	}
	out.close();
}

/*
Loads random values from file.
*/
void CShaker::Start(const std::string &RandomValuesFile)
{
	std::ifstream source;
	source.open(RandomValuesFile, std::ios_base::in);
	if (!source)
		assert(false);
	else
	{
		unsigned int i = 0;
		float l_Random;
		for (std::string line; std::getline(source, line);)
		{
			std::istringstream in(line);

			if (i == 0)
			{
				in >> m_Duration;
				++i;
			}
			else if (i == 1)
			{
				in >> m_Frequency;
				++i;
			}
			else
			{
				in >> l_Random;
				m_Randoms.push_back(l_Random);
			}
		}
	}
}

void CShaker::Update(float ElapsedTime)
{
	m_Timer += (int)(ElapsedTime*1000);
}

/*
Returns the amplitue at a specified time if t is passed. Otherwise uses internal time
*/
float CShaker::GetAmplitude(int Time) const
{
	float l_Amplitue = 0.0f;
	if (Time == 0)
	{
		Time = m_Timer;
	}

	float s = ((float)Time / 1000.0f) * (float)m_Frequency;
	unsigned int s0 = (unsigned int)(floor(s));
	unsigned int s1 = s0 + 1;

	float k = GetDecay(m_Timer);

	float l_Far = s - (float)s0;
	float l_Slope = Noise(s1) - Noise(s0);
	float l_B = Noise(s0);

	l_Amplitue = (l_B + (l_Far)*(l_Slope)) * k;

	return l_Amplitue;
}

/* 
Retrieve a random value from the samples
*/
float CShaker::Noise(unsigned int Sample) const
{
	
	if (Sample >= m_Randoms.size()) return 0;
	return m_Randoms[Sample];
}

/*
Returns the shake's decay (between 0.0 and 1.0).
*/
float CShaker::GetDecay(int Time) const
{
	if (Time >= m_Duration){ return 0.0f; }
	return (float(m_Duration - Time)) / m_Duration;
};

void CShaker::FinishShaking()
{
	m_Timer = 0;
	m_Randoms.clear();
}