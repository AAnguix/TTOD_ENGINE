#ifndef _SHAKER_H
#define _SHAKER_H

#include <vector>

class CShaker
{

private:
	//Milliseconds
	int m_Duration;
	int m_Frequency;
	int m_Timer;

	std::vector<float> m_Randoms;

	float GetDecay(int Time) const;
	float Noise(unsigned int Sample) const;

public:
	CShaker();
	
	//This will be called only one time
	//void AddRandoms(const std::string &RandomValuesFile);

	/*Generate random values*/
	void Start(int Duration, int Frequency, float MinRandom, float MaxRandom, unsigned int SameSignMaxConsecutiveNumbers = 2);
	
	/*Generate random values and writes them to a file*/
	void Start(int Duration, int Frequency, float MinRandom, float MaxRandom, const std::string &FileName, unsigned int SameSignMaxConsecutiveNumbers = 2);
	
	/*Load random values from file*/
	void Start(const std::string &RandomValuesFile);
	
	void Update(float ElapsedTime);
	float GetAmplitude(int Time = 0) const;
	void FinishShaking();

};

#endif