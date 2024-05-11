#include "Random.h"

std::mt19937 Random::randomizer;
std::uniform_int_distribution<std::mt19937::result_type> Random::distribution;