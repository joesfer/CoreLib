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
// List< type, AllocPolicy >::List( int )
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline List< type, AllocPolicy >::List( size_t newgranularity )
	:	allocedSize( 0 ),
		granularity( newgranularity ),
		list( NULL ) {
	assert( granularity > 0 );
	clear();
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::List( const List &other )
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline List< type, AllocPolicy >::List( const List &other ) 
	:	allocedSize( 0 ),
		list( NULL ) {
	*this = other;
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::~List
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline List< type, AllocPolicy >::~List( void ) {
	clear();
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::clear
//
// Frees up the memory allocated by the list.  Assumes that type 
// automatically handles freeing up memory.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::clear() {
	AllocPolicy::free( list, (int)allocedSize );
	list		= NULL;
	numElements	= 0;
	allocedSize	= 0;	
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::size
//
// Returns the number of elements currently contained in the list.
// Note that this is NOT an indication of the memory allocated.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline size_t List< type, AllocPolicy >::size( void ) const {
	return numElements;
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::resize
//
// Resize to the exact allocedSize specified irregardless of granularity
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::resize( size_t newnum, bool resize ) {
	assert( newnum >= 0 );
	if ( resize || newnum > allocedSize ) {
		setSize( newnum );
	}
	numElements = newnum;
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::setGranularity
//
// Sets the base allocedSize of the array and resizes the array to match.
//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::getGranularity
// 
// Get the current granularity.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline size_t List< type, AllocPolicy >::getGranularity( void ) const {
	return granularity;
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::setSize
// 
// Allocates memory for the amount of elements requested while keeping the 
// contents intact. Contents are copied using their = operator so that data 
// is correctly instantiated.
//////////////////////////////////////////////////////////////////////////
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
	size_t oldAllocedSize = allocedSize;
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

//////////////////////////////////////////////////////////////////////////
// List< class type >::PreAllocate
//
// Makes sure the list has at least the given number of elements
// allocated but don't actually change the number of items.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::preAllocate( size_t newSize ) {
	if ( newSize > allocedSize ) {
		newSize += granularity - 1;
		newSize -= newSize % granularity;
		setSize( newSize );
	}
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::operator=
//
// Copies the contents and allocedSize attributes of another list.
//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::operator[] const
//
// Access operator.  Index must be within range or an assert will be issued 
// in debug builds. Release builds do no range checking.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline const type &List< type, AllocPolicy >::operator[]( int index ) const {
	assert( index >= 0 );
	assert( index < (int)numElements );

	return list[ index ];
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::operator[]
//	
// Access operator.	Index must be within range or an assert will be issued 
// in debug builds. Release builds do no range checking.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline type &List< type, AllocPolicy >::operator[]( int index ) {
	assert( index >= 0 );
	assert( index < (int)numElements );

	return list[ index ];
}


//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::operator[] const
//
// Access operator.  Index must be within range or an assert will be issued 
// in debug builds. Release builds do no range checking.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline const type &List< type, AllocPolicy >::operator[]( size_t index ) const {
	assert( index < numElements );
	return list[ index ];
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::operator[]
//	
// Access operator.	Index must be within range or an assert will be issued 
// in debug builds. Release builds do no range checking.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline type &List< type, AllocPolicy >::operator[]( size_t index ) {
	assert( index < numElements );
	return list[ index ];
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::operator[] const
//
// Access operator.  Index must be within range or an assert will be issued 
// in debug builds. Release builds do no range checking.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline const type &List< type, AllocPolicy >::operator[]( unsigned int index ) const {
	assert( index < numElements );
	return list[ index ];
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::operator[]
//	
// Access operator.	Index must be within range or an assert will be issued 
// in debug builds. Release builds do no range checking.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline type &List< type, AllocPolicy >::operator[]( unsigned int index ) {
	assert( index < numElements );
	return list[ index ];
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::append
//
// Returns a reference to a new data element at the end of the list.
//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::append
//
// Increases the allocedSize of the list by one element and copies the 
// supplied data into it.
//
// Returns the index of the new element.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline int List< type, AllocPolicy >::append( type const & obj ) {

	// appending one of the list items does not work because the list may be reallocated
	assert( &obj < list || &obj >= list + numElements );

	if ( !list ) {
		setSize( granularity );
	}

	if ( numElements == allocedSize ) {
		size_t newsize;
		newsize = allocedSize + granularity;
		setSize( newsize - newsize % granularity );
	}

	list[ numElements ] = obj;
	numElements++;

	return (int)(numElements - 1);
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::Append
//
// adds the other list to this one
// correctly handles this->append( *this );
// 
// Returns the allocedSize of the new combined list
//////////////////////////////////////////////////////////////////////////
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

	return (int)size();
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::addUnique
// 
// Adds the data to the list if it doesn't already exist.  Returns the 
// index of the data in the list.
//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::FindIndex
//
// Searches for the specified data in the list and returns it's index.  
// Returns -1 if the data is not found.
//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::removeIndexFast
//
// Removes the element at the specified index and moves the last element into
// it's spot, rather than moving the whole array down by one.  Of course, this 
// doesn't maintain the order of elements!
// The number of elements in the list is reduced by one.  Returns false if the 
// index is outside the bounds of the list. Note that the element is not destroyed, 
// so any memory used by it may not be freed until the destruction of the list.
//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::removeFast
//
// Removes the element if it is found within the list and moves the last 
// element into the gap. The number of elements in the list is reduced by 
// one.  Returns false if the data is not found in the list.  Note that
// the element is not destroyed, so any memory used by it may not be freed 
// until the destruction of the list.
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline bool List< type, AllocPolicy >::removeFast( ConstType& obj ) {
	int index;

	index = findIndex( obj );
	if ( index >= 0 ) {
		return removeIndexFast( index );
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::sort
//
// Performs a qsort on the list using the supplied comparison function.  
// Note that the data is merely moved around the list, so any pointers to 
// data within the list may no longer be valid.
// FIXME: These don't obey the element destruction policy
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::sort( cmp_t *compare ) {
	if ( !list ) {
		return;
	}
	typedef int cmp_c(const void *, const void *);

	cmp_c *vCompare = (cmp_c *)compare;
	qsort( ( void * )list, numElements, sizeof( type ), vCompare );
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::swap
//
// Swaps the contents of two lists
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline void List< type, AllocPolicy >::swap( List< type, AllocPolicy > &other ) {
	std::swap( numElements, other.numElements );
	std::swap( allocedSize, other.allocedSize );
	std::swap( granularity, other.granularity );
	std::swap( list, other.list );
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::begin
//
// Returns the first element of the list
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline typename List< type, AllocPolicy >::Iterator List< type, AllocPolicy >::begin( void ) {
	if( numElements == 0 ) {
		return end();
	}
	return list;
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::end
//
// Returns one past the end of the list
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline typename List< type, AllocPolicy >::Iterator List< type, AllocPolicy >::end( void ) {
	return list + numElements;
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::begin
//
// Returns the first element of the list
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline typename List< type, AllocPolicy >::ConstIterator List< type, AllocPolicy >::begin( void ) const {
	if( numElements == 0 ) {
		return end();
	}
	return list;
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::End
//
// Returns one past the end of the list
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline typename List< type, AllocPolicy >::ConstIterator List< type, AllocPolicy >::end( void ) const {
	return list + numElements;
}

//////////////////////////////////////////////////////////////////////////
// List< type, AllocPolicy >::empty
//////////////////////////////////////////////////////////////////////////
template< typename type, class AllocPolicy >
inline bool List< type, AllocPolicy >::empty() const {
	return numElements == 0 || list == NULL;
}