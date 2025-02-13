// string_token.hpp
// Copyright (c) 2017 Ben Hanson (http://www.benhanson.net/)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef WILDCARDTL_STRING_TOKEN_HPP
#define WILDCARDTL_STRING_TOKEN_HPP

#include "consts.hpp"

#include <algorithm>
#include <string>

namespace wildcardtl
{
template<typename char_type>
struct basic_string_token
{
    using string = std::basic_string<char_type>;

    bool _negated;
    string _charset;

    basic_string_token() :
        _negated(false)
    {
    }

    void remove_duplicates()
    {
        const char_type* start_ = _charset.c_str();
        const char_type* end_ = start_ + _charset.size();

        // Optimisation for very large charsets:
        // sorting via pointers is much quicker than
        // via iterators...
        std::sort(const_cast<char_type*>(start_),
            const_cast<char_type*>(end_));
        _charset.erase(std::unique(_charset.begin(), _charset.end()),
            _charset.end());
    }

    void normalise()
    {
        const std::size_t max_chars_ = sizeof(char_type) == 1 ?
            num_chars : num_wchar_ts;

        if (_charset.length() == max_chars_)
        {
            _negated = !_negated;
            _charset.clear();
        }
        else if (_charset.length() > max_chars_ / 2)
        {
            negate();
        }
    }

    void negate()
    {
        const std::size_t max_chars_ = sizeof(char_type) == 1 ?
            num_chars : num_wchar_ts;
        char_type curr_char_ = sizeof(char_type) == 1 ? -128 : 0;
        string temp_;
        const char_type* curr_ = _charset.c_str();
        const char_type* chars_end_ = curr_ + _charset.size();

        _negated = !_negated;
        temp_.resize(max_chars_ - _charset.size());

        char_type* ptr_ = &temp_.front();
        std::size_t i_ = 0;

        while (curr_ < chars_end_)
        {
            while(*curr_ > curr_char_)
            {
                *ptr_ = curr_char_;
                ++ptr_;
                ++curr_char_;
                ++i_;
            }

            ++curr_char_;
            ++curr_;
            ++i_;
        }

        for (; i_ < max_chars_; ++i_)
        {
            *ptr_ = curr_char_;
            ++ptr_;
            ++curr_char_;
        }

        _charset = temp_;
    }

    friend bool operator <(const basic_string_token& lhs_,
        const basic_string_token& rhs_)
    {
        return lhs_._negated < rhs_._negated ||
            (lhs_._negated == rhs_._negated && lhs_._charset < rhs_._charset);
    }

    bool empty() const
    {
        return _charset.empty() && !_negated;
    }

    bool any() const
    {
        return _charset.empty() && _negated;
    }

    void clear()
    {
        _negated = false;
        _charset.clear();
    }

    void intersect(basic_string_token& rhs_, basic_string_token& overlap_)
    {
        if ((any() && rhs_.any()) || (_negated == rhs_._negated &&
            !any() && !rhs_.any()))
        {
            intersect_same_types(rhs_, overlap_);
        }
        else
        {
            intersect_diff_types(rhs_, overlap_);
        }
    }

private:
    void intersect_same_types(basic_string_token& rhs_,
        basic_string_token& overlap_)
    {
        if (any())
        {
            clear();
            overlap_._negated = true;
            rhs_.clear();
        }
        else
        {
            auto iter_ = _charset.begin();
            auto end_ = _charset.end();
            auto rhs_iter_ = rhs_._charset.begin();
            auto rhs_end_ = rhs_._charset.end();

            overlap_._negated = _negated;

            while (iter_ != end_ && rhs_iter_ != rhs_end_)
            {
                if (*iter_ < *rhs_iter_)
                {
                    ++iter_;
                }
                else if (*iter_ > *rhs_iter_)
                {
                    ++rhs_iter_;
                }
                else
                {
                    overlap_._charset += *iter_;
                    iter_ = _charset.erase(iter_);
                    end_ = _charset.end();
                    rhs_iter_ = rhs_._charset.erase(rhs_iter_);
                    rhs_end_ = rhs_._charset.end();
                }
            }

            if (_negated)
            {
                // duplicates already merged, so safe to merge
                // using std lib.

                // src, dest
                merge(_charset, overlap_._charset);
                // duplicates already merged, so safe to merge
                // using std lib.

                // src, dest
                merge(rhs_._charset, overlap_._charset);
                _negated = false;
                rhs_._negated = false;
                std::swap(_charset, rhs_._charset);
                normalise();
                overlap_.normalise();
                rhs_.normalise();
            }
            else if (!overlap_._charset.empty())
            {
                normalise();
                overlap_.normalise();
                rhs_.normalise();
            }
        }
    }

    void intersect_diff_types(basic_string_token& rhs_,
        basic_string_token& overlap_)
    {
        if (any())
        {
            intersect_any(rhs_, overlap_);
        }
        else if (_negated)
        {
            intersect_negated(rhs_, overlap_);
        }
        else // _negated == false
        {
            intersect_charset(rhs_, overlap_);
        }
    }

    void intersect_any(basic_string_token& rhs_, basic_string_token& overlap_)
    {
        if (rhs_._negated)
        {
            rhs_.intersect_negated(*this, overlap_);
        }
        else // rhs._negated == false
        {
            rhs_.intersect_charset(*this, overlap_);
        }
    }

    void intersect_negated(basic_string_token& rhs_,
        basic_string_token& overlap_)
    {
        if (rhs_.any())
        {
            overlap_._negated = true;
            overlap_._charset = _charset;
            rhs_._negated = false;
            rhs_._charset = _charset;
            clear();
        }
        else // rhs._negated == false
        {
            rhs_.intersect_charset(*this, overlap_);
        }
    }

    void intersect_charset(basic_string_token& rhs_,
        basic_string_token& overlap_)
    {
        if (rhs_.any())
        {
            overlap_._charset = _charset;
            rhs_._negated = true;
            rhs_._charset = _charset;
            clear();
        }
        else // rhs_._negated == true
        {
            auto iter_ = _charset.begin();
            auto end_ = _charset.end();
            auto rhs_iter_ = rhs_._charset.begin();
            auto rhs_end_ = rhs_._charset.end();

            while (iter_ != end_ && rhs_iter_ != rhs_end_)
            {
                if (*iter_ < *rhs_iter_)
                {
                    overlap_._charset += *iter_;
                    rhs_iter_ = rhs_._charset.insert(rhs_iter_, *iter_);
                    ++rhs_iter_;
                    rhs_end_ = rhs_._charset.end();
                    iter_ = _charset.erase(iter_);
                    end_ = _charset.end();
                }
                else if (*iter_ > *rhs_iter_)
                {
                    ++rhs_iter_;
                }
                else
                {
                    ++iter_;
                    ++rhs_iter_;
                }
            }

            if (iter_ != end_)
            {
                // nothing bigger in rhs_ than iter_,
                // so safe to merge using std lib.
                string temp_(iter_, end_);

                // src, dest
                merge(temp_, overlap_._charset);
                _charset.erase(iter_, end_);
            }

            if (!overlap_._charset.empty())
            {
                merge(overlap_._charset, rhs_._charset);
                // possible duplicates, so check for any and erase.
                rhs_._charset.erase(std::unique(rhs_._charset.begin(),
                    rhs_._charset.end()), rhs_._charset.end());
                normalise();
                overlap_.normalise();
                rhs_.normalise();
            }
        }
    }

    void merge(string& src_, string& dest_)
    {
        string tmp_(src_.size() + dest_.size(), 0);

        std::merge(src_.begin(), src_.end(), dest_.begin(), dest_.end(),
            tmp_.begin());
        dest_ = tmp_;
    }
};
}

#endif
