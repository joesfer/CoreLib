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

namespace CoreLib {
namespace Memory {

	////////////////////////////////////////////////////////////////////////////
	// StandardAllocator
	//
	// Use the standard heap allocation
	////////////////////////////////////////////////////////////////////////////
	template< class T >
	class StandardAllocator {
	public:
		inline static T* alloc( size_t count ) {
			return new T[ count ];
		}

		inline static void free( T* objects ) {
			delete[] objects;
		}

		inline static void free( T* objects, size_t /*count*/ ) {
			delete[] objects;
		}
	};
}
}
