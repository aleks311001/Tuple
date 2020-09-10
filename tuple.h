//
// Created by aleks311001 on 22.05.2020.
//

#ifndef TUPLE_TUPLE_H
#define TUPLE_TUPLE_H

#include "iostream"

template<size_t i, typename First, typename ...Tail>
struct getElementType {
    using type = typename getElementType<i - 1, Tail...>::type;
};
template<typename First, typename ...Tail>
struct getElementType<0, First, Tail...> {
    using type = First;
};

template<typename T, typename First, typename ...Args>
struct getTypeInd {
    static const size_t index = getTypeInd<T, Args...>::index + 1;
};
template<typename First, typename ...Args>
struct getTypeInd<First, First, Args...> {
    static const size_t index = 0;
};

template<typename ...Args>
class Tuple;

template<typename First, typename ...Tail>
class Tuple<First, Tail...> {
public:
    Tuple();

    Tuple(const First& first, const Tail&... tail);
    Tuple(First&& first, Tail&&... tail);

    Tuple(const Tuple& other);
    Tuple(Tuple&& other);

    Tuple& operator=(const Tuple& other);
    Tuple& operator=(Tuple&& other);

    void swap(Tuple& other);

    template<size_t i, typename ...Args>
    friend typename getElementType<i, Args...>::type& get(Tuple<Args...>& tuple);
    template<size_t i, typename ...Args>
    friend typename getElementType<i, Args...>::type const& get(const Tuple<Args...>& tuple);
    template<size_t i, typename ...Args>
    friend typename getElementType<i, Args...>::type&& get(Tuple<Args...>&& tuple);
    template<size_t i, typename ...Args>
    friend typename getElementType<i, Args...>::type const&& get(const Tuple<Args...>&& tuple);

    template<typename ...ArgsLeft, typename ...ArgsRight>
    friend Tuple<ArgsLeft..., ArgsRight...> concatTwoTuples(const Tuple<ArgsLeft...>& left, const Tuple<ArgsRight...>& right);
    template<typename Left, typename ...ArgsRight>
    friend Tuple<Left, ArgsRight...> concatTwoTuples(const Tuple<Left>& left, const Tuple<ArgsRight...>& right);

    template<typename ...ArgsLeft, typename ...ArgsRight>
    friend bool operator<(const Tuple<ArgsLeft...>& left, const Tuple<ArgsRight...>& right);
    template<typename Left, typename Right>
    friend bool operator<(const Tuple<Left>& left, const Tuple<Right>& right);
private:
    First first_;
    Tuple<Tail...> tail_;

    Tuple(const First& first, const Tuple<Tail...>& tail);
    Tuple(First&& first, Tuple<Tail...>&& tail);
};

template<typename First, typename... Tail>
Tuple<First, Tail...>::Tuple(): first_(), tail_() {}

template<typename First, typename... Tail>
Tuple<First, Tail...>::Tuple(const First& first, const Tail& ... tail): first_(first), tail_(tail...) {}

template<typename First, typename... Tail>
Tuple<First, Tail...>::Tuple(First&& first, Tail&& ... tail)
        : first_(std::move(first)),
          tail_(std::move(tail)...) {}

template<typename First, typename... Tail>
Tuple<First, Tail...>::Tuple(const Tuple& other)
        : first_(other.first_),
          tail_(other.tail_) {}

template<typename First, typename... Tail>
Tuple<First, Tail...>::Tuple(Tuple&& other)
        : first_(std::move(other.first_)),
          tail_(std::move(other.tail_)) {}

template<typename First, typename... Tail>
Tuple<First, Tail...>::Tuple(const First& first, const Tuple<Tail...>& tail)
        : first_(first),
          tail_(tail) {}

template<typename First, typename... Tail>
Tuple<First, Tail...>::Tuple(First&& first, Tuple<Tail...>&& tail)
        : first_(std::move(first)),
          tail_(std::move(tail)) {}

template<typename First, typename... Tail>
Tuple<First, Tail...>& Tuple<First, Tail...>::operator=(const Tuple& other) {
    Tuple tmp = other;
    this->swap(tmp);

    return *this;
}

template<typename First, typename... Tail>
Tuple<First, Tail...>& Tuple<First, Tail...>::operator=(Tuple&& other) {
    Tuple tmp = std::move(other);
    this->swap(tmp);

    return *this;
}

template<typename First, typename... Tail>
void Tuple<First, Tail...>::swap(Tuple& other) {
    std::swap(first_, other.first_);
    tail_.swap(other.tail_);
}

template<>
class Tuple<> {
public:
    void swap(Tuple&) {}
};

template<typename ...Args>
Tuple<typename std::decay<Args>::type...> makeTuple(Args&& ...args) {
    return Tuple<typename std::decay<Args>::type...>(std::forward<Args>(args)...);
}


template<size_t i, typename ...Args>
typename getElementType<i, Args...>::type& get(Tuple<Args...>& tuple) {
    if constexpr (i > 0) {
        return get<i - 1>(tuple.tail_);
    } else {
        return tuple.first_;
    }
}
template<size_t i, typename ...Args>
typename getElementType<i, Args...>::type const& get(const Tuple<Args...>& tuple) {
    if constexpr (i > 0) {
        return get<i - 1>(tuple.tail_);
    } else {
        return tuple.first_;
    }
}

template<size_t i, typename ...Args>
typename getElementType<i, Args...>::type&& get(Tuple<Args...>&& tuple) {
    if constexpr (i > 0) {
        return get<i - 1>(std::move(tuple.tail_));
    } else {
        return std::move(tuple.first_);
    }
}
template<size_t i, typename ...Args>
typename getElementType<i, Args...>::type const&& get(const Tuple<Args...>&& tuple) {
    if constexpr (i > 0) {
        return get<i - 1>(std::move(tuple.tail_));
    } else {
        return std::move(tuple.first_);
    }
}

template<typename T, typename ...Args>
T get(Tuple<Args...>& tuple) {
    return get<getTypeInd<std::remove_reference_t<T>, Args...>::index>(tuple);
}
template<typename T, typename ...Args>
T get(Tuple<Args...>&& tuple) {
    return get<getTypeInd<std::remove_reference_t<T>, Args...>::index>(std::move(tuple));
}



template<typename ...ArgsLeft, typename ...ArgsRight>
Tuple<ArgsLeft..., ArgsRight...> concatTwoTuples(const Tuple<ArgsLeft...>& left, const Tuple<ArgsRight...>& right) {
    return Tuple<ArgsLeft..., ArgsRight...>(left.first_, concatTwoTuples(left.tail_, right));
}
template<typename Left, typename ...ArgsRight>
Tuple<Left, ArgsRight...> concatTwoTuples(const Tuple<Left>& left, const Tuple<ArgsRight...>& right) {
    return Tuple<Left, ArgsRight...>(left.first_, right);
}

template<typename FirstTuple, typename ...Tuples>
auto tupleCat(FirstTuple&& firstTuple, Tuples&& ...tuples) {
    return concatTwoTuples(std::forward<FirstTuple>(firstTuple), tupleCat(std::forward<Tuples>(tuples)...));
}
template<typename FirstTuple>
FirstTuple tupleCat(FirstTuple&& firstTuple) {
    return firstTuple;
}



template<typename ...ArgsLeft, typename ...ArgsRight>
bool operator<(const Tuple<ArgsLeft...>& left, const Tuple<ArgsRight...>& right) {
    return (left.first_ < right.first_) || ((left.first_ == right.first_) && (left.tail_ < right.tail_));
}
template<typename Left, typename Right>
bool operator<(const Tuple<Left>& left, const Tuple<Right>& right) {
    return left.first_ < right.first_;
}

template<typename ...ArgsLeft, typename ...ArgsRight>
bool operator>(const Tuple<ArgsLeft...>& left, const Tuple<ArgsRight...>& right) {
    return right < left;
}
template<typename ...ArgsLeft, typename ...ArgsRight>
bool operator<=(const Tuple<ArgsLeft...>& left, const Tuple<ArgsRight...>& right) {
    return !(left > right);
}
template<typename ...ArgsLeft, typename ...ArgsRight>
bool operator>=(const Tuple<ArgsLeft...>& left, const Tuple<ArgsRight...>& right) {
    return !(left < right);
}
template<typename ...ArgsLeft, typename ...ArgsRight>
bool operator==(const Tuple<ArgsLeft...>& left, const Tuple<ArgsRight...>& right) {
    return (left <= right) && (left >= right);
}
template<typename ...ArgsLeft, typename ...ArgsRight>
bool operator!=(const Tuple<ArgsLeft...>& left, const Tuple<ArgsRight...>& right) {
    return !(left == right);
}


#endif //TUPLE_TUPLE_H
