#pragma once

#include<random>

class Random {
	private:
		static std::mt19937 randomizer;
		static std::uniform_int_distribution<std::mt19937::result_type> distribution;
	
	public:
		static void init()
		{
			randomizer.seed(std::random_device()());
		}

		static float Float()
		{
			return (float)distribution(randomizer) / (float)std::numeric_limits<uint32_t>::max();
		}
};