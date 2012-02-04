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

//////////////////////////////////////////////////////////////////////////
// Note: the List class is based on code from the idLib, which is released 
// under GPL license. Since I'm releasing my code under LGPL license, I may 
// have to remove it or change it at some point due to license incompatibilities.
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <stdlib.h>
#include <assert.h>
#include <memory/standardAllocator.h>

namespace CoreLib {
	template< typename T >
	inline int ListSortCompare( const T *a, const T *b ) { return *a - *b; }

	//////////////////////////////////////////////////////////////////////////
	// class List
	//
	// Similar to std::vector, but allowing for more efficient element removal
	// copy and granularity control.
	//////////////////////////////////////////////////////////////////////////
	template< typename T, class Allocator = CoreLib::Memory::StandardAllocator<T> >
	class List {
	public:
		typedef const T			ConstType;
		typedef T				Type;
		typedef Type*			Iterator;
		typedef ConstType*		ConstIterator;
		typedef int				cmp_t( const T *, const T * );

		explicit List( size_t granularity = DEFAULT_GRANULARITY );
		List( const List& other);
		~List();

		size_t size() const;		// number of elements in the list
		size_t capacity() const;	// total number of elements that can be stored without resizing the list
		bool empty() const;

		void clear();	// clears the list and storage
		void resize( size_t newNum, bool resizeCapacity = true );	// set number of elements in list and resize to exactly this number if necessary
		void preAllocate( size_t newCapacity ); // makes sure the list has capacity for newSize number of elements, without changing the current element count

		void setGranularity( size_t granularity );
		size_t getGranularity() const;

		List&			operator=( const List& other );
		Type&			operator[]( int index );
		ConstType&		operator[]( int index ) const;	

		Type&		append();							// returns reference to a new data element at the end of the list
		int			append( ConstType& obj );			// append element
		int			append( const List &other );		// append list

		int			addUnique( ConstType& obj );		// add unique element

		int			findIndex( ConstType& obj ) const;				// find the index for the given element

		bool		removeFast( ConstType& obj );		// remove the element, move the last element into its spot
		bool		removeIndexFast( size_t i );		// remove i-th element, move the last element into its spot
		void		sort( cmp_t *compare = ListSortCompare<T> );	// sort the list
		void		swap( List &other );				// swap the contents of the lists

		Iterator		begin();						// return list[ 0 ]
		Iterator		end();							// return list[ numElements ]	(one past end)

		ConstIterator	begin() const;					// return list[ 0 ]
		ConstIterator	end() const;					// return list[ numElements ]	(one past end)

	private:
		void setSize(size_t numElem);

	private:
		Type *			list;
		size_t			numElements;
		size_t			allocedSize;
		size_t			granularity;
		const static size_t DEFAULT_GRANULARITY = 16;
	};

	#include "List.inl"
}