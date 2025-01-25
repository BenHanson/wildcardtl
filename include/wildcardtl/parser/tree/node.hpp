// node.hpp
// Copyright (c) 2017 Ben Hanson (http://www.benhanson.net/)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef WILDCARDTL_NODE_HPP
#define WILDCARDTL_NODE_HPP

#include "../../runtime_error.hpp"
#include "../../string_token.hpp"

#include <memory>
#include <stack>
#include <vector>

namespace wildcardtl
{
namespace detail
{
template<typename char_type>
class basic_node
{
public:
    enum class node_type {LEAF, SEQUENCE, ITERATION, END};

    using bool_stack = std::stack<bool>;
    // stack and vector not owner of node pointers
    using node_stack = std::stack<basic_node*>;
    using const_node_stack = std::stack<const basic_node*>;
    using node_vector = std::vector<basic_node*>;
    using node_ptr_vector = std::vector<std::unique_ptr<basic_node>>;
    using string_token = basic_string_token<char_type>;

    basic_node() :
        _nullable(false)
    {
    }

    explicit basic_node(const bool nullable_) :
        _nullable(nullable_)
    {
    }

    virtual ~basic_node()
    {
    }

    bool nullable() const
    {
        return _nullable;
    }

    void append_firstpos(node_vector& firstpos_) const
    {
        firstpos_.insert(firstpos_.end(),
            _firstpos.begin(), _firstpos.end());
    }

    void append_lastpos(node_vector& lastpos_) const
    {
        lastpos_.insert(lastpos_.end(),
            _lastpos.begin(), _lastpos.end());
    }

    virtual void append_followpos(const node_vector& /*followpos_*/)
    {
        throw runtime_error("Internal error node::append_followpos()");
    }

    node_vector& firstpos()
    {
        return _firstpos;
    }

    const node_vector& firstpos() const
    {
        return _firstpos;
    }

    // _lastpos modified externally, so not const &
    node_vector& lastpos()
    {
        return _lastpos;
    }

    virtual bool end_state() const
    {
        return false;
    }

    virtual const string_token& token() const
    {
        throw runtime_error("Internal error node::token()");
    }

    virtual const node_vector& followpos() const
    {
        throw runtime_error("Internal error node::followpos()");
    }

    virtual node_vector& followpos()
    {
        throw runtime_error("Internal error node::followpos()");
    }

private:
    const bool _nullable;
    node_vector _firstpos;
    node_vector _lastpos;

    // No copy construction.
    basic_node(const basic_node&) = delete;
    // No assignment.
    const basic_node&operator =(const basic_node&) = delete;
};
}
}

#endif
