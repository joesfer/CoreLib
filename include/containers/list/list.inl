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

/*
================
List< type, AllocPolicy >::List( int )
================
*/
template< typename type, class AllocPolicy >
inline List< type, AllocPolicy >::List( size_t newgranularity )
	:	allocedSize( 0 ),
		granularity( newgranularity ),
		list( NULL ) {
	assert( granularity > 0 );
	clear();
}

/*
================
List< type, AllocPolicy >::List( const List &other )
================
*/
template< typename type, class AllocPolicy >
inline List< type, AllocPolicy >::List( const List &other ) 
	:	allocedSize( 0 ),
		list( NULL ) {
	*this = other;
}

/*
================
List< type, AllocPolicy >::~List
================
*/
template< typename type, class AllocPolicy >
inline List< type, AllocPolicy >::~List( void ) {
	clear();
}

/*
================
List< type, AllocPolicy >::clear

Frees up the memory allocated by the list.  Assumes that type automatically handles freeing up memory.
================
*/
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::clear() {
	AllocPolicy::free( list, allocedSize );
	list		= NULL;
	numElements	= 0;
	allocedSize		= 0;	
}
#if 0
/*
================
List< type, AllocPolicy >::allocated

return total memory allocated for the list in bytes, but doesn't take into account additional memory allocated by type
================
*/
template< typename type, class AllocPolicy >
inline size_t List< type, AllocPolicy >::Allocated( void ) const {
	return allocedSize * sizeof( type );
}

/*
================
List< type, AllocPolicy >::size

return total allocedSize of list in bytes, but doesn't take into account additional memory allocated by type
================
*/
template< typename type, class AllocPolicy >
inline size_t idList< type, AllocPolicy >::Size( void ) const {
	return /*sizeof( List< type, heap, AllocPolicy > ) + */Allocated();
}

/*
================
List< type, AllocPolicy >::MemoryUsed
================
*/
template< typename type, class AllocPolicy >
inline size_t idList< type, AllocPolicy >::MemoryUsed( void ) const {
	return numElements * sizeof( *list );
}
#endif
/*
================
List< type, AllocPolicy >::size

Returns the number of elements currently contained in the list.
Note that this is NOT an indication of the memory allocated.
================
*/
template< typename type, class AllocPolicy >
inline size_t List< type, AllocPolicy >::size( void ) const {
	return numElements;
}

/*
================
List< type, AllocPolicy >::resize

Resize to the exact allocedSize specified irregardless of granularity
================
*/
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::resize( size_t newnum, bool resize ) {
	assert( newnum >= 0 );
	if ( resize || newnum > allocedSize ) {
		setSize( newnum );
	}
	numElements = newnum;
}

/*
================
List< type, AllocPolicy >::setGranularity

Sets the base allocedSize of the array and resizes the array to match.
================
*/
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::setGranularity( size_t newgranularity ) {
	size_t newsize;

	assert( newgranularity > 0 );
	granularity = newgranularity;

	if ( list ) {
		// resize it to the closest level of granularity
		newsize = numElements + granularity - 1;
		newsize -= newsize % granularity;
		if ( newsize != allocedSize ) {
			setSize( newsize );
		}
	}
}

/*
================
List< type, AllocPolicy >::getGranularity

Get the current granularity.
================
*/
template< typename type, class AllocPolicy >
inline size_t List< type, AllocPolicy >::getGranularity( void ) const {
	return granularity;
}

/*
================
List< type, AllocPolicy >::setSize

Allocates memory for the amount of elements requested while keeping the contents intact.
Contents are copied using their = operator so that data is correctly instantiated.
================
*/
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::setSize( size_t newsize ) {
	assert( newsize >= 0 );

	// free up the list if no data is being reserved
	if ( newsize <= 0 ) {
		clear();
		return;
	}

	if ( newsize == allocedSize ) {
		// not changing the allocedSize, so just exit
		return;
	}

	type* temp	= list;
	int oldAllocedSize = allocedSize;
	allocedSize		= newsize;
	if ( allocedSize < numElements ) {
		numElements = allocedSize;
	}

	list = AllocPolicy::alloc( allocedSize );

	if ( temp != NULL ) {
		// copy the old list into our new one
		// can't use memcopy, we're potentially copying classes
		for( size_t i = 0; i < numElements; i++ ) {
			list[ i ] = temp[ i ];
		}

		// delete the old list if it exists	
		AllocPolicy::free( temp, oldAllocedSize );
	}
}

#if 0
/*
================
List< type, AllocPolicy >::Resize

Allocates memory for the amount of elements requested while keeping the contents intact.
Contents are copied using their = operator so that data is correctly instantiated.
================
*/
template< typename type, class AllocPolicy >
inline void idList< type, AllocPolicy >::Resize( int newsize, int newgranularity ) {
	assert( newsize >= 0 );

	assert( newgranularity > 0 );
	granularity = newgranularity;

	// free up the list if no data is being reserved
	if ( newsize <= 0 ) {
		Clear();
		return;
	}

	if ( newsize == allocedSize ) {
		// not changing the allocedSize, so just exit
		return;
	}

	type* temp	= list;
	int oldAllocedSize = allocedSize;
	allocedSize		= newsize;

	if ( allocedSize < numElements ) {
		numElements = allocedSize;
	}

	// copy the old list into our new one
	list = AllocPolicy::Alloc( allocedSize );

	if ( temp ) {
		// copy the old list into our new one
		// can't use memcopy, we're potentially copying classes
		for( int i = 0; i < numElements; i++ ) {
			list[ i ] = temp[ i ];
		}
		
		// delete the old list if it exists
		AllocPolicy::Free( temp, oldAllocedSize );
	}
}

/*
================
List< type, AllocPolicy >::AssureSize

Makes sure the list has at least the given number of elements.
================
*/
template< typename type, class AllocPolicy >
inline void idList< type, AllocPolicy >::AssureSize( int newSize ) {
	int newNum = newSize;

	if ( newSize > allocedSize ) {
		newSize += granularity - 1;
		newSize -= newSize % granularity;
		Resize( newSize );
	}

	numElements = newNum;
}

/*
================
List< type, AllocPolicy >::AssureSize

Makes sure the list has at least the given number of elements and initialize any elements not yet initialized.
================
*/
template< typename type, class AllocPolicy >
inline void idList< type, AllocPolicy >::AssureSize( int newSize, const type &initValue ) {
	int newNum = newSize;

	if ( newSize > allocedSize ) {
		newSize += granularity - 1;
		newSize -= newSize % granularity;
		numElements = allocedSize;
		Resize( newSize );

		for ( int i = numElements; i < newSize; i++ ) {
			list[i] = initValue;
		}
	}
	numElements = newNum;
}
#endif
/*
================
List< class type >::PreAllocate

Makes sure the list has at least the given number of elements
allocated but don't actually change the number of items.
================
*/
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::preAllocate( size_t newSize ) {
	if ( newSize > allocedSize ) {
		newSize += granularity - 1;
		newSize -= newSize % granularity;
		setSize( newSize );
	}
}

/*
================
List< type, AllocPolicy >::operator=

Copies the contents and allocedSize attributes of another list.
================
*/
template< typename type, class AllocPolicy >
inline List< type, AllocPolicy > &List< type, AllocPolicy >::operator=( const List &other ) {
	if( &other == this ) {
		return *this;
	}

	clear();

	numElements	= other.numElements;
	allocedSize	= other.allocedSize;
	granularity	= other.granularity;

	if ( allocedSize ) {
		list = AllocPolicy::alloc( allocedSize );

		// copy the old list into our new one
		// can't use memcopy, we're potentially copying classes
		for( size_t i = 0; i < numElements; i++ ) {
			list[ i ] = other.list[ i ];
		}
	}

	return *this;
}

/*
================
List< type, AllocPolicy >::operator[] const

Access operator.  Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template< typename type, class AllocPolicy >
inline const type &List< type, AllocPolicy >::operator[]( int index ) const {
	assert( index >= 0 );
	assert( index < (int)numElements );

	return list[ index ];
}

/*
================
List< type, AllocPolicy >::operator[]
	
Access operator.	 Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template< typename type, class AllocPolicy >
inline type &List< type, AllocPolicy >::operator[]( int index ) {
	assert( index >= 0 );
	assert( index < (int)numElements );

	return list[ index ];
}

/*
================
List< type, AllocPolicy >::append

Returns a reference to a new data element at the end of the list.
================
*/
template< typename type, class AllocPolicy >
inline type &List< type, AllocPolicy >::append( void ) {
	if ( !list ) {
		setSize( granularity );
	}

	if ( numElements == allocedSize ) {
		setSize( allocedSize + granularity );
	}

	return list[ numElements++ ];
}

/*
================
List< type, AllocPolicy >::append

Increases the allocedSize of the list by one element and copies the supplied data into it.

Returns the index of the new element.
================
*/
template< typename type, class AllocPolicy >
inline int List< type, AllocPolicy >::append( type const & obj ) {

	// appending one of the list items does not work because the list may be reallocated
	assert( &obj < list || &obj >= list + numElements );

	if ( !list ) {
		setSize( granularity );
	}

	if ( numElements == allocedSize ) {
		int newsize;

		newsize = allocedSize + granularity;
		setSize( newsize - newsize % granularity );
	}

	list[ numElements ] = obj;
	numElements++;

	return numElements - 1;
}

#if 0

/*
================
List< type, AllocPolicy >::Insert

Increases the allocedSize of the list by at least one element if necessary 
and inserts the supplied data into it.

Returns the index of the new element.
================
*/
template< typename type, class AllocPolicy >
inline int idList< type, AllocPolicy >::Insert( type const & obj, int index ) {

	// inserting one of the list items does not work because the list may be reallocated
	assert( &obj < list || &obj >= list + numElements );

	if ( !list ) {
		Resize( granularity );
	}

	if ( numElements == allocedSize ) {
		int newsize;

		newsize = allocedSize + granularity;
		Resize( newsize - newsize % granularity );
	}

	if ( index < 0 ) {
		index = 0;
	} else if ( index > numElements ) {
		index = numElements;
	}
	for ( int i = numElements; i > index; --i ) {
		list[i] = list[i-1];
	}
	numElements++;
	list[index] = obj;
	return index;
}
#endif
/*
================
List< type, AllocPolicy >::Append

adds the other list to this one
correctly handles this->Append( *this );

Returns the allocedSize of the new combined list
================
*/
template< typename type, class AllocPolicy >
inline int List< type, AllocPolicy >::append( const List &other ) {

	// appending the list itself does not work because the list may be reallocated
	assert( &other != this );

	size_t n = other.size();
	if ( !list ) {
		resize( granularity + n );
	} else {
		resize( numElements + n );
	}	
	
	for ( size_t i = 0; i < n; i++ ) {
		append( other[i] );
	}

	return size();
}

/*
================
List< type, AllocPolicy >::addUnique

Adds the data to the list if it doesn't already exist.  Returns the index of the data in the list.
================
*/
template< typename type, class AllocPolicy >
inline int List< type, AllocPolicy >::addUnique( type const & obj ) {

	// inserting one of the list items does not work because the list may be reallocated
	assert( &obj < list || &obj >= list + numElements );

	int index = findIndex( obj );
	if ( index < 0 ) {
		index = append( obj );
	}

	return index;
}

///*
//================
//List< type, AllocPolicy >::FindIndexBinary
//Assumes the list is sorted and does a binary search for the given key
//================
//*/
//template< typename type, class AllocPolicy >
//inline int List< type, AllocPolicy >::FindIndexBinary ( const type & key, cmp_t *compare ) const {
//
//	typedef int cmp_c(const void *, const void *);
//	cmp_c *vCompare = (cmp_c *)compare;
//
//	type* found = (type*) bsearch( ( void * )( &key ), ( void * )list, ( size_t )numElements, sizeof( type ), vCompare );
//	if (found) {
//		return IndexOf(found);
//	}
//	return -1;
//}

/*
================
List< type, AllocPolicy >::FindIndex

Searches for the specified data in the list and returns it's index.  Returns -1 if the data is not found.
================
*/
template< typename type, class AllocPolicy >
//template< typename T >
inline int List< type, AllocPolicy >::findIndex( ConstType& obj ) const {
	for( size_t i = 0; i < numElements; i++ ) {
		if ( list[ i ] == obj ) {
			return (int)i;
		}
	}

	// Not found
	return -1;
}
#if 0
/*
================
List< type, AllocPolicy >::Find

Searches for the specified data in the list and returns its address. Returns NULL if the data is not found.
================
*/
template< typename type, class AllocPolicy >
inline type *idList< type, AllocPolicy >::FindElement( type const & obj ) const {
	int i;

	i = FindIndex( obj );
	if ( i >= 0 ) {
		return &list[ i ];
	}

	return NULL;
}

/*
================
List< type, AllocPolicy >::FindNull

Searches for a NULL pointer in the list.  Returns -1 if NULL is not found.

NOTE: This function can only be called on lists containing pointers. Calling it
on non-pointer lists will cause a compiler error.
================
*/
template< typename type, class AllocPolicy >
inline int idList< type, AllocPolicy >::FindNull( void ) const {
	int i;

	for( i = 0; i < numElements; i++ ) {
		if ( list[ i ] == NULL ) {
			return i;
		}
	}

	// Not found
	return -1;
}

/*
================
List< type, AllocPolicy >::IndexOf

Takes a pointer to an element in the list and returns the index of the element.
This is NOT a guarantee that the object is really in the list. 
Function will assert in debug builds if pointer is outside the bounds of the list,
but remains silent in release builds.
================
*/
template< typename type, class AllocPolicy >
inline int idList< type, AllocPolicy >::IndexOf( type const *objptr ) const {
	int index;

	index = objptr - list;

	assert( index >= 0 );
	assert( index < numElements );

	return index;
}

/*
================
List< type, AllocPolicy >::removeIndex

Removes the element at the specified index and moves all data following the element down to fill in the gap.
The number of elements in the list is reduced by one.  Returns false if the index is outside the bounds of the list.
Note that the element is not destroyed, so any memory used by it may not be freed until the destruction of the list.
================
*/
template< typename type, class AllocPolicy >
inline bool List< type, AllocPolicy >::removeIndex( size_t index ) {
	assert( list != NULL );
	assert( index >= 0 );
	assert( index < numElements );

	if ( ( index < 0 ) || ( index >= numElements ) ) {
		return false;
	}

	numElements--;

	// fill in the gap
	const int elementCount = numElements - index;

	memmove_s( &list[ index ], Allocated(), &list[ index + 1 ], elementCount * sizeof( type ) );

	return true;
}
#endif
/*
===============
List< type, AllocPolicy >::removeIndexFast

Removes the element at the specified index and moves the last element into
it's spot, rather than moving the whole array down by one.  Of course, this 
doesn't maintain the order of elements!
The number of elements in the list is reduced by one.  Returns false if the 
index is outside the bounds of the list. Note that the element is not destroyed, 
so any memory used by it may not be freed until the destruction of the list.
===============
*/
template< typename type, class AllocPolicy >
inline bool List< type, AllocPolicy >::removeIndexFast( size_t index ) {
	assert( list != NULL );
	assert( index >= 0 );
	assert( index < numElements );

	if ( ( index < 0 ) || ( index >= numElements ) ) {
		return false;
	}

	numElements--;	

	// nothing to do
	if( index == numElements )  {
		return true;
	}

	list[ index ] = list[ numElements ];	

	return true;
}
#if 0
/*
================
List< type, AllocPolicy >::Remove

Removes the element if it is found within the list and moves all data following the element down to fill in the gap.
The number of elements in the list is reduced by one.  Returns false if the data is not found in the list.  Note that
the element is not destroyed, so any memory used by it may not be freed until the destruction of the list.
================
*/
template< typename type, class AllocPolicy >
inline bool idList< type, AllocPolicy >::Remove( type const & obj ) {
	int index;

	index = FindIndex( obj );
	if ( index >= 0 ) {
		return RemoveIndex( index );
	}

	return false;
}
#endif
/*
================
List< type, AllocPolicy >::removeFast

Removes the element if it is found within the list and moves the last element into the gap.
The number of elements in the list is reduced by one.  Returns false if the data is not found in the list.  Note that
the element is not destroyed, so any memory used by it may not be freed until the destruction of the list.
================
*/
template< typename type, class AllocPolicy >
inline bool List< type, AllocPolicy >::removeFast( ConstType& obj ) {
	int index;

	index = findIndex( obj );
	if ( index >= 0 ) {
		return removeIndexFast( index );
	}

	return false;
}

/*
================
List< type, AllocPolicy >::sort

Performs a qsort on the list using the supplied comparison function.  Note that the data is merely moved around the
list, so any pointers to data within the list may no longer be valid.
FIXME: These don't obey the element destruction policy
================
*/
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::sort( cmp_t *compare ) {
	if ( !list ) {
		return;
	}
	typedef int cmp_c(const void *, const void *);

	cmp_c *vCompare = (cmp_c *)compare;
	qsort( ( void * )list, numElements, sizeof( type ), vCompare );
}

/*
================
List< type, AllocPolicy >::swap

Swaps the contents of two lists
================
*/
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::swap( List< type, AllocPolicy > &other ) {
	std::swap( numElements, other.numElements );
	std::swap( allocedSize, other.allocedSize );
	std::swap( granularity, other.granularity );
	std::swap( list, other.list );
}

/*
================
List< type, AllocPolicy >::begin

Returns the first element of the list
================
*/
template< typename type, class AllocPolicy >
inline typename List< type, AllocPolicy >::Iterator List< type, AllocPolicy >::begin( void ) {
	if( numElements == 0 ) {
		return end();
	}
	return list;
}

/*
================
List< type, AllocPolicy >::end

Returns one past the end of the list
================
*/
template< typename type, class AllocPolicy >
inline typename List< type, AllocPolicy >::Iterator List< type, AllocPolicy >::end( void ) {
	return list + numElements;
}

/*
================
List< type, AllocPolicy >::begin

Returns the first element of the list
================
*/
template< typename type, class AllocPolicy >
inline typename List< type, AllocPolicy >::ConstIterator List< type, AllocPolicy >::begin( void ) const {
	if( numElements == 0 ) {
		return end();
	}
	return list;
}

/*
================
List< type, AllocPolicy >::End

Returns one past the end of the list
================
*/
template< typename type, class AllocPolicy >
inline typename List< type, AllocPolicy >::ConstIterator List< type, AllocPolicy >::end( void ) const {
	return list + numElements;
}
#if 0
/*
================
List< type, AllocPolicy >::Back

returns the last element
================
*/
template< typename type, class AllocPolicy >
inline type& idList< type, AllocPolicy >::Back() {
	assert( numElements != 0 );
	return list[ numElements - 1 ];
}

/*
================
List< type, AllocPolicy >::Back

returns the last element
================
*/
template< typename type, class AllocPolicy >
inline const type& idList< type, AllocPolicy >::Back() const {
	assert( numElements != 0 );
	return list[ numElements - 1 ];
}

/*
================
List< type, AllocPolicy >::Front

returns the first element
================
*/
template< typename type, class AllocPolicy >
inline type& idList< type, AllocPolicy >::Front() {
	assert( numElements != 0 );
	return list[ 0 ];
}

/*
================
List< type, AllocPolicy >::Front

returns the first element
================
*/
template< typename type, class AllocPolicy >
inline const type& idList< type, AllocPolicy >::Front() const {
	assert( numElements != 0 );
	return list[ 0 ];
}

/*
================
List< type, AllocPolicy >::Remove
================
*/
template< typename type, class AllocPolicy >
inline bool idList< type, AllocPolicy >::Remove( Iterator iter ) {
	int index = Math::PtrDifftoi( iter - list );
	return RemoveIndex( index );
}
#endif
/*
================
List< type, AllocPolicy >::empty
================
*/
template< typename type, class AllocPolicy >
inline bool List< type, AllocPolicy >::empty() const {
	return numElements == 0 || list == NULL;
}
#if 0
/*
============
List< type, AllocPolicy >::Fill
============
*/
template< typename type, class AllocPolicy >
inline void idList< type, AllocPolicy >::Fill( int numElements, const type & obj ) {

	// inserting one of the list items does not work because the list may be reallocated
	assert( &obj < list || &obj >= list + numElements );

	AssureSize( numElements );
	for( int i = 0; i < numElements; i++ ) {
		list[ i ] = obj;
	}
}
#endif
