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

vector<process_stats> stats;
//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	return atoi(myString);
}

bool checkCorruptedRow(string line) {
	vector<string> result;
	stringstream s_stream(line);
	while (s_stream.good()){
		string substr;
		getline(s_stream, substr, CHAR_TO_SEARCH_FOR);
		if (substr != ""){
			result.push_back(substr);
		}
	}

	if (result.size() != 4){
		return false;
	}

	return true;
}

process_stats getValuesFromString(string line){
	vector<int> tempVector;

	string val;
	int temp = 0;
	for (unsigned int i = 0; i < line.length(); i++){
		if (line[i] == CHAR_TO_SEARCH_FOR) {
			stringstream ss(val);
			ss >> temp;
			tempVector.push_back(temp);
			val = "";
		}
		else {
			val += line[i];
		}
	}
	stringstream ss(val);
	ss >> temp;
	tempVector.push_back(temp);

	process_stats myStats = {
			tempVector[0],
			tempVector[1],
			tempVector[2],
			tempVector[3]
	};
	return myStats;
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
		if (checkCorruptedRow(line) == true){
			temp.push_back(line);
		}
	}
	myInputfile.close();

	// checks bool ignoreFirstRow and removes it if is true
	vector<vector<int>> val;
	for (unsigned int i = 0; i < temp.size(); i++){
		if (ignoreFirstRow && temp[i] == temp[0]){
			temp.erase(temp.begin());
		}
	}

	for (unsigned int i = 0; i < temp.size(); i++){
		stats.push_back(getValuesFromString(temp[i]));
	}
	return SUCCESS;
}

bool ascendingCPU(const process_stats& a, const process_stats b) {
	return a.cpu_time < b.cpu_time;
}
bool ascendingProcess(const process_stats& a, const process_stats b) {
	return a.process_number < b.process_number;
}
bool ascendingStart(const process_stats& a, const process_stats b) {
	return a.start_time < b.start_time;
}
bool ascendingIO(const process_stats& a, const process_stats b) {
	return a.io_time < b.io_time;
}

//will sort according to user preference
void sortData(SORT_ORDER mySortOrder) {
	if (mySortOrder == SORT_ORDER::START_TIME){
		sort(stats.begin(), stats.end(), ascendingStart);
	}
	if (mySortOrder == SORT_ORDER::CPU_TIME){
		sort(stats.begin(), stats.end(), ascendingCPU);
	}
	if (mySortOrder == SORT_ORDER::IO_TIME){
		sort(stats.begin(), stats.end(), ascendingIO);
	}
	if (mySortOrder == SORT_ORDER::PROCESS_NUMBER){
		sort(stats.begin(), stats.end(), ascendingProcess);
	}
}

process_stats getNext() {
	process_stats myFirst;
	myFirst = stats[0];
	stats.erase(stats.begin());
	return myFirst;
}

//returns number of process_stats structs in the vector holding them
int getNumbRows(){
	return stats.size();
}


