// CSC-482_Lab6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <algorithm>

#define NUMBEROFTRIALS 1
#define MAXINPUTSIZE 64
#define MININPUTSIZE 1

std::string FILE_PATH = "C:/Users/gssvd/Documents/Lab7/Improved/TwoBooks/";

std::string BOOK_ONE = "";
std::string BOOK_TWO = "";

void RunFullExperiment(std::string resultsFileName);
std::string GenerateRandomString(int length);
std::string GenerateSameCharacterStringOfSize(int length, char character);
std::string GetRandomSubStringOfSize(int length, std::string& source);
void InsertSubstringAtRandomLocation(std::string* stringToInsertInto, std::string& stringThatWillBeInserted);
int BruteForceLCS(std::string firstString, std::string secondString, std::string* out_LongestFoundString = nullptr);
std::string LCSubStr(std::string X, std::string Y);
void ValidateBruteForceLCS();
void ValidateImprovedLCS();

int main() {

	//std::cout << GenerateRandomString(100) << std::endl;
	//ValidateBruteForceLCS();
	//ValidateImprovedLCS();

	// Load in bookOne and bookTwo
	std::ifstream file("C:/Users/gssvd/Documents/Lab7/ArgumentsOfCelcus.txt");
	std::stringstream buffer;
	buffer << file.rdbuf();
	BOOK_ONE = buffer.str();

	std::ifstream file2("C:/Users/gssvd/Documents/Lab7/ADialogueConcerningOratory.txt");
	buffer << file.rdbuf();
	BOOK_TWO = buffer.str();

	std::string longestString = "";
	std::string randomString = "";

	randomString = GetRandomSubStringOfSize(10, BOOK_TWO);
	longestString = LCSubStr(BOOK_ONE, randomString);

	std::cout << "Title of the book being searched: Arguments Of Celsus, Porphyry, And The Emperor Julian, Against The Christians" << std::endl;
	std::cout << "Title of the book the substring is coming from: A Dialogue Concerning Oratory, Or The Causes Of Corrupt Eloquence" << std::endl;
	std::cout << "String from book 2 that we are searching for: " << randomString << std::endl;
	std::cout << "Result: " << longestString << std::endl;

	//RunFullExperiment("BruteForce-Exp1-ThrowAway.txt");
	//RunFullExperiment("BruteForce-Exp2.txt");
	//RunFullExperiment("BruteForce-Exp3.txt");

	return 0;
}

void RunFullExperiment(std::string resultsFileName) {

	std::ofstream myfile;
	myfile.open(FILE_PATH + resultsFileName);

	myfile << "#InputSize      AverageTime\n";

	std::string stringOne;
	std::string stringTwo;

	for (int inputSize = MININPUTSIZE; inputSize <= MAXINPUTSIZE; inputSize *= 2) {

		//stringOne = GenerateSameCharacterStringOfSize(inputSize, 'X'); // Worst Case
		
		//stringOne = GenerateRandomString(inputSize); // Random strings of equal size
		//stringTwo = GenerateRandomString(inputSize); // Random strings of equal size

		//stringOne = GetRandomSubStringOfSize(inputSize, BOOK_ONE); // Random Substring from BOOK_ONE
		stringTwo = GetRandomSubStringOfSize(inputSize, BOOK_TWO); // Random Substring from BOOK_ONE


		auto t1 = std::chrono::high_resolution_clock::now(); // Start time

		for (int trial = 0; trial < NUMBEROFTRIALS; trial++) {

			// Call function for experiment here:
			LCSubStr(BOOK_ONE, stringTwo); // Worst Case
		}

		auto t2 = std::chrono::high_resolution_clock::now(); // End time
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count(); // Elapsed time

		// Write results to the file:
		myfile << inputSize << "         " << duration << "\n";
		std::cout << "inputSize " << inputSize << " Done!" << std::endl;
	}

	myfile.close();
}

/** Generates a random string of the provided length. */
std::string GenerateRandomString(int length) {

	std::string randomString = "";

	std::random_device rd; // Get random number from the hardware.
	std::mt19937 eng(rd()); // Seed the generator.
	std::uniform_int_distribution<> distr(32, 122); // Range of the random numbers.

	for (int c = 0; c < length; c++) {
	
		randomString.push_back((char)distr(eng));
	}

	return randomString;
}

std::string GenerateSameCharacterStringOfSize(int length, char character) {

	std::string generatedString;

	for (int c = 0; c < length; c++) {
		generatedString.push_back(character);
	}

	return generatedString;
}

std::string GetRandomSubStringOfSize(int length, std::string& source) {

	std::string subString;

	std::random_device rd; // Get random number from the hardware.
	std::mt19937 eng(rd()); // Seed the generator.
	std::uniform_int_distribution<> distr(0, source.size() - (length + 1)); // Range of the random numbers... (length + 1) to ensure we dont overrun source

	subString = source.substr(distr(eng), length);
	
	return subString;
}

void InsertSubstringAtRandomLocation(std::string* stringToInsertInto, std::string& stringThatWillBeInserted) {

	// Makes sure the receiving string isnt null.
	if (stringToInsertInto) {

		std::random_device rd; // Get random number from the hardware.
		std::mt19937 eng(rd()); // Seed the generator.
		std::uniform_int_distribution<> distr(0, stringToInsertInto->size() - 1); // Range of the random numbers.

		stringToInsertInto->insert(distr(eng), stringThatWillBeInserted.c_str());
	}
}

/** Returns the size of the largest common string that was found. The located string itself is returned the (optional) out_LongestFoundString variable. */
int BruteForceLCS(std::string firstString, std::string secondString, std::string* out_LongestFoundString) { 
	
	int lengthOfLongestLCS = 0;
	int lengthOfCurrentLCS = 0;

	std::string largestFound = "NONE FOUND";
	std::string currentString;

	std::string::iterator strComparisonIter;
	
	// Iterates through the characters of the strings using C++ iterators.
	for (std::string::iterator iter1 = firstString.begin(); iter1 != firstString.end(); iter1++) {
	
		strComparisonIter = iter1;

		for (std::string::iterator iter2 = secondString.begin(); iter2 != secondString.end(); iter2++) {
		
			// Makes sure the strComparisonIter doesn't go beyond the end of the firstString
			if (strComparisonIter == firstString.end()) {

				// If it is then we break because we got to the end of the first string.
				break;
			}

			// If the values are the same we increment the lengthOfCurrentLCS and push a character back
			// into the currentString.
			if (*strComparisonIter == *iter2) {
			
				currentString.push_back(*iter2);
				lengthOfCurrentLCS++;
				strComparisonIter++;
			} else {
				
				// Checks if we found a string larger than the previous one.
				if (lengthOfCurrentLCS > lengthOfLongestLCS) {

					largestFound = currentString;
					lengthOfLongestLCS = lengthOfCurrentLCS;
				}

				// If they are not equal then we reset the lengthOfCurrentLCS and the currentString
				lengthOfCurrentLCS = 0;
				currentString.clear();
			}
		}

		// Checks if we found a string larger than the previous one.
		if (lengthOfCurrentLCS > lengthOfLongestLCS) {

			largestFound = currentString;
			lengthOfLongestLCS = lengthOfCurrentLCS;
		}

		// Resets variables for the next iteration.
		currentString.clear();
		lengthOfCurrentLCS = 0;
	}

	// Makes sure out_LongestFoundString is not null, then sets it to our longest found string if it isn't
	if (out_LongestFoundString) {
	
		*out_LongestFoundString = largestFound;
	}

	return lengthOfLongestLCS;
}

// Function to find longest common substring. 
std::string LCSubStr(std::string X, std::string Y) {

	// Find length of both the strings. 
	int m = X.length();
	int n = Y.length();

	// Variable to store length of longest 
	// common substring. 
	int result = 0;

	// Variable to store ending point of 
	// longest common substring in X. 
	int end;

	// Matrix to store result of two 
	// consecutive rows at a time. 
	int len[2][70000];

	// Variable to represent which row of 
	// matrix is current row. 
	int currRow = 0;

	// For a particular value of i and j, 
	// len[currRow][j] stores length of longest 
	// common substring in string X[0..i] and Y[0..j]. 
	for (int i = 0; i <= m; i++) {
		for (int j = 0; j <= n; j++) {
			if (i == 0 || j == 0) {
				len[currRow][j] = 0;
			}
			else if (X[i - 1] == Y[j - 1]) {
				len[currRow][j] = len[1 - currRow][j - 1] + 1;
				if (len[currRow][j] > result) {
					result = len[currRow][j];
					end = i - 1;
				}
			}
			else {
				len[currRow][j] = 0;
			}
		}

		// Make current row as previous row and 
		// previous row as new current row. 
		currRow = 1 - currRow;
	}

	// If there is no common substring, print -1. 
	if (result == 0) {
		return "-1";
	}

	// Longest common substring is from index 
	// end - result + 1 to index end in X. 
	return X.substr(end - result + 1, result);
}

void ValidateBruteForceLCS() {

	std::string longestFound;
	std::string randomOne;
	std::string randomTwo;
	std::string randomThree;
	
	std::cout << "BruteForceLCS Verifications For Pre-Constructed Strings:\n" << std::endl;

	BruteForceLCS("asd as das das da33L0 W0RLDasHELLO as das da ", "HELLO WORLD H33L0 W0RLD", &longestFound);
	std::cout << "String One:   asd as das das da33L0 W0RLDasHELLO as das da " << std::endl;
	std::cout << "String Two:   HELLO WORLD H33L0 W0RLD" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n";

	BruteForceLCS("h3h3h3h3h3h3h3h31oia fo;uas hous jvo;s", "ibs os s vh3h3h3h3h3h3h3h31owe no fhwwo ieh", &longestFound);
	std::cout << "String One:   h3h3h3h3h3h3h3h31oia fo;uas hous jvo;s" << std::endl;
	std::cout << "String Two:   ibs os s vh3h3h3h3h3h3h3h31owe no fhwwo ieh" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n";

	BruteForceLCS("11111", "22222", &longestFound);
	std::cout << "String One:   11111" << std::endl;
	std::cout << "String Two:   22222" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n" << "Random Strings of Length 15:\n\n" << std::endl;

	randomOne = GenerateRandomString(15);
	randomTwo = GenerateRandomString(15);
	BruteForceLCS(randomOne, randomTwo, &longestFound);
	std::cout << "String One:   " << randomOne << std::endl;
	std::cout << "String Two:   " << randomTwo << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n" << "Random With Several Matching Substrings:\n\n" << std::endl;

	BruteForceLCS("2398903bf4 FFEF 1320847 FGET 1093248 FPOLYT", "01283-39rhnf ffef 12-938235 FPOLYT 12-93834 FGET", &longestFound);
	std::cout << "String One:   2398903bf4 FFEF 1320847 FGET 1093248 FPOLYT" << std::endl;
	std::cout << "String Two:   01283-39rhnf ffef 12-938235 FPOLYT 12-93834 FGET" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n" << "Matching Substrings Half The Length of The Longest String:\n\n" << std::endl;

	BruteForceLCS("774hf 237092h3fuf 7gh8hd", "98jg 237092h3fuf 33", &longestFound);
	std::cout << "String One:   774hf 237092h3fuf 7gh8hd" << std::endl;
	std::cout << "String Two:   98hjg 237092h3fuf 33" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n" << "Random Strings of Length 200 with matching substring of length 20 inserted into them at a random location:\n\n" << std::endl;

	randomOne = GenerateRandomString(200);
	randomTwo = GenerateRandomString(200);
	randomThree = GenerateRandomString(20);
	InsertSubstringAtRandomLocation(&randomOne, randomThree);
	InsertSubstringAtRandomLocation(&randomTwo, randomThree);

	BruteForceLCS(randomOne, randomTwo, &longestFound);
	std::cout << "String One:   " << randomOne << std::endl;
	std::cout << "String Two:   " << randomTwo << std::endl;
	std::cout << "String that was inserted: " << randomThree << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;
}

void ValidateImprovedLCS() {

	std::string longestFound;
	std::string randomOne;
	std::string randomTwo;
	std::string randomThree;

	std::cout << "ImprovedLCS Verifications For Pre-Constructed Strings:\n" << std::endl;

	longestFound = LCSubStr("asd as das das da33L0 W0RLDasHELLO as das da ", "HELLO WORLD H33L0 W0RLD");
	std::cout << "String One:   asd as das das da33L0 W0RLDasHELLO as das da " << std::endl;
	std::cout << "String Two:   HELLO WORLD H33L0 W0RLD" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n";

	longestFound = LCSubStr("h3h3h3h3h3h3h3h31oia fo;uas hous jvo;s", "ibs os s vh3h3h3h3h3h3h3h31owe no fhwwo ieh");
	std::cout << "String One:   h3h3h3h3h3h3h3h31oia fo;uas hous jvo;s" << std::endl;
	std::cout << "String Two:   ibs os s vh3h3h3h3h3h3h3h31owe no fhwwo ieh" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n";

	longestFound = LCSubStr("11111", "22222");
	std::cout << "String One:   11111" << std::endl;
	std::cout << "String Two:   22222" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n" << "Random Strings of Length 15:\n\n" << std::endl;

	randomOne = GenerateRandomString(15);
	randomTwo = GenerateRandomString(15);
	longestFound = LCSubStr(randomOne, randomTwo);
	std::cout << "String One:   " << randomOne << std::endl;
	std::cout << "String Two:   " << randomTwo << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n" << "Random With Several Matching Substrings:\n\n" << std::endl;

	longestFound = LCSubStr("2398903bf4 FFEF 1320847 FGET 1093248 FPOLYT", "01283-39rhnf ffef 12-938235 FPOLYT 12-93834 FGET");
	std::cout << "String One:   2398903bf4 FFEF 1320847 FGET 1093248 FPOLYT" << std::endl;
	std::cout << "String Two:   01283-39rhnf ffef 12-938235 FPOLYT 12-93834 FGET" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n" << "Matching Substrings Half The Length of The Longest String:\n\n" << std::endl;

	longestFound = LCSubStr("774hf 237092h3fuf 7gh8hd", "98jg 237092h3fuf 33");
	std::cout << "String One:   774hf 237092h3fuf 7gh8hd" << std::endl;
	std::cout << "String Two:   98hjg 237092h3fuf 33" << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;

	std::cout << "\n\n" << "Random Strings of Length 200 with matching substring of length 20 inserted into them at a random location:\n\n" << std::endl;

	randomOne = GenerateRandomString(200);
	randomTwo = GenerateRandomString(200);
	randomThree = GenerateRandomString(20);
	InsertSubstringAtRandomLocation(&randomOne, randomThree);
	InsertSubstringAtRandomLocation(&randomTwo, randomThree);

	longestFound = LCSubStr(randomOne, randomTwo);
	std::cout << "String One:   " << randomOne << std::endl;
	std::cout << "String Two:   " << randomTwo << std::endl;
	std::cout << "String that was inserted: " << randomThree << std::endl;
	std::cout << "Longest Common String: " << longestFound << std::endl;
}