#pragma once
/*!
 * Copyright (c) 2017 Grgo Mariani @ Include Ltd.
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

/* Classes Memory and Flag are here should you want to edit the HashTable to access HDD-SSD     *
   or other memory type other than RAM
 * Edit only methods commented with !                                                           */

template <typename KEY, typename VALUE>
class Memory{
public:
    //! Allocate
    Memory(unsigned int memsize):_size(memsize){
        mempair = new std::pair<KEY, VALUE>*[memsize];
    }
    //! Deallocate
    ~Memory(){
        delete[] mempair;
    }
    //! Returns pointer to element
    std::pair<KEY, VALUE>* pointerToPosition(const unsigned int& whatposition){
         return mempair[whatposition];
    }
    //! Deletes element from memory
    void free(const unsigned int& whatposition){
        delete mempair[whatposition];
    }
    //! Returns const element reference
    const std::pair<KEY, VALUE>& getElement(const unsigned int& whatposition){
        return *(mempair[whatposition]);
    }
    //! Sets Element on whatposition to given pointer
    void setPointerToPosition(const unsigned int& whatposition, std::pair<KEY,VALUE>* whatpointer){
        mempair[whatposition]=whatpointer;
    }
private:
    const unsigned int _size;
    std::pair<KEY, VALUE> ** mempair;
};

/* Flag class can be left as is to work from RAM directly                                       */

enum{ FLAG_EMPTY=0, FLAG_TAKEN=1, FLAG_ERASED=2 };

class Flag{
public:
    //! Allocate
    Flag(unsigned int memsize):_size(memsize){
        flags = new unsigned char[(memsize>>2)+1](); //Automatically sets all to 0, if you should change this be sure to do the same
    }
    //! Deallocate
    ~Flag(){
        delete[] flags;
    }
    //! Read from memory
    unsigned char& getFlagBlock(const unsigned int& whatblock){
        return flags[whatblock];
    }
    //! Write to memory
    void setFlagBlock(const unsigned int& whatposition, const unsigned char& whatblock){
        flags[whatposition] = whatblock;
    }
    // Save 1/4th of memory this way
    void setFlag(const unsigned int& position, const unsigned char& flag){
        unsigned char flagblock = getFlagBlock(position>>2);
        flagblock &=  ~( 0b11<<( (position&0b11)<<1 ) ) ;
        flagblock |= flag<<( (position&0b11)<<1 );
        setFlagBlock(position>>2, flagblock);
    }
    unsigned char getFlag(const unsigned int& position){
        return ( getFlagBlock(position>>2) >>( (position&0b11)<<1) ) & 0b11;
    }
private:
    const unsigned char _size;
    unsigned char * flags;
};

}
