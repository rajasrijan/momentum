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
#ifndef MAP_H
#define MAP_H

#include <vector>

namespace std
{

    template < class Key, // map::key_type
    class T // map::mapped_type
    >
    class map
    {
        vector<Key> k;
        vector<T> v;
    public:

        T& operator[](Key& _k)
        {
            for (int i = 0; i < k.size(); i++) {
                if (k[i] == _k)
                    return v[i];
            }
            T t(0);
            k.push_back(_k);
            v.push_back(t);
            return v.back();
        }
    };
}


#endif /* MAP_H */

