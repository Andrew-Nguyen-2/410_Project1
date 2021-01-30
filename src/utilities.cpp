/***
 * KP Utils
 */
#include <numeric>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utilities.h"
#include "stdlib.h"

using namespace std;

//********************** private to this compilation unit **********************

vector<vector<process_stats>> stats;
//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	return atoi(myString);
}

bool checkCorruptedRow(string line){
	int commas = 0;
	int values = 0;

	for (unsigned int i = 0; i < line.length(); i++){
		if (line[i] == CHAR_TO_SEARCH_FOR){
			commas++;
		}
		if (isdigit(line[i])){
			values++;
		}
	}
	if (commas == 3 && values == 4){
		return true;
	}
	return false;
}

vector<process_stats> getValuesFromString(string line){
	vector<int> tempVector;
	vector<process_stats> out;

	string val;
	int temp = 0;
	for (unsigned int i = 0; i < line.length(); i++){
		if (line[i] != CHAR_TO_SEARCH_FOR) {
			val += line[i];
		}
		else {
			stringstream ss(val);
			ss >> temp;
			tempVector.push_back(temp);
			val = "";
			temp = 0;
		}
	}
	process_stats myStats = {
			tempVector[0],
			tempVector[1],
			tempVector[2],
			tempVector[3]
	};
	out.push_back(myStats);
	return out;
}

int loadData(const char* filename, bool ignoreFirstRow) {
	stats.clear();
	fstream myInputfile;
	myInputfile.open(filename);

	if (!myInputfile.is_open()){
		return COULD_NOT_OPEN_FILE;
	}

	string line;
	vector<string> temp;
	while (!myInputfile.eof()){
		getline(myInputfile, line);
		temp.push_back(line);
	}
	myInputfile.close();

	// remove first row if ignoreFirstRow is true
	if (ignoreFirstRow){
		temp.erase(temp.begin());
	}

	// search and remove corrupted rows
	// appends clean rows to process_stats vector
	vector<vector<int>> val;
	for (unsigned int i = 0; i < temp.size(); i++){
		if (checkCorruptedRow(temp[i]) == false){
			temp.erase(temp.begin() + i);
		}
		else {
			stats.push_back(getValuesFromString(temp[i]));
		}
	}
	return SUCCESS;
}


//will sort according to user preference
void sortData(SORT_ORDER mySortOrder) {

}

process_stats getNext() {
	process_stats myFirst;
	myFirst = stats[0][0], stats[0][1], stats[0][2], stats[0][3];
	stats.erase(stats.begin());
	return myFirst;
}

//returns number of process_stats structs in the vector holding them
int getNumbRows(){
	return stats.size();
}


