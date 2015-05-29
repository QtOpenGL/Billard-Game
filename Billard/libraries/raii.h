/*
 Title        : raii wrapper
 Project      : Billard EPFL MA 1 - Zoé Snijders, Julien Harbulot
 File         : raii.h
 Despcription : This wrapper class is a c++ equivalent of the Java's finally for exception handling. 
 |              See Soustrup's article about RAII
 License      : MIT License (MIT)
 |              Copyright (c) 2015 Julien HARBULOT
 */
#ifndef Billard_raii_h
#define Billard_raii_h

//
// Il s'agit d'un équivalent c++ du finally de Java
// Note : La technique RAII est bien connue en c++
//
template <class F>
class RAIIWrapper{
    F func;
public:
    explicit RAIIWrapper(F func) : func { std::move(func) } {}
    ~RAIIWrapper() { func(); } //finally, call the functor
};

template <class F>
inline RAIIWrapper<F> RAII(F func){
    return RAIIWrapper<F>{ std::move(func) };
}
#endif
