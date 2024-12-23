// StudentsInformationSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const string MENU = "1. Input n-count students.\n2. Show all students.\n3. Search in the students list.\n4. Sort students alphabetically by first name.\n5. Export students to binary file.\n6. Import students from binary file.\n0. Exit.\nPlease input command number: ";

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
};

void showStudents(const Student students[30], const unsigned short& studentsCount);
unsigned short searchForFailed(const Student students[30], const unsigned short& studentsCount, Student found[30]);
unsigned short searchByProgrammingBasics(const Student students[30], const unsigned short& studentsCount, int min, int max, Student found[30]);

void importStudents(Student students[30], unsigned short& studentsCount) {
	ifstream inFile("students.dat", ios::binary);
	if (!inFile) {
		cerr << "Error opening file for reading!" << endl;
		return;
	}

	studentsCount = 0;
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

		studentsCount++;
		if (studentsCount >= 30) break; // Prevent overflow
	}

	inFile.close();
	cout << "Students imported successfully!" << endl;
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
	}

	outFile.close();
	cout << "Students exported successfully!" << endl;
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
		cout << SEARCH_SUBMENU;
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
						cout << "Please input min and max grade again. This time try making the min lesser than the max" <<		endl;
					}
					cout << "Please input the minimum grade: ";
					cin >> minGrade;
					while (minGrade < 2 || minGrade > 6)
					{
						cout << "Please input valid minimum grade: ";
						cin >> minGrade;
					}
					cout << "Please input the maximum grade: ";
					cin >> maxGrade;
					while (maxGrade < 2 || maxGrade > 6)
					{
						cout << "Please input valid minimum grade: ";
						cin >> minGrade;
					}
					minMaxValid = minGrade <= maxGrade;
				} while (minGrade > maxGrade);
				count = searchByProgrammingBasics(students, studentsCount, minGrade, maxGrade, found);

				if (count == 0)
				{
					cout << "No students found with grade between " << minGrade << " and " << maxGrade << " in Programming		Basics!" << endl;
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
					cout << "All students passed their exams!" << endl;
				}
				else
				{
					showStudents(found, count);
				}
				break;
			default:
				if (cmd != 0)
					cout << "Invalid command number!" << endl;
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
	cout << left << setw(10) << "Faculty#"
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
		<< endl;

	for (int i = 0; i < studentsCount; i++)
	{
		cout << left << setw(10) << students[i].facultyNumber
			<< setw(12) << students[i].EGN
			<< setw(15) << students[i].firstName
			<< setw(15) << students[i].middleName
			<< setw(15) << students[i].lastName
			<< setw(8) << (students[i].gender == Male ? "Male" : "Female")
			<< setw(10) << (students[i].status == Active ? "Active" : (students[i].status == Dropout ? "Dropout" : "Graduated"));

		for (int j = 0; j < 5; j++)
		{
			cout << setw(15) << students[i].subjects[j].Name
				<< setw(6) << students[i].subjects[j].Grade;
		}
		cout << endl;
	}
}

void inputStudents(Student students[30], unsigned short &studentsCount)
{
	unsigned short initCount = studentsCount;
	unsigned short countToInput;
	cout << "Please input the number of students that you want to add: ";
	cin >> countToInput;
	if (countToInput + studentsCount > 30)
	{
		cout << "There is no space left for " << countToInput << " students in the group!" << endl;
		return;
	}
	for (int i = 0; i < countToInput; i++)
	{
		cin.ignore(1000, '\n');
		cout << "Student #" << initCount + i + 1 << ":\n";
		cout << "First name: ";
		getline(cin, students[initCount + i].firstName);

		cout << "Middle name: ";
		getline(cin, students[initCount + i].middleName);

		cout << "Last name: ";
		getline(cin, students[initCount + i].lastName);

		cout << "Faculty number: ";
		cin.getline(students[initCount + i].facultyNumber, 9);

		cout << "EGN: ";
		cin.getline(students[initCount + i].EGN, 11);

		cout << "Gender (1 for male and 2 for female): ";
		unsigned short genderInput;
		cin >> genderInput;
		while (genderInput != 1 && genderInput != 2)
		{
			cout << "Please enter a valid gender: ";
			cin >> genderInput;
		}
		students[initCount + i].gender = Gender(genderInput);

		cout << "Status (1 for active, 2 for dropout and 3 for graduated): ";
		unsigned short statusInput;
		cin >> statusInput;
		while (statusInput < 1 || statusInput > 3)
		{
			cout << "Please enter a valid status: ";
			cin >> statusInput;
		}
		students[initCount + i].status = Status(statusInput);

		cout << "Subjects: " << endl;
		for (int j = 0; j < 5; j++)
		{
			cin.ignore(1000, '\n');
			cout << "	Name: ";
			getline(cin, students[initCount + i].subjects[j].Name);
			cout << "	Grade for " << students[initCount + i].subjects[j].Name << " : ";
			cin >> students[initCount + i].subjects[j].Grade;
			while ((students[initCount + i].subjects[j].Grade < 2 || students[initCount + i].subjects[j].Grade > 6) && students[initCount + i].subjects[j].Grade != 0)
			{
				cout << "Please enter a valid grade: ";
				cin >> students[initCount + i].subjects[j].Grade;
			}
		}
		studentsCount++;
	}
}

int mainMenu(Student students[30], unsigned short &studentsCount)
{
	unsigned short cmd;
	do
	{
		cout << MENU;
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
		default:
			if (cmd != 0)
				cout << "Invalid command number!" << endl;
			break;
		}
	} while (cmd != 0);
	return 0;
}

int main()
{
	Student students[30];
	unsigned short studentsCount = 0;
	mainMenu(students, studentsCount);
}