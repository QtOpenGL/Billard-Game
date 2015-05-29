/*
 Title        : cpp_sscanf
 Project      : string parser
 File         : cpp_sscanf.h
 Despcription : defines the cpp_sscanf::sscanf function
 License      : MIT License (MIT)
 Copyright (c) 2015 Julien HARBULOT
 */

#ifndef string_parser_cpp_sscanf_rec_h
#define string_parser_cpp_sscanf_rec_h

#include <utility>
#include <stdexcept>
#include <regex>
#include "tokenize.h"

//=====================================================
// cpp_sscanf::sscanf prototypes
//=====================================================


namespace cpp_sscanf {
    
    template <class strIterator, class... Out>
    inline strIterator
    sscanf(strIterator const& input_begin, strIterator const& input_end, std::vector<std::string> const& patterns, Out&... outs);
    
    template <class strIterator, class... Out>
    inline strIterator
    sscanf(strIterator const& input_begin, strIterator const& input_end, std::string const& pattern, char delim, Out&... outs);
    
    template <class... Out>
    inline std::string::const_iterator
    sscanf(std::string const& input, std::string const& pattern, char delim, Out&... outs);
}

//=====================================================
// exceptions
//=====================================================


namespace cpp_sscanf
{
    struct PatternNotFoundException : public std::runtime_error
    { PatternNotFoundException()    : std::runtime_error{"could not parse pattern"} {} };
    
    struct CouldNotParseArgumentException : public std::runtime_error
    { CouldNotParseArgumentException()    : std::runtime_error{"could not parse argument"} {} };
    
    struct TooManyOutArguments : public std::logic_error
    { TooManyOutArguments()    : std::logic_error{"too many output arguments for sscanf"} {} };
    
    struct NotEnoughOutArguments : public std::logic_error
    { NotEnoughOutArguments()    : std::logic_error{"not enough output arguments for sscanf"} {} };
    
    struct InputTooShort : public std::length_error
    { InputTooShort()    : std::length_error{"input is too short for pattern"} {} };
}

//=====================================================
// implementation details
// SHOULD BE READ FROM BOTTOM TO TOP
//=====================================================


namespace cpp_sscanf {
    namespace trait{
        using strIterator = std::string::const_iterator;
        
        template <class T>
        struct parser_trait;
        
        template <class T>
        inline strIterator parse(strIterator const& begin, strIterator const& end, T& t)
        {
            return parser_trait<T>::parse(begin, end, t);
        }
    }

    //=====================================================
    
    namespace sscanf_impl
    {
        template <class Iterator>
        Iterator
        pattern_match(Iterator input_cur_pos, Iterator const& input_end, std::string const& pattern)
        {
            if( std::distance(input_cur_pos, input_end) < pattern.size() ){
                throw InputTooShort{};
            }
            
            bool pattern_matches = std::equal(pattern.begin(), pattern.end(), input_cur_pos);
            
            if(not pattern_matches) {
                throw PatternNotFoundException{};
            }
            
            return input_cur_pos + pattern.size();
        }
        template <class Iterator, class Out>
        Iterator
        unit_match(Iterator cur_position, Iterator const& end, std::string const& pattern, Out& out_arg)
        {
            auto position_after_pattern = pattern_match(cur_position, end, pattern);
            auto position_after_parsing = trait::parse(position_after_pattern, end, out_arg);
            return position_after_parsing;
        }

        //=====================================================
        
        template <class strIterator, class vecIterator>
        inline strIterator
        sscanf(strIterator input_begin, strIterator input_end, vecIterator final_pattern, vecIterator pattern_end)
        {
            // end of recursion
            if(final_pattern != pattern_end){
                // if there is a final pattern
                return pattern_match(input_begin, input_end, *final_pattern);
            }else{
                return input_begin;
            }
        }
        
        template <class strIterator, class vecIterator, class Out1, class... Outs>
        inline strIterator
        sscanf(strIterator input_begin, strIterator input_end, vecIterator cur_pattern, vecIterator pattern_end, Out1& out1, Outs&... outs)
        {
            // recursion on Outs...
            auto next_pos = unit_match( input_begin, input_end, *cur_pattern, out1 );
            return sscanf(next_pos, input_end, cur_pattern+1, pattern_end, outs...);
        }
        
        //=====================================================
        
        void
        check_output_size(size_t pattern_size, size_t output_list_size)
        {
            if( pattern_size  < output_list_size ){
                throw TooManyOutArguments{};
            }
            if( pattern_size > 1 + output_list_size ) {
                throw NotEnoughOutArguments{};
            }
        }
    }
    
    //=====================================================
    
    template <class strIterator, class... Out>
    inline strIterator
    sscanf(strIterator const& input_begin, strIterator const& input_end, std::vector<std::string> const& patterns, Out&... outs)
    {
        sscanf_impl:: check_output_size( patterns.size(), sizeof...(outs) );
        return sscanf_impl:: sscanf(input_begin, input_end, patterns.begin(), patterns.end(), outs...);
    }
    
    template <class strIterator, class... Out>
    inline strIterator
    sscanf(strIterator const& input_begin, strIterator const& input_end, std::string const& pattern, char delim, Out&... outs)
    {
        auto tokens = tokenize(pattern, delim);
        return sscanf(input_begin, input_end, std::move(tokens), outs...);
    }
    
    template <class... Out>
    inline std::string::const_iterator
    sscanf(std::string const& input, std::string const& pattern, char delim, Out&... outs)
    {
        return sscanf(input.begin(), input.end(), pattern, delim, outs...);
    }
}

#endif