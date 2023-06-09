#include "school_manager.h"

using namespace std;

namespace academic{
    class school;
    class club;

    int total_schools = 0;
    vector<school*> school_directory;

    class student{
        public:
            string first_name;
            string last_name;
            int id;
            school* student_school;

            student(string first, string last, int id_num, school* s){
                first_name = first;
                last_name = last;
                id = id_num;
                student_school = s;
            }

            vector<club*> getClubs();
    };

    class school{

        public:
            string name;
            string address;
            int id;
            vector<club> club_list;
            vector<student> student_list;

            school(string school_name, string school_address){
                name = school_name;
                address = school_address;
                id = total_schools;
                club_list = {};
                student_list = {};

                total_schools++;
                school_directory.push_back(this);
            }

            school(string school_name, string school_address, vector<club> clubs, vector<student> students){
                name = school_name;
                address = school_address;
                id = total_schools;
                club_list = clubs;
                student_list = students;

                total_schools++;
                school_directory.push_back(this);
            }

            student* getStudentByID(int id){
                for (auto& s: student_list){
                    if (s.id == id) return &s;
                }
                return NULL;
            }

            student* getStudentByName(string first_name, string last_name){
                for (auto& s: student_list){
                    if (s.first_name == first_name && s.last_name == last_name) return &s;
                }
                return NULL;
            }

            void addStudent(student s){
                student_list.push_back(s);
            }

            void removeStudentByName(string first_name, string last_name){
                for (int i = 0; i < student_list.size(); i++){
                    if (student_list.at(i).first_name == first_name &&
                        student_list.at(i).last_name == last_name) student_list.erase(student_list.begin() + i);
                }
            }

            club* getClubByID(int club_id);
            club* getClubByName(string club_name);
            void addClub(club c);
            void removeClubByName(string club_name);
            void printAllStudents();
            void printAllClubs();
    };

    
    class club{
        public:
            string name;
            student* president;
            int id;
            vector<student*> members_list;

            club(string club_name, student* club_president, int club_id){
                name = club_name;
                president = club_president;
                id = club_id;
                members_list = {};
            }

            club(string club_name, student* club_president, int club_id, vector<student*> members){
                name = club_name;
                president = club_president;
                id = club_id;
                members_list = members;
            }

            void addMember(student* s){
                members_list.push_back(s);
            }

            void removeMemberByID(int student_id){
                for (int i = 0; i < members_list.size(); i++){
                    if (members_list.at(i)->id == student_id) members_list.erase(members_list.begin() + i);
                }
            }
    };

    vector<club*> student::getClubs(){
        vector<club> student_clubs = student_school -> club_list;
        vector<club*> club_pointers;
        for(auto c: student_clubs){
            club_pointers.push_back(&c);
        }
        return club_pointers;
    }

    club* school::getClubByID(int club_id){
        for(auto& c: club_list){
            if (c.id == club_id) return &c;
        }
        return NULL;
    }

    club* school::getClubByName(string club_name){
        for (auto& c: club_list){
            if (c.name == club_name) return &c;
        }
        return NULL;
    }

    void school::addClub(club c){
        club_list.push_back(c);
    }

    void school::removeClubByName(string club_name){
        for (int i = 0; i < club_list.size(); i++){
            if (club_list.at(i).name == club_name) club_list.erase(club_list.begin() + i);
        }
    }

    void school::printAllStudents(){
        for (auto s: student_list){
            std::cout << s.id << ": " << s.first_name << " " << s.last_name << endl;
        }
    }

    void school::printAllClubs(){
        for (auto c: club_list){
            std::cout << c.id << ": " << c.name << " (president " << (c.president) -> first_name << " " << (c.president) -> last_name << ")" << endl;
            std::cout << "Members: ";
            bool first = true;
            for (auto m: c.members_list){
                if (!first) std::cout << ", ";
                first = false;
                std::cout << m -> first_name << " " << m -> last_name;
            }
            std::cout << endl;
        }
    }

    
}

namespace loadStore{

    academic::school loadSchoolFromCSV(string studentsFilename, string clubsFilename, string schoolFilename){
        vector<string> lines, row;
        vector<int> members;
        string line,cell;

        //read in the school name and address
        ifstream fin(schoolFilename);  
        if (!fin){
            std::cout << "Error opening file: " << schoolFilename << std::endl;
            exit(1);
        }
        while (std::getline(fin, line)){
            lines.push_back(line);
        }
        fin.close();  
        academic::school newSchool(lines.at(0), lines.at(1));
        academic::school* school_ref = &newSchool;
        lines.clear();

        //read in all of the students
        ifstream fin1(studentsFilename);
        if (!fin1){
            std::cout << "Error opening file: " << studentsFilename << std::endl;
            exit(1);
        }

        std::getline(fin1, line);
        while (std::getline(fin1, line)){
            stringstream ss(line);
            while(!ss.eof()){
                std::getline(ss, cell, ',');
                row.push_back(cell);
            }
            academic::student newStudent(row.at(1), row.at(2), stoi(row.at(0)), school_ref);
            newSchool.addStudent(newStudent);
            row.clear();
        }

        fin1.close();
        row.clear();


        //read in all of the clubs
        ifstream fin2(clubsFilename);
        if (!fin2){
            std::cout << "Error opening file: " << clubsFilename << std::endl;
            exit(1);
        }

        std::getline(fin2, line);
        while (std::getline(fin2, line)){
            stringstream ss(line);
            while(!ss.eof()){
                std::getline(ss, cell, ',');
                row.push_back(cell);
            }
            
            academic::club newClub(row.at(1), newSchool.getStudentByID(stoi(row.at(2))), stoi(row.at(0)));

            stringstream ss2(row.at(3));
            while(!ss2.eof()){
                std::getline(ss2, cell, ' ');
                newClub.addMember(newSchool.getStudentByID(stoi(cell)));
            }

            newSchool.addClub(newClub);
            row.clear();
        }

        fin2.close();
        return newSchool;

    }

    void writeSchoolToCSV(academic::school* school_ref, std::string studentsFilename, std::string clubsFilename){
        int x = 3;
    }


}

int main(){
    academic::school s = loadStore::loadSchoolFromCSV("students.csv", "clubs.csv", "school_info.txt");
    std::cout << s.name << endl;
    std::cout << s.address << endl;
    std::cout << "Students: \n";
    s.printAllStudents();
    std::cout << "Clubs: \n";
    s.printAllClubs();
}