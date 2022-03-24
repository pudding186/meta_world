#pragma once

//////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t N, typename... Args>
struct indexOf;

template <typename T, std::size_t N, typename... Args>
struct indexOf<T, N, T, Args...>
{
    enum { value = N };
};

template <typename T, std::size_t N, typename U, typename... Args>
struct indexOf<T, N, U, Args...>
{
    enum { value = indexOf<T, N + 1, Args...>::value };
};

template <typename T, std::size_t N>
struct indexOf<T, N>
{
    enum { value = -1 };
    static_assert(value != -1, "the type is not exist");
};

//////////////////////////////////////////////////////////////////////////
template <size_t N, typename FARGS> struct SListElement;

template <typename T, typename... Rest, template<typename... Args> class List>
struct SListElement<0, List<T, Rest...>>
{
    typedef T type;
    typedef List<T, Rest...> SListType;
};

template <size_t N, typename T, typename... Rest, template<typename... Args> class List>
struct SListElement<N, List<T, Rest...>>
    :public SListElement<N - 1, List<Rest...>> {};

template <size_t N, typename... Rest, template<typename... Args> class List>
typename SListElement<N, List<Rest...>>::type& ListGetValueByIndex(const List<Rest...>& stp) {
    typedef typename SListElement<N, List<Rest...>>::SListType type;
    return ((type&)stp).value;
}

template <typename T, typename... Args, template<typename... Values> class List>
T& ListGetValueByType(const List<Args...>& t)
{
    return ListGetValueByIndex<indexOf<T, 0, Args...>::value>(t);
}

//////////////////////////////////////////////////////////////////////////
template<bool...>
struct value_check {};

template<typename T, typename FARGS>
struct ListHasValue;

template<typename T, typename First, typename... Rest, template<typename... Args> class List>
struct ListHasValue<T, List<First, Rest...>>
{
    const static bool value = !std::is_same<
        value_check<false, std::is_same<First, T>::value, std::is_same<T, Rest>::value...>,
        value_check<std::is_same<First, T>::value, std::is_same<T, Rest>::value..., false>
    >::value;
};

template<typename List, typename... T>
struct ListValueCross
{
    const static bool value = !std::is_same<
        value_check<false, ListHasValue<T, List>::value...>,
        value_check<ListHasValue<T, List>::value..., false>
    >::value;
};
