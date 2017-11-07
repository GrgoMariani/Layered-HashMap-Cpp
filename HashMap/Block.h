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

enum{ FLAG_EMPTY=0, FLAG_TAKEN=1, FLAG_ERASED=2 };
/*
 * Block is a part of memory where the current hash table is stored
 */
template <typename KEY, typename VALUE>
class Block{
public:
    //Constructor allocates memory && sets flags to zero
    Block(const unsigned int& prime) : _prime(prime){
        mempair = new std::pair<KEY, VALUE>*[prime];
        flags   = new unsigned char[(prime>>2)+1]();                           //All flags set to FLAG_EMPTY
        blockId = (++Block<KEY,VALUE>::n_blocks);
    }
    ~Block(){
        Block<KEY,VALUE>::n_blocks--;
        free_memory();
    }

    const std::pair<KEY, VALUE>& getElement(const KEY& key){
        return getIntElement(recommendedPosition(key));
    }
    bool deleteElement(const KEY& key){                                     //Returns true if element deleted from this block
        unsigned int position = recommendedPosition(key);
        if(isKeyOnPosition(key, position) && !isIntPositionFree(position)){
            deleteIntElement(position);
            return true;
        }
        return false;
    }
    const unsigned int& getOccupancy(){
        return occupancy;
    }
    const unsigned int& getMaxOccupancy(){
        return _prime;
    }
    const unsigned int recommendedPosition(const std::pair<KEY,VALUE>& pairkeyvalue){
        return hashFunction(pairkeyvalue.first);
    }
    const unsigned int recommendedPosition(const KEY& key){
        return hashFunction(key);
    }
    const unsigned int& getBlockId(){
        return blockId;
    }
    //Only for Debug purposes
    /*! void print_all(){
        std::cout<<std::endl<<"------BLOCK "<<blockId<<"------"<<std::endl;
        for(unsigned int i=0; i<_prime; i++){
            std::cout<<i<<"  Flag: "<<(getFlag(i)==FLAG_EMPTY?" EMPTY ":(getFlag(i)==FLAG_TAKEN?" TAKEN ":"DELETED"));
            if( getFlag(i) == FLAG_TAKEN )
                std::cout<<"       KEY: "<<getIntElement(i)->first<<" VALUE: "<<getIntElement(i)->second<<std::endl;
                std::cout<<"       KEY: "<<getIntElement(i)->first<<" VALUE: "<<getIntElement(i)->second<<std::endl;
            else std::cout<<std::endl;
        }
    }*/
public:
    //! Added at last minute, no need to always count the key position, we can take only the last used one
    //!
    void setKey_opt(const KEY& key){
        _position=recommendedPosition(key);
    }
    bool isFlagEmpty_opt(){
        if(getFlag(_position)==FLAG_EMPTY)
            return true;
        return false;
    }
    bool isFlagFree_opt(){
        return isIntPositionFree( _position );
    }
    bool isFlagErased_opt(){
        return isIntPositionErased(_position);
    }
    bool isKeyHere_opt(const KEY& key){
        return isKeyOnPosition(key, _position);
    }
    const std::pair<KEY, VALUE>& getElement_opt(){
        return getIntElement(_position);
    }
    bool deleteElement_opt(const KEY& key){                                     //Returns true if element deleted from this block
        if(isKeyOnPosition(key, _position) && !isIntPositionFree(_position)){
            deleteIntElement(_position);
            return true;
        }
        return false;
    }
    const KEY& getKeyHere_opt(){
        return getElement_opt().first;
    }
    bool setElement_opt(std::pair<KEY, VALUE>* whatkeyvalue){             //Returns true if element set, false if wrong element
        if(!isIntPositionFree(_position) ){
            if(!isKeyOnPosition(whatkeyvalue->first, _position))
                return false;
            else deleteIntElement(_position);
        }
        setElementPointer(_position, whatkeyvalue);
        return true;
    }
    bool isKeyHereOrPositionEmpty_opt(const KEY& key){
        if(isKeyOnPosition(key, _position)||isIntPositionFree(_position))
            return true;
        return false;
    }
    std::pair<KEY, VALUE>* getElementPointer_opt(){ return mempair[_position]; }
    bool setPointer_opt(std::pair<KEY, VALUE>* whatkeyvalue){             //Returns true if element set, false if wrong element
        mempair[_position]=whatkeyvalue;
        return true;
    }
protected:
    /*To change your hashFunction change this function*/
    unsigned int hashFunction(const KEY& key){
        return std::hash<KEY>{}(key)%_prime;
    }
private:
    bool isKeyOnPosition(const KEY& key, const unsigned int& position){
        if( !isIntPositionFree(position) && getIntElement(position).first==key )
            return true;
        return false;
    }
    bool isIntPositionErased(const unsigned int& whatposition){
        if(getFlag(whatposition)==FLAG_ERASED)
            return true;
        return false;
    }
    bool isIntPositionFree(const unsigned int& whatposition){
        if(getFlag(whatposition)==FLAG_TAKEN)
            return false;
        return true;
    }
    void deleteIntElement(const unsigned int& whatposition){
        delete mempair[whatposition];
        occupancy--;
        setFlag(whatposition, FLAG_ERASED);
    }
    const std::pair<KEY, VALUE>& getIntElement(const unsigned int& whatposition){
        return *(mempair[whatposition]);
    }
    void setElementPointer(const unsigned int& whatposition, std::pair<KEY, VALUE>* whatkeyvalue){
        mempair[whatposition]=whatkeyvalue;
        occupancy++;
        setFlag(whatposition, FLAG_TAKEN);
    }
    void free_memory(){
        for(register unsigned int i=0; occupancy>0; ++i){
            register unsigned char regchar = flags[i] & 0b01010101;
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
        delete[] mempair;
        delete[] flags;
    }
    //! Save a lot of memory this way
    unsigned char getFlag(const unsigned int& position){
        return ( flags[position>>2] >>( (position&0b11)<<1) ) & 0b11;
    }
    void setFlag(const unsigned int& position, const unsigned char& flag){
        flags[position>>2] &=  ~( 0b11<<( (position&0b11)<<1 ) ) ;
        flags[position>>2] |= flag<<( (position&0b11)<<1 );
    }
private:
    const unsigned int _prime;
    std::pair<KEY, VALUE> ** mempair;
    unsigned char * flags;
    unsigned int occupancy=0;

    static unsigned int n_blocks;
    unsigned int blockId;
    unsigned int _position=0;
};

template <typename KEY, typename VALUE>
unsigned int Block<KEY,VALUE>::n_blocks = 0;

}
