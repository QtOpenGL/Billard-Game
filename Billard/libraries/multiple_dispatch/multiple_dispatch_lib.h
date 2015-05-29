#ifndef Multiple_Dispatch_Via_Visitor_Wrapper_h
#define Multiple_Dispatch_Via_Visitor_Wrapper_h
/************************************************************************
 Project : Multiple Dispatch Wrappers
 File    : multiple_dispatch.h ,
           Single header, sans fonctionnalité avancée
 
 Author  : HARBULOT Julien
 
 Copyright (C) 2014 HARBULOT Julien Edmond René
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 email : info [at] julienh.fr
 
 ************************************************************************
 * Système de multiple dispatch à N arguments (pour tout N > 0)
 *
 * Utilisation :
 *  Chaque classe dont on voudra reconnaître le type implémente le
 *  pattern visitor [aka : elle possède une methode accept(...) ]
 *  Les wrappers prennent un nombre quelconque d'arguments et une action.
 *  Ils s'occupent de retrouver le type de tous les arguments via leurs
 *  méthodes accept(...), puis apellent l'action demandée.
 *
 * Version : 5 - CircularBuffer
 
 ************************************************************************/


#include <tuple> //utile pour CircularBuffer uniquement.

//========================================================================
//================================ LIBRARY ===============================
//========================================================================
namespace dispatch{
namespace multiple_dispatch_impl{

    //====================================================================
    //====================================================================
    //
    // === Family ===
    //
    
    // A partir du type BaseClass, un dispatcher doit retrouver le bon type dérivé.
    // Pour cela, il doit connaitre la liste des types dérivés (DerivedList)
    // Et il doit pouvoir visiter la hiérarchie en héritant de VisitorBase
    // Ces données constituent une famille :
    
    template <class VisitorBase, class BaseClass, class... DerivedList>
    class Family{
    public:
        using Visitor = VisitorBase;
        using Base = BaseClass;
        
        Family(Base& b) : base_{b} {}
        Base& base() const {return base_;}
        
    private:
        Base& base_;
    };

    //====================================================================
    //====================================================================
    //
    // === FamilyVisitor ===
    //

    // Etant donnée une famille,
    // un FamilyVisitor implémente une fonction visit(...) pour chaque
    // membre de la famille.
    template <class Action, class Family>
    class FamilyVisitor;
    
    // Récursion : Pour chaque dérivée, implémenter une fonction visit(...)
    // A chaque étape, le parameter_pack <DerivedTail...> diminue.
    template <class Action, class VisitorBase, class Base, class DerivedHead, class... DerivedTail>
    class FamilyVisitor<Action, Family<VisitorBase, Base, DerivedHead, DerivedTail...>>
    : public FamilyVisitor<Action, Family<VisitorBase, Base, DerivedTail...>>{
    private:
        using Parent = FamilyVisitor<Action, Family<VisitorBase, Base, DerivedTail...>>;
    public:
        using Parent::Parent;
        using Parent::visit;
        void visit(DerivedHead& derived){
            Parent::action(derived);
        }
    };
    
    // Cas d'arret : Il n'y a plus de dérivée.
    template <class Action, class VisitorBase, class Base >
    class FamilyVisitor< Action, Family<VisitorBase, Base> >
    : public VisitorBase
    {
    protected:
        Action action;
    public:
        FamilyVisitor(Action a): action{a} {}
        void start_visit(Base& b){b.accept(*this);}
        void visit(){} //permet d'utiliser Parent::visit;
    };
    
    //====================================================================
    //====================================================================
    //
    // === MultiDispatch ===
    //

    // Le MultiDispatch retrouve le bon type dérivé de chaque référence de CircularBuffer
    // en effectuant une récursion sur l'ensemble des familles à traiter.
    template <class Action, class CircularBuffer, class... Families>
    class MultiDispatch;
    
    // Recursion : Pour chaque famille, lancer une visite sur cette famille.
    // A chaque étape <OtherFamilies...> diminue.
    template <class Action, class CircularBuffer, class FamilyHead, class... OtherFamilies>
    class MultiDispatch<Action, CircularBuffer, FamilyHead, OtherFamilies...>{
    private:
        Action& final_action;
        CircularBuffer deriveds;
    
    public:
        MultiDispatch(Action& init_action, CircularBuffer init_deriveds)
        : final_action(init_action), deriveds{init_deriveds}
        {}
        
        void operator()() const{
            start_visit(deriveds);
        }
        
        template <class DerivedType>
        void operator()(DerivedType& to_insert) const{
            //maj puis recursion.
            auto buffer = deriveds.template insert<DerivedType&>(to_insert);
            start_visit(buffer);
        }
    private:
        template <class Buffer>
        void start_visit(Buffer buffer) const{
            auto visitor = get_visitor(buffer);
            visitor.start_visit(buffer.top());
        }
        
        template <
        class Buffer,
        class NextVisitorAction = MultiDispatch<Action, Buffer, OtherFamilies...>,
        class NewVisitor = FamilyVisitor<NextVisitorAction, FamilyHead>
        >
        NewVisitor get_visitor(Buffer buffer) const{
            NextVisitorAction visitor_action{final_action, buffer};
            NewVisitor visitor{visitor_action};
            return visitor;
        }
    };
    
    // Cas d'arret : Toutes les familles ont été traitées.
    template <class Action, class CircularBuffer>
    class MultiDispatch<Action, CircularBuffer>{
    public:
        Action& final_action;
        CircularBuffer deriveds;
    public:
        MultiDispatch(Action& a, CircularBuffer deriveds)
        : final_action(a), deriveds{deriveds}
        {}
        
        template <class DerivedType>
        void operator()(DerivedType& to_insert) const{
            //maj puis fin.
            auto buffer = deriveds.template insert<DerivedType&>(to_insert);
            buffer.apply(final_action);
        }
    };
    
    //====================================================================
    //====================================================================
    //
    // === CircularBuffer ===
    //
    template <class T, class... Ts>
    class CircularBuffer{
        private :
        // Simplification d'écriture
        template <size_t... Ns> using Seq = std::index_sequence<Ns ...>;
        template <size_t N> using MakeSeq = std::make_index_sequence<N>;
        using Data_t = std::tuple<T, Ts...>;
        
        // Variables
        static constexpr size_t size = 1+sizeof...(Ts);
        const Data_t data;
        
    public:
        CircularBuffer(T item1, Ts... items)
        : data{item1, items...}
        {}
        
        // Acces au 1er element.
        T top() const { return std::get<0>(data);}
        
        // Insertion en fin et supression du 1er element (circular)
        template <class NewT>
        CircularBuffer<Ts..., NewT> insert(NewT newItem) const{
            return insert_impl<NewT>(newItem, MakeSeq<size>{});
        }
        
        // Appelle la fonction avec tous les éléments en argument.
        template <class Func>
        void apply(Func& f) const{
            return apply_impl(f, MakeSeq<size>{});
        }
        
    private:
        // Implémentations :
        
        template <class NewT, size_t I0, size_t... Is>
        CircularBuffer<Ts..., NewT> insert_impl(NewT newItem, Seq<I0, Is...>) const{
            // omet le premier element d'indexe I0, newItem est ajouté en fin.
            return {std::get<Is>(data)..., newItem};
        }
        
        template <class Func, size_t... Is>
        void apply_impl(Func& f, Seq<Is...> ) const{
            f(std::get<Is>(data)...);
        }
    };
    //====================================================================
    //====================================================================
    //
    // === Proposition d'Interface ===
    //
    
    // Gestion de la mémoire :
    // Tout ce qui est fourni par l'user (Action, Derived) est manipulé
    // via des références.
    // Tout ce qui est créé par la bibliothèque est en variable locale.
    
    template <class Action, class... Families>
    void execute(Action& action, Families... items){
        using Data_t = CircularBuffer<typename Families::Base& ...>;
        using DispatcherImpl = MultiDispatch<Action, Data_t, Families...>;
        DispatcherImpl dispatch{action, Data_t{items.base() ...} };
        dispatch();
    }

} //namespace implementation

using multiple_dispatch_impl::Family;
using multiple_dispatch_impl::execute;
    
} //namespace public

#endif // defined Multiple_Dispatch_Via_Visitor_Wrapper_h */
