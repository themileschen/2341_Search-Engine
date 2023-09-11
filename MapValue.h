// Class Owner: Miles

#ifndef MAPVALUE_H
#define MAPVALUE_H

#include <string>
#include <vector>

using namespace std;

/**
 * MapValue acts as a key-value pair where ObjectA is the key and Object B is the value.
*/
template <typename ObjectA, typename ObjectB>
class MapValue
{
    public:

        MapValue() {}
        MapValue(ObjectA a, ObjectB b)
        {
            key = a;
            values = b;
        }

        const ObjectA getKey() const 
        {
            return key;
        }

        const ObjectB getValues() const
        {
            return values;
        }

        //Sorts the key-value pair based on the value of the keys.
        bool operator< (const MapValue<ObjectA, ObjectB>& otherMapValue) const {
            return key < otherMapValue.key;
        }

    private:

        ObjectA key;
        ObjectB values;

};

#endif