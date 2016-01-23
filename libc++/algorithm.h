#ifndef ALGORITHM_H
#define ALGORITHM_H

namespace std
{

template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    for (InputIterator it = first; it != last; it++)
        *result++ = *it;
    return result;
}
}

#endif /* ALGORITHM_H */

