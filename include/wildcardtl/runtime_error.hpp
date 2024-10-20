// runtime_error.hpp
// Copyright (c) 2017 Ben Hanson (http://www.benhanson.net/)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef WILDCARDTL_RUNTIME_ERROR_HPP
#define WILDCARDTL_RUNTIME_ERROR_HPP

#include <stdexcept>

namespace wildcardtl
{
class runtime_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};
}

#endif
