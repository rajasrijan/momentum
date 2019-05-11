/*
 * Copyright 2009-2019 Srijan Kumar Sharma
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
#include <utility>

namespace std
{
template <typename Key, // map::key_type
          typename T    // map::mapped_type
          >
class map
{
  public:
    class iterator
    {
        friend class map;
        typename vector<pair<Key, T>>::iterator mapIt;

      public:
        iterator() {}
        ~iterator() {}
        bool operator==(const iterator &it) const
        {
            return (mapIt == it.mapIt);
        }
        iterator(typename vector<pair<Key, T>>::iterator It) : mapIt(It) {}
        pair<Key, T> *operator->()
        {
            return mapIt.operator->();
        }
    };

  private:
    vector<pair<Key, T>> k;

  public:
    map() : k() {}
    T &operator[](const Key &_k)
    {
        auto found_it = lower_bound(k.begin(), k.end(), _k, [](const pair<Key, T> &it, const Key &value) { return value > it.first; });
        if (found_it != k.end() && found_it->first == _k)
            return found_it->second;
        k.insert(found_it, make_pair<Key, T>(_k, T()));
        return found_it->second;
    }

    iterator find(const Key &_k)
    {
        auto found_it = lower_bound(k.begin(), k.end(), _k, [](const pair<Key, T> &it, const Key &value) { return value > it.first; });
        if (found_it->first == _k)
            return iterator(found_it);
        return end();
    }

    iterator end()
    {
        return iterator(k.end());
    }

    void erase(const Key &_k)
    {
        auto where_it = find(_k);
        if (where_it == end())
            return;
        k.erase(where_it.mapIt);
    }
};
} // namespace std
#endif /* MAP_H */
