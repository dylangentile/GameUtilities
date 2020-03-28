#pragma once
#include "expert/allocator.h"
#include <cstdlib>



namespace Expert
{
	class Vector
	{
	public:
		Vector();
		Vector(size_t size, size_t step = 16);
		Vector(const Allocator& allocator_, size_t size = 16, size_t step = 16);
		~Vector();

	public:
		class Iterator
		{
		public:
			Iterator& operator+(const int& i_val);
			Iterator& operator+=(const int& i_val);
			Iterator& operator++(int);

			Iterator& operator-(const int& i_val);
			Iterator& operator-=(const int& i_val);
			Iterator& operator--(int);
		private:
			friend class Vector;
			Iterator(Vector* parent, void** ptr, bool reverse);
			~Iterator();

			


		private:
			bool mReverse;
			Vector* pVector;
			void** mPtr;
		};

	public:


		Iterator insert(void* ptr);
		void erase(uintptr_t offset);
		void clear();

		void resize(size_t size);

		

		

	


	private:
		friend class Iterator;
		Allocator mAllocator;
		size_t mSize, mStep;
		size_t mCount;
		void** mData;
		void** current;

	};
}








