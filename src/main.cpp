#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <sstream>

using namespace std;

/// name: the name of the student
/// test_scores: all of the students test scores
/// average_score: the average score of the student across all tests
/// id: the students identification number
/// grade: the students letter grade based on their average score
struct Student
{
  string name;
  double* test_scores;
  double average_score;
  int id;
  char grade;
};

/// num_students: the total number of students
/// num_tests: the total number of tests
/// students: dyn. allocated array of Student structs (1 per student)
struct StudentSet 
{
  int num_students, num_tests;
  Student* students;
};

/// const filesystem::path &data_path : 
///   the path to the student data file.
///
/// It is expected that data_path represents a real file of conforming format,
/// otherwise the function will exit the program. 
///
/// Loads the name, test scores, and id into an array of Student structs 
/// according to the headers given number of students, and for the test scores,
/// the headers given number of test scores. Every student within a StudentSet's
/// students field array does not have their average_score or grade fields filled out
/// until calcAverages or letterGrades, respectively, is called with the 
/// StudentSet as it's argument.
StudentSet getStudentData(const filesystem::path &data_path);

/// StudentSet &student_data : 
///   StudentSet whose average_scores fields are filled out for each student
///   in the students field array.
///
/// It is expected that the given StudentSet argument has been returned by the
/// getStudentData function.  
///
/// Calculates and sets the average_score field for every Student struct within
/// the StudentSet given based on the number of scores and the respective 
/// student test scores. This function expects that the num_tests and 
/// num_students fields in the StudentSet are accurate.  
void calcAverages(StudentSet &student_data);

/// StudentSet &student_data : 
///   StudentSet whose grade fields are filled out for each student in the
///   students field array.
///
/// If the calcAverages function hasn't been called before calling this function
/// then the state of the grade field will be non-determinate.  
///
/// Sets the grade field for each Student struct within the given StudentSet
/// based on a standard grading rubric and the average_score field for each
/// respective Student struct.
void letterGrades(StudentSet &Student_data);

/// const StudentSet &student_set : 
///   StudentSet whose data is used to produce the outputted report.
///
/// It is expected that the given StudentSet has had all of it's fields within
/// each Student struct inside the students field array completely and validly
/// filled out using the following functions in order:
///   1. getStudentData
///   2. calcAverages
///   3. letterGrades
///
/// Outputs a columnar report including each student and their respecive values
/// to stdout formatted like below (not scaled to exact column numbers): 
///  | Name | ID | Average | Grade |
void printReport(const StudentSet &student_set);

/// const StudentSet &student_set : 
///   StudentSet which has it's memory deleted.
///
/// StudentSet is expected to have been obtained via the getStudentData
/// function, the pointers do dynamically allocated memory, i.e. students field
/// and within a Student, test_scores, must point to the base of dynamically allocated
/// memory or be nullptr's (you can safely delete nullptr).
///
/// Calls delete [] on the StudentSet students field after calling delete for
/// each Student test_scores field within the StudentSet students field array.
/// This effectively prevents memory leaks for StudentSet structs which no 
/// longer need to be used.
void deallocate(StudentSet &student_set);

int main(int argc, char *argv[]) 
{
  const int EXPECTED_NUM_ARGS { 2 };
  const char* ARG_ERR = 
    "Error: pass student scores file path as argument, i.e. \
    ( ./CourseGrades student_scores.txt )";

  // check for file path argument
  if (argc < EXPECTED_NUM_ARGS) {
    cerr << ARG_ERR << endl;
    exit(EXIT_FAILURE); 
  }

  // set the student data path as the given program argument
  filesystem::path student_data_path { argv[1] };

  // get the StudentSet data from the student_data_path file
  StudentSet student_set { getStudentData(student_data_path) };

  // calculate the average_score for each student
  calcAverages(student_set);

  // assign a letter grade for each student
  letterGrades(student_set);
  
  // print a report of all the student data
  printReport(student_set);

  // delete the dynamically allocated memory
  deallocate(student_set);

  return 0;
}

StudentSet getStudentData(const filesystem::path &data_path) 
{
  const char* FOPEN_ERR = "Error: file failed to open";
  const char* FFORMAT_ERR = "Error: file format is incorrect";

  // open the file
  ifstream data_file { data_path };

  // file open err check
  if (!data_file) 
  {
    cerr << FOPEN_ERR << " @" << data_path << endl;
    exit(EXIT_FAILURE);
  }

  int num_students, num_tests;

  // get the header values
  data_file >> num_students;
  data_file >> num_tests;

  // check for read errors
  if (!data_file) 
  {
    cerr << FFORMAT_ERR << " @" << data_path << endl;
    exit(EXIT_FAILURE);
  }

  // init stood set, store header values for iter, init the dyn array
  StudentSet student_set { num_students, num_tests, new Student[num_students] };

  for (int i {}; i < num_students; ++i) 
  {
    Student &student { student_set.students[i] };

    // init dyn array with num_tests
    student.test_scores = new double[num_tests];

    data_file >> student.name;

    data_file >> student.id;

    // only going to run the error check here on data_file because it could 
    // loop infinitely otherwise and it will pick up the other extraction
    // errors if they happen anyways.
    for (int ii {}; ii < num_tests && data_file; ++ii) 
    {
      data_file >> student.test_scores[ii];
    }
  }

  return student_set;
}

void calcAverages(StudentSet &student_data) 
{
  // iterate over students
  for (int i {}; i < student_data.num_students; ++i) 
  {
    // helps with readability
    Student &student { student_data.students[i] };

    // iterate over students test_scores to produce a total
    for (int ii {}; ii < student_data.num_tests; ++ii)
    {
      student.average_score += student.test_scores[ii];
    }

    // set the average score for the student
    student.average_score /= student_data.num_tests;
  }
}

void letterGrades(StudentSet &student_data) 
{
  // iterate over students and assign grade based on average_score
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

void printReport(const StudentSet &student_set) 
{
  const char *SEPR { " | " };
  const int ID_WDTH { 4 }; 
  const int AVG_SCORE_WDTH { 7 };
  const int GRADE_WDTH { 5 };

  // expected number characters lengths outputted
  // | ? | 4 | 3 | 1 |

  // get the num of chars in the longest name (excluding null term)
  size_t longest_name {};
  for (int i {}; i < student_set.num_students; ++i)
  {
    if (longest_name < student_set.students[i].name.size()) 
      longest_name = student_set.students[i].name.size();
  }
  
  // generate the report header
  stringstream header_stream {};
  header_stream << left << SEPR << setw(longest_name) << "Name" << SEPR
    << setw(ID_WDTH) << "ID" << SEPR
    << setw(AVG_SCORE_WDTH) << "Average" << SEPR
    << setw(GRADE_WDTH) << "Grade" << SEPR;
  const string header = header_stream.str();

  // generate the line seperator based on the header length
  const string LINE_SEPR(header.size(), '-');

  // print the header
  cout << '\n' << LINE_SEPR << '\n' << header << '\n' << LINE_SEPR << endl;


  // iterate over student info and print to stdout 
  for (int i {}; i < student_set.num_students; ++i)
  {
    const Student &student { student_set.students[i] };

    cout << left << SEPR << setw(longest_name) << student.name << SEPR
      << setw(ID_WDTH) << student.id << SEPR
      << setw(AVG_SCORE_WDTH) << fixed << setprecision(1)
        << student.average_score << SEPR
      << setw(GRADE_WDTH) << student.grade << SEPR
      << '\n' << LINE_SEPR << endl;
  }                                     
}

void deallocate(StudentSet &student_set) 
{
  // deallocates all the test_scores arrays for each student
  for (int i {}; i < student_set.num_students; ++i) 
  {
    delete [] student_set.students[i].test_scores;
  }

  // deallocates the students structs
  delete [] student_set.students;
}
