#ifndef __CRT_BLOCK_H__
#define __CRT_BLOCK_H__

#include "Memory.h"
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
/* Block is a part of memory where the current hash table is stored         */
template <typename KEY, typename VALUE>
class Block{
public:
    
    Block(const unsigned int prime);                                       // Constructor allocates memory && sets flags to zero
    ~Block();

    const std::pair<KEY, VALUE>& getElement(const KEY& key);
    bool deleteElement(const KEY& key);                                     //Returns true if element deleted from this block
        
    const unsigned int getOccupancy();
    const unsigned int getMaxOccupancy();
    const unsigned int recommendedPosition(const std::pair<KEY,VALUE>& pairkeyvalue);
    const unsigned int recommendedPosition(const KEY& key);
    //Only for Debug purposes
#ifdef VERBOSE_DEBUG
    void print_all();
#endif
public:
    //! No need to always count the key position, we can take only the last used one
    //! _position
    void setKey_opt(const KEY& key);
    bool isFlagEmpty_opt();
    bool isFlagFree_opt();
    bool isFlagErased_opt();
    bool isKeyHere_opt(const KEY& key);
    const std::pair<KEY, VALUE>& getElement_opt();
    bool deleteElement_opt(const KEY& key);                                 //Returns true if element deleted from this block
    const KEY& getKeyHere_opt();
    bool setElement_opt(std::pair<KEY, VALUE>* whatkeyvalue);               //Returns true if element set, false if wrong element
        
    bool isKeyHereOrPositionEmpty_opt(const KEY& key);
    std::pair<KEY, VALUE>* getElementPointer_opt();
    void setPointer_opt(std::pair<KEY, VALUE>* whatkeyvalue);
protected:
    /*To change your hashFunction change this function*/
    unsigned int hashFunction(const KEY& key);
private:
    bool isKeyOnPosition(const KEY& key, const unsigned int position);
    bool isIntPositionErased(const unsigned int whatposition);
    bool isIntPositionFree(const unsigned int whatposition);
    void deleteIntElement(const unsigned int whatposition);
    const std::pair<KEY, VALUE>& getIntElement(const unsigned int whatposition);
    void setElementPointer(const unsigned int whatposition, std::pair<KEY, VALUE>* whatkeyvalue);
    void free_memory();                                                     //Implemented for faster memory deallocation, you can also get one element at a time and delete it
private:
    const unsigned int  _prime;
    unsigned int        occupancy;

    unsigned int        _position;

    Memory<KEY,VALUE> * memory;
    Flag *              flags;
};

}

#endif//__CRT_BLOCK_H__
