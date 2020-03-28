#pragma once
#include "allocator.h"
#include <cstdlib>



static Expert::Allocator defaultAllocator(malloc, free);
