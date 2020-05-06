#pragma once

#include <iostream>
#include <vector>
#include <string>

///// Constants /////
std::string const INPUT_FOLDER = "../Input-Output/Input/";
std::string const A_SET_FOLDER = INPUT_FOLDER + "A_set/";
std::string const OUTPUT_FOLDER = "../Input-Output/Output/";

const std::string TXT_EXTENSION = ".txt";
const std::string LP_EXTENSION = ".lp";
const std::string SPACE = " ";


///// Functions /////

/* display the content of a vector of objects of any type that has a << operator overload */
template <typename T>
std::ostream& operator<<(std::ostream& ar_stream, const std::vector<T>& ar_vect)
{
	for (T item : ar_vect)
	{
		ar_stream << item << SPACE;
	}
	return ar_stream;
}

/* display the content of a matrix of objects of any type that has a << operator overload */
template <typename T>
std::ostream& operator<<(std::ostream& ar_stream, const std::vector<std::vector<T>>& ar_vect)
{
	for (std::vector<T> item : ar_vect)
	{
		ar_stream << item << endl;
	}
	return ar_stream;
}