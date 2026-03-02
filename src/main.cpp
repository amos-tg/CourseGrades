#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>

using namespace std;

struct Student
{
  string name;
  double* test_scores;
  double average_score;
  int id;
  char grade;
};

struct StudentSet 
{
  int num_students, num_tests;
  Student* students;
};

StudentSet getStudentData(const filesystem::path &data_path);

void calcAverages(StudentSet &student_data);

void letterGrades(StudentSet &Student_data);

void printReport(const StudentSet &Student_data);

void deallocate(StudentSet &student_set);

int main(int argc, char *argv[]) 
{
  const int EXPECTED_NUM_ARGS { 2 };
  const char* ARG_ERR = 
    "Error: pass student scores file path as argument, i.e. \
    ( ./CourseGrades student_scores.txt )";

  if (argc < EXPECTED_NUM_ARGS) {
    cerr << ARG_ERR << endl;
    exit(EXIT_FAILURE); 
  }

  filesystem::path student_data_path { argv[1] };

  StudentSet student_set { getStudentData(student_data_path) };

  calcAverages(student_set);
  letterGrades(student_set);
  
  printReport(student_set);

  deallocate(student_set);

  return 0;
}

StudentSet getStudentData(const filesystem::path &data_path) 
{
  const char* FOPEN_ERR = "Error: file failed to open";
  const char* FFORMAT_ERR = "Error: file format is incorrect";

  ifstream data_file { data_path };

  if (!data_file) 
  {
    cerr << FOPEN_ERR << " @" << data_path << endl;
    exit(EXIT_FAILURE);
  }

  int num_students, num_tests;

  data_file >> num_students;
  data_file >> num_tests;

  if (!data_file) 
  {
    cerr << FFORMAT_ERR << " @" << data_path << endl;
    exit(EXIT_FAILURE);
  }

  StudentSet student_set { num_students, num_tests, new Student[num_students] };

  for (int i {}; i < num_students; ++i) 
  {
    student_set.students[i].test_scores = new double[num_tests];

    data_file >> student_set.students[i].name;

    data_file >> student_set.students[i].id;

    // only going to run the error check here on data_file because it could 
    // loop infinitely otherwise and it will pick up the other extraction
    // errors if they happen anyways.
    for (int ii {}; i < num_tests && !data_file; ++i) 
    {
      data_file >> student_set.students[i].test_scores[ii];
    }
  }

  return student_set;
}

void calcAverages(StudentSet &student_data) 
{
  for (int i {}; i < student_data.num_students; ++i) 
  {
    Student &student { student_data.students[i] };

    for (int ii {}; i < student_data.num_tests; ++i)
    {
      student.average_score += student.test_scores[ii];
    }

    student.average_score /= student_data.num_tests;
  }
}

void letterGrades(StudentSet &student_data) 
{
  for (int i {}; i < student_data.num_students; ++i) 
  {
    Student &student { student_data.students[i] };

    if (student.average_score >= 90) 
    {
      student.grade = 'A';
    }
    else if (student.average_score >= 80)
    {
      student.grade = 'B';
    }
    else if (student.average_score >= 70)
    {
      student.grade = 'C';
    }
    else if (student.average_score >= 60)
    {
      student.grade = 'D';
    }
    else 
    {
      student.grade = 'F';
    }
  }
}

void printReport(const StudentSet &student_data) 
{
}

void deallocate(StudentSet &student_set) 
{
  // deallocates all the test_scores arrays
  for (int i {}; i < student_set.num_students; ++i) 
  {
    delete [] student_set.students[i].test_scores;
  }

  // deallocates the students structs
  delete [] student_set.students;
}
