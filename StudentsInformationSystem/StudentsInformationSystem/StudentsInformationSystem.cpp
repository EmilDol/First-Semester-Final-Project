// StudentsInformationSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

using namespace std;

const string MENU = "1. Input n-count students.\n2. Show all students.\n3. Search in the students list.\n4. Sort students alphabetically by first name.\n5. Options for working with external files.\n0. Exit.\nPlease input command number: ";

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
	int Grade;
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

void inputStudents(Student students[30], unsigned short &studentsCount)
{
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
		cout << "Student ¹" << i + 1 << ":\n";
		cout << "First name: ";
		getline(cin, students[studentsCount + i].firstName);

		cout << "Middle name: ";
		getline(cin, students[studentsCount + i].middleName);

		cout << "Last name: ";
		getline(cin, students[studentsCount + i].lastName);

		cout << "Faculty number: ";
		cin.getline(students[studentsCount + i].facultyNumber, 9);

		cout << "EGN: ";
		cin.getline(students[studentsCount + i].EGN, 11);

		cout << "Gender (1 for male and 2 for female): ";
		unsigned short genderInput;
		cin >> genderInput;
		while (genderInput != 1 && genderInput != 2)
		{
			cout << "Please enter a valid gender: ";
			cin >> genderInput;
		}
		students[studentsCount + i].gender = Gender(genderInput);

		cout << "Status (1 for active, 2 for dropout and 3 for graduated): ";
		unsigned short statusInput;
		cin >> statusInput;
		while (statusInput < 1 || statusInput > 3)
		{
			cout << "Please enter a valid status: ";
			cin >> statusInput;
		}
		students[studentsCount + i].status = Status(statusInput);

		cout << "Subjects: " << endl;
		for (int j = 0; j < 5; j++)
		{
			cin.ignore(1000, '\n');
			cout << "	Name: ";
			getline(cin, students[studentsCount + i].subjects[j].Name);
			cout << "	Grade for " << students[studentsCount + i].subjects[j].Name << " : ";
			cin >> students[studentsCount + i].subjects[j].Grade;
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
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
