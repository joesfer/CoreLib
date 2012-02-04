/*
	================================================================================
	This software is released under the LGPL-3.0 license: http://www.opensource.org/licenses/lgpl-3.0.html

	Copyright (c) 2012, Jose Esteve. http://www.joesfer.com

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 3.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
	================================================================================
*/

#include <memory/staticPool.h>
#include <memory.h>
#include <assert.h>

namespace CoreLib {
namespace Memory {

char*  StaticMemoryPoolBase::memory = NULL;
int StaticMemoryPoolBase::size = 0;
int StaticMemoryPoolBase::used = 0;

////////////////////////////////////////////////////////////////////////////////
//StaticMemoryPool::init
////////////////////////////////////////////////////////////////////////////////
void StaticMemoryPoolBase::init( int poolSize ) {
	assert( poolSize > 0 );
	size = poolSize;
	memory = (char*)malloc( size );
	used = 0;
}

////////////////////////////////////////////////////////////////////////////////
// StaticMemoryPool::destroy
////////////////////////////////////////////////////////////////////////////////
void StaticMemoryPoolBase::destroy() {
	clearMemory();
	if ( memory != NULL ) {
		free( memory );
		memory = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
// StaticMemoryPool::clearMemory
//
// Wipes the memory chunk without freeing the memory and resets the 
// allocator internal state. Call this before reusing the pool.
////////////////////////////////////////////////////////////////////////////////
void StaticMemoryPoolBase::clearMemory() {
#if _DEBUG
	if ( used > 0 ) {
		memset( memory, 0xFF, used );
	}
#endif
	used = 0;
}

} // namespace Memory
} // namespace CoreLib




