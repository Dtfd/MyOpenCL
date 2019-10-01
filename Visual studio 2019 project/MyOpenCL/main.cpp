// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>
#include "MyOpenCL/Mocl.h"
void main000() {

	MyOpenCL::Mocl::Initialize(CL_DEVICE_TYPE_ACCELERATOR);
	MOCL_CHECK_ERROR(MyOpenCL::Mocl::Error(), "Initialize", );
	MyOpenCL::Mocl::Test4();
	MyOpenCL::Mocl::Destroy();
	MOCL_CHECK_ERROR(MyOpenCL::Mocl::Error(), "Destroy", );
}

void main001() {
	std::ifstream t("file.txt");
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	std::cout << str << std::endl;
}
void main002() {
	std::ifstream inFile;
	inFile.open("file.txt"); //open the input file

	std::stringstream strStream;
	strStream << inFile.rdbuf(); //read the file
	std::string str = strStream.str(); //str holds the content of the file

	std::cout << str << "\n";
	inFile.close();
}
void main003() {

	std::string line;
	std::ifstream myfile("file.txt");
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			std::cout << line << '\n';
		}
		myfile.close();
	}

	else std::cout << "Unable to open file";
}
int main()
{

	main000();
	std::cin.ignore();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
