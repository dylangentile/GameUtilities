#include "expert/allocator.h"
#include <cstring> //memset

namespace Expert 
{
	
	Allocator::Allocator(void* (allocator)(size_t), void (free_er)(void*)) : mAllocator(allocator), mFree(free_er)
	{
		
	}

	Allocator::Allocator(const Allocator& allocator_)
	{
		this->mAllocator = allocator_.mAllocator;
		this->mFree = allocator_.mFree;
	}

	Allocator& 
	Allocator::operator=(const Allocator& allocator_)
	{
		this->mAllocator = allocator_.mAllocator;
		this->mFree = allocator_.mFree;
		return *this;
	}

	Allocator::~Allocator()
	{

	}

	

	void*
	Allocator::operator()(size_t num, size_t size)
	{
		size_t sz = num * size;
		void* ptr = mAllocator(sz);
		memset(ptr, 0, sz);
		return ptr;
	}

	void*
	Allocator::alloc(size_t size)
	{
		return mAllocator(size);
	}

	void*
	Allocator::realloc(void* ptr, size_t size)
	{
		void* data = alloc(size);
		memcpy(data, ptr, size);
		free(ptr);
		return data;
	}

	void
	Allocator::free(void* ptr)
	{
		mFree(ptr);
	}
}