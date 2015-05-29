#include <iostream>
#include <bandit/bandit_alt.h>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

#include "tokenize.h"

BEGIN_BANDIT

Describe(tokenize){
    When(the input is empty){
        string input = "";
        auto resultat = tokenize(input, '%');
        vector<string> expected{ input };
        AssertThat(resultat, EqualsContainer(expected));
    });
    When(there is no delimiter at all){
        string input = "abc";
        auto resultat = tokenize(input, '%');
        vector<string> expected{ input };
        AssertThat(resultat, EqualsContainer(expected));
    });
    When(there is one delimiter){
        string input = "a_c";
        auto resultat = tokenize(input, '_');
        vector<string> expected{ "a", "c" };
        AssertThat(resultat, EqualsContainer(expected));
    });
    When(there is one delimiter at the begining){
        string input = "_a";
        auto resultat = tokenize(input, '_');
        vector<string> expected{ "", "a" };
        AssertThat(resultat, EqualsContainer(expected));
    });
    When(there is one delimiter at the end){
        string input = "a_";
        auto resultat = tokenize(input, '_');
        vector<string> expected{ "a", "" };
        AssertThat(resultat, EqualsContainer(expected));
    });
    When(there are two delimiters){
        string input = "a%b|%c";
        auto resultat = tokenize(input, '%');
        vector<string> expected{ "a", "b|", "c" };
        AssertThat(resultat, EqualsContainer(expected));
    });
    When(there are two consecutive delimiters){
        string input = "a||b";
        auto resultat = tokenize(input, '|');
        vector<string> expected{ "a", "", "b" };
        AssertThat(resultat, EqualsContainer(expected));
    });
    When(there are delimiters at the begining and the end){
        string input = "%a%b|%c%";
        auto resultat = tokenize(input, '%');
        vector<string> expected{ "", "a", "b|", "c", "" };
        AssertThat(resultat, EqualsContainer(expected));
    });
});

END_BANDIT
BANDIT_RUN_TESTS()