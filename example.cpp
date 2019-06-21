#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>

#include "example_util.h"

#include "HashMap/HashMap.h"

/*!  Copyright (c) 2017 Grgo Mariani                *
 * ------------------------------------------------ *
 * A simple example to check the HashMap            *
 *                                                  */

using namespace std;

template <typename K, typename V>
void print_stats(CRT::HashMap<K,V>& hashmap){
    cout << endl;
    cout << "   -------------------------------------------"  << endl;
    cout << "  |                                           |" << endl;
    cout << "  |             ------STATS------             |" << endl;
    cout << "  |                                           |" << endl;
    cout << "   -------------------------------------------"  << endl << endl;
    cout << "Average complexity to find element is          : " << hashmap.getSpeed() << "   /MAX: " << hashmap.getDepth() << endl;
    cout << "Average complexity to insert element is        : " << hashmap.putSpeed() << "   /MAX: " << hashmap.getDepth()*(hashmap.getDepth()+1)/2+1 << endl;
    cout << "Depth of HashMap is                            : " << hashmap.getDepth() << endl;
    cout << "Occupied space                                 : " << hashmap.getOccupancy() << endl;
    cout << "                                                /" << hashmap.getMaxOccupancy() << " =" << (float)hashmap.getOccupancy()*100/hashmap.getMaxOccupancy() << "%" << endl;
    cout << "Chance the next element will be in collision   : " << hashmap.chanceToCollideOnNext() << "%" << endl << endl;
    hashmap.print_get_vs_put();
    //hashmap.print_all();
}

void int_example(unsigned int number_of_elements, unsigned int size_of_first_block){
    cout << endl << "random <int,int> example : " << number_of_elements << " " << size_of_first_block << endl;
    CRT::HashMap<int, int> hashmap(size_of_first_block);
    for(unsigned int i=0; i<number_of_elements; i++)
        hashmap.put( rand()*(2<<16)+rand(), rand()*(2<<16)+rand() );  // Concatenates two 16bit rand() int to a 32bit int
    print_stats(hashmap);
}

void string_example(unsigned int number_of_elements, unsigned int size_of_first_block){
    cout << endl << "random <string,string> example : " << number_of_elements << " " << size_of_first_block << endl;
    CRT::HashMap<string, string> hashmap(size_of_first_block);
    for(unsigned int i=0; i<number_of_elements; i++)
        hashmap.put(random_string(20), random_string(20));
    print_stats(hashmap);
}

void usage_example(){
    cout << endl << endl << endl << endl << endl << endl << "STANDARD USAGE EXAMPLE : " << endl;
    CRT::HashMap<string, string> hm(15);
    string value;
    cout<<"Using put()"<<endl;
    hm.put("Key 1", "Value 1");   hm.put("Key 2", "Value 2");   hm.put("Key 3", "Value 3");   hm.put("Key 4", "Value 4");   hm.put("Key 5", "Value 5");   hm.put("Key 6", "Value 6");
    hm.put("Key 7", "Value 7");   hm.put("Key 8", "Value 8");   hm.put("Key 9", "Value 9");   hm.put("Key 10", "Value 10"); hm.put("Key 11", "Value 11"); hm.put("Key 12", "Value 12");
    hm.put("Key 13", "Value 13"); hm.put("Key 14", "Value 14"); hm.put("Key 15", "Value 15"); hm.put("Key 16", "Value 16"); hm.put("Key 17", "Value 17"); hm.put("Key 18", "Value 18");
    hm.put("Key 19", "Value 19"); hm.put("Key 20", "Value 20"); hm.put("Key 21", "Value 21"); hm.put("Key 22", "Value 22"); hm.put("Key 23", "Value 23"); hm.put("Key 24", "Value 24");
    hm.put("Key 25", "Value 25"); hm.put("Key 26", "Value 26"); hm.put("Key 27", "Value 27"); hm.put("Key 28", "Value 28"); hm.put("Key 29", "Value 29"); hm.put("Key 30", "Value 30");
    hm.put("Key 31", "Value 31"); hm.put("Key 32", "Value 32"); hm.put("Key 33", "Value 33"); hm.put("Key 34", "Value 34"); hm.put("Key 35", "Value 35"); hm.put("Key 36", "Value 36");
    print_stats(hm);
    cout << "Using get()" << endl;
    if( hm.get("NonExistingKey", value))
        cout <<"!!! Something went wrong: "<<endl;
    else
        cout << "Key 'NonExistingKey' does not exist" << endl;
    if( hm.get("Key 2", value))
        cout << "The value @ 'Key 2' is: " << value << endl;
    else
        cout << "!!! Something went wrong" << endl;
    cout << "Using remove() @      'Key 3'      &      'Key 7'" << endl;
    hm.remove("Key 3");
    if( hm.get("Key 3", value))
        cout << "!!! Something went wrong " << endl;
    else
        cout << "'Key 3' removed" << endl;
    hm.remove("Key 7");
    if( hm.get("Key 7", value))
        cout << "!!! Something went wrong " << endl;
    else
        cout << "'Key 7' removed";
    print_stats(hm);
    cout << "Using clear()" << endl;
    hm.clear();
    print_stats(hm);
}

int main()
{
    cout << std::fixed;
    int_example(10000000, 3000000);
    string_example(6000000, 3000000);
    usage_example();
    return 0;
}
