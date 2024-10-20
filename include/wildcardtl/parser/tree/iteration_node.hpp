// iteration_node.hpp
// Copyright (c) 2017 Ben Hanson (http://www.benhanson.net/)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef WILDCARDTL_ITERATION_NODE_HPP
#define WILDCARDTL_ITERATION_NODE_HPP

#include "node.hpp"

namespace wildcardtl
{
namespace detail
{
template<typename char_type>
class basic_iteration_node : public basic_node<char_type>
{
public:
    using node = basic_node<char_type>;
    using bool_stack = typename node::bool_stack;
    using const_node_stack = typename node::const_node_stack;
    using node_ptr_vector = typename node::node_ptr_vector;
    using node_stack = typename node::node_stack;
    using node_vector = typename node::node_vector;
    using node_type = typename node::node_type;

    explicit basic_iteration_node(node *next_) :
        node(true),
        _next(next_)
    {
        _next->append_firstpos(node::firstpos());
        _next->append_lastpos(node::lastpos());

        for (node *node_ : node::lastpos())
        {
            node_->append_followpos(node::firstpos());
        }
    }

    ~basic_iteration_node() override
    {
    }

private:
    // Not owner of this pointer...
    node *_next = nullptr;

    // No copy construction.
    basic_iteration_node(const basic_iteration_node &) = delete;
    // No assignment.
    const basic_iteration_node &operator =
        (const basic_iteration_node &) = delete;
};
}
}

#endif
