#pragma once

#include <memory>
#include <iterator>
#include <iostream>

using std::shared_ptr;

/** An array based buffer class.
 *  The buffer is created with initial capacity, and expanded every time filled to it's maximum capacity.
 */
template <class V>
class ExpandableBuffer
{
public:
	/** Create a new buffer with the default capacity */
	ExpandableBuffer() : _size(0), _capacity(DEFAULT_CAPACITY)
	{
		initialize(_capacity);
	}

	/** Write a single value to the buffer */
	void write(V& value)
	{
		if (_size == _capacity)
			expand();

		_arr.get()[_size] = value;
		_size++;
	}

	/** Returns a pointer to the inner data contained within the buffer.
	 *  Note: Changes made to this data are reflected within the buffer.
	 */
	shared_ptr<V> rawData()
	{
		return _arr;
	}

	/** Resets the buffer to its initial status - the capacity is decreased to default and all data cleared */
	void reset()
	{
		_capacity = DEFAULT_CAPACITY;
		_size = 0;
		initialize(_capacity);
	}

	/** Returns the buffer size in bytes */
	int size() const
	{
		return _size;
	}

	/** Returns the buffer size in bytes */
	int sizeInBytes()
	{
		return _size * sizeof(V);
	}

private:
	static const int DEFAULT_CAPACITY = 512; // Initial capacity by number of elements

	shared_ptr<V> _arr; // Holds the data itself, expanded every time it reaches its limit
	int _size;
	int _capacity;

	/** Initializes the inner data array with the given capacity. */
	void initialize(int capacity)
	{
		_arr = shared_ptr<V>(new V[capacity], std::default_delete<V[]>());
	}

	/** Creates a new inner data array with double capacity and copies the elements to this new array.
	 *  The old array is deleted in the end of this process.
	 */
	void expand()
	{
		_capacity *= 2;

		// Creates a new expanded array with double capacity
		shared_ptr<V> expandedArr = shared_ptr<V>(new V[_capacity], std::default_delete<V[]>());

		// Copies the elements to the new array
		std::copy(_arr.get(), _arr.get() + _size,
			stdext::checked_array_iterator<V*>(expandedArr.get(), _size));

		// Set the new array as active one, old array is released by shared_ptr
		_arr = expandedArr;
	}
};

