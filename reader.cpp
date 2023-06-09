#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


using namespace std;

int main() {
    ifstream fin("students.csv");  // Using ifstream for input
    if (!fin) {
        std::cout << "Error opening file." << std::endl;
        return 1;
    }

    string line, cell;
    vector<string> row;
    std::getline(fin, line);  // ignore first line
    while(std::getline(fin, line)){
        stringstream ss(line);
        while(!ss.eof()) {
            std::getline(ss, cell, ',');
            row.push_back(cell);
        }
        std::cout << row.at(0) << ": " << row.at(1) << " " << row.at(2) << std::endl;
        row.clear();
        line,cell = "";
    }



    fin.close();  // Remember to close the file after reading

    return 0;
}



    
