/*
 * Copyright 2009-2017 Srijan Kumar Sharma
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

namespace std
{

	template <class InputIterator, class OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		for (InputIterator it = first; it != last; it++)
			*result++ = *it;
		return result;
	}

	template<class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& val)
	{
		while (first != last) {
			if (*first == val) return first;
			++first;
		}
		return last;
	}

	template<class InputIterator, class UnaryPredicate>
	InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		while (first != last) {
			if (pred(*first)) return first;
			++first;
		}
		return last;
	}

	template<class T>
	const T& min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}
}

#endif /* ALGORITHM_H */

