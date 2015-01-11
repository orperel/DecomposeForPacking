#pragma once

#include <vector>

using std::vector;
using std::string;

class PrimeNumbersModule
{
public:
	PrimeNumbersModule();
	~PrimeNumbersModule();

	void loadPrimes();

	vector<int> _primes;
private:
	static const string PRIMES_EXTERNAL_FILE;
};

