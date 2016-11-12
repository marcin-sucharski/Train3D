#ifndef TRAIN_DI_CONTAINER
#define TRAIN_DI_CONTAINER

#include <memory>
#include <utility>
#include <type_traits>
#include <tuple>
#include <functional>
#include <vector>
#include <stack>
#include <limits>
#include <cassert>

namespace train {
    namespace di {
        /// Represents binding between interface and implementation.
        template<typename TInterface, typename TImplementation, typename... Args>
        struct InjectObject {
            typedef TInterface Interface;
            typedef TImplementation Implementation;

            InjectObject(Args &&... args) : args(std::forward<Args>(args)...) {}

            InjectObject(InjectObject &&) = default;

            InjectObject(const InjectObject &) = delete;

            InjectObject &operator=(InjectObject &&) = default;

            InjectObject &operator=(const InjectObject &) = delete;

            std::tuple<Args...> args;
        };

        template<typename TInterface, typename TImplementation, typename... Args>
        InjectObject<TInterface, TImplementation, Args...> Inject(Args &&... args) {
            return InjectObject<TInterface, TImplementation, Args...>(std::forward<Args>(args)...);
        };

        template<typename... Interfaces>
        struct Depends {
        };

        namespace detail {
            template<typename T, typename Deleter = std::default_delete<T>>
            struct SingleImplementation {
                typedef SingleImplementation SelfType;
                typedef std::unique_ptr<T, Deleter> PtrType;

                SingleImplementation() = default;

                SingleImplementation(T *impl) : impl{impl} {}

                T &get() const noexcept {
                    return next ? next->get() : *impl;
                }

                bool hasNext() const noexcept {
                    return next != nullptr;
                }

                void push(std::unique_ptr<SelfType> next) noexcept {
                    if (!next)
                        next.swap(next);
                    else
                        next->push(move(next));
                }

                void pop() noexcept {
                    if (!next)
                        return;

                    if (next->hasNext())
                        next->pop();
                    else
                        next = nullptr;
                }

            private:
                PtrType impl;
                std::unique_ptr<SelfType> next = nullptr;
            };

            template<typename... Args>
            struct Seq {
            };
            template<typename T, typename... Args>
            struct Seq<T, Args...> {
                // sequence without first element
                typedef Seq<Args...> next;
                // value of first element
                typedef T value;
            };
            template<typename T>
            struct Seq<T> {
                typedef T value;
            };

            template<typename... Types>
            struct Seq<Depends<Types...>> {
                typedef Seq<Types...> value;
            };

            template<>
            struct Seq<Depends<>> {
                typedef Seq<> value;
            };

            template<typename T, typename... Types>
            struct Seq<T, Seq<Types...>> {
                typedef Seq<T, Types...> value;
            };

            template<typename... Args>
            struct SeqSizeof {
                static const std::size_t value = sizeof...(Args);
            };
            template<typename... Args>
            struct SeqSizeof<Seq<Args...>> {
                static const std::size_t value = SeqSizeof<Args...>::value;
            };

            template<int... S>
            struct IntSeqType {
            };

            template<int N, int... S>
            struct IntSeq : IntSeq<N - 1, N - 1, S...> {
            };

            template<int... S>
            struct IntSeq<0, S...> {
                typedef IntSeqType<S...> value;
            };

            template<
                std::size_t N,
                std::size_t idx,
                typename T,
                typename Curr,
                typename... Types
            >
            struct IndexOfImpl {
                static const std::size_t value =
                    std::is_same<T, Curr>::value ? idx : IndexOfImpl<N, idx + 1, T, Types...>::value;
            };

            template<std::size_t N,
                std::size_t idx,
                typename T,
                typename Curr
            >
            struct IndexOfImpl<N, idx, T, Curr> {
                static const std::size_t value =
                    std::is_same<T, Curr>::value ? idx : std::numeric_limits<std::size_t>::max();
            };

            /// Gets index of specified type in list of types
            template<typename T, typename... Types>
            struct IndexOf {
                static const std::size_t value = IndexOfImpl<sizeof...(Types), 0, T, Types...>::value;
            };

            template<std::size_t N, typename T, typename... Types>
            struct GetNth {
                typedef typename GetNth<N - 1, Types...>::value value;
            };

            template<std::size_t N, typename T, typename... Types>
            struct GetNth<N, Seq<T, Types...>> {
                typedef typename GetNth<N, T, Types...>::value value;
            };

            template<typename T, typename... Types>
            struct GetNth<0, T, Types...> {
                typedef T value;
            };

            template<typename T, typename... Types>
            struct SeqInsertAtBeginning {
                typedef Seq<T, Types...> value;
            };

            template<typename T, typename... Types>
            struct SeqInsertAtBeginning<T, Seq<Types...>> {
                typedef typename SeqInsertAtBeginning<T, Types...>::value value;
            };

            /// Checks if specified type is in list of types
            template<typename T, typename A, typename... Types>
            struct Contains {
                static const bool value = std::is_same<T, A>::value || Contains<T, Types...>::value;
            };

            template<typename T, typename A>
            struct Contains<T, A> {
                static const bool value = std::is_same<T, A>::value;
            };

            template<typename T, typename... Types>
            struct Contains<T, Seq<Types...>> {
                static const bool value = Contains<T, Types...>::value;
            };

            template<std::size_t N, typename A, typename B>
            struct AnyInImpl {
                static const bool value =
                    Contains<typename A::value, B>::value || AnyInImpl<N - 1, typename A::next, B>::value;
            };

            template<typename A, typename B>
            struct AnyInImpl<1, A, B> {
                static const bool value = Contains<typename A::value, B>::value;
            };

            template<typename A, typename B>
            struct AnyInImpl<0, A, B> {
                static const bool value = false;
            };

            /// Checks if any element of first sequence is in second
            template<typename A, typename B>
            struct AnyIn {
                static const bool value = AnyInImpl<SeqSizeof<A>::value, A, B>::value;
            };

            template<typename T, typename... Types>
            struct TopologicalSortImpl {
                typedef typename T::Implementation Implementation;
                typedef typename Seq<typename Implementation::DEPS>::value DepsSeq;

                typedef typename std::conditional<
                    AnyIn<DepsSeq, Seq<typename Types::Interface...>>::value,
                    TopologicalSortImpl<Types..., T>,
                    Seq<T, typename TopologicalSortImpl<Types...>::value>
                >::type::value value;
            };

            template<typename T>
            struct TopologicalSortImpl<T> {
                typedef Seq<T> value;
            };

            template<typename... Types>
            struct TopologicalSort {
                typedef typename TopologicalSortImpl<Types...>::value value;
            };

            template<typename... Types>
            struct TopologicalSort<Seq<Types...>> {
                typedef typename TopologicalSort<Types...>::value value;
            };
        }

        template<typename... Types> using Services = detail::Seq<Types...>;

        template<typename...>
        class DIContainer {
        };

        /// Represents dependency injection container.
        template<typename... TServices>
        class DIContainer<Services<TServices...>> {
            template<typename T> using Impl = detail::SingleImplementation<T>;
            template<typename T> using GetServiceIndex = detail::IndexOf<T, DIContainer, TServices...>;

            typedef std::function<void()> UndoSingleOverride;
            typedef std::vector<UndoSingleOverride> UndoOverrideContainer;

            struct SelfDeleter {
                void operator()(DIContainer *) {}
            };

            typedef detail::SingleImplementation<DIContainer, SelfDeleter> SelfImpl;

            std::tuple<SelfImpl, detail::SingleImplementation<TServices>...> services;
            std::stack<UndoOverrideContainer> undoOverride;

        public:
            DIContainer() {
                std::get<GetServiceIndex<DIContainer>::value>(services)
                    .push(std::unique_ptr<SelfImpl>(new SelfImpl{this}));
            }

            DIContainer(const DIContainer &) = delete;

            DIContainer(DIContainer &&) = default;

            DIContainer &operator=(const DIContainer &) = delete;

            DIContainer &operator=(DIContainer &&) = default;

            /// Checks if specified service has implementation.
            template<typename T>
            bool hasService() const noexcept {
                static_assert(detail::Contains<T, detail::Seq<DIContainer, TServices...>>::value,
                    "Service not registered");
                return std::get<detail::IndexOf<T, DIContainer, TServices...>::value>(services).hasNext();
            }

            /// Returns reference to specified service.
            template<typename T>
            T &getService() const noexcept {
                static_assert(detail::Contains<T, detail::Seq<DIContainer, TServices...>>::value,
                    "Service not registered");
                assert(hasService<T>());
                return std::get<detail::IndexOf<T, DIContainer, TServices...>::value>(services).get();
            }

            /// Overrides services specified by injects.
            template<typename... Injects>
            void override(Injects &&... injects) {
                typedef typename detail::TopologicalSort<Injects...>::value SortedInjects;
                undoOverride.push(overrideAdapter(SortedInjects(),
                    std::tuple<Injects &&...>(std::forward<Injects>(injects)...)));
            }

            /// Reverts single override.
            void undo() {
                for (auto &func : undoOverride.top())
                    func();
                undoOverride.pop();
            }

            template<typename T, typename... Args>
            T *newObject(Args &&... args) const noexcept {
                using DependenciesType = typename T::DEPS;
                return newImpl<T>(DependenciesType{}, std::forward<Args>(args)...);
            };

        private:
            template<
                typename T,
                typename... Args,
                typename... Dependencies
            >
            T *newImpl(Depends<Dependencies...>, Args &&... args) const noexcept {
                return new T{getService<Dependencies>()..., std::forward<Args>(args)...};
            }

            template<typename T, int... IntSeq, typename... Args>
            T *newWithTupleArguments(std::tuple<Args...> args, detail::IntSeqType<IntSeq...>) {
                ((void) args);
                return newObject<T>(std::get<IntSeq>(args)...);
            };

            template<typename... Injects, typename... Sorted>
            UndoOverrideContainer overrideAdapter(detail::Seq<Sorted...>, std::tuple<Injects &&...> injects) {
                return overrideImpl(std::tuple<Sorted &&...>(
                    std::forward<Sorted>(std::get<detail::IndexOf<Sorted, Injects...>::value>(injects))...));
            };

            template<typename Interface, typename Implementation, typename... Args>
            UndoOverrideContainer overrideImpl(std::tuple<InjectObject<Interface, Implementation, Args...> &&> inject,
                                               UndoOverrideContainer container = {}) {
                static_assert(detail::Contains<Interface, detail::Seq<TServices...>>::value);

                std::get<GetServiceIndex<Interface>::value>(services)
                    .Push(std::unique_ptr<Impl<Interface>>(
                        new detail::SingleImplementation<Interface>{newWithTupleArguments<Implementation>(
                            std::move(std::get<0>(inject).args), typename detail::IntSeq<sizeof...(Args)>::value())
                        }
                    ));
                container.push_back(
                    std::bind(&Impl<Interface>::pop, &std::get<GetServiceIndex<Interface>::value>(services)));
                return std::move(container);
            }

            template<
                typename FirstInject,
                typename... Injects
            >
            UndoOverrideContainer overrideImpl(std::tuple<FirstInject &&, Injects &&...> injects,
                                               UndoOverrideContainer container = {}) {
                container = overrideImpl(std::tuple<FirstInject &&>(
                    std::forward<FirstInject>(std::get<0>(injects))), std::move(container));
                container = overrideImpl(
                    std::tuple<Injects &&...>(
                        std::forward<Injects>(
                            std::get<detail::IndexOf<Injects, FirstInject, Injects...>::value>(injects))...),
                    std::move(container));
                return container;
            }
        };

#define DI_DEPENDS using DEPS = Game::app::di::Depends
    }
}

#endif