
#ifndef BOOST_MPL_BOOL_HPP_INCLUDED
#define BOOST_MPL_BOOL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: bool.hpp 49239 2008-10-10 09:10:26Z agurtovoy $
// $Date: 2008-10-10 12:10:26 +0300 (Пт, 10 окт 2008) $
// $Revision: 49239 $

#include <boost/mpl/bool_fwd.hpp>
#include <boost/mpl/integral_c_tag.hpp>
#include <boost/mpl/aux_/config/static_constant.hpp>

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

template< bool C_ > struct bool_
{
    BOOST_STATIC_CONSTANT(bool, value = C_);
    typedef integral_c_tag tag;
    typedef bool_ type;
    typedef bool value_type;
    operator bool() const { return this->value; }
};

#if !defined(BOOST_NO_INCLASS_MEMBER_INITIALIZATION)
template< bool C_ >
bool const bool_<C_>::value;
#endif

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

#endif // BOOST_MPL_BOOL_HPP_INCLUDED
