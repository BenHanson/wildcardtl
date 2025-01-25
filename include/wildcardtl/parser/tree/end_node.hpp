// end_node.hpp
// Copyright (c) 2017 Ben Hanson (http://www.benhanson.net/)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef WILDCARDTL_END_NODE_HPP
#define WILDCARDTL_END_NODE_HPP

#include "node.hpp"

namespace wildcardtl
{
namespace detail
{
template<typename char_type>
class basic_end_node : public basic_node<char_type>
{
public:
    using node = basic_node<char_type>;
    using bool_stack = typename node::bool_stack;
    using const_node_stack = typename node::const_node_stack;
    using node_ptr_vector = typename node::node_ptr_vector;
    using node_stack = typename node::node_stack;
    using node_vector = typename node::node_vector;
    using node_type = typename node::node_type;

    basic_end_node() :
        node(false)
    {
        node::firstpos().push_back(this);
        node::lastpos().push_back(this);
    }

    ~basic_end_node() override
    {
    }

    const node_vector& followpos() const override
    {
        // _followpos is always empty..!
        return _followpos;
    }

    bool end_state() const override
    {
        return true;
    }

private:
    node_vector _followpos;

    // No copy construction.
    basic_end_node(const basic_end_node&) = delete;
    // No assignment.
    const basic_end_node& operator =(const basic_end_node&) = delete;
};
}
}

#endif
