## Chinese Remainder Theorem based HashMap (Layered Hash Table)
A C++11 header for HashMap structure

### Info
A project of mine made some time ago. While tinkering about HashMaps I realized I could do a really simple auto-dynamically-allocating one.
By using the properties of Chinese Remainder Theorem we can achieve really quick read and writes, as well as keeping the memory allocation low as it can be.
Tested on ridiculously big number of objects. Could be used to find 1 in 10^9 (1000000000) objects by using 16GB of space - average search depth is 2.26 reads, maximum search depth is 7 reads - so yeah, really really fast.
It can be set to go even faster (avg 1.4 max 5) but requires twice the memory usage.
More in the included .PDF file attached

### Install
Simply copy the HashMap directory to your project and include it in your file
```
 #include "HashMap/HashMap.h"
```
When compiling don't forget to add -std=c++11

### Usage
The namespace used is CRT (as in Chinese Remainder Theorem). You can check sample usage in example.cpp file.
To create an empty HashMap use
```
 CRT::HashMap<string, int> hash;   //! string to int mapping
```
This creates an empty HashMap of size 10000 elements, that uses ascending memory allocation.
Should you want to create a bigger default HashMap or set the memory allocation as descending you can use the constructor. I would recommend setting the default HashMap to 0.7*Probable_number_of_your_objects
```
 CRT::HashMap<string, float> hash(10000000, true); 
 // Reserves 10 000 000 objects in the memory and sets the dynamic memory allocation to true
 // Key=string , Value=float
```

### Example
To compile the included example
```
 g++ -std=c++11 example.cpp -o example
```
### Methods

#### bool get(key, value)

![get-algo](https://github.com/GrgoMariani/Layered-HashMap-Cpp/tree/master/GIFS/get.gif)

```
 if( hashmap.get("requested key", value)
 {
	result=value;
	//If this is true then the Value is stored in value (nonreferenced). //Copy constructor on value needed
 }
 else
 {
	//no "requested key" found
 }
```
The method automatically optimizes the hashmap where it can

#### void put(Key, Value)

![put-algo](https://github.com/GrgoMariani/Layered-HashMap-Cpp/tree/master/GIFS/put.gif)

```
 hashmap.put("requested key", Value)
 // maps Value to "requested key", if it fails to do so in this block, it allocates new block of memory for the next block
 // Overwrites the old value if found
```
The method also automatically optimizes the hashmap
#### void remove(key)

![put-algo](https://github.com/GrgoMariani/Layered-HashMap-Cpp/tree/master/GIFS/remove.gif)

```
 hashmap.remove( "requested key") )
 // removes the "requested key" mapping from memory
```
#### void clear()
```
 hashmap.clear()
 //deallocates all blocks and allocates one block of the same size as it started with
```

### Other
Should you use your custom object as value don't forget to assign the correct copy constructor for it.

I've commented a few methods I didn't use but you could find useful. HashMap::print_all() prints all elements of the table and the flags on positions. Feel free to uncomment it in 'Block.h' 'and HashMap.h' and use.

The structure could be easily repurposed to read from HardDrive instead of RAM should you have even more elements, or even made with multithread support to get the best out of the speed and memory allocation.

Or both.