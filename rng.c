#include"rng.h"
#include<time.h>
#include<stdint.h>

// TODO: Find good set of parameters.
const uint64_t RngParam_a = 6364136223846793005llu;
const uint64_t RngParam_c = 9754186451795953191llu;

int SeedGenerated = 0;
uint64_t Seed;

uint64_t GetNewSeed(void) {
	Seed = clock() + time(NULL);
	return Seed;
}

uint64_t CurrentSeed;
uint64_t Generator(void) {
	if (SeedGenerated == 0) {
		Seed = GetNewSeed();
		SeedGenerated = 1;
	}

	// m input not needed. the mod is UINT64_MAX
	uint64_t NewResult = (RngParam_a * Seed) + RngParam_c;
	Seed = NewResult;
	return NewResult;
}

uint64_t GetRandomInt64(void) {
	return Generator();
}

uint32_t GetRandomInt32(void) {
	return Generator();
}

uint64_t GetRandomIntBetween(uint64_t Min, uint64_t Max) {
	if (Min == Max) return Min;

	int Difference = Max - Min;
	uint64_t n = Generator() - Min;
	n %= Difference;
	n += Min;

	return n;
}
