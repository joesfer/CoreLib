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

#pragma once
#include <stdlib.h>

namespace CoreLib {
namespace Memory {

	////////////////////////////////////////////////////////////////////////////
	// class StaticMemoryPool
	// Allocates a static chunk of memory once, and allocates memory from it
	// the mem pool is deleted at once when the allocator is destroyed
	////////////////////////////////////////////////////////////////////////////

	class StaticMemoryPoolBase {
	public:
		static void init( int poolSize );
		static void destroy();
		
		static void clearMemory(); // Wipes the memory chunk without freeing the memory and resets the allocator internal state. Call this before reusing the pool.
		
	protected:
		static char*				memory;
		static int				size;
		static int				used;
	};
	
	template< class T, int alignment = 4 >
	class StaticMemoryPool : protected StaticMemoryPoolBase {
	public:
		
		inline static T* alloc( int count ) {
			const int bytes = count * sizeof(T);
			const int padding = bytes % alignment;
			const int required = bytes + padding;
			
			// the array version of placement new stores extra information
			// at the beginning of the provided buffer corresponding to the  
			// size of the array!, therefore the pointer returned by new
			// will be >= buffer, and we must adjust the actual
			// memory used.
			// http://www.devx.com/cplus/10MinuteSolution/30508/1954
			// http://efreedom.com/Question/1-4011577/Placement-New-Array-Alignment
			// The biggest problem is that it's not specified how much extra mem
			// is used - according to my debug it's just 4 bytes (an int?) but
			// it invalidates the used + required >= size test if used + required == size
			// which we compensate with the placement_new_baggage constant

			const int placement_new_baggage = 4; // FIXME! empiric, not portable

			if ( used + required + placement_new_baggage >= size ) {
				return NULL; 
			}

			char* buffer = memory + used;
			 
			T* ptr = new( buffer )T[ count ];

			used = reinterpret_cast< char* >( ptr ) - memory + required;
			return ptr;
		};

		inline static void free( T* objects ) { /* do nothing */ }
		inline static void free( T* objects, int count ) { /* do nothing */ }

	private:
		template< class U >
		inline static U* allocIntegralType( int count ) {
			// besides the requested space, we'll need to add some extra padding
			// because "new" will return a pointer aligned with the requested type
			// which might slightly shifted from "memory + used"
			const int bytes = count * sizeof(T);
			const int padding = bytes % alignment;
			const int required = bytes + padding;
			if ( used + required >= size ) {
				return NULL; 
			}

			T* ptr = reinterpret_cast< T* >( memory + used );
			used += required;
			return ptr;
		};
	};

	template<>
	inline static int* StaticMemoryPool<int>::alloc( int count ) {
		return allocIntegralType<int>( count );
	};
	template<>
	inline static float* StaticMemoryPool<float>::alloc( int count ) {
		return allocIntegralType<float>( count );
	};
	template<>
	inline static double* StaticMemoryPool< double >::alloc( int count ) {
		return allocIntegralType<double>( count );
	};
	template<>
	inline static char* StaticMemoryPool< char >::alloc( int count ) {
		return allocIntegralType<char>( count );
	};

} // namespace Memory
} // namespace CoreLib