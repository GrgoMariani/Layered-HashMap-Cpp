/*!  Copyright (c) 2017 Grgo Mariani @ Include Ltd. *
 * ------------------------------------------------ *
 * A simple example to check the HashMap            *
 * To compile :                                     *
               g++ -std=c++11 example.cpp -o example
 *                                                  */

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <stdlib.h>

#include "HashMap/HashMap.h"

using namespace std;
 /* StackOverflow is always great to get these kinds of functions in no time                      *
  * https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c */
std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

template <typename K, typename V>
void print_stats(CRT::HashMap<K,V>& hashmap){
    cout<<endl;
    cout<<"   -------------------------------------------"<<endl;
    cout<<"  |                                           |"<<endl;
    cout<<"  |             ------STATS------             |"<<endl;
    cout<<"  |                                           |"<<endl;
    cout<<"   -------------------------------------------"<<endl<<endl;
    cout<<"Average complexity to find element is          : "<<hashmap.getSpeed()<<endl;
    cout<<"Depth of HashMap is                            : "<<hashmap.getDepth()<<endl;
    cout<<"Occupied space                                 : "<<hashmap.getOccupancy()<<endl;
    cout<<"                                                /"<<hashmap.getMaxOccupancy()<<" ="<<(float)hashmap.getOccupancy()*100/hashmap.getMaxOccupancy()<<"%"<<endl;
    cout<<"Chance the next element will be in collision   : "<<hashmap.chanceToCollideOnNext()<<"%"<<endl<<endl;
    //hashmap.print_all();
}

void int_example(unsigned int number_of_elements, unsigned int size_of_first_block){
    cout<<endl<<"random <int,int> example : "<<number_of_elements<<" "<<size_of_first_block<<endl;
    CRT::HashMap<int, int> hashmap(size_of_first_block);
    for(unsigned int i=0; i<number_of_elements; i++)
        hashmap.put( rand()*(2<<16)+rand(), rand()*(2<<16)+rand() );  // Concatenates two 16bit rand() int to a 32bit int
    print_stats(hashmap);
}

void string_example(unsigned int number_of_elements, unsigned int size_of_first_block){
    cout<<endl<<"random <string,string> example : "<<number_of_elements<<" "<<size_of_first_block<<endl;
    CRT::HashMap<string, string> hashmap(size_of_first_block);
    for(unsigned int i=0; i<number_of_elements; i++)
        hashmap.put(random_string(20),random_string(20));
    print_stats(hashmap);
}

void usage_example(){
    cout<<endl<<" STANDARD USAGE EXAMPLE : "<<endl;
    CRT::HashMap<string, string> hm(15);
    string value;
    cout<<"Using put()"<<endl;
    hm.put("Key 1", "Value 1"); hm.put("Key 2", "Value 2"); hm.put("Key 3", "Value 3"); hm.put("Key 4", "Value 4"); hm.put("Key 5", "Value 5"); hm.put("Key 6", "Value 6");
    hm.put("Key 7", "Value 7"); hm.put("Key 8", "Value 8"); hm.put("Key 9", "Value 9"); hm.put("Key 10", "Value 10"); hm.put("Key 11", "Value 11"); hm.put("Key 12", "Value 12");
    print_stats(hm);
    cout<<"Using get()"<<endl;
    if( hm.get("NonExistingKey",value))
        cout<<"!!! Something went wrong: "<<endl;
    else
        cout<<"Key 'NonExistingKey' does not exist"<<endl;
    if( hm.get("Key 2",value))
        cout<<"The value @ 'Key 2' is: "<<value<<endl;
    else
        cout<<"!!! Something went wrong"<<endl;
    cout<<"Using remove() @ 'Key 3'"<<endl;
    hm.remove("Key 3");
    if( hm.get("Key 3",value))
        cout<<"!!! Something went wrong "<<endl;
    else
        cout<<"'Key 3' removed"<<endl;
    hm.remove("Key 7");
    if( hm.get("Key 7",value))
        cout<<"!!! Something went wrong "<<endl;
    else
        cout<<"'Key 7' removed";
    print_stats(hm);
    cout<<"Using clear()"<<endl;
    hm.clear();
    print_stats(hm);
}

int main()
{
    cout<<std::fixed;
    int_example(10000000, 3000000);
    string_example(6000000, 3000000);
    usage_example();
    return 0;
}
