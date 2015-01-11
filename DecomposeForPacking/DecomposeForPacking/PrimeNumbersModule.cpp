#include "PrimeNumbersModule.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "DFPLogger.h"

using std::istringstream;
using std::ifstream;
using std::getline;

const string PrimeNumbersModule::PRIMES_EXTERNAL_FILE = "PrimeNumbers.txt";

PrimeNumbersModule::PrimeNumbersModule()
{
}


PrimeNumbersModule::~PrimeNumbersModule()
{
}

void PrimeNumbersModule::loadPrimes()
{
	ifstream primesFile;
	primesFile.open(PRIMES_EXTERNAL_FILE.c_str());

	if (!primesFile.good()) {
		DFPLogger::getInstance().log("Error reading prime numbers file: " + PRIMES_EXTERNAL_FILE, DFPLogger::ERROR);
		return;
	}

	string nextLine;

	while (getline(primesFile, nextLine))
	{
		istringstream lineStream(nextLine);
		int nextPrime;

		while (lineStream >> nextPrime)
		{
			_primes.push_back(nextPrime);
		}
	}

	// Close the file to release IO resources
	primesFile.close();
}