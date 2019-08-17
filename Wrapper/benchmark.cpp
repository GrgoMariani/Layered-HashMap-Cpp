#include "benchmark.h"

/* Just a way to make sure the pages are actually cleared
 *
 * */
void clearPagesUsed()
{
	for(short i=0; i<256; i++)
	{
		char *c = new char[(i+1)*1024*1024];
		delete[] c;
	}
}
