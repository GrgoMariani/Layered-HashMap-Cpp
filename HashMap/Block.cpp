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
#include "Block.h"
#include <iostream>     // used for hash

namespace CRT{

    template <typename KEY, typename VALUE>
    Block<KEY, VALUE>::Block(const unsigned int& prime) : _prime(prime), occupancy(0), _position(0), memory(nullptr), flags(nullptr) {
        memory = new Memory<KEY,VALUE>(prime);
        flags = new Flag(prime);
    }
    
    template <typename KEY, typename VALUE>
    Block<KEY, VALUE>::~Block(){
        free_memory();
        delete memory;
        delete flags;
    }

    template <typename KEY, typename VALUE>
    const std::pair<KEY, VALUE>& Block<KEY, VALUE>::getElement(const KEY& key){
        return getIntElement(recommendedPosition(key));
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::deleteElement(const KEY& key){                                     //Returns true if element deleted from this block
        unsigned int position = recommendedPosition(key);
        if( !isIntPositionFree(position) && isKeyOnPosition(key, position) ){
            deleteIntElement(position);
            return true;
        }
        return false;
    }

    template <typename KEY, typename VALUE>
    const unsigned int& Block<KEY, VALUE>::getOccupancy(){
        return occupancy;
    }

    template <typename KEY, typename VALUE>
    const unsigned int& Block<KEY, VALUE>::getMaxOccupancy(){
        return _prime;
    }

    template <typename KEY, typename VALUE>
    const unsigned int Block<KEY, VALUE>::recommendedPosition(const std::pair<KEY,VALUE>& pairkeyvalue){
        return hashFunction(pairkeyvalue.first);
    }

    template <typename KEY, typename VALUE>
    const unsigned int Block<KEY, VALUE>::recommendedPosition(const KEY& key){
        return hashFunction(key);
    }

    //Only for Debug purposes
    /*! template <typename KEY, typename VALUE>
        void Block<KEY, VALUE>::print_all(){
        for(unsigned int i=0; i<_prime; i++){
            std::cout<<i<<"  Flag: "<<(flags->getFlag(i)==FLAG_EMPTY?" EMPTY ":(flags->getFlag(i)==FLAG_TAKEN?" TAKEN ":"DELETED"));
            if( flags->getFlag(i) == FLAG_TAKEN )
                std::cout<<"       KEY: "<<getIntElement(i).first<<" VALUE: "<<getIntElement(i).second<<std::endl;
            else std::cout<<std::endl;
        }
    }*/

    template <typename KEY, typename VALUE>
    void Block<KEY, VALUE>::setKey_opt(const KEY& key){
        _position=recommendedPosition(key);
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::isFlagEmpty_opt(){
        if(flags->getFlag(_position)==FLAG_EMPTY)
            return true;
        return false;
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::isFlagFree_opt(){
        return isIntPositionFree( _position );
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::isFlagErased_opt(){
        return isIntPositionErased(_position);
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::isKeyHere_opt(const KEY& key){
        return isKeyOnPosition(key, _position);
    }

    template <typename KEY, typename VALUE>
    const std::pair<KEY, VALUE>& Block<KEY, VALUE>::getElement_opt(){
        return getIntElement(_position);
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::deleteElement_opt(const KEY& key){                                     //Returns true if element deleted from this block
        if( !isIntPositionFree(_position) && isKeyOnPosition(key, _position) ){
            deleteIntElement(_position);
            return true;
        }
        return false;
    }

    template <typename KEY, typename VALUE>
    const KEY& Block<KEY, VALUE>::getKeyHere_opt(){
        return getElement_opt().first;
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::setElement_opt(std::pair<KEY, VALUE>* whatkeyvalue){             //Returns true if element set, false if wrong element
        if(!isIntPositionFree(_position) ){
            if(!isKeyOnPosition(whatkeyvalue->first, _position))
                return false;
            else deleteIntElement(_position);
        }
        setElementPointer(_position, whatkeyvalue);
        return true;
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::isKeyHereOrPositionEmpty_opt(const KEY& key){
        if(isKeyOnPosition(key, _position)||isIntPositionFree(_position))
            return true;
        return false;
    }

    template <typename KEY, typename VALUE>
    std::pair<KEY, VALUE>* Block<KEY, VALUE>::getElementPointer_opt(){
        return memory->pointerToPosition(_position);
    }

    template <typename KEY, typename VALUE>
    void Block<KEY, VALUE>::setPointer_opt(std::pair<KEY, VALUE>* whatkeyvalue){
        memory->setPointerToPosition(_position,whatkeyvalue);
    }

    /*To change your hashFunction change this function*/
    template <typename KEY, typename VALUE>
    unsigned int Block<KEY, VALUE>::hashFunction(const KEY& key){
        return std::hash<KEY>{}(key)%_prime;
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::isKeyOnPosition(const KEY& key, const unsigned int& position){
        if( !isIntPositionFree(position) && getIntElement(position).first==key )
            return true;
        return false;
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::isIntPositionErased(const unsigned int& whatposition){
        if(flags->getFlag(whatposition)==FLAG_ERASED)
            return true;
        return false;
    }

    template <typename KEY, typename VALUE>
    bool Block<KEY, VALUE>::isIntPositionFree(const unsigned int& whatposition){
        if(flags->getFlag(whatposition)==FLAG_TAKEN)
            return false;
        return true;
    }

    template <typename KEY, typename VALUE>
    void Block<KEY, VALUE>::deleteIntElement(const unsigned int& whatposition){
        memory->free(whatposition);
        occupancy--;
        flags->setFlag(whatposition, FLAG_ERASED);
    }

    template <typename KEY, typename VALUE>
    const std::pair<KEY, VALUE>& Block<KEY, VALUE>::getIntElement(const unsigned int& whatposition){
        return memory->getElement(whatposition);
    }

    template <typename KEY, typename VALUE>
    void Block<KEY, VALUE>::setElementPointer(const unsigned int& whatposition, std::pair<KEY, VALUE>* whatkeyvalue){
        memory->setPointerToPosition(whatposition, whatkeyvalue);
        occupancy++;
        flags->setFlag(whatposition, FLAG_TAKEN);
    }

    template <typename KEY, typename VALUE>
    void Block<KEY, VALUE>::free_memory(){
        //Implemented for faster memory deallocation, you can also get one element at a time and delete it
        for(register unsigned int i=0; occupancy>0; ++i){
            register unsigned char regchar = flags->getFlagBlock(i) & 0b01010101;
            if(regchar){
                if( regchar & 0b00000001 )
                    deleteIntElement( i<<2 );
                if( regchar & 0b00000100 )
                    deleteIntElement( (i<<2)|0b01 );
                if( regchar & 0b00010000 )
                    deleteIntElement( (i<<2)|0b10 );
                if( regchar & 0b01000000 )
                    deleteIntElement( (i<<2)|0b11 );
            }
        }
    }

}


#include <string>
template class CRT::Block<int, int>;
template class CRT::Block<std::string, std::string>;
