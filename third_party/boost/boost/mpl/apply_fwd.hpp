
#if !defined(BOOST_PP_IS_ITERATING)

///// header body

#ifndef BOOST_MPL_APPLY_FWD_HPP_INCLUDED
#define BOOST_MPL_APPLY_FWD_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: apply_fwd.hpp 85961 2013-09-26 14:10:37Z skelly $
// $Date: 2013-09-26 17:10:37 +0300 (Чт, 26 сен 2013) $
// $Revision: 85961 $

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include <boost/mpl/aux_/na.hpp>
#endif

#include <boost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER apply_fwd.hpp
#   include <boost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <boost/mpl/limits/arity.hpp>
#   include <boost/mpl/aux_/preprocessor/params.hpp>
#   include <boost/mpl/aux_/preprocessor/default_params.hpp>
#   include <boost/mpl/aux_/config/ctps.hpp>

#   include <boost/preprocessor/comma_if.hpp>
#   include <boost/preprocessor/iterate.hpp>
#   include <boost/preprocessor/cat.hpp>

namespace boost { namespace mpl {

// local macro, #undef-ined at the end of the header
#   define AUX778076_APPLY_DEF_PARAMS(param, value) \
    BOOST_MPL_PP_DEFAULT_PARAMS( \
          BOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        , value \
        ) \
    /**/

#   define AUX778076_APPLY_N_COMMA_PARAMS(n, param) \
    BOOST_PP_COMMA_IF(n) \
    BOOST_MPL_PP_PARAMS(n, param) \
    /**/

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
// forward declaration
template<
      typename F, AUX778076_APPLY_DEF_PARAMS(typename T, na)
    >
struct apply;
#else
namespace aux {
template< BOOST_AUX_NTTP_DECL(int, arity_) > struct apply_chooser;
}
#endif

#define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, BOOST_MPL_LIMIT_METAFUNCTION_ARITY, <boost/mpl/apply_fwd.hpp>))
#include BOOST_PP_ITERATE()


#   undef AUX778076_APPLY_N_COMMA_PARAMS
#   undef AUX778076_APPLY_DEF_PARAMS

}}

#endif // BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // BOOST_MPL_APPLY_FWD_HPP_INCLUDED

///// iteration

#else
#define i_ BOOST_PP_FRAME_ITERATION(1)

template<
      typename F AUX778076_APPLY_N_COMMA_PARAMS(i_, typename T)
    >
struct BOOST_PP_CAT(apply,i_);

#undef i_
#endif // BOOST_PP_IS_ITERATING
