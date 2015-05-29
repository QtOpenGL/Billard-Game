/*
 Title        : tokenize
 Project      : string parser
 File         : tokenize.h
 Despcription : defines the tokenize function
 License      : MIT License (MIT)
 Copyright (c) 2015 Julien HARBULOT
 */


#ifndef string_parser_tokenize_h
#define string_parser_tokenize_h

#include <vector>
#include <string>

std::vector<std::string> tokenize(std::string const& str, char delim){
    
    //
    // Creates a std::vector big enough for holding the tokens
    //
    size_t number_of_delimiter = 0;
    for(const auto car : str){
        if(car == delim) ++number_of_delimiter;
    }
    std::vector<std::string> tokens;
    tokens.reserve(number_of_delimiter+1);
    
    //
    // Fills the std::vector with the tokens
    //
    long int delimiter_index[2] = {-1, -1}; //the first delimiter is a sentinel at pos -1
    int cur_i = 0;
    for(size_t str_index = 0; str_index != str.size(); ++str_index){
        if(str[str_index] == delim){
            const auto old_i = cur_i;
            cur_i = (old_i+1)%2;
            
            const auto previous_delim = delimiter_index[old_i];
            auto&       current_delim = delimiter_index[cur_i];
            
            current_delim = str_index;
            
            const auto delta = current_delim - previous_delim - 1;
            const auto token_length = (delta < 0) ? 0 : delta;
            tokens.emplace_back(str.substr( previous_delim + 1, token_length ));
        }
    }
    
    const auto last_delimiter_index = delimiter_index[cur_i];
    tokens.emplace_back( str.substr( last_delimiter_index + 1 ) );
    
    return tokens;
}
#endif
