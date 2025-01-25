// leaf_node.hpp
// Copyright (c) 2017 Ben Hanson (http://www.benhanson.net/)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef WILDCARDTL_LEAF_NODE_HPP
#define WILDCARDTL_LEAF_NODE_HPP

#include "node.hpp"

namespace wildcardtl
{
namespace detail
{
template<typename char_type>
class basic_leaf_node : public basic_node<char_type>
{
public:
    using node = basic_node<char_type>;
    using bool_stack = typename node::bool_stack;
    using const_node_stack = typename node::const_node_stack;
    using node_ptr_vector = typename node::node_ptr_vector;
    using node_stack = typename node::node_stack;
    using node_vector = typename node::node_vector;
    using string_token = typename node::string_token;
    using node_type = typename node::node_type;

    explicit basic_leaf_node(const string_token& token_) :
        node(false),
        _token(token_)
    {
        if (!node::nullable())
        {
            node::firstpos().push_back(this);
            node::lastpos().push_back(this);
        }
    }

    ~basic_leaf_node() override
    {
    }

    void append_followpos(const node_vector& followpos_) override
    {
        _followpos.insert(_followpos.end(),
            followpos_.begin(), followpos_.end());
    }

    const string_token& token() const override
    {
        return _token;
    }

    const node_vector& followpos() const override
    {
        return _followpos;
    }

    node_vector& followpos() override
    {
        return _followpos;
    }

private:
    string_token _token;
    node_vector _followpos;

    // No copy construction.
    basic_leaf_node(const basic_leaf_node&) = delete;
    // No assignment.
    const basic_leaf_node& operator =(const basic_leaf_node&) = delete;
};
}
}

#endif
