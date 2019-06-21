#ifndef __CRT_MEMORY_H__
#define __CRT_MEMORY_H__

#include <utility>
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

/* Classes Memory and Flag are here should you want to edit the HashTable to access HDD-SSD     *
   or other memory type other than RAM
 * Edit only methods commented with !                                                           */

    template <typename KEY, typename VALUE>
    class Memory{
    public:
        
        Memory(unsigned int memsize);                                                                   //! Allocate
        ~Memory();                                                                                      //! Deallocate
        
        std::pair<KEY, VALUE>* pointerToPosition(const unsigned int& whatposition);                     //! Returns pointer to element
        
        void                            free(const unsigned int& whatposition);                         //! Deletes element from memory
        const std::pair<KEY, VALUE>&    getElement(const unsigned int& whatposition);                   //! Returns const element reference
        
        void setPointerToPosition(const unsigned int& whatposition, std::pair<KEY,VALUE>* whatpointer); //! Sets Element on whatposition to given pointer
    private:
        const unsigned int          _size;
        std::pair<KEY, VALUE> **    mempair;
    };

    /* Flag class can be left as is to work from RAM directly                                       */

    enum {
        FLAG_EMPTY  = 0,
        FLAG_TAKEN  = 1,
        FLAG_ERASED = 2
    };

    class Flag{
    public:
        
        Flag(unsigned int memsize);                                                                     //! Allocate
        ~Flag();                                                                                        //! Deallocate
        
        unsigned char&  getFlagBlock(const unsigned int& whatblock);                                    //! Read from memory
        void            setFlagBlock(const unsigned int& whatposition, const unsigned char& whatblock); //! Write to memory
        // Save 1/4th of memory by using flags
        void            setFlag(const unsigned int& position, const unsigned char& flag);
        unsigned char   getFlag(const unsigned int& position);

    private:
        const unsigned char     _size;
        unsigned char *         flags;
    };

}

#endif//__CRT_MEMORY_H__
