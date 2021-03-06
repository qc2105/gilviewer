/*
    Copyright 2007-2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_IO_ERROR_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_IO_ERROR_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Christian Henning \n
///
/// \date   2010 \n
///
////////////////////////////////////////////////////////////////////////////////////////

namespace boost { namespace gil { namespace detail {

inline 
void io_error( const std::string& descr )
{
   throw std::ios_base::failure( descr );
}

inline 
void io_error_if( bool expr, const std::string& descr )
{
   if( expr ) 
      io_error( descr );
}

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_IO_ERROR_HPP_INCLUDED