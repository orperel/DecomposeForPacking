#include "PrimeNumbersModule.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "DFPLogger.h"

using std::istringstream;
using std::ifstream;
using std::getline;

// Static variables re-decleration
vector<int> PrimeNumbersModule::_primes;

// External file containing a list of prime numbers
const string PrimeNumbersModule::PRIMES_EXTERNAL_FILE = "PrimeNumbers.txt";

void PrimeNumbersModule::initialize()
{
	// Load prime numbers from external file
	loadPrimes();
}

/** Returns a generator of prime numbers. Primes will be generated in an ascending order. */
unique_ptr<PrimeNumbersGenerator> PrimeNumbersModule::createGenerator()
{
	// If the prime numbers haven't been loaded yet, initialize them now
	if (_primes.empty())
	{
		initialize(); // Loads the prime numbers from an external source file
	}

	unique_ptr<vector<int>::iterator> generatorIter(new vector<int>::iterator(_primes.begin()));
	unique_ptr<PrimeNumbersGenerator> generator(new PrimeNumbersGenerator(std::move(generatorIter)));
	return generator;
}

void PrimeNumbersModule::loadPrimes()
{
	ifstream primesFile;
	primesFile.open(PRIMES_EXTERNAL_FILE.c_str());

	// Locate the PrimeNumbers external file to read the prime numbers from it.
	// This file should be a resource of the project and should always exist.
	if (!primesFile.good()) {
		DFPLogger::getInstance().log("PrimeNumbersModule load error: failed to read prime numbers file: " + PRIMES_EXTERNAL_FILE, DFPLogger::ERROR);
		return;
	}

	string nextLine;

	// Read the lines of the tabular prime numbers file one by one
	while (getline(primesFile, nextLine))
	{
		istringstream lineStream(nextLine);
		int nextPrime;

		// For each line read the prime numbers one by one
		while (lineStream >> nextPrime)
		{
			_primes.push_back(nextPrime);
		}
	}

	// Close the file to release IO resources
	primesFile.close();
}

int PrimeNumbersGenerator::nextPrime()
{
	int currNum = **_generatorIter;
	(*_generatorIter)++;

	return currNum;
}

PrimeNumbersGenerator::PrimeNumbersGenerator(unique_ptr<vector<int>::iterator> generatorIter)
{
	_generatorIter = std::move(generatorIter);
}

PrimeNumbersGenerator::~PrimeNumbersGenerator()
{
	// Mem freed by smart pointers
}