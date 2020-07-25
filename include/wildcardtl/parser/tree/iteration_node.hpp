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

    basic_iteration_node(node *next_) :
        node(true),
        _next(next_)
    {
        _next->append_firstpos(node::_firstpos);
        _next->append_lastpos(node::_lastpos);

        for (node *node_ : node::_lastpos)
        {
            node_->append_followpos(node::_firstpos);
        }
    }

    virtual ~basic_iteration_node() override
    {
    }

    virtual node_type what_type() const override
    {
        return node::node_type::ITERATION;
    }

    virtual bool traverse(const_node_stack &node_stack_,
        bool_stack &perform_op_stack_) const override
    {
        perform_op_stack_.push(true);
        node_stack_.push(_next);
        return true;
    }

private:
    // Not owner of this pointer...
    node *_next;

    virtual void copy_node(node_ptr_vector &node_ptr_vector_,
        node_stack &new_node_stack_, bool_stack &perform_op_stack_,
        bool &down_) const override
    {
        if (perform_op_stack_.top())
        {
            node *ptr_ = new_node_stack_.top();

            node_ptr_vector_.emplace_back(std::make_unique
                <basic_iteration_node>(ptr_));
            new_node_stack_.top() = node_ptr_vector_.back().get();
        }
        else
        {
            down_ = true;
        }

        perform_op_stack_.pop();
    }

    // No copy construction.
    basic_iteration_node(const basic_iteration_node &) = delete;
    // No assignment.
    const basic_iteration_node &operator =
        (const basic_iteration_node &) = delete;
};
}
}

#endif
