#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const string MENU = "1. Input n-count students.\n2. Show all students.\n3. Search in the students list.\n4. Sort students alphabetically by first name.\n5. Export students to binary file.\n6. Import students from binary file.\n7. Show students with specific status ordered by faculty number.\n8. Show students with average grade [min - max] ordered by EGN.\n9. Update grades by faculty number\n10. Status update by faculty number.\n0. Exit.\nPlease input command number: ";

const string SEARCH_SUBMENU = "1. By programming basics grade [min - max].\n2. By failed courses.\n0. Back.\nPlease input command number: ";

enum Gender
{	
	Male = 1,
	Female = 2
};

enum Status
{
	Active = 1,
	Dropout = 2,
	Graduated = 3
};

struct Subject
{
	string Name;
	unsigned short Grade;
};

struct Student
{
	char facultyNumber[9];
	char EGN[11];
	string firstName;
	string middleName;
	string lastName;
	Gender gender;
	Status status;
	Subject subjects[5];
	double averageGrade;
};

void showStudents(const Student students[30], const unsigned short& studentsCount);
unsigned short searchForFailed(const Student students[30], const unsigned short& studentsCount, Student found[30]);
unsigned short searchByProgrammingBasics(const Student students[30], const unsigned short& studentsCount, int min, int max, Student found[30]);

int findStudentByFacultyNumber(const Student students[30], const unsigned short& studentsCount, const char facultyNumber[9])
{
    for (int i = 0; i < studentsCount; i++)
    {
        if (strcmp(students[i].facultyNumber, facultyNumber) == 0)
        {
            return i;
        }
    }
    return -1;
}

void updateStatus(Student students[30], const unsigned short& studentsCount, const char facultyNumber[9])
{
	int index = findStudentByFacultyNumber(students, studentsCount, facultyNumber);
	if (index == -1)
	{
		std::cout << "Student with faculty number " << facultyNumber << " not found!" << std::endl;
		return;
	}

	if (students[index].status == Graduated)
	{
		std::cout << "Student has already graduated and cannot have his/her status updated!" << endl;
		return;
	}

	std::cout << "Updating status for student " << students[index].firstName << " " << students[index].lastName << std::endl;
	unsigned short status;
	std::cout << "Please input the new status (1 for active, 2 for dropout and 3 for graduated): ";
	cin >> status;
	while (status < 1 || status > 3)
	{
		std::cout << "Please enter a valid status: ";
		cin >> status;
	}
	students[index].status = Status(status);
	std::cout << "Status updated successfully!" << std::endl;
}

void updateGrades(Student students[30], const unsigned short& studentsCount, const char facultyNumber[9])
{
    int index = findStudentByFacultyNumber(students, studentsCount, facultyNumber);
    if (index == -1)
    {
        std::cout << "Student with faculty number " << facultyNumber << " not found!" << std::endl;
        return;
    }

	if (students[index].status != Active)
	{
		std::cout << "Student is not active and cannot have his/her grades updated!" << endl;
		return;
	}

    std::cout << "Updating grades for student " << students[index].firstName << " " << students[index].lastName << std::endl;
    double gradeSum = 0;
    int gradeCount = 0;
    for (int j = 0; j < 5; j++)
    {
		unsigned short tempGrade, answer;
        std::cout << "Current grade for " << students[index].subjects[j].Name << ": " << students[index].subjects[j].Grade << std::endl;
        std::cout << "Enter new grade for " << students[index].subjects[j].Name << ": ";
        std::cin >> tempGrade;
        while ((tempGrade < 2 || tempGrade > 6) && tempGrade != 0)
        {
            std::cout << "Please enter a valid grade: ";
            std::cin >> tempGrade;
        }

		if (students[index].subjects[j].Grade != 0)
		{
			std::cout << "Are you sure you want to update the grade for " << students[index].subjects[j].Name << " to " << tempGrade << "? (1 for yes, 2 for no): ";
			std::cin >> answer;

			while (answer != 1 && answer != 2)
			{
				std::cout << "Please enter a valid answer: ";
				std::cin >> answer;
			}

			if (answer == 1)
			{
				students[index].subjects[j].Grade = tempGrade;
			}

			
		}
		else
		{
			students[index].subjects[j].Grade = tempGrade;
		}

		if (students[index].subjects[j].Grade != 0)
		{
			gradeCount++;
			gradeSum += students[index].subjects[j].Grade;
		}
    }
    students[index].averageGrade = gradeCount == 0 ? 0 : gradeSum / gradeCount;
    std::cout << "Grades updated successfully!" << std::endl;
}

void orderByEGN(Student students[30], const unsigned short& studentsCount)
{
	for (int i = 0; i < studentsCount - 1; i++)
	{
		bool swapped = false;
		for (int j = i + 1; j < studentsCount; j++)
		{
			if (strcmp(students[i].EGN, students[j].EGN) > 0)
			{
				swapped = true;
				Student temp = students[i];
				students[i] = students[j];
				students[j] = temp;
			}
		}
		if (!swapped)
		{
			break;
		}
	}
}

void orderByFacultyNumber(Student students[30], const unsigned short& studentsCount)
{
	for (int i = 0; i < studentsCount - 1; i++)
	{
		bool swapped = false;
		for (int j = i + 1; j < studentsCount; j++)
		{
			if (strcmp(students[i].facultyNumber, students[j].facultyNumber) > 0)
			{
				swapped = true;
				Student temp = students[i];
				students[i] = students[j];
				students[j] = temp;
			}
		}
		if (!swapped)
		{
			break;
		}
	}
}

void filterByAverageGrades(const Student students[30], const unsigned short& studentsCount)
{
	Student found[30];
	unsigned short count = 0;
	int min, max;
	bool minMaxValid = true;
	do
	{
		if (!minMaxValid)
		{
			std::cout << "Please input min and max grade again. This time try making the min lesser than the max" << endl;
		}
		std::cout << "Please input the minimum average grade: ";
		cin >> min;
		while (min < 2 || min > 6)
		{
			std::cout << "Please input a valid minimum average grade: ";
			cin >> min;
		}
		std::cout << "Please input the maximum average grade: ";
		cin >> max;
		while (max < 2 || max > 6)
		{
			std::cout << "Please input a valid maximum average grade: ";
			cin >> max;
		}
		minMaxValid = min <= max;
	} while (min > max);

	for (int i = 0; i < studentsCount; i++)
	{
		if (students[i].averageGrade >= min && students[i].averageGrade <= max)
		{
			found[count++] = students[i];
		}
	}
	if (count == 0)
	{
		std::cout << "No students found with average grade between " << min << " and " << max << "!" << endl;
	}
	else
	{
		orderByEGN(found, count);
		showStudents(found, count);
	}
}

void filterByStatus(const Student students[30], const unsigned short& studentsCount)
{
	Student found[30];
	unsigned short count = 0;
	unsigned short status;
	std::cout << "Please input the status (1 for active, 2 for dropout and 3 for graduated): ";
	cin >> status;
	while (status < 1 || status > 3)
	{
		std::cout << "Please enter a valid status: ";
		cin >> status;
	}
	for (int i = 0; i < studentsCount; i++)
	{
		if (students[i].status == Status(status))
		{
			found[count++] = students[i];
		}
	}

	if (count == 0)
	{
		std::cout << "No students found with status " << (status == 1 ? "active" : status == 2 ? "dropout" : "graduated") << "!" << endl;
	}
	else
	{
		orderByFacultyNumber(found, count);
		showStudents(found, count);
	}
}

void importStudents(Student students[30], unsigned short& studentsCount) {
	ifstream inFile("students.dat", ios::binary);
	if (!inFile) {
		cerr << "Error opening file for reading!" << endl;
		return;
	}

	while (inFile.read(students[studentsCount].facultyNumber, sizeof(students[studentsCount].facultyNumber))) {
		inFile.read(students[studentsCount].EGN, sizeof(students[studentsCount].EGN));

		char buffer[256];
		inFile.getline(buffer, 256, '\0');
		students[studentsCount].firstName = buffer;

		inFile.getline(buffer, 256, '\0');
		students[studentsCount].middleName = buffer;

		inFile.getline(buffer, 256, '\0');
		students[studentsCount].lastName = buffer;

		inFile.read(reinterpret_cast<char*>(&students[studentsCount].gender), sizeof(students[studentsCount].gender));
		inFile.read(reinterpret_cast<char*>(&students[studentsCount].status), sizeof(students[studentsCount].status));

		for (int j = 0; j < 5; ++j) {
			inFile.getline(buffer, 256, '\0');
			students[studentsCount].subjects[j].Name = buffer;
			inFile.read(reinterpret_cast<char*>(&students[studentsCount].subjects[j].Grade), sizeof(students[studentsCount].subjects[j].Grade));
		}

		inFile.read(reinterpret_cast<char*>(&students[studentsCount].averageGrade), sizeof(students[studentsCount].averageGrade));

		studentsCount++;
		if (studentsCount >= 30) break;
	}

	inFile.close();
	std::cout << "Students imported successfully!" << endl;
}

void exportStudents(const Student students[30], const unsigned short& studentsCount) 
{
	ofstream outFile("students.dat", ios::binary);
	if (!outFile) {
		cerr << "Error opening file for writing!" << endl;
		return;
	}

	for (int i = 0; i < studentsCount; ++i) {
		outFile.write(students[i].facultyNumber, sizeof(students[i].facultyNumber));
		outFile.write(students[i].EGN, sizeof(students[i].EGN));
		outFile.write(students[i].firstName.c_str(), students[i].firstName.size() + 1);
		outFile.write(students[i].middleName.c_str(), students[i].middleName.size() + 1);
		outFile.write(students[i].lastName.c_str(), students[i].lastName.size() + 1);
		outFile.write(reinterpret_cast<const char*>(&students[i].gender), sizeof(students[i].gender));
		outFile.write(reinterpret_cast<const char*>(&students[i].status), sizeof(students[i].status));
		for (int j = 0; j < 5; ++j) {
			outFile.write(students[i].subjects[j].Name.c_str(), students[i].subjects[j].Name.size() + 1);
			outFile.write(reinterpret_cast<const char*>(&students[i].subjects[j].Grade), sizeof(students[i].subjects[j].Grade));
		}
		outFile.write(reinterpret_cast<const char*>(&students[i].averageGrade), sizeof(students[i].averageGrade));
	}

	outFile.close();
	std::cout << "Students exported successfully!" << endl;
}

void orderByName(Student students[30], const unsigned short& count)
{
	for (int i = 0; i < count - 1; i++)
	{
		bool swapped = false;
		for (int j = i + 1; j < count; j++)
		{
			if (students[i].firstName > students[j].firstName)
			{
				swapped = true;
				Student temp = students[i];
				students[i] = students[j];
				students[j] = temp;
			}
		}
		if (!swapped)
		{
			break;
		}
	}
}

void searchSubmenu(const Student students[30], const unsigned short studentsCount)
{
	unsigned short cmd, count;
	Student found[30];
	do
	{
		std::cout << SEARCH_SUBMENU;
		cin >> cmd;
		switch (cmd)
		{
			case 1:
			{
				unsigned short minGrade, maxGrade;
				bool minMaxValid = true;
				do
				{
					if (!minMaxValid)
					{
						std::cout << "Please input min and max grade again. This time try making the min lesser than the max" <<		endl;
					}
					std::cout << "Please input the minimum grade: ";
					cin >> minGrade;
					while (minGrade < 2 || minGrade > 6)
					{
						std::cout << "Please input valid minimum grade: ";
						cin >> minGrade;
					}
					std::cout << "Please input the maximum grade: ";
					cin >> maxGrade;
					while (maxGrade < 2 || maxGrade > 6)
					{
						std::cout << "Please input valid minimum grade: ";
						cin >> minGrade;
					}
					minMaxValid = minGrade <= maxGrade;
				} while (minGrade > maxGrade);
				count = searchByProgrammingBasics(students, studentsCount, minGrade, maxGrade, found);

				if (count == 0)
				{
					std::cout << "No students found with grade between " << minGrade << " and " << maxGrade << " in Programming Basics!" << endl;
				}
				else
				{
					showStudents(found, count);
				}
				break;
			}
			case 2:
				count = searchForFailed(students, studentsCount, found);

				if (count == 0)
				{
					std::cout << "All students passed their exams!" << endl;
				}
				else
				{
					showStudents(found, count);
				}
				break;
			default:
				if (cmd != 0)
					std::cout << "Invalid command number!" << endl;
				break;
		}
	} while (cmd != 0);
}

unsigned short searchForFailed(const Student students[30], const unsigned short &studentsCount, Student found[30])
{
	unsigned short count = 0;
	for (int i = 0; i < studentsCount; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (students[i].subjects[j].Grade == 2)
			{
				found[count++] = students[i];
				break;
			}
		}
	}

	return count;
}

unsigned short searchByProgrammingBasics(const Student students[30], const unsigned short &studentsCount, int min, int max, Student found[30])
{
	unsigned short count = 0;
	for (int i = 0; i < studentsCount; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (students[i].subjects[j].Name == "PB" && students[i].subjects[j].Grade >= min && students[i].subjects[j].Grade <= max)
			{
				found[count++] = students[i];
				break;
			}
		}
	}
	return count;
}

void showStudents(const Student students[30], const unsigned short &studentsCount)
{
	std::cout << left << setw(10) << "Faculty#"
		<< setw(12) << "EGN"
		<< setw(15) << "First Name"
		<< setw(15) << "Middle Name"
		<< setw(15) << "Last Name"
		<< setw(8) << "Gender"
		<< setw(10) << "Status"
		<< setw(15) << "Subject 1"
		<< setw(6) << "Grade"
		<< setw(15) << "Subject 2"
		<< setw(6) << "Grade"
		<< setw(15) << "Subject 3"
		<< setw(6) << "Grade"
		<< setw(15) << "Subject 4"
		<< setw(6) << "Grade"
		<< setw(15) << "Subject 5"
		<< setw(6) << "Grade"
		<< setw(10) << "Average Grade"
		<< endl;

	for (int i = 0; i < studentsCount; i++)
	{
		std::cout << left << setw(10) << students[i].facultyNumber
			<< setw(12) << students[i].EGN
			<< setw(15) << students[i].firstName
			<< setw(15) << students[i].middleName
			<< setw(15) << students[i].lastName
			<< setw(8) << (students[i].gender == Male ? "Male" : "Female")
			<< setw(10) << (students[i].status == Active ? "Active" : (students[i].status == Dropout ? "Dropout" : "Graduated"));

		for (int j = 0; j < 5; j++)
		{
			std::cout << setw(15) << students[i].subjects[j].Name
				<< setw(6) << students[i].subjects[j].Grade;
		}
		std::cout << setw(10) << std::fixed << std::setprecision(2) << students[i].averageGrade;
		std::cout << endl;
	}
}

void inputStudents(Student students[30], unsigned short &studentsCount)
{
	unsigned short initCount = studentsCount;
	unsigned short countToInput;
	std::cout << "Please input the number of students that you want to add: ";
	cin >> countToInput;
	if (countToInput + studentsCount > 30)
	{
		std::cout << "There is no space left for " << countToInput << " students in the group!" << endl;
		return;
	}
	for (int i = 0; i < countToInput; i++)
	{
		cin.ignore(1000, '\n');
		std::cout << "Student #" << initCount + i + 1 << ":\n";
		std::cout << "First name: ";
		getline(cin, students[initCount + i].firstName);

		std::cout << "Middle name: ";
		getline(cin, students[initCount + i].middleName);

		std::cout << "Last name: ";
		getline(cin, students[initCount + i].lastName);

		std::cout << "Faculty number: ";
		cin.getline(students[initCount + i].facultyNumber, 9);

		std::cout << "EGN: ";
		cin.getline(students[initCount + i].EGN, 11);

		std::cout << "Gender (1 for male and 2 for female): ";
		unsigned short genderInput;
		cin >> genderInput;
		while (genderInput != 1 && genderInput != 2)
		{
			std::cout << "Please enter a valid gender: ";
			cin >> genderInput;
		}
		students[initCount + i].gender = Gender(genderInput);

		std::cout << "Status (1 for active, 2 for dropout and 3 for graduated): ";
		unsigned short statusInput;
		cin >> statusInput;
		while (statusInput < 1 || statusInput > 3)
		{
			std::cout << "Please enter a valid status: ";
			cin >> statusInput;
		}
		students[initCount + i].status = Status(statusInput);

		std::cout << "Subjects: " << endl;
		double gradeSum = 0;
		int gradeCount = 0;
		for (int j = 0; j < 5; j++)
		{
			cin.ignore(1000, '\n');
			std::cout << "	Name: ";
			getline(cin, students[initCount + i].subjects[j].Name);
			std::cout << "	Grade for " << students[initCount + i].subjects[j].Name << " : ";
			cin >> students[initCount + i].subjects[j].Grade;
			while ((students[initCount + i].subjects[j].Grade < 2 || students[initCount + i].subjects[j].Grade > 6) && students[initCount + i].subjects[j].Grade != 0)
			{
				std::cout << "Please enter a valid grade: ";
				cin >> students[initCount + i].subjects[j].Grade;
				
			}
			if (students[initCount + i].subjects[j].Grade != 0)
			{
				gradeCount++;
				gradeSum += students[initCount + i].subjects[j].Grade;
			}
		}
		students[initCount + i].averageGrade = gradeCount == 0 ? 0 : gradeSum / gradeCount;
		studentsCount++;
	}
}

int mainMenu(Student students[30], unsigned short &studentsCount)
{
	unsigned short cmd;
	char facultyNumber[9];
	do
	{
		std::cout << MENU;
		cin >> cmd;
		switch (cmd)
		{
		case 1:
			inputStudents(students, studentsCount);
			break;
		case 2:
			showStudents(students, studentsCount);
			break;
		case 3:
			searchSubmenu(students, studentsCount);
			break;
		case 4:
			orderByName(students, studentsCount);
			showStudents(students, studentsCount);
			break;
		case 5:
			exportStudents(students, studentsCount);
			break;
		case 6:
			importStudents(students, studentsCount);
			break;
		case 7:
			filterByStatus(students, studentsCount);
			break;
		case 8:
			filterByAverageGrades(students, studentsCount);
			break;
		case 9:
			std::cout << "Faculty number: ";
			cin.ignore(1000, '\n');
			cin.getline(facultyNumber, 9);
			updateGrades(students, studentsCount, facultyNumber);
			break;
		case 10:
			std::cout << "Faculty number: ";
			cin.ignore(1000, '\n');
			cin.getline(facultyNumber, 9);
			updateStatus(students, studentsCount, facultyNumber);
			break;
		default:
			if (cmd != 0)
				std::cout << "Invalid command number!" << endl;
			break;
		}
	} while (cmd != 0);
	return 0;
}

int main()
{
	Student students[30];
	unsigned short studentsCount = 0;
	importStudents(students, studentsCount);
	mainMenu(students, studentsCount);
	exportStudents(students, studentsCount);
}