/*
 * Copyright 2009-2021 Srijan Kumar Sharma
 *
 * This file is part of Momentum.
 *
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <functional>
#include <utility>
#include <sys/types.h>

namespace std
{

template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    for (InputIterator it = first; it != last; it++)
    {
        *result = *it;
        result++;
    }
    return result;
}

template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T &val)
{
    while (first != last)
    {
        if (*first == val)
            return first;
        ++first;
    }
    return last;
}

template <class InputIterator, class UnaryPredicate>
InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred)
{
    while (first != last)
    {
        if (pred(*first))
            return first;
        ++first;
    }
    return last;
}

template <class T>
constexpr const T &min(const T &a, const T &b)
{
    return (b > a) ? a : b;
}

template <class T>
const T &max(const T &a, const T &b)
{
    return (b > a) ? b : a;
}

template <class T, class... Args>
const T &max(const T &a, const Args &...b)
{
    const auto &tmp = max(b...);
    return (tmp > a) ? tmp : a;
}

template <class ForwardIt>
ssize_t distance(ForwardIt first, ForwardIt last)
{
    return last - first;
}

template <class ForwardIt>
void advance(ForwardIt &first, size_t step)
{
    first = first + step;
}
template <class ForwardIt, class T, class Compare>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T &value, Compare comp)
{
    ForwardIt it;
    size_t count, step;
    count = std::distance(first, last);

    while (count > 0)
    {
        it = first;
        step = count / 2;
        std::advance(it, step);
        if (!comp(value, *it))
        {
            first = ++it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}

template <class ForwardIt, class T, class Compare>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T &value, Compare comp)
{
    ForwardIt it;
    size_t count, step;
    count = std::distance(first, last);

    while (count > 0)
    {
        it = first;
        step = count / 2;
        std::advance(it, step);
        if (comp(*it, value))
        {
            first = ++it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}

template <class T>
constexpr void swap(T &a, T &b) // constexpr since C++20
{
    T t = b;
    b = a;
    a = t;
}

template <class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b) // constexpr since C++20
{
    swap(*a, *b);
}

template <typename T>
bool less_than(const T &a, const T &b)
{
    return a < b;
}

template <class it, typename pred>
void quick_sort(it low, it high, pred predicate)
{
    if (distance(low, high) > 0)
    {
        /* pi is partitioning index, arr[pi] is now
       at right place */
        auto pi = partition(low, high - 1, predicate);

        quick_sort(low, pi - 1, predicate);  // Before pi
        quick_sort(pi + 1, high, predicate); // After pi
    }
}

template <class it, typename pred>
it partition(it low, it high, pred predicate)
{
    // pivot (Element to be placed at right position)
    auto &pivot = high;

    auto i = (low - 1); // Index of smaller element

    for (auto j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (predicate(*j, *pivot))
        {
            i++; // increment index of smaller element
            std::iter_swap(i, j);
        }
    }
    std::iter_swap(i + 1, high);
    return (i + 1);
}
} // namespace std

#endif /* ALGORITHM_H */
