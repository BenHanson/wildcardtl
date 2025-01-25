// sequence_node.hpp
// Copyright (c) 2017 Ben Hanson (http://www.benhanson.net/)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef WILDCARDTL_SEQUENCE_NODE_HPP
#define WILDCARDTL_SEQUENCE_NODE_HPP

#include "node.hpp"

namespace wildcardtl
{
namespace detail
{
template<typename char_type>
class basic_sequence_node : public basic_node<char_type>
{
public:
    using node = basic_node<char_type>;
    using bool_stack = typename node::bool_stack;
    using const_node_stack = typename node::const_node_stack;
    using node_ptr_vector = typename node::node_ptr_vector;
    using node_stack = typename node::node_stack;
    using node_vector = typename node::node_vector;
    using node_type = typename node::node_type;

    basic_sequence_node(node* left_, node* right_) :
        node(left_->nullable() && right_->nullable()),
        _left(left_),
        _right(right_)
    {
        _left->append_firstpos(node::firstpos());

        if (_left->nullable())
        {
            _right->append_firstpos(node::firstpos());
        }

        if (_right->nullable())
        {
            _left->append_lastpos(node::lastpos());
        }

        _right->append_lastpos(node::lastpos());

        node_vector& lastpos_ = _left->lastpos();
        const node_vector& firstpos_ = _right->firstpos();

        for (node* node_ : lastpos_)
        {
            node_->append_followpos(firstpos_);
        }
    }

    ~basic_sequence_node() override
    {
    }

private:
    // Not owner of these pointers...
    node* _left;
    node* _right;

    // No copy construction.
    basic_sequence_node(const basic_sequence_node&) = delete;
    // No assignment.
    const basic_sequence_node& operator =(const basic_sequence_node&) = delete;
};
}
}

#endif
