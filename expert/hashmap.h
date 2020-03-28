#pragma once
#include <cstdint>
#include "expert/allocator.h"
/*
NOTES:

The Content* returned by HashMap::find is modifiable so long as no other methodas are called after.
	i.e. other methods will move that pointer and make it garbage.

The Content* returned by HashMap::find can be used to iterate the hashmap(assuming you know the size)


*/


namespace Expert
{

	class HashMap
	{
	public:
		HashMap(uint64_t(hasher)(uintptr_t), size_t initialSize = 8);
		HashMap(const Allocator& allocator_, uint64_t(hasher)(uintptr_t), size_t initialSize = 8);
		HashMap(const HashMap& map);
		~HashMap();



		class Content
		{
		public:
			void* key;
			void* value;
		private:
			friend class HashMap;
			Content* dupe;
		};


		Content* insert(void* key, void* value);
		void erase(void* key);
		Content* find(void* key);
		void resize(size_t size);
		size_t size();
		size_t length();


	private:
		Content* rawInsert(void* key, void* value); //raw Insert doesn't perform resizes


		Allocator mAllocator;
		uint64_t(*mHasher)(uintptr_t);


		Content* mData;
		size_t mSize, elementCount;
		size_t mStep;
		size_t collisonThreshold;



	};

	/*
	template<typename Key, typename Value>
	class HashMapWrapper
	{
		HashMapWrapper();
		~HashMapWrapper();

		typedef struct {
			const T key;
			T& value;
		}Pair;

	};
	*/




}

