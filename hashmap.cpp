#include "expert/hashmap.h"
#include "expert/defaultAllocator.h"
#include <cassert>
#include <cstring> //memset



namespace Expert
{

	HashMap::HashMap(uint64_t(hasher)(uintptr_t), size_t initialSize) : mAllocator(defaultAllocator), mHasher(hasher), mSize(initialSize), mStep(16), collisonThreshold(256), elementCount(0)
	{
		mData = (Content*)mAllocator(mSize, sizeof(Content));
		assert(mData != nullptr && "The Allocator failed to allocate the memory for the HashMap!");
	}

	HashMap::HashMap(const Allocator& allocator_, uint64_t(hasher)(uintptr_t), size_t initialSize) : mAllocator(allocator_), mHasher(hasher), mSize(initialSize), mStep(16), collisonThreshold(256), elementCount(0)
	{
		mData = (Content*)mAllocator(mSize, sizeof(Content));
		assert(mData != nullptr && "The Allocator failed to allocate the memory for the HashMap!");
	}

	HashMap::HashMap(const HashMap& map) : mAllocator(map.mAllocator)
	{
		this->mHasher = map.mHasher;
		this->mSize = map.mSize;
		this->mStep = map.mStep;
		this->elementCount = map.elementCount;
		this->collisonThreshold = map.collisonThreshold;

		this->mData = (Content*)mAllocator(this->mSize, sizeof(Content));
		memcpy(this->mData, map.mData, this->mSize * sizeof(Content));
		
	}

	HashMap::~HashMap()
	{
		mAllocator.free(mData);
	}

	void
	HashMap::resize(size_t size)
	{
		assert(size > mSize && "Invalid HashMap resize!");
		size_t oldSize = mSize;
		mSize = size;

		Content* oldData = mData;
		mData = (Content*)mAllocator(mSize, sizeof(Content));
		assert(mData != nullptr && "The Allocator failed to allocate the memory for the HashMap!");

		

		elementCount = 0;
		for (Content* ptr = oldData; ptr != oldData + oldSize; ptr++)
		{
			rawInsert(ptr->key, ptr->value);
			Content* it = ptr;
			while (it->dupe != nullptr)
			{
				it = it->dupe;
				rawInsert(it->key, it->value);
			}
			
		}

		mAllocator.free(oldData);

	}


	

	HashMap::Content*
	HashMap::insert(void* key, void* value)
	{
		elementCount++;

		if (elementCount >= mSize)
		{
			resize(mSize + mStep);
		}

		uint64_t hash_val = mHasher((uintptr_t)key);
		hash_val %= mSize;

		Content* bucket_ptr = mData + hash_val;
		if (bucket_ptr->key == nullptr)
		{
			bucket_ptr->key = key;
			bucket_ptr->value = value;
		}
		else if (mSize >= collisonThreshold)
		{
			while (bucket_ptr->dupe != nullptr)
				bucket_ptr = bucket_ptr->dupe;

			bucket_ptr->dupe = (Content*)mAllocator(1, sizeof(Content));
			bucket_ptr = bucket_ptr->dupe;
			bucket_ptr->key = key;
			bucket_ptr->value = value;
			return bucket_ptr;

		}
		else
		{
			resize(mSize + mStep);
			return insert(key, value);
		}

		return bucket_ptr;



	}

	HashMap::Content*
	HashMap::rawInsert(void* key, void* value)
	{
		elementCount++;
		uint64_t hash_val = mHasher((uintptr_t)key);
		hash_val %= mSize;

		Content* bucket_ptr = mData + hash_val;
		if (bucket_ptr->key == nullptr)
		{
			bucket_ptr->key = key;
			bucket_ptr->value = value;
		}
		else 
		{
			while (bucket_ptr->dupe != nullptr)
				bucket_ptr = bucket_ptr->dupe;

			bucket_ptr->dupe = (Content*)mAllocator(1, sizeof(Content));
			bucket_ptr = bucket_ptr->dupe;
			bucket_ptr->key = key;
			bucket_ptr->value = value;
			return bucket_ptr;

		}
		

		return bucket_ptr;
	}

	HashMap::Content*
	HashMap::find(void* key)
	{
		uintptr_t keyVal = (uintptr_t)key;
		uint64_t hash = mHasher(keyVal);
		hash %= mSize;

		Content* bucket_ptr = mData + hash;


		if (bucket_ptr == nullptr)
		{
			return nullptr;
		}
		else if ((uintptr_t)bucket_ptr->key == keyVal)
		{
			return bucket_ptr;
		}
		else if (bucket_ptr->dupe == nullptr)
			return nullptr;
		else
		{
			while ((uintptr_t)bucket_ptr->key != keyVal)
			{
				bucket_ptr = bucket_ptr->dupe;
				if (bucket_ptr == nullptr)
					return nullptr;
			}
			return bucket_ptr;
		}



	}


	void
	HashMap::erase(void* key)
	{
		
		Content* pair = find(key);
		if (pair != nullptr)
			pair->key = nullptr;
		//pair->value is now garbage. Not gonna bother setting it to nullptr;

		elementCount--;

	}
	
	size_t
	HashMap::length()
	{
		return mSize;
	}

	size_t
	HashMap::size()
	{
		return elementCount;
	}
}
