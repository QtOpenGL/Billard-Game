// La bibliothèque utilise une fonctionnalité de c++1y : les séquences.
// Pour la rendre compatible avec c++11, il suffit d'inclure ce header avant d'inclure multiple_dispatch.h

#ifndef Multiple_Dispatch_Release_cpp11compatibility_h
#define Multiple_Dispatch_Release_cpp11compatibility_h

// Utilisation du header integer_seq de Jonathan Wakely.
// Url : https://gitorious.org/redistd/integer_seq/source/24293c100405bf3982732f53897b4beb962a9086:integer_seq.h

#include "integer_seq.h"
namespace std {
    using namespace redi;
    //Note : la bibliothèque s'attend à ce que les séquences soient disponibles dans le namespace std;
}

#endif
