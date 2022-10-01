#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <random>
#include <sstream>
#include <chrono>
#include "sudokostack.h"
using namespace std;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;

//Function declerations
void easy(string gameboard[9][9], string spaceboard[9][9]);
void medium(string gameboard[9][9], string spaceboard[9][9]);
void hard(string gameboard[9][9], string spaceboard[9][9]);
void display(string gameboard[9][9]);
bool check(string gameboard[9][9] , string gamenumbers[9][9]);
void hint(string gameboard[9][9], string gamenumbers[9][9], string spaceboard[9][9]);
void restart();
void undo();
void redo();
void score(int time, int uHints, int choice, int stepsnum);
int convert(std::chrono::steady_clock::time_point t1, std::chrono::steady_clock::time_point t2);

//Variables used in the project
string gameboard[9][9], spaceboard[9][9], gamereset[9][9], gamenumbers[9][9];
string Input_S, YesorNoChoice, UserInput;

StackArr UndoValue, UndoRow, UndoCol, RedoValue, RedoRow, RedoCol;

int MainMenuChoice, Row, Column, Input_int, Difficulty, UndoCounter = 0, Steps = 0, RedoCounter = 0, uHints = 0;

bool InnerGameLoop = true, MainGameLoop = true, ResetBoard = true;

ifstream FileInput("easy sudoku.txt");

std::chrono::steady_clock::time_point StartTime, EndTime;

void main()
{
	//Getting the Sudoko board form the file
	while (FileInput)
	{
		for (int i = 0; i < 9; i++) {

			for (int j = 0; j < 9; j++) {

				getline(FileInput, gamenumbers[i][j], ',');
			}
		}
	}
	if (FileInput.eof()) {

		FileInput.close();
	}

	//Main loop starts
	do 
	{
		if (ResetBoard) {
		
			uHints = 0, UndoCounter = 0, Steps = 0, RedoCounter = 0, MainMenuChoice = 0;
			cout << "\t\t\t\tWelcome to SUDOKO, the classical game that never dies!\n\n";
			cout << "\t\t\t\t\tEnter 1 if You Are New to The Game\n";
			cout << "\t\t\t\t\t   Enter 2 For a Challenge\n";
			cout << "\t\t\t\t\t  Enter 3 if You Are a MASTER!\n";
			cout << "\t\t\t\t\t   Enter 4 to Exit The Game \n";

			do {

				cin >> MainMenuChoice;

				switch (MainMenuChoice) {

					case 1: {

						Difficulty = 1;
						easy(gameboard, spaceboard);
						break;
					}
					case 2: {

						Difficulty = 2;
						medium(gameboard, spaceboard);
						break;
					}
					case 3: {

						Difficulty = 3;
						hard(gameboard, spaceboard);
						break;
					}
					case 4: {

						exit(EXIT_SUCCESS);
						break;
					}
					default: cout << "Sorry This is Invalid Input!\n";
						break;
				}
			} while (MainMenuChoice < 1 || MainMenuChoice > 3);

			InnerGameLoop = true;
			StartTime = high_resolution_clock::now();
		}

		//Inner game loop starts
		while (InnerGameLoop) {

			int counter = 0;

			cout << "Please Enter 'u' to Undo or 'r' to Redo or 'h' to Get a Hint or 'x' to Restart or 'f' to Return to The Main Menu,"
				 << endl << "or Enter The Number That You Want to Add: ";
			cin >> UserInput;

			//Taking appropiate action depending on the user's input
			if (UserInput == "h" || UserInput == "H") {
				switch (MainMenuChoice) {

					case 1: {

						if (uHints < 3) {

							uHints++;
							hint(gameboard, gamenumbers, spaceboard);

							system("cls");
						}
						else {

							system("cls");
							cout << "Sorry.. You Have Reached The Limit of Using Hints.." << endl;
						}
						break;
					}

					case 2: {

						if (uHints < 6) {
							uHints++;
							hint(gameboard, gamenumbers, spaceboard);

							system("cls");
						}
						else {

							system("cls");
							cout << "Sorry.. You Have Reached The Limit of Using Hints.." << endl;
						}
						break;
					}

					case 3: {

						if (uHints < 9) {

							uHints++;
							hint(gameboard, gamenumbers, spaceboard);

							system("cls");
						}
						else {

							system("cls");
							cout << "Sorry.. You Have Reached The Limit of Using Hints.." << endl;
						}
						break;
					}
				}
			}
			else if (UserInput == "u" || UserInput == "U") {

				undo();
				system("cls");
			}
			else if (UserInput == "r" || UserInput == "R") {

				redo();
				system("cls");
			}
			else if (UserInput == "X" || UserInput == "x") {

				for (int i = 0; i < RedoValue.Length(); i++) {

					UndoValue.Pop();
					UndoRow.Pop();
					UndoCol.Pop();
					RedoValue.Pop();
					RedoRow.Pop();
					RedoCol.Pop();
				}
				system("cls");
				restart();
				ResetBoard = false;
				InnerGameLoop = false;
				break;
			}
			else if (UserInput == "f" || UserInput == "F") {

				for (int i = 0; i < RedoValue.Length(); i++) {

					UndoValue.Pop();
					UndoRow.Pop();
					UndoCol.Pop();
					RedoValue.Pop();
					RedoRow.Pop();
					RedoCol.Pop();
				}
				system("cls");
				InnerGameLoop = false;
				ResetBoard = true;
				break;
			}
			//If the input is not a charectar and is a valid number in the game, the user is asked to enter the row and coloumn of choice
			else {

				Input_S = UserInput;
				istringstream(Input_S) >> Input_int;

				if (Input_int < 10 && Input_int > 0) {

					cout << "Please Enter Row and Column That You Want to Add in 'from 1 to 9': ";
					cin >> Row >> Column;
				
					if (Column < 10 && Row < 10 && Column > 0 && Row > 0) {

						if (spaceboard[Row - 1][Column - 1] == " " || gamenumbers[Row - 1][Column - 1] != gameboard[Row - 1][Column - 1]) {

							UndoValue.Push(Input_int);
							UndoRow.Push(Row);
							UndoCol.Push(Column);
							UndoCounter++;
							Steps++;
							gameboard[Row - 1][Column - 1] = Input_S;
							system("cls");
						}
						else {

							system("cls");
							cout << "Invalide input!\n" << endl;
						}
					}
					else {

						system("cls");
						cout << "Invalide input!\n" << endl;
					}
				}
				else {

					system("cls");
					cout << "Invalide input!\n" << endl;
				}
			}
			
			//display updated board
			display(gameboard);

			//check if the board is full and done
			for (int i = 0; i < 9; i++) {

				for (int j = 0; j < 9; j++) {

					if (gameboard[i][j] != " ") {

						counter++;
					}
				}
			}

			if (counter == 81) {

				if (check(gameboard, gamenumbers) == true) {

					//if the board is correct, score and time are diplayed with options of replay and exit
					do {

						cout << "\n\nCongratulations !!" << endl;
						cout << "The num of steps is: " << Steps << endl;
						EndTime = high_resolution_clock::now();
						int time = convert(StartTime, EndTime);
						score(time, uHints, Difficulty, Steps);
						cout << "\nDo you wanna play again? yes or no?" << endl;
						cin >> YesorNoChoice;
						if (YesorNoChoice != "yes" && YesorNoChoice != "no") {

							cout << "invalid input." << endl;
						}
						system("cls");
					} while (YesorNoChoice != "yes" && YesorNoChoice != "no");

					if (YesorNoChoice == "no") {

						MainGameLoop = false;
						system("exit");
					}
					else {
					
						InnerGameLoop = false;
					}

					system("cls");
					break;
				}
				else if (check(gameboard, gamenumbers) == false) {

					cout << "ooops there are mistake/s\n" << endl;
				}
			}
		}

		if (!ResetBoard) {	
			InnerGameLoop = true;
		}
	} while (MainGameLoop);
}

void easy(string gameboard[9][9], string spaceboard[9][9]) {

	//creates one empty space within every 3x3 square of the board
	int v1, v2, count;

	for (int i = 0; i < 9; i++) {

		for (int j = 0; j < 9; j++) {

			gameboard[i][j] = gamenumbers[i][j];
		}
	}
	
	for (int i = 0; i < 9; i = i + 3) {

		for (int j = 0; j < 9; j = j + 3) {

			count = 0;

			while (count < 1) {

				v1 = (rand() % 3) + i;
				v2 = (rand() % 3) + j;

				if (gameboard[v1][v2] != " ") {

					gameboard[v1][v2] = " ";
					count++;
				}
			}
		}
	}

	for (int r = 0; r < 9; r++) {

		for (int c = 0; c < 9; c++) {

			spaceboard[r][c] = gameboard[r][c];
			gamereset[r][c] = gameboard[r][c];
		}
	}

	system("cls");
	display(gameboard);
}

void medium(string gameboard[9][9], string spaceboard[9][9]) {

	//creates 4 empty spaces within every 3x3 square of the board
	int v1, v2, count;

	for (int i = 0; i < 9; i++) {

		for (int j = 0; j < 9; j++) {

			gameboard[i][j] = gamenumbers[i][j];
		}
	}

	for (int i = 0; i < 9; i = i + 3) {

		for (int j = 0; j < 9; j = j + 3) {

			count = 0;

			while (count < 4) {

				v1 = (rand() % 3) + i;
				v2 = (rand() % 3) + j;

				if (gameboard[v1][v2] != " ") {

					gameboard[v1][v2] = " ";
					count++;
				}
			}
		}
	}

	for (int r = 0; r < 9; r++) {

		for (int c = 0; c < 9; c++) {

			spaceboard[r][c] = gameboard[r][c];
			gamereset[r][c] = gameboard[r][c];
		}
	}

	system("cls");
	display(gameboard);
}

void hard(string gameboard[9][9], string spaceboard[9][9]) {
	
	//creates 6 empty spaces within every 3x3 square of the board
	int v1, v2, count;

	for (int i = 0; i < 9; i++) {

		for (int j = 0; j < 9; j++) {

			gameboard[i][j] = gamenumbers[i][j];
		}
	}

	for (int i = 0; i < 9; i = i + 3) {

		for (int j = 0; j < 9; j = j + 3) {

			count = 0;

			while (count < 6) {

				v1 = (rand() % 3) + i;
				v2 = (rand() % 3) + j;

				if (gameboard[v1][v2] != " ") {

					gameboard[v1][v2] = " ";
					count++;
				}
			}
		}
	}

	for (int r = 0; r < 9; r++) {

		for (int c = 0; c < 9; c++) {

			spaceboard[r][c] = gameboard[r][c];
			gamereset[r][c] = gameboard[r][c];
		}
	}

	system("cls");
	display(gameboard);
}

void display(string gameboard[9][9]) {

	//diplays the Sudoko board
	for (int y = 0; y < 37; y++) {

		cout << "-";
	}

	cout << endl;

	for (int i = 0; i < 9; i++) {
		
		for (int j = 0; j < 9; j++) {

			if (j == 0 || j == 3 || j == 6) {

				cout << "|" << " " << gameboard[i][j] << " ";
			}
			else {

				cout << " " << " " << gameboard[i][j] << " ";
			}
		}

		cout << "|";
		cout << endl;

		if (i == 2 || i == 5) {

			for (int y = 0; y < 37; y++) {

				cout << "-";
			}

			cout << endl;
		}
	}

	for (int y = 0; y < 37; y++) {

		cout << "-";
	}

	cout << endl;
}

void undo() {

	//undos the user last move
	gameboard[UndoRow.Top() - 1][UndoCol.Top() - 1] = " ";
	RedoValue.Push(UndoValue.Top());
	RedoRow.Push(UndoRow.Top());		
	RedoCol.Push(UndoCol.Top());
	UndoRow.Pop();
	UndoCol.Pop();
	UndoValue.Pop();
	Steps++;
	UndoCounter--;
	RedoCounter++;
	if (UndoRow.Length() != 0) {

		if (UndoRow.Top() == RedoRow.Top() && UndoCol.Top() == RedoCol.Top())
		gameboard[UndoRow.Top() - 1][UndoCol.Top() - 1] = to_string(UndoValue.Top());
	}
	system("cls");
	display(gameboard);
}

void redo() {

	//redos the user last move
	if (RedoCounter > 0) {

		gameboard[RedoRow.Top() - 1][RedoCol.Top() - 1] = to_string(RedoValue.Top());
		UndoValue.Push(RedoValue.Top());
		UndoRow.Push(RedoRow.Top());
		UndoCol.Push(RedoCol.Top());
		RedoRow.Pop();
		RedoCol.Pop();
		RedoValue.Pop();
		Steps++;
		UndoCounter++;
		RedoCounter--;
	}
}

void restart() {

	//restarts the level, keeping the same board with the same empty spaces, reseting everything except time
	system("cls");

	for (int i = 0; i < 9; i++) {

		for (int j = 0; j < 9; j++) {

			gameboard[i][j] = gamereset[i][j];
		}
	}

	UndoCounter = 0;
	Steps = 0;
	RedoCounter = 0;
	uHints = 0;

	display(gameboard);
}

void hint(string gameboard[9][9], string gamenumbers[9][9], string spaceboard[9][9])
{

	//Writes a hint for the user to make it easier to solve the board
	bool found = 0;

	for (int m = 0; m < 9; m++) {

		for (int n = 0; n < 9; n++) {

			if (gameboard[m][n] == " ") {

				found = 1;
				gameboard[m][n] = gamenumbers[m][n];
				break;
			}
		}

		if (found) {

			break;
		}
	}

	if (!found) {

		for (int m = 0; m < 9; m++) {

			for (int n = 0; n < 9; n++) {

				if (gameboard[m][n] != gamenumbers[m][n]) {

					found = 1;
					gameboard[m][n] = gamenumbers[m][n];
					break;
				}
			}

			if (found) {

				break;
			}
		}
	}

	for (int m = 0; m < 9; m++) {

		for (int n = 0; n < 9; n++) {

			spaceboard[m][n] = gameboard[m][n];
		}
	}
}

bool check(string gameboard[9][9], string gamenumbers[9][9]) {

	//checks if the whole board is correct, coparing the userboard with the correct full board
	int counterT = 0, counterF = 0;

	for (int i = 0; i < 9; i++) {

		for (int j = 0; j < 9; j++) {

			if (gameboard[i][j] == gamenumbers[i][j]) {

				counterT++;
			}
			else {

				counterF++;
			}
		}
	}

	if (counterT == 81 && counterF == 0) {

		return true;
	}
	else {

		return false;
	}
}

void score(int time, int uHints, int choice, int stepsnum) {
	
	//Calculates the score based on time, how mnay hints used, and number of steps
	int Score = 0;

	switch (choice)
	{
		case 1: {

			Score = (20000 / time) + (5000 / stepsnum) - (uHints * 100);
		}
		case 2: {

			Score = (200000 / time) + (50000 / stepsnum) - (uHints * 100);
		}
		case 3: {

			Score = (2000000 / time) + (500000 / stepsnum) - (uHints * 100);
		}
	}

	cout << "Your score is: " << Score << '\n';
}

int convert(std::chrono::steady_clock::time_point t1, std::chrono::steady_clock::time_point t2) {

	//calculates the time of play and converts it form millisecinds to minutes and seconds
	auto ms_int = duration_cast<milliseconds>(t2 - t1);
	int t_msec = ms_int.count();
	int t_sec = t_msec / 1000;
	int t_min = t_sec / 60;
	t_sec = t_sec - t_min * 60;

	if (t_min > 0) {
	
		cout << "You finished in " << t_min << " minutes and " << t_sec << " seconds\n";
	}
	else {
	
		cout << "You finished in " << t_sec << " seconds! Amazing!\n";
	}

	t_sec = t_sec + t_min * 60;
	return t_sec;
}