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
#include "Block.h"
#include "Utils.h"
#include <vector>

namespace CRT{

template <typename KEY, typename VALUE>
class HashMap{
public:
    /* Constructor creates one block of recommended size  *
     * If not defined automatically set to 10000          *
     * Destructor clears all memory used                  */
     HashMap(){
        nextSize=nextPrime(desiredSize);
        blocks.push_back( new Block<KEY, VALUE>(nextSize));
    }
    HashMap(const unsigned int& expectedSizeOfHashMap, bool descending=false) : desiredSize(expectedSizeOfHashMap),_descending(descending){
        nextSize=nextPrime(expectedSizeOfHashMap-1);
        blocks.push_back( new Block<KEY, VALUE>(nextSize));
    }
    ~HashMap(){
        for(auto& block : blocks)
            delete block;
    }
    //! We only use 4 methods
    //! bool get(kev, &value) which returns true if key found
    //! void put(key,value)
    //! void remove(key)
    //! void clear() - starts anew
    bool get(const KEY& key, VALUE& value){
        register bool dooptimization=false;
        register unsigned int remember=0;
        for(unsigned int i=0; i<blocks.size(); i++){
            blocks[i]->setKey_opt(key);
            if( blocks[i]->isKeyHere_opt(key) ){
                value=blocks[i]->getElement_opt().second;
                if( dooptimization ){
                    blocks[i]->deleteElement_opt(key);               //We know the element is in this array so delete it now and put again
                    blocks[remember]->setElement_opt(new std::pair<KEY,VALUE>(key,value));
                    value=blocks[remember]->getElement_opt().second;
                }
                return true;
            }
			if( !dooptimization && blocks[i]->isFlagErased_opt() ) dooptimization=true, remember=i;
			if( blocks[i]->isFlagEmpty_opt() ) return false;      //The element cannot exist if it is found on flag empty
        }
        return false;
    }

    void put(const KEY& key, const VALUE& value){
        std::pair<KEY, VALUE>* keyvalue=new std::pair<KEY, VALUE>(key,value);
        register bool dooptimization=false;
        register unsigned int remember=0;
        std::vector<KEY> keyvector;
        for(unsigned int i=0; i<blocks.size(); i++){
            blocks[i]->setKey_opt(key);
            if( !blocks[i]->isFlagFree_opt() )
                keyvector.push_back( blocks[i]->getKeyHere_opt() );
            if( blocks[i]->isKeyHereOrPositionEmpty_opt(key) ){
                if(dooptimization){
                    blocks[i]->deleteElement_opt(key);
                    blocks[remember]->setElement_opt(keyvalue);
                }
                else blocks[i]->setElement_opt(keyvalue);
                return;
            }
			if( !dooptimization && blocks[i]->isFlagErased_opt() ) dooptimization=true, remember=i;
        }
        if(dooptimization){
            blocks[remember]->setElement_opt(keyvalue);
            return;
        }
        //! A touch of optimization
        for(unsigned int i=1 ; i<keyvector.size(); ++i){
            for(unsigned int j=0; j<=i; ++j){
                blocks[i]->setKey_opt( keyvector[j] );
                if( blocks[i]->isFlagFree_opt() ){
                    blocks[i]->setElement_opt(blocks[j]->getElementPointer_opt());
                    blocks[j]->setPointer_opt(keyvalue);
                    return;
                }
            }
        }
        //! This is the part of the code where we insert the new Block,
        //! We also could do some optimizations should we want to reduce memory allocation even more
        //! This part could vary from implementation to implementation
        //! We could also pass a vector<size> as reference to constructor so we know what values the first few blocks should be
        //! We also could use PreviousPrime variation instead of NextPrime w/ _descending=true
        if(_descending)
            nextSize=previousPrime(nextSize);
        else
            nextSize=nextPrime(nextSize);
        blocks.push_back( new Block<KEY, VALUE>(nextSize) );
        blocks[blocks.size()-1]->setKey_opt(key);
        blocks[blocks.size()-1]->setElement_opt(keyvalue);
    }
    void remove(const KEY& key){
        for(auto& block : blocks)
            if(block->deleteElement(key))
                return;
    }
    void clear(){
        for(auto& block : blocks)
            delete block;
        blocks.clear();
        blocks.push_back( new Block<KEY, VALUE>(nextPrime(desiredSize)) );
    }
    //Used for debug purposes
    unsigned int getMaxOccupancy(){
        unsigned int result=0;
        for(auto &block : blocks)
            result+=block->getMaxOccupancy();
        return result;
    }
    unsigned int getOccupancy(){
        unsigned int result=0;
        for(auto &block : blocks)
            result+=block->getOccupancy();
        return result;
    }
    unsigned int getEmptySpace(){
        return getMaxOccupancy()-getOccupancy();
    }
    float getSpeed(){
        unsigned int sum=0;
        for(auto& block : blocks)
            sum += block->getBlockId()*block->getOccupancy();
        return (float)sum/(getOccupancy()>1?getOccupancy():1);
    }
    //Only for Debug purposes
    /*! void print_all(){
        for(auto& block : blocks){
            block->print_all();
        }
    }*/
    unsigned int getDepth(){
        return blocks.size();
    }
    double chanceToCollideOnNext(){
        double result=100;
        for(unsigned int i=0; i<blocks.size(); ++i)
            for(unsigned int j=i; j<blocks.size(); ++j)
                result*=(double) blocks[j]->getOccupancy()/blocks[j]->getMaxOccupancy();
        return result;
    }
private:
    unsigned int desiredSize=10000;
    bool _descending=false;
    unsigned int nextSize;
    std::vector< Block<KEY, VALUE>* > blocks;
};

}
