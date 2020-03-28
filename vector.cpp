#include "expert/vector.h"
#include "expert/defaultAllocator.h"

#include <cassert>
#include <cstring>

namespace Expert
{ 

	Vector::Vector() : mAllocator(defaultAllocator), mSize(16), mStep(16), mCount(0)
	{
		mData = (void**)mAllocator(mSize, sizeof(void*));
		current = mData;
	}

	Vector::Vector(size_t size, size_t step) : mAllocator(defaultAllocator), mSize(size), mStep(step), mCount(0)
	{
		mData = (void**)mAllocator(mSize, sizeof(void*));
		current = mData;
	}

	Vector::Vector(const Allocator& allocator_, size_t size, size_t step) : mAllocator(allocator_), mSize(size), mStep(step), mCount(0)
	{
		mData = (void**)mAllocator(mSize, sizeof(void*));
		current = mData;
	}

	Vector::~Vector()
	{

	}

	Vector::Iterator
	Vector::insert(void* ptr)
	{
		mCount++;
		
		*current = ptr;

		if (current - mData == mSize - 1)
			resize(mSize + mStep);
		
		return current;
		
	}

	void
	Vector::erase(uintptr_t offset)
	{
		void** ptr = mData + offset;
		for (void** it = ptr; it != mData + mCount; it++)
		{
			if (ptr == it)
				continue;

			*ptr = *it;
			ptr++;
		}

		if (mSize - mCount > mStep + mStep / 4)
		{
			resize(mSize - mStep);
		}

		mCount--;
	}


	void
	Vector::clear()
	{
		resize(mStep);
		mCount = 0;
		memset(mData, 0, mSize);
	}

	void
	Vector::resize(size_t size)
	{
		//assert(size > mSize && "Invalid vector resize!");
		mAllocator.realloc(mData, size);
		mSize = size;
		if (mCount > mSize)
			mCount = mSize;
	}





	Vector::Iterator::Iterator(Vector* parent, void** loc, bool reverse) : pVector(parent),  mPtr(loc), mReverse(reverse)
	{

	}

	Vector::Iterator::~Iterator()
	{

	}


}