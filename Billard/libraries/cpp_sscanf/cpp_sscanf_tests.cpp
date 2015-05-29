/*
 Title        : cpp sscanf
 Project      : string parser
 File         : cpp_sscanf_tests.cpp
 Despcription : tests for the cpp_sscanf::sscanf function
 License      : MIT License (MIT)
 Copyright (c) 2015 Julien HARBULOT
 */

#include <iostream>
#include <tuple>
#include <bandit/bandit_alt.h>

#include "cpp_sscanf.h"
#include "parser_trait.h"

using namespace std;
using namespace cpp_sscanf;

BEGIN_BANDIT
Describe(sscanf){
    // 
    // Empty
    //
    
    When(the input and the pattern are empty){
        string input{};
        string pattern{};
        sscanf(input, pattern, '_');
    });
    When(the input is empty but not the pattern){
        string input{};
        string pattern{" a "};
        
        AssertThrows(InputTooShort, sscanf(input, pattern, '_') );
    });
    When(the pattern is empty but not the input){
        string input{ "A" };
        string pattern {};
        auto end_iterator = sscanf(input, pattern, '_');
        AssertThat(end_iterator, Equals(input.begin()));
    });
    
    //
    // Size
    //
    When(the input is too short for the pattern){
        string input{ "123" };
        string pattern{ "1234" };
        
        AssertThrows(InputTooShort, sscanf(input, pattern, '_') );
    });
    When(the input is longer than the pattern){
        string input{ "1234" };
        string pattern{ "123" };
        
        auto end_iterator = sscanf(input, pattern, '_');
        AssertThat(end_iterator, Equals( input.begin() + pattern.length() ));
    });

    //
    // Parsing
    //
    When(there is one out_argument at the begining){
        string input { "5.4abc" };
        string pattern { "_abc" };
        double out2 = 0;
        
        auto end_iterator = sscanf(input, pattern, '_', out2);
        
        AssertThat(out2, Equals( 5.4 ));
        AssertThat(end_iterator, Equals( input.end() ));
    });
    When(there is one out_argument at the end){
        string input   { "abc5.4" };
        string pattern { "abc_" };
        double out2 = 0;
        
        auto end_iterator = sscanf(input, pattern, '_', out2);
        
        AssertThat(out2, Equals( 5.4 ));
        AssertThat(end_iterator, Equals( input.end() ));
    });
    When(there are multiple out_arguments in the middle){
        string input { "abc100def05.5e-1ghi" };
        string pattern { "abc_def_ghi" };
        int out1; double out2;
        
        auto end_iterator = sscanf(input, pattern, '_', out1, out2);
        
        AssertThat(out1, Equals( 100 ));
        AssertThat(out2, Equals( 05.5e-1 ));

        AssertThat(end_iterator, Equals( input.end() ));
    });
    When(the input is longer than the pattern and ends with a parsed argument){
        string suffix  { "trailing_string"   };
        string input   { "input123" + suffix };
        string pattern { "input_"            };
        int out1;
        
        auto end_iterator = sscanf(input, pattern, '_', out1);
        
        AssertThat(out1, Equals( 123 ));
        
        string suffix_result{end_iterator, input.cend() };
        AssertThat(suffix_result , Equals( suffix ));
    });
    
    //
    // Parsing with error
    //
    When(the pattern is not found){
        string input  { "aaa 10" };
        string pattern{ "xxx _"  };
        int out1;
        
        AssertThrows(PatternNotFoundException, sscanf(input, pattern, out1) );
    });
    
    When(the argument could not be parsed){
        string input  { "aaa bbb" };
        string pattern{ "aaa _"   };
        int out1;
        
        AssertThrows(CouldNotParseArgumentException, sscanf(input, pattern, '_', out1) );
    });
    
    When(the int/double argument is not at the begining of the substring){
        string input{ "aaa b10" };
        string pattern{ "aaa _" };
        int out1; double out2;
        
        AssertThrows(CouldNotParseArgumentException, sscanf(input, pattern, '_', out1));
        AssertThrows(CouldNotParseArgumentException, sscanf(input, pattern, '_', out2));
    });
    
    When(the int/double argument is after an unwanted white space){
        string input{ "aaa  10" };
        string pattern{ "aaa _" };
        int out1; double out2;
        
        AssertThrows(CouldNotParseArgumentException, sscanf(input, pattern, '_', out1));
        AssertThrows(CouldNotParseArgumentException, sscanf(input, pattern, '_', out2));
    });
});

//
// Other Parser traits test
//

Describe("parsing a string argument"){
    When("there is no parsable string"){
        string input = "";
        string pattern = "_";
        string output;
        string expected_output = "";
        
        auto end_pos = sscanf(input, pattern, '_', output);
        auto expected_end_pos = input.begin();
        
        AssertThat(output , Equals( expected_output ));
        AssertThat(end_pos, Equals( expected_end_pos ));
    });
    When("it is at the begining"){
        string input = "julien diane";
        string pattern = "_";
        string output;
        string expected = "julien";
        
        auto end_pos = cpp_sscanf::sscanf(input, pattern, '_', output);
        
        AssertThat( output, Equals( expected ));
        AssertThat( end_pos, Equals( input.begin() + expected.size() ));
    });
    When("it is at the end"){
        string input = "julien diane";
        string pattern = "julien _";
        string output;
        string expected = "diane";
        
        auto end_pos = cpp_sscanf::sscanf(input, pattern, '_', output);
        
        AssertThat( output, Equals( expected ));
        AssertThat( end_pos, Equals( input.end() ));
    });
    When("it is in the middle"){
        string input = "abc xxx def";
        string pattern = "abc _ def";
        string output;
        string expected_output = "xxx";
        
        string::const_iterator end_pos = sscanf(input, pattern, '_', output);
        string suffix( end_pos, input.cend() );
        
        AssertThat(output , Equals( expected_output ));
    });
    When("there is a leading white space"){
        string input = " something";
        string pattern = "_";
        string output;
        string expected_output = "";
        
        auto end_pos = sscanf(input, pattern, '_', output);
        auto expected_end_pos = input.begin();
        
        AssertThat(output , Equals( expected_output ));
        AssertThat(end_pos, Equals( expected_end_pos ));
    });
    When("there is a leading white space"){
        string suffix = " hello";
        string input = "inputxxx" + suffix;
        string pattern = "input_";
        string output;
        string expected_output = "xxx";
        
        auto end_pos = sscanf(input, pattern, '_', output);
        auto expected_end_pos = input.cend() - suffix.size();
        
        AssertThat(output , Equals( expected_output ));
        AssertThat(end_pos, Equals( expected_end_pos ));
        AssertThat(string(end_pos, input.cend()), Equals(suffix));
    });

});

//
// Demo
//

Feature("demo"){
    
    When(the returned position is reused){
        string input = "Julien 20 ans" "\n" "Diane 25 ans" "\n" "Gabriel 23 ans" "\n";
        string pattern = "_ _ ans\n";
        vector<pair<string, int>> personnes;
    
        for(auto cur_pos = input.cbegin(); cur_pos != input.cend();  )
        {
            string nom; int age;
            cur_pos = sscanf(cur_pos, input.cend(), pattern, '_', nom, age);
            personnes.emplace_back(nom, age);
        }
        
        vector<pair<string, int>> expected{ {"Julien", 20} , {"Diane", 25} , {"Gabriel", 23} };
        bool ok = std::equal(expected.begin(), expected.end(), personnes.begin());
        AssertThat( ok, Is().True() );
    });

    When(the string is first tokenized){
        string input = "Julien 20 ans, Diane 25 ans, Gabriel 23 ans";
        vector<pair<string, int>> personnes;
        
        vector<string> lines = tokenize(" "+input, ',');
        
        for(auto const& line : lines){
            string nom; int age;
            sscanf(line, " _ _ ans", '_', nom, age);
            personnes.emplace_back(nom, age);
        }
        
        vector<pair<string, int>> expected{ {"Julien", 20} , {"Diane", 25} , {"Gabriel", 23} };
        bool ok = std::equal(expected.begin(), expected.end(), personnes.begin());
        AssertThat( ok, Is().True() );
    });
});
END_BANDIT
BANDIT_RUN_TESTS()