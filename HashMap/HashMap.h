#ifndef __CRT_HASHMAP_H__
#define __CRT_HASHMAP_H__

#include "Block.h"
#include "Utils.h"
#include <vector>

/*!
 * Copyright (c) 2017 Grgo Mariani
 * Gnu GPL license
 * This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


namespace CRT{

template <typename KEY, typename VALUE>
class HashMap{
public:
    /* Constructor creates one block of recommended size  *
     * If not defined automatically set to 10000          *
     * Destructor clears all memory used                  */
    HashMap();
    HashMap(const unsigned int& expectedSizeOfHashMap, bool descending=false);
    ~HashMap();
    //! We only use 4 methods
    //! bool get(kev, &value) which returns true if key found
    //! void put(key,value)
    //! void remove(key)
    //! void clear() - starts anew
    bool get(const KEY& key, VALUE& value);
    void put(const KEY& key, const VALUE& value);
    void remove(const KEY& key);
    void clear();
    //! Used for debug purposes. You can delete or comment all of these
    unsigned int getMaxOccupancy();
    unsigned int getOccupancy();
    unsigned int getEmptySpace();
    // Get() is slightly faster than put, maximum path is blocks.size()
    float getSpeed();
    float putSpeed();
    //Only for Debug purposes
    /*! void print_all();*/
    void            print_get_vs_put();
    unsigned int    getDepth();
    double          chanceToCollideOnNext();
private:
    unsigned int    desiredSize=10000;
    bool            _descending=false;
    unsigned int    nextSize;
    
    std::vector< Block<KEY, VALUE>* > blocks;
};

}

#endif//__CRT_HASHMAP_H__
