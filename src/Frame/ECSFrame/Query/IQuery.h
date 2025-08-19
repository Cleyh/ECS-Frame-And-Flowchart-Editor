#pragma once

#include <type_traits>

#include "ECSFrame/Model/System/ISystem.h"
#include "ECSFrame/Model/Component/IComponent.h"
#include "ECSFrame/Model/Entity/IEntity.h"
#include "ECSFrame/Pointer/EPointer.h"
#include "ECSFrame/SystemUtils.h"

/**
 * Query namespace
 */
namespace ECS
{
    namespace QueryUtils
    {
        template <typename... Args>
        static EPointer<IEntity<Args...>> ConvertToQueryEntity(EPointer<IEntityObject> entity)
        {
            // Construct result.
            auto queryEntity = EPointer<IEntity<Args...>>::make();
            IdSet ids = IEntity<Args...>::ComponentIds(); // the component IDs we are looking for

            for (auto id : ids)
            {
                // Get the component from original entity.
                auto component = entity->getComponent(id);
                if (component)
                {
                    queryEntity->setComponent(id, component);
                }
            }

            return queryEntity;
        }
    }

    namespace Query
    {

        template <typename... Args>
        concept NonEmpty = (sizeof...(Args) > 0);

        /**
         * Basic Condition Node
         */

        template <NonEmpty... Args>
        struct And;

        template <NonEmpty... Args>
        struct Or;

        template <NonEmpty... Arg>
        struct Not;

        /**
         * Condition Type Traits
         * */

        template <typename T>
        inline constexpr bool is_and_v = std::is_same_v<T, And<typename T::Args...>>;

        template <typename T>
        inline constexpr bool is_or_v = std::is_same_v<T, Or<typename T::Args...>>;

        template <typename T>
        inline constexpr bool is_not_v = std::is_same_v<T, Not<typename T::Arg>>;

        template <typename T>
        inline constexpr bool is_leaf_v = !is_and_v<T> && !is_or_v<T> && !is_not_v<T>;

        /**
         * Condition Evaluator
         * usage:
         * - Evaluator<And<A, B, Not<C>, Or<D,E>>> query; == (A && B && !C && (D || E))
         * - And<...T>  == (T && ...)
         * - Or<...T>  == (T || ...)
         * - Not<...T>  == (!T && ...)
         */
        template <typename Cond>
        struct Evaluator
        {
            static bool matches(EPointer<IEntityObject> entity)
            {
                // Component
                const IdSet need = IEntity<Cond>::ComponentIds();
                const IdSet have = entity->getComponentIds();
                return have.contains(need);
            }
        };

        template <NonEmpty... Args>
        struct Evaluator<And<Args...>>
        {
            static bool matches(EPointer<IEntityObject> entity)
            {
                return (Evaluator<Args>::matches(entity) && ...);
            }
        };

        template <NonEmpty... Args>
        struct Evaluator<Or<Args...>>
        {
            static bool matches(EPointer<IEntityObject> entity)
            {
                return (Evaluator<Args>::matches(entity) || ...);
            }
        };

        template <typename... Args>
        struct Evaluator<Not<Args...>>
        {
            static bool matches(EPointer<IEntityObject> entity)
            {
                return (!Evaluator<Args>::matches(entity) && ...);
            }
        };
    }
} // namespace ECS

/*
 * Query Interface
 * This interface defines the basic structure for queries in the ECS framework.
 */
class IQuery
{
    using QueryResult = EPointer<IEntityObject>;
    using QueryResultList = EVector<EPointer<IEntityObject>>;
    using MatchFunc = std::function<bool(EPointer<IEntityObject>)>;

public:
    // doQuery() is the main function to execute the query logic.
    // Implementations should call base function
    virtual void doQuery();

    // Global Query call this function to update the results.
    // This function will check itself needs to update.
    virtual void update(/* some args */);

public:
    bool isDirty() const { return dirty; }

public:
    QueryResultList getAllResults();
    QueryResult operator[](size_t index) const;

protected:
    bool dirty = true;
    MatchFunc matchFunc = [](EPointer<IEntityObject> entity) { return false; };
    QueryResultList results;
};

/*
 * Query Condition
 * This class provides a way to define complex query conditions
 * using logical operators like AND, OR, and NOT.
 *
 * usage:
 * QueryCondition<And<A, B, Not<C>, Or<D,E>>> query;
 */
template <typename... Args>
class QueryCondition
    : public IQuery
{
public:
    QueryCondition()
    {
        matchFunc = [](EPointer<IEntityObject> entity)
        {
            return (ECS::Query::Evaluator<Args>::matches(entity) && ...);
        };
    }
};