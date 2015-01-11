#pragma once

#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;
using std::string;

// Forward declerations
class PrimeNumbersGenerator;

/** The PrimeNumbersModule is a utility that assists generating prime numbers.
 *  Due to the fact that prime numbers are expensive to find, a list of pre-found primes
 *  is supplied via an external file.
 */
class PrimeNumbersModule
{
public:
	~PrimeNumbersModule() = default;

	/** Initializes the PrimeNumbersModule with the list of prime numbers.
	 *  After this call generators can be created.
	 */
	static void initialize();

	/** Returns a generator of prime numbers. Primes will be generated in an ascending order. */
	static unique_ptr<PrimeNumbersGenerator> createGenerator();
private:
	// Consts
	static const string PRIMES_EXTERNAL_FILE;

	// Fields
	static vector<int> _primes;

	// Methods
	PrimeNumbersModule() = default;

	/** Loads the prime numbers from an external file. */
	static void loadPrimes();
};

/** Generator of prime numbers. Primes will be generated in an ascending order. */
class PrimeNumbersGenerator
{
public:
	PrimeNumbersGenerator(unique_ptr<vector<int>::iterator> generatorIter);
	~PrimeNumbersGenerator();

	/** Returns the next prime number sequentially */
	int nextPrime();
private:

	/** Holds the position of the current generator over */
	unique_ptr<vector<int>::iterator> _generatorIter;

	// Allow access to private ctor to create generators
	friend class PrimeNumbersModule;
};