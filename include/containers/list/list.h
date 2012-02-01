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
#include <assert.h>
#include <memory/standardAllocator.h>

namespace CoreLib {
	template< typename T >
	inline int ListSortCompare( const T *a, const T *b ) { return *a - *b; }

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
#if 0
namespace idLib {
	/*
	================
	ListSortCompare
	================
	*/
	template< class type >
	inline int ListSortCompare( const type *a, const type *b ) {
		return *a - *b;
	}

	/*
	================
	class List
	================
	*/
	template< class type, class AllocPolicy = StandardAllocator< type > >
	class idList {
	public:
		typedef const type		ConstType;
		typedef type			Type;
		typedef Type*			Iterator;
		typedef ConstType*		ConstIterator;
		typedef int				cmp_t( const type *, const type * );

		static const int		DEFAULT_GRANULARITY = 16;

		explicit		idList( int newgranularity = DEFAULT_GRANULARITY );
		idList( const idList &other );

		template< class Iter >	
		explicit		idList( Iter begin, Iter end ) :
			list( NULL ), 
			allocedSize( 0 ),
			granularity( DEFAULT_GRANULARITY ),
			allocator( NULL ) {
				Clear();
				CopyFromRange( begin, end );
		}

		~idList( void );

		void			Clear( void );										// clear the list
		int				Num( void ) const;									// returns number of elements in list
		bool			Empty( void ) const;								// returns true if number elements in list is 0
		void			SetGranularity( int newgranularity );				// set new granularity
		int				GetGranularity( void ) const;						// get the current granularity

		size_t			Allocated( void ) const;							// returns total allocedSize of allocated memory
		size_t			Size( void ) const;									// returns total allocedSize of allocated memory including allocedSize of list type
		size_t			MemoryUsed( void ) const;							// returns allocedSize of the used elements in the list

		idList&			operator=( const idList &other );
		type &			operator[]( int index );
		const type &	operator[]( int index ) const;	

		//void			Condense( void );									// resizes list to exactly the number of elements it contains
		void			Resize( int newsize );								// resizes list to the given number of elements
		void			Resize( int newsize, int newgranularity );			// resizes list and sets new granularity
		void			SetNum( int newnum, bool resize = true );			// set number of elements in list and resize to exactly this number if necessary
		void			AssureSize( int newSize );							// assure list has given number of elements, but leave them uninitialized
		void			AssureSize( int newSize, const type &initValue );	// assure list has given number of elements and initialize any new elements
		void			PreAllocate( int newSize );

		type &			Alloc( void );										// returns reference to a new data element at the end of the list
		int				Append( const type & obj );							// append element
		int				Append( const idList &other );						// append list
		int				AddUnique( const type & obj );						// add unique element
		int				Insert( const type & obj, int index = 0 );			// insert the element at the given index	

		template< typename T >
		int				FindIndex( T obj ) const;				// find the index for the given element
		int				FindIndexBinary( const type & key, cmp_t *compare = &ListSortCompare<type> ) const;
		type *			FindElement( const type & obj ) const;				// find pointer to the given element
		int				FindNull( void ) const;								// find the index for the first NULL pointer in the list

		void			Fill( int numElements, const type & obj );			// resize the list, if neccessary and set all items to obj

		int				IndexOf( const type *obj ) const;					// returns the index for the pointer to an element in the list
		bool			RemoveIndex( int index );							// remove the element at the given index
		bool			RemoveIndexFast( int index );						// remove the element at the given index and put the last element into its spot
		bool			Remove( const type & obj );							// remove the element
		bool			RemoveFast( const type & obj );						// remove the element, move the last element into its spot
		void			Sort( cmp_t *compare = ListSortCompare<type> );	// sort the list
		void			Swap( idList &other );								// swap the contents of the lists

		// members for compatibility with STL algorithms
		Iterator		Begin( void );										// return list[ 0 ]
		Iterator		End( void );										// return list[ numElements ]	(one past end)

		ConstIterator	Begin( void ) const;								// return list[ 0 ]
		ConstIterator	End( void ) const;									// return list[ numElements ]	(one past end)

		type&			Front();											// return *list[ 0 ]
		const type&		Front() const;										// return *list[ 0 ]

		type&			Back();												// return *list[ numElements - 1 ]
		const type&		Back() const;										// return *list[ numElements - 1 ]

		bool			Remove( Iterator iter );

		template< class Iter >
		void			CopyFromRange( Iter begin, Iter end ) {
			AssureSize( end - begin );
			for( int i = 0; i < numElements; i++, ++begin ) {
				list[ i ] = *begin;
			}
		}
	protected:
		type *			list;
		int				numElements;
		int				allocedSize;
		int				granularity;
	};

	#include "List.inl"
}

#endif