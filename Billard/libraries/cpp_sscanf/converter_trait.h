/*
 Title        : converter trait
 Project      : string parser
 File         : converter_trait.h
 Despcription : defines the converting functions for the basic types
 License      : MIT License (MIT)
 Copyright (c) 2015 Julien HARBULOT
 */

#ifndef string_parser_converter_trait_h
#define string_parser_converter_trait_h

#include <string>

namespace cpp_sscanf {
    namespace trait{
        
        template <class T>
        struct converter_trait;
        
        template <>
        struct converter_trait<int>{
            static void convert(const std::string& str, int& value){
                value = std::stoi(str);
            }
        };
        
        template <>
        struct converter_trait<double>{
            static void convert(const std::string& str, double& value){
                value = std::stod(str);
            }
        };
    }//trait
}//cpp_scanf

#endif
