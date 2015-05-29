//
// Tests for the string_matcher
// Author : Julien Harbulot 2015
//

#include "bandit.h"
#include <string>
#include <sstream>
#include "string_matcher.h"
using namespace std;
using namespace string_matcher;

BEGIN_BANDIT
Describe(string_matcher){
    const StringPattern pattern {"abc"};
    const string suffix = "def";
    
    const auto check_suffix = [&](stringstream& stream, string const& suffix){
        string real_suffix;
        stream >> real_suffix;
        AssertThat(real_suffix, Equals( suffix ));
    };
    
    When(the pattern is empty){
        StringPattern empty_pattern {""};
        stringstream stream{ suffix };
        stream >> empty_pattern;
        
        AssertThat(stream.good(), Equals( true ));
        check_suffix(stream, suffix);
    });
    
    When(the pattern does not exist){
        stringstream stream{ suffix };
        stream >> pattern;
        
        AssertThat(stream.fail(), Equals( true ));
        stream.clear();
        
        check_suffix(stream, suffix);
    });
    
    When(the pattern is too long){
        stringstream stream{ "a" };
        stream >> pattern;
        
        AssertThat(stream.fail(), Equals( true  ));
        AssertThat(stream.eof() , Equals( false ));
        stream.clear(ios_base::goodbit);
        
        check_suffix(stream, stream.str());
        
    });
    
    When(the stream is empty){
        stringstream stream{ "" };
        stream.setstate(ios_base::eofbit);
        
        stream >> pattern;
        
        AssertThat(stream.eof() , Is().True()  );
        AssertThat(stream.fail(), Is().False() );
    });
    
    When(the pattern exists){
        stringstream stream{ pattern.str + suffix };
        stream >> pattern;
        
        AssertThat(stream.fail(), Equals( false ));
        check_suffix(stream, suffix);
    });
    
    Feature("the whole stream is used"){
        stringstream  stream { pattern.str };
        
        It("is initially good"){ AssertThat(stream.good(), Is().True()); });
        
        stream >> pattern;
        
        It("is not yet at eof"){ AssertThat(stream.eof() , Is().False() ); });
        It("has not failed")   { AssertThat(stream.fail(), Is().False() ); });
        
        It("is just before eof"){
            char c; stream >> c;
            AssertThat(stream.eof() , Is().True () );
        });
    });
    
    When(the pattern is a const char){
        stringstream stream{ "abcd" };
        const char* pattern = "ab";
        stream >> pattern;
        
        AssertThat(string(pattern), Equals("ab"));
        check_suffix(stream, "cd");
    });
    
    When(the pattern is a const string){
        stringstream stream{ "abcd" };
        const string pattern = "ab";
        stream >> pattern;
        
        AssertThat(pattern, Equals("ab"));
        check_suffix(stream, "cd");
    });
    
    When(a string is not a pattern){
        stringstream stream{ "abcd" };
        string pattern = "ab";
        stream >> pattern;
        
        AssertThat(pattern, Equals(  stream.str()  ));
        check_suffix(stream, "");
    });
    
    Feature("exception handling"){
        stringstream stream;
        
        Before_each(){
            stream = stringstream("abcdefgh");
            stream.exceptions(istream::eofbit | istream::failbit | istream::badbit);
        });
        
        When("no exception should be thrown"){
            StringPattern pattern{"abcd"};
            string suffix{"efgh"};
            
            stream >> pattern;
            
            stream.exceptions( ios_base::io_state{} );
            check_suffix(stream, suffix);
        });
        When("the pattern is too long"){
            StringPattern pattern(stream.str() + '\0');
            AssertThrows(std::exception, stream >> pattern);
        });
        When("the pattern do not match"){
            StringPattern pattern("x");
            AssertThrows(std::exception, stream >> pattern);
        });
    });
});
END_BANDIT
BANDIT_RUN_TESTS();