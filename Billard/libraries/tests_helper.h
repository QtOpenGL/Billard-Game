#ifndef Billard_vecteur_tests_helper_h
#define Billard_vecteur_tests_helper_h

#include "vecteur.h"
#include "boule.h"
#include "almost_equal.h"

namespace snowhouse {
    
    template <class ExpectedType>
    struct EpsilonEqualsConstraint;
    
    
    template <>
    struct EpsilonEqualsConstraint<Vecteur> : Expression< EpsilonEqualsConstraint<Vecteur> >
    {
        using ExpectedType = Vecteur;
        using DeltaType = unsigned long;
        
        EpsilonEqualsConstraint(const ExpectedType& expected, const DeltaType& delta)
        : m_expected(expected), m_delta(delta)
        {
        }
        
        template<typename ActualType>
        bool operator()(const ActualType& actual) const
        {
            return epsilon(actual, m_expected, m_delta);
        }
        
        ExpectedType m_expected;
        DeltaType m_delta;
    };
    template <>
    struct EpsilonEqualsConstraint<Boule> : Expression< EpsilonEqualsConstraint<Boule> >
    {
        using ExpectedType = Boule;
        using DeltaType = size_t;
        
        EpsilonEqualsConstraint(const ExpectedType& expected, const DeltaType& delta)
        : m_expected(expected), m_delta(delta)
        {
        }
        
        template<typename ActualType>
        bool operator()(const ActualType& actual) const
        {
            return actual.epsilon(m_expected, m_delta);
        }
        
        ExpectedType m_expected;
        DeltaType m_delta;
    };
    template <>
    struct EpsilonEqualsConstraint<double> : Expression< EpsilonEqualsConstraint<double> >
    {
        using ExpectedType = double;
        using DeltaType = size_t;
        
        EpsilonEqualsConstraint(const ExpectedType& expected, const DeltaType& delta)
        : m_expected(expected), m_delta(delta)
        {
        }
        
        template<typename ActualType>
        bool operator()(const ActualType& actual) const
        {
            return almost_equal(actual, m_expected, m_delta);
        }
        
        ExpectedType m_expected;
        DeltaType m_delta;
    };
    
    template <class ExpectedType>
    inline EpsilonEqualsConstraint<ExpectedType> EpsilonEquals(const ExpectedType& expected, unsigned long delta = 10)
    {
        return EpsilonEqualsConstraint<ExpectedType>(expected, delta);
    }
    inline EpsilonEqualsConstraint<double> EpsilonEquals(int expected, unsigned long delta = 10)
    {
        return EpsilonEqualsConstraint<double>(expected, delta);
    }
    
    template< typename ExpectedType >
    struct Stringizer< EpsilonEqualsConstraint< ExpectedType > >
    {
        static std::string ToString(const EpsilonEqualsConstraint< ExpectedType >& constraint)
        {
            std::ostringstream builder;
            builder << "almost equal to " << snowhouse::Stringize(constraint.m_expected) << " with delta " << constraint.m_delta;
            
            return builder.str();
        }
    };
}

#endif
