#include "HashMap.h"
#include <iostream>         // Used for std::cout
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
    HashMap<KEY, VALUE>::HashMap(){
        nextSize=nextPrime(desiredSize);
        blocks.push_back( new Block<KEY, VALUE>(nextSize));
    }

    template <typename KEY, typename VALUE>
    HashMap<KEY, VALUE>::HashMap(const unsigned int& expectedSizeOfHashMap, bool descending) : desiredSize(expectedSizeOfHashMap), _descending(descending){
        nextSize=nextPrime(expectedSizeOfHashMap-1);
        blocks.push_back( new Block<KEY, VALUE>(nextSize));
    }

    template <typename KEY, typename VALUE>
    HashMap<KEY, VALUE>::~HashMap(){
        for(auto& block : blocks)
            delete block;
    }
    
    template <typename KEY, typename VALUE>
    bool HashMap<KEY, VALUE>::get(const KEY& key, VALUE& value){
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

    template <typename KEY, typename VALUE>
    void HashMap<KEY, VALUE>::put(const KEY& key, const VALUE& value){
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
        for(unsigned int i=1 ; i<keyvector.size(); ++i)
            for(unsigned int j=0; j<=i; ++j){
                blocks[i]->setKey_opt( keyvector[j] );
                if( blocks[i]->isFlagFree_opt() ){
                    blocks[i]->setElement_opt(blocks[j]->getElementPointer_opt());
                    blocks[j]->setPointer_opt(keyvalue);
                    return;
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

    template <typename KEY, typename VALUE>
    void HashMap<KEY, VALUE>::remove(const KEY& key){
        for(auto& block : blocks)
            if(block->deleteElement(key))
                return;
    }

    template <typename KEY, typename VALUE>
    void HashMap<KEY, VALUE>::clear(){
        for(auto& block : blocks)
            delete block;
        blocks.clear();
        blocks.push_back( new Block<KEY, VALUE>(nextPrime(desiredSize)) );
    }

    template <typename KEY, typename VALUE>
    unsigned int HashMap<KEY, VALUE>::getMaxOccupancy(){
        unsigned int result=0;
        for(auto &block : blocks)
            result+=block->getMaxOccupancy();
        return result;
    }

    template <typename KEY, typename VALUE>
    unsigned int HashMap<KEY, VALUE>::getOccupancy(){
        unsigned int result=0;
        for(auto &block : blocks)
            result+=block->getOccupancy();
        return result;
    }
    
    template <typename KEY, typename VALUE>
    unsigned int HashMap<KEY, VALUE>::getEmptySpace(){
        return getMaxOccupancy()-getOccupancy();
    }
    
    template <typename KEY, typename VALUE>
    float HashMap<KEY, VALUE>::getSpeed(){
        unsigned int sum=0, iter=0;
        for(auto& block : blocks)
            sum += (++iter)*block->getOccupancy();
        return (float)sum/(getOccupancy()>1?getOccupancy():1);
    }
    
    template <typename KEY, typename VALUE>
    float HashMap<KEY, VALUE>::putSpeed(){
        float result=0., chance=1.;
        int iter=0;
        for(auto& block : blocks){
            result += chance *( ++iter)*( (float)(block->getMaxOccupancy()-block->getOccupancy() )/block->getMaxOccupancy());
            chance *= (float)block->getOccupancy()/block->getMaxOccupancy();
        }
        for(unsigned int i=1; i<blocks.size(); ++i)
            for(unsigned int j=0; j<i; ++j){
                result += (float) ( blocks.size() + j+1 +(i-1)*i/2 )*( (float)(blocks[i]->getMaxOccupancy()-blocks[i]->getOccupancy() )/ blocks[i]->getMaxOccupancy() )*chance;
                chance *= (float) blocks[i]->getOccupancy()/blocks[i]->getMaxOccupancy();
            }
        return result + chance * ( blocks.size()+1 )*blocks.size()/2;
    }

    //Only for Debug purposes
    /*! template <typename KEY, typename VALUE>
        void HashMap<KEY, VALUE>::print_all(){
        int iter=0;
        for(auto& block : blocks){
            std::cout<<"------BLOCK "<<(++iter)<<"------"<<std:endl;
            block->print_all();
        }
    }*/

    template <typename KEY, typename VALUE>
    void HashMap<KEY, VALUE>::print_get_vs_put(){
        float chance=100.;
        unsigned int iter=0;
        std::cout<<std::endl<<"   get()   vs    put()"<<std::endl;
        for(auto& block : blocks){
            float res1 = 100*((float)block->getOccupancy()/(getOccupancy()>0?getOccupancy():1));
            float res2 = chance * (float)(block->getMaxOccupancy()-block->getOccupancy())/block->getMaxOccupancy();
            std::cout<<(res1<10.?" ":"")<<res1<<"%    "<<(res2<10.?" ":"")<<res2<<"%   in "<<(++iter)<<" tries"<<std::endl;
            chance *= (float)block->getOccupancy()/block->getMaxOccupancy();
        }
        for(size_t i=0; i<blocks.size(); ++i){
            for(size_t j=0; j<i; ++j){
                float res2=(chance * (float)(blocks[i]->getMaxOccupancy()-blocks[i]->getOccupancy())/blocks[i]->getMaxOccupancy());
                std::cout<<"              "<<(res2<10.?" ":"")<<res2<<"%   in "<<(++iter)<<" tries"<<std::endl;
                chance *= (float)blocks[i]->getOccupancy()/blocks[i]->getMaxOccupancy();
            }
        }
        std::cout<<"               "<<chance<<"%   in "<<(++iter)<<" tries"<<std::endl;
    }

    template <typename KEY, typename VALUE>
    unsigned int HashMap<KEY, VALUE>::getDepth(){
        return blocks.size();
    }

    template <typename KEY, typename VALUE>
    double HashMap<KEY, VALUE>::chanceToCollideOnNext(){
        double result=100;
        for(unsigned int i=0; i<blocks.size(); ++i)
            for(unsigned int j=i; j<blocks.size(); ++j)
                result*=(double) blocks[j]->getOccupancy()/blocks[j]->getMaxOccupancy();
        return result;
    }

}

#include <string>
template class CRT::HashMap<int, int>;
template class CRT::HashMap<std::string, std::string>;