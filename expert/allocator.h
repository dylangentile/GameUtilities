#pragma once

namespace Expert 
{
	

	class Allocator
	{
	public:
		Allocator(void* (allocator)(size_t), void (free_er)(void*));
		Allocator(const Allocator& allocator_);
		Allocator& operator=(const Allocator& allocator_);
		~Allocator();
		void* operator()(size_t num, size_t size);
		

		void* alloc(size_t size);
		void* realloc(void* ptr, size_t size);
		void free(void* ptr);

	private:
	
		void* (*mAllocator)(size_t);
		void (*mFree)(void*);
	};



}