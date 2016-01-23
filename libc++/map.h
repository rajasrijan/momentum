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

