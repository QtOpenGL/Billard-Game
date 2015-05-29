/*
 Title        : parser trait
 Project      : string parser / cpp_sscanf
 File         : parser_trait.h
 Despcription : function called when an output argument should be parsed
 License      : MIT License (MIT)
 Copyright (c) 2015 Julien HARBULOT
 */

#ifndef string_parser_parser_trait_h
#define string_parser_parser_trait_h

#include <cstdio>
#include <cctype>
#include <functional>
#include "regex_trait.h"
#include "converter_trait.h"

namespace cpp_sscanf{
    namespace trait{
        template <class T>  struct regex_trait;
        template <class T>  struct converter_trait;
        
        template <class T>
        std::regex regex(T const& t){ return regex_trait<T>::regex(t); }
        
        template <class T>
        void convert(const std::string& str, T& output) { converter_trait<T>::convert(str, output); }
    
        //
        // Default behaviour : use a regex to extract the substring and then convert the substring
        //
        template <class T>
        struct parser_trait{
            using strIterator = std::string::const_iterator;
        
            static strIterator parse(strIterator const& cur_pos, strIterator const& end, T& out_arg){
                std::smatch match_results;
                bool pattern_found = regex_search(cur_pos, end, match_results, trait::regex(out_arg), std::regex_constants::match_continuous);
                //match_continuous asserts that the result begins at cur_pos
                
                if( not pattern_found ){
                    throw CouldNotParseArgumentException{};
                }
                
                std::string const& result = match_results[0];
                convert(result, out_arg);
                
                return cur_pos + result.size();
            }
        };
        
        //
        // Hepler function for the optimizations
        //
        void assert_not_whitespace(char c){
            if( std::isspace(c) ){
                throw CouldNotParseArgumentException{};
            }
        }
        //
        // Optimization for int
        //
        template <>
        struct parser_trait<int>{
            using strIterator = std::string::const_iterator;
            
            static strIterator parse(strIterator const& cur_pos, strIterator const& end, int& out_arg){
                assert_not_whitespace(*cur_pos);
                
                const char * cur_pos_ptr = &*cur_pos;
                char* end_ptr = nullptr;
                
                out_arg = strtol_safe(cur_pos_ptr, &end_ptr);
                
                const auto number_of_char_read = end_ptr - cur_pos_ptr;
                return cur_pos + number_of_char_read;
            }
            
            static inline int strtol_safe(const char* input, char** end){
                const auto errno_sav = errno;
                errno = 0;
                
                int out_arg = static_cast<int>( strtol(input, end, 10) );
                
                if(errno != 0 or *end == input){
                    throw CouldNotParseArgumentException{};
                }
                errno = errno_sav;
                
                return out_arg;
            }
        };
        
        
        //
        // Optimization for double
        //
        template <>
        struct parser_trait<double>{
            using strIterator = std::string::const_iterator;
            
            static strIterator parse(strIterator const& cur_pos, strIterator const& end, double& out_arg){
                assert_not_whitespace(*cur_pos);
                
                const char * cur_pos_ptr = &*cur_pos;
                char* end_ptr = nullptr;
                
                out_arg = strtod_safe(cur_pos_ptr, &end_ptr);
                
                const auto number_of_char_read = end_ptr - cur_pos_ptr;
                return cur_pos + number_of_char_read;
            }
            
            static inline double strtod_safe(const char* input, char** end){
                const auto errno_sav = errno;
                errno = 0;
                
                double out_arg = strtod(input, end);
                
                if(*end == input){
                    throw CouldNotParseArgumentException{};
                }
                if(errno == ERANGE){ //overflow or underflow
                    if(out_arg > 1){ //overflow
                        throw CouldNotParseArgumentException{};
                    } //underflow are ok
                }
                errno = errno_sav;
                
                return out_arg;
            }
        };
        
        //
        // Optimization for std::string
        //
        template <>
        struct parser_trait<std::string>{
            using strIterator = std::string::const_iterator;
            
            static strIterator parse(strIterator cur_pos, strIterator const& end, std::string& out_arg){
                out_arg.clear();
                
                auto end_of_arg = std::find_if(cur_pos, end, isspace);
                out_arg = std::string(cur_pos, end_of_arg);
                
                return end_of_arg;
            }
        };

        //*/
    }
}

#endif
