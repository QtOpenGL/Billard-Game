//
//  Aliases in order to provide an Igloo like syntax for the bandit framework.
//  (C) Julien HARBULOT 2015 Free licence

#ifndef Igloo_bandit_alt_h
#define Igloo_bandit_alt_h

#include <bandit/bandit_lib.h>
using namespace bandit;

#define BEGIN_BANDIT            go_bandit([](){
#define END_BANDIT              });

#define Before_each()           before_each([&]()
#define After_each()            after_each ([&]()

#define Context(context_str)    describe(context_str, [&]()
#define Feature(context_str)    describe(context_str, [&]()
#define Describe(context_str)   describe(context_str, [&]()

#define Given(statement)        statement
#define When(it_str)            it(it_str, [&]()
#define It(it_str)              it(it_str, [&]()
#define Then(obj, predicate)    AssertThat(obj, predicate);

#define BANDIT_RUN_TESTS() /*--------------------*/\
int main(int argc, char* argv[]){           \
char* argv_opt[] = {"/", "--no-color"}; \
return bandit::run(2, argv_opt);        \
} /*----------------------------------------*/

#endif
