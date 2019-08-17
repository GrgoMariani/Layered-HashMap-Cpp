#include <iostream>
#include <unordered_map>

#include "example_util.h"
#include "Wrapper/main_func.h"

using namespace std;

// Just a simple application with which to compare the HashMap
// Only writing to the hashmap is considered here for now

void int_example(unsigned int number_of_elements){
    cout << endl << "random <int,int> example : writing " << number_of_elements << " random <int,int> elements" << endl;
    std::unordered_map<int, int> htmap;
    for(unsigned int i=0; i<number_of_elements; i++)
    {
    	int key = rand()*(2<<16)+rand();
    	int value = rand()*(2<<16)+rand();
    	htmap[key] = value;
    }
}

void string_example(unsigned int number_of_elements){
    cout << endl << "random <string,string> example : writing " << number_of_elements << " random <string,string> elements" <<  endl;
    std::unordered_map<string, string> htmap;
    for(unsigned int i=0; i<number_of_elements; i++)
    	htmap[random_string(20)] = random_string(20);
}

int _main_(int argc, char *argv[])
{
    cout << std::fixed;
    int_example(10000000);
    string_example(6000000);
    return 0;
}
