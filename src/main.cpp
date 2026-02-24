#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;


struct Student
{
  string name;
  double average_score;
  int id;
  char grade;
};

Student* getStudentData(const filesystem::path &data_path);

void calcAverages(Student* student_data);

void letterGrade(Student* Student_data);

void printReport(const Student* Student_data);

int main(void) 
{
  getStudentData(

  return 0;
}
