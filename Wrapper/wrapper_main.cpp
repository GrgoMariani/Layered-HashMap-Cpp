#include "benchmark.h"
#include "Useful/getRSS.h"
#include "Useful/ansi_escapes.h"

#include "main_func.h"

#include <iostream>
#include <ctime>


#define T_LIGHT_BLUE 	"\033[1;39m"
#define T_YELLOW 		"\033[1;33m"
#define T_DEFAULT		"\033[0m"


int main(int argc, char *argv[])
{
	int result;

	std::clock_t t_start, t_end;

	setupConsole();

	std::cout << T_LIGHT_BLUE << " Executing _main_" << std::endl;
	std::cout << T_LIGHT_BLUE << "-------------------------------------------------";
	std::cout << T_DEFAULT    << std::endl;

	t_start = std::clock();

	result = _main_(argc, argv);

	t_end = std::clock();

	clearPagesUsed();

	std::cout << std::endl;
	std::cout << T_LIGHT_BLUE << "-------------------------------------------------" << std::endl;
	std::cout << T_LIGHT_BLUE << " Done executing _main_ " << std::endl;
	std::cout << T_LIGHT_BLUE << " Clocks needed         " << T_YELLOW << (t_end-t_start) 			<< " clocks" 	<< std::endl;
	std::cout << T_LIGHT_BLUE << " VmPeakRSS:            " << T_YELLOW << getPeakRSS() 				<< "" 			<< std::endl;
	std::cout << T_LIGHT_BLUE << " VmSize:               " << T_YELLOW << getVMSize() 				<< "" 			<< std::endl;
	std::cout << T_LIGHT_BLUE << " VmRSS :               " << T_YELLOW << getCurrentRSS()			<< "" 			<< std::endl;
	std::cout << T_LIGHT_BLUE << " Process returned with " << T_YELLOW << result      				<< std::endl;
	std::cout << std::endl;
	std::cout << T_DEFAULT    << "press ENTER key to continue" << std::endl;

	restoreConsole();

	std::cin.ignore();

	return result;
}
