/**
	**************************************** CHESS CONSOLE BASED GAME WITH TIME MODS **************************************
	**************************************** DEVELOPED BY ABDUL MATEEN ****************************************
	**************************************** PROGRAMMING FUNDAMENTALS PROJECT *****************************

**/

#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<string>
#include<Windows.h>
#include<fstream>
#include<thread>

//#include "Support.h" //it only include the functions to convert the input character into numbers (0-9)

using namespace std;


// --------------------------------------- CHESS Controlling Functions ------------------------------//
void printBoard(char[][8]);
void initBoard(char[][8]);
void saveGame(int, string[], int);
int loadGame(int slotNo, char board[][8], bool);
void playGame(char board[][8]);
void gotoxy(int xpos, int ypos);
void whitePlayerTimeManager();
void blackPlayerTimeManager();


// --------------------------- Simple Moves and Checkmate and Supporting functions ------------------------------

bool isValidInput(char[][8], int, int, int, int, int, bool);
bool pawnMove(char[][8],int, int, int, int, int);
bool rookMove(char[][8], int, int, int, int, int);
bool knightMove(char[][8], int, int, int, int, int);
bool bishopMove(char[][8], int, int, int, int, int);
bool queenMove(char[][8], int, int, int, int, int);
bool kingMove(char[][8], int, int, int, int, int);
bool whiteKingCheckMate(char[][8], bool[8]);
bool blackKingCheckMate(char[][8], bool [8]);
void inputMove(int &x1, int &y1, int &x2, int &y2, bool &x, int);
bool whiteKingCheck(char[][8]);
bool whiteKingCheck(char[][8]);
bool blackKingCheck(char[][8]);

//-------------------------------- Slatemate and all Supporting functions for slatemate- -------------------------
bool isPawnStale(char[][8], char, int);
bool isBishopStale(char[][8],char,int);
bool isRookStale(char[][8],char,int);
bool isKnightStale(char[][8],char,int);
bool isQueenStale(char[][8],char,int);
bool isKingSlate(char[][8], char, int);
bool whitePlayerSlateMate(char[][8]);
bool blackPlayerSlateMate(char[][8]);

//--------------------------------------- ALL GLOBAL VARIABLES ----------------------------------//

string movesTrack[10000]; //It stores the moves of player 1 and player 2 to store them in file
int movesTrackIndex = 0;  // It stores the index of movesTrack array
string tempMove; //It will maintaine the copy of latest move you made
int totalTime = 15; // stores the total amount of time
int time1; // It stores the total amount of time player 1 will get
int time2; // It stores the total amount of time player 2 will get
int player = 3; // Which player is currently Active 1 for White and 2 for Black
int timeMod = 1; // Stores the info about which time mod you are playing in

int enPassant[] = {2};
int castling[][2] = {{0,0},{0,0}};
bool isWhitekingMoved = false;
bool isblackKingMoved = false;
bool isCastling = false;

int whiteEnPassant = 0;
int blackEnPassant = 0;

int main() {
	bool isGameOver = false;
	char board[8][8];
    initBoard(board);
	int option;
	cout << "Please Choose following: "<<endl;
	cout << "Press 1: For starting new game."<<endl;
	cout << "Press 2: For loading the game."<<endl;
	cin >> option;

	if(option == 2) {
		int type;
		cout << "Press 0: for to load instantaniously"<<endl;
		cout << "Press 1: for to load with replay"<<endl;
		cin >> type;

		while(player == 3) {

			int slot;
			cout << "Please enter the slot of save game: [1 for slot 1, 2 for slot 2, 3 for slot 3]"<<endl;
			cin >> slot ;

			player = loadGame(slot, board, type);
			if(player == 3)
				cout << "File is Empty. Load other one."<<endl;
		}

		playGame(board);
		
	}
	
	else if(option == 1) {

		int mod;

		cout << "Please Select the mod: "<<endl;
		cout << "Press 1: for Classical mode (unlimited time)"<<endl;
		cout << "Press 2: for Rapid mode (Total Time of 10 minutes with +10s increment)"<<endl;
		cout << "Press 3: for Blitz mode (Total time of 5 minutes)"<<endl;
		cout << "Press 4: for Lightening mode (fixed time of 15s for each move)"<<endl;
		cin >> mod;

		if(mod == 1) {
			player = 1;
			printBoard(board);
			playGame(board);
		} else if(mod == 2) {
			thread player1(whitePlayerTimeManager);
			thread player2(blackPlayerTimeManager);
			timeMod = 2;
			time1 = 600;
			time2 = 600;
			printBoard(board);
			player = 1;
			player1.detach();
			player2.detach();
			playGame(board);


		
		} else if(mod == 3) {
			thread player1(whitePlayerTimeManager);
			thread player2(blackPlayerTimeManager);
			timeMod = 3;
			time1 = 300;
			time2 = 300;
			printBoard(board);
			player = 1;
			player1.detach();
			player2.detach();
			playGame(board);


			
		} else if(mod == 4) {
			thread player1(whitePlayerTimeManager);
			thread player2(blackPlayerTimeManager);

			timeMod = 4;
			totalTime = 15;
			printBoard(board);
			player = 1;
			player1.detach();
			player2.detach();
			playGame(board);
		}

		
		
		
	}
}

void whitePlayerTimeManager() {
	while(true) {
		if(player == 1) {
			Sleep(1000);
			if(timeMod == 4)
				time1++;
			if(timeMod == 2 || timeMod == 3)
				time1--;
			if(time1 == 15 && timeMod == 4) {
				cout<<endl << "Time out: Black Player won (0-1)"<<endl;
				exit(0);
			} else if(time1 <= 0 && (timeMod == 2 || timeMod == 3)) {
				cout<<endl << "Time out: Black Player won (0-1)"<<endl;
				exit(0);
			}
			gotoxy(4,0);
			if(timeMod == 4) {
			if((totalTime - time1)>= 10)
				cout << "Time Reamining: " << (totalTime - time1)<<endl;
			else
				cout << "Time Reamining: 0" << (totalTime - time1)<<endl;
				
			} else if(timeMod == 2 || timeMod == 3) {
				cout << "Time Reamining: " << (time1)<<endl;
			}
				gotoxy(25,29);
		}
	}
}

void blackPlayerTimeManager() {
	
	while(true) {
		if(player == 2) {
			Sleep(1000);
			if(timeMod == 4)
				time2++;
			if(timeMod == 2 || timeMod == 3)
				time2--;
			if(time2 >= totalTime && timeMod == 4) {
				cout << endl<<"Time out: White Player won (1-0)"<<endl;
				exit(0);
			} else if(time2 <= 0 && (timeMod == 2 || timeMod == 3)) {
				cout << endl<<"Time out: White Player won (1-0)"<<endl;
				exit(0);
			}

			gotoxy(4,0);
			if(timeMod == 4) {
				if((totalTime - time2)>= 10)
					cout << "Time Reamining: " << (totalTime - time2)<<endl;
				else
					cout << "Time Reamining: 0" << (totalTime - time2)<<endl;
			} else if(timeMod == 2 || timeMod == 3) {
					cout << "Time Reamining: " << time2<<endl;
				}
				gotoxy(25,29);
		}
	}
}

void playGame(char board[][8]) {
	bool isGameOver = false;
	while(!isGameOver) {
			bool isCorrectInput = false;

			while(!isCorrectInput) {
				//White Player Turn........
				if(player == 1) {
					bool locations[8];
					bool isCheck = false;
					if(whiteKingCheck(board)) {
						cout << "White King Under Check!"<<endl;
						isCheck = true;
						if(whiteKingCheckMate(board, locations)) {
							cout << "CheckMate....Black Player Won! (0-1)"<<endl;
							exit(0);
						}
					} else {
						if(whitePlayerSlateMate(board)) {
							cout << "Slatemate! Game Draw" <<endl;
							exit(0);
						}
					}

					cout << "White player make your move: ";
					int x1,x2,y1,y2;
					inputMove(x1,y1,x2,y2, isGameOver, player);
					

					if(isCheck) {
					

						if(isValidInput(board,x1,y1,x2,y2,1,true)) {
							bool flag = true;
							if(x2 == x1-1 && y1 == y2) {
								if(locations[0] != false)
									flag = false;
							} else if(x2 == x1-1 && y2 == y1+1) {
								if(locations[1] != false)
									flag = false;
							} else if(x2 == x1 && y2 == y1+1) {
								if(locations[2] != false)
									flag = false;
							} else if(x2 == x1+1 && y2 == y1+1) {
								if(locations[3] != false)
									flag = false;
							} else if(x2 == x1+1 && y2 == y1) {
								if(locations[4] != false)
									flag = false;
							} else if(x2 == x1+1 && y2 == y1-1) {
								if(locations[5] != false)
									flag = false;
							} else if(x2 == x1 && y2 == y1-1) {
								if(locations[6] != false)
									flag = false;
							} else if(x2 == x1-1 && y2 == y1-1) {
								if(locations[7] != false)
									flag = false;
							} else
								flag = false;

							if(flag) {
								if(!isCastling) {
								char ch = board[x1][y1];
								board[x1][y1] = ' ';
								board[x2][y2] = ch;
								movesTrack[movesTrackIndex] = tempMove;
								movesTrackIndex++;
								} else
									isCastling = false;
							printBoard(board);
							isCorrectInput = true;
							if(timeMod == 4)
								time1 =0;
							if(timeMod == 2)
								time1+= 10;
							player = 2;
							
							
							} else
								cout << "Invalid Input"<<endl;


						} else
							cout << "Invalid Input"<<endl;


					} else {				
						if(isValidInput(board, x1,y1,x2,y2,player,false)) {
							if(board[x2][y2] != 'k') { 
							if(!isCastling) {
								char ch = board[x1][y1];
								board[x1][y1] = ' ';
								board[x2][y2] = ch;
								movesTrack[movesTrackIndex] = tempMove;
								movesTrackIndex++;
							} else
								isCastling = false;
							printBoard(board);
							isCorrectInput = true;
							//Time Code
							if(timeMod == 4)
								time1 =0;
							if(timeMod == 2)
								time1 += 10;


							player = 2;

							

							if(blackKingCheck(board)) {
								bool locations[8];
								if(blackKingCheckMate(board, locations)) {
									cout << "CheckMate....White Player Won! (1-0)";
									getch();
									isGameOver = true;
								}
							}
							} else
								cout << "Invalid Move!"<<endl;

						} else
							cout << "Invalid Move!"<<endl;
					}
				
				}
				//Black Player Turn.......
				else {
					bool locations[8];
					bool isCheck = false;
					if(blackKingCheck(board)) {
						cout << "Black King Under Check!"<<endl;
						isCheck = true;
						if(blackKingCheckMate(board, locations)) {
							cout << "CheckMate....White Player Won! (0-1)"<<endl;
								exit(0);
						}
						
					}  else {
						if(blackPlayerSlateMate(board)) {
							cout << "Slatemate! Game Draw"<<endl;
							exit(0);
						}
					}

					cout << "Black player make your move: ";
					int x1,x2,y1,y2;
					inputMove(x1,y1,x2,y2, isGameOver, player);

					if(isCheck) {
					
						if(isValidInput(board,x1,y1,x2,y2,player,true)) {
							bool flag = true;
							if(x2 == x1-1 && y1 == y2) {
								if(locations[0] != false)
									flag = false;
							} else if(x2 == x1-1 && y2 == y1+1) {
								if(locations[1] != false)
									flag = false;
							} else if(x2 == x1 && y2 == y1+1) {
								if(locations[2] != false)
									flag = false;
							} else if(x2 == x1+1 && y2 == y1+1) {
								if(locations[3] != false)
									flag = false;
							} else if(x2 == x1+1 && y2 == y1) {
								if(locations[4] != false)
									flag = false;
							} else if(x2 == x1+1 && y2 == y1-1) {
								if(locations[5] != false)
									flag = false;
							} else if(x2 == x1 && y2 == y1-1) {
								if(locations[6] != false)
									flag = false;
							} else if(x2 == x1-1 && y2 == y1-1) {
								if(locations[7] != false)
									flag = false;
							} else
								flag = false;

							if(flag) {
								if(!isCastling) {
									char ch = board[x1][y1];
									board[x1][y1] = ' ';
									board[x2][y2] = ch;
									movesTrack[movesTrackIndex] = tempMove;
									movesTrackIndex++;
								} else
									isCastling = false;
							printBoard(board);
							isCorrectInput = true;
							if(timeMod == 4)
								time2 =0;
							if(timeMod == 2)
								time2 += 10;
							player = 1;

							
							
							} else
								cout << "Invalid Input"<<endl;
					
						} else
							cout << "Invalid Move!";
					}
					else {

						if(isValidInput(board, x1, y1, x2, y2, player,false)) {
							if(board[x2][y2] != 'K') {
								if(!isCastling) {
									char ch = board[x1][y1];
									board[x1][y1] = ' ';
									board[x2][y2] = ch;
										movesTrack[movesTrackIndex] = tempMove;
										movesTrackIndex++;
								} else
									isCastling = false;
							printBoard(board);
							isCorrectInput = true;
							if(timeMod == 4)
								time2 =0;
							if(timeMod == 2)
								time2 += 10;
							player = 1;

						

							if(whiteKingCheck(board)) {
								bool locations[8];
								if(whiteKingCheckMate(board, locations)) {
									cout << "CheckMate....Black Player Won! (0-1)";
									getch();
									isGameOver = true;
								}
							}
							} else
								cout << "Invalid Move!"<<endl;
						} else
							cout << "Invalid Move!"<<endl;
					}
				}
			}
		}
}

void inputMove(int &x1, int &y1, int &x2, int &y2, bool &gameOver ,int player) {
	string input;
	input = "9999";
	cin >> input;
	if(input.length() > 2 && input.length() < 5) {
	while(input[0] == 's' && input[1] == 'a' && input[2] == 'v' && (input[3] == '1' || input[3] == '2' || input[3] == '3')) {
		if(input[3] == '1')
			saveGame(1, movesTrack, movesTrackIndex);
		else if(input[3] == '2')
			saveGame(2, movesTrack, movesTrackIndex);
		else if(input[3] == '3')
			saveGame(3, movesTrack, movesTrackIndex);
		cout << "Game Saved!"<<endl;
		cout << "Make your move now: ";
		cin >> input;
	}

	if(input == "resi") {
		if(player == 1) {
			cout << "Black Player won (0-1)"<<endl;
			exit(0);
		} else {
			cout << "White Player won (1-0)"<<endl;
			exit(0);
		}
	}

	if(input == "shak") {
		if(player == 1) {
			cout << "Black Player do you want to hand shake? [y,n]";
			char ch;
			cin >> ch;
			if(ch == 'y') {
				gameOver = true;
				cout << "Game Draw!"<<endl;
				exit(1);
			}
		} else {
			cout << "White Player do you want to hand shake? [y,n]";
			char ch;
			cin >> ch;
			if(ch == 'y') {
				gameOver = true;
				cout << "Game Draw"<<endl;
				exit(0);
			}
		}
	}
	tempMove = input;
	int b1 = charToNum(input[0]);
	int a1 = asciiConverter(input[1]) - 1;
	int b2 = charToNum(input[2]);
	int a2 = asciiConverter(input[3]) - 1;

	a1 = 7 - a1;
	a2 = 7 - a2;
	x1 = a1;
	x2 = a2;
	y1 = b1;
	y2 = b2;
	}

}

void saveGame(int slotNo, string moves[], int size) {
	ofstream file;

	if(slotNo == 1)
		file.open("ChessSlot1.txt");
	else if(slotNo == 2)
		file.open("ChessSlot2.txt");
	else if(slotNo == 3)
		file.open("ChessSlot3.txt");

	if(file.is_open()) {
		for(int i=0; i<size; i++) {
			
			file<<moves[i]<< endl;
		}

		file.close();
	} 
	else 
		cout<<"Unable to open file";
}

int loadGame(int slotNo, char board[][8], bool loadWithReaply) {
	ifstream file;

	if(slotNo == 1)
		file.open("ChessSlot1.txt");
	else if(slotNo == 2)
		file.open("ChessSlot2.txt");
	else if(slotNo == 3)
		file.open("ChessSlot3.txt");

	string input;
	int player = 1;
	printBoard(board);
	int count = 0;

    while(getline(file, input)) {
		int b1 = charToNum(input[0]);
		int a1 = asciiConverter(input[1]) - 1;
		int b2 = charToNum(input[2]);
		int a2 = asciiConverter(input[3]) - 1;
		a1 = 7 - a1;
		a2 = 7 - a2;
		int x1 = a1;
		int x2 = a2;
		int y1 = b1;
		int y2 = b2;

		
		char ch = board[x1][y1];
		board[x1][y1] = ' ';
		board[x2][y2] = ch;
		if(loadWithReaply) {
			Sleep(1100);
			printBoard(board);
		}
		if(player == 1)
			player = 2;
		else
			player = 1;
		count++;
     }

	if(!loadWithReaply)
		printBoard(board);

	if(count == 0) {
		system("cls");
		return 3;
	}

	return player;

}

bool isValidInput(char board[][8], int x1, int y1, int x2, int y2, int player, bool isCheck) {
	bool isValidInput = true;

	if(player == 1) { //White Player
		if(isCheck)
			isValidInput = kingMove(board,x1,y1,x2,y2,player);
		else {
			if(x1 < 0 || x1 > 7 || x2 < 0 || x2 > 7 || y1 < 0 || y1 > 7 || y2 > 7 || y2 < 0)
				isValidInput = false;
			else if(x1 == x2 && y1 == y2)
				isValidInput = false;
			else if(board[x1][y1] == ' ')
				isValidInput = false; 
			else if(board[x1][y1] != 'P' && board[x1][y1] != 'R' && board[x1][y1] != 'N' &&
				board[x1][y1] != 'B' && board[x1][y1] != 'Q' && board[x1][y1] != 'K')
				isValidInput = false;
			else if(board[x1][y1] == 'P') {
				isValidInput = pawnMove(board, x1,y1,x2,y2,player);
				if(isValidInput) {
					if(board[x1][y1] == 'P') {
						if(board[x2][y2] == 'K' || board[x2][y2] == 'Q' || board[x2][y2] == 'N' || board[x2][y2] == 'B' || board[x2][y2] == 'R' || board[x2][y2] == 'P')
							isValidInput = false;
					}
				}
				if(isValidInput) {
					if(x2 == 0) {
						char ch = 'X';
						while(ch != 'B' && ch != 'R' && ch != 'Q' && ch != 'N') {
							cout << "Upgrade pawn-> B: for bishop, R: for Rook, Q: for Queen, N: for knigt -> ";
							ch = getche();
							cout <<endl;
						}

						board[x1][y1] = ch;

					}
				}
			}
			else if(board[x1][y1] == 'R') {
				isValidInput = rookMove(board,x1,y1,x2,y2,player);
				if(isValidInput) {
					if(x1 == 7 && y1 == 7)
						castling[0][1] = 1;
					else if(x1 == 7 && y1 == 0)
						castling[0][0] = 1;
				}

			}
			else if(board[x1][y1] == 'N')
				isValidInput = knightMove(board, x1, y1, x2, y2, player);
			else if(board[x1][y1] == 'B')
				isValidInput = bishopMove(board, x1, y1, x2, y2, player);
			else if(board[x1][y1] == 'Q')
				isValidInput = queenMove(board,x1,y1,x2,y2,player);
			else if(board[x1][y1] == 'K') {
				isValidInput = kingMove(board,x1,y1,x2,y2,player);
				if(isValidInput && !isWhitekingMoved)
					isWhitekingMoved = true;
				if(!isValidInput && !isWhitekingMoved) {
					if(x1 == x2 && y2 == y1+2) {
						if(board[x2][y1+1] == ' ' && board[x2][y1+1] == ' ' && castling[0][1] == 0) {
							isValidInput = true;
							board[x2][7] = 'K';
							board[x2][6] = 'R';
							board[x1][y1] = ' ';
							movesTrack[movesTrackIndex++] = "h1g1";
							movesTrack[movesTrackIndex++] = "e1h1";
							isCastling = true;
							isWhitekingMoved = true;
							castling[0][1] = 1;
						}						
					} else if(x1 == x2 && y2 == y1-2){
						if(board[x2][y1-1] == ' ' && board[x2][y1-2] == ' ' && board[x2][y1-3] && castling[0][0] == 0) {
							isValidInput = true;
							board[x2][y1-3] = 'K';
							board[x2][y1-2] = 'R';
							board[x2][y1-4] = ' ';
							board[x1][y1] = ' ';
							movesTrack[movesTrackIndex++] = "e1b1";
							movesTrack[movesTrackIndex++] = "a1c1";
							isCastling = true;
							castling[0][0] = 1;
							isWhitekingMoved = true;
						}		
					}
				}
			}
			else
				isValidInput = false;
		} 
	}
	else { // Black player
		if(isCheck)
			isValidInput = kingMove(board,x1,y1,x2,y2,player);
		
		else {
			if(x1 < 0 || x1 > 7 || x2 < 0 || x2 > 7 || y1 < 0 || y1 > 7 || y2 > 7 || y2 < 0)
				isValidInput = false;
			else if(x1 == x2 && y1 == y2)
				isValidInput = false;
			else if(board[x1][y1] == ' ')
				isValidInput = false; 
			else if(board[x1][y1] != 'p' && board[x1][y1] != 'r' && board[x1][y1] != 'n' &&
				board[x1][y1] != 'b' && board[x1][y1] != 'q' && board[x1][y1] != 'k')
				isValidInput = false;
			else if(board[x1][y1] == 'p') {
				isValidInput = pawnMove(board,x1,y1,x2,y2,player);
				if(isValidInput) {
					if(board[x1][y1] == 'p') {
						if(board[x2][y2] == 'k' || board[x2][y2] == 'q' || board[x2][y2] == 'n' || board[x2][y2] == 'b' || board[x2][y2] == 'r' || board[x2][y2] == 'p')
							isValidInput = false;
					}
				}
				if(isValidInput) {
					if(x2 == 7) {
						char ch = 'X';
						while(ch != 'b' && ch != 'r' && ch != 'q' && ch != 'n') {
							cout << "Upgrade pawn-> b: for bishop, r: for Rook, q: for Queen, n: for knigt -> ";
							ch = getche();
							cout <<endl;
						}

						board[x1][y1] = ch;
					}
				}
			}
			else if(board[x1][y1] == 'r')
				isValidInput = rookMove(board,x1,y1,x2,y2,player);
			else if(board[x1][y1] == 'n')
				isValidInput = knightMove(board, x1, y1, x2, y2, player);
			else if(board[x1][y1] == 'b')
				isValidInput = bishopMove(board, x1, y1, x2, y2, player);
			else if(board[x1][y1] == 'q')
				isValidInput = queenMove(board,x1,y1,x2,y2,player);
			else if(board[x1][y1] == 'k') {
				isValidInput = kingMove(board,x1,y1,x2,y2,player);
				if(isValidInput && !isblackKingMoved)
					isblackKingMoved = true;
				if(!isValidInput && !isblackKingMoved) {
					if(x1 == x2 && y2 == y1+2) {
						if(board[x2][y1+1] == ' ' && board[x2][y1+1] == ' ' && castling[1][1] == 0) {
							isValidInput = true;
							board[x2][7] = 'k';
							board[x2][6] = 'r';
							board[x1][y1] = ' ';
							movesTrack[movesTrackIndex++] = "h8g8";
							movesTrack[movesTrackIndex++] = "e8h8";
							isCastling = true;
							isblackKingMoved = true;
							castling[1][1] = 1;
						}						
					} else if(x1 == x2 && y2 == y1-2){
						if(board[x2][y1-1] == ' ' && board[x2][y1-2] == ' ' && board[x2][y1-3] && castling[1][0] == 0) {
							isValidInput = true;
							board[x2][y1-3] = 'k';
							board[x2][y1-2] = 'r';
							board[x2][y1-4] = ' ';
							board[x1][y1] = ' ';
							movesTrack[movesTrackIndex++] = "e8b8";
							movesTrack[movesTrackIndex++] = "a8c8";
							isCastling = true;
							castling[1][0] = 1;
							isblackKingMoved = true;
						}		
					}
				}
			}
			else
				isValidInput = false;
		}
	}

	return isValidInput;
}

bool pawnMove(char board[][8], int x1, int y1, int x2, int y2, int player) {
	bool isValidInput = true;
	if(player == 1) {
		if(x2 >= x1 || y2 > y1+1 || y2 < y1-1)
			isValidInput = false;
		else if(x1 == 6 && x2 == x1-2 && y1 == y2) {
			if(x2 == x1-2 && board[x1-1][y2] != ' ')
				isValidInput = false;
		}

		else if(x2 < (x1-1) && y1 == y2 || x2 < (x1-1) && y1 != y2)
			isValidInput = false;
		
		else if(x2 == (x1-1) && y1!=y2) {
			if(board[x2][y2] == ' ')
				isValidInput = false;
			else if(board[x2][y2] == 'P' && board[x2][y2] == 'R' && board[x2][y2] == 'N' &&
				board[x2][y2] == 'B' && board[x2][y2] == 'Q' && board[x2][y2] == 'K' && board[x2][y2] == 'k')
				isValidInput = false;
		} else if(x2 == x1-1 && y1==y2 && board[x2][y2] != ' ')
			isValidInput = false;

		if(isValidInput)
			if(board[x2][y2] == 'k')
				isValidInput = false;
	}
	else if(player == 2) {
		if(x2 <= x1 || y2 > y1+1 || y2 < y1-1)
			isValidInput = false;
		else if(x1 == 1 && x2 == x1 + 2 && y1 == y2) {
			if(x2 == x1+2 && board[x1+1][y2] != ' ')
				isValidInput = false;
		}

		else if(x2 > (x1+1) && y1 == y2 || x2 > (x1+1) && y1!=y2)
			isValidInput = false;
		else if(x2 == (x1+1) && y1!=y2) {
			if(board[x2][y2] == ' ')
				isValidInput = false;
			else if(board[x2][y2] == 'p' && board[x2][y2] == 'r' && board[x2][y2] == 'n' &&
				board[x2][y2] == 'b' && board[x2][y2] == 'q' && board[x2][y2] == 'k' && board[x2][y2] == 'K')
				isValidInput = false;
		} else if(x2 == x1+1 && y1==y2 && board[x2][y2] != ' ')
			isValidInput = false;

		if(isValidInput)
			if(board[x2][y2] == 'K')
				isValidInput = false;
	}

	return isValidInput;
}

bool rookMove(char board[][8], int x1, int y1, int x2, int y2, int player) {
	bool isValidInput = true;
	if(player == 1) {
		if(x1 == x2 && y1 == y2)
			isValidInput = false;
		else if(x1 == x2) {
			int blockedYIndex = -1;
			if(y2 > y1) {
				for(int i=y1+1; i < 8; i++) {
					if(board[x1][i] == 'P' || board[x1][i] == 'R' || board[x1][i] == 'N' ||
					board[x1][i] == 'B' || board[x1][i] == 'Q' || board[x1][i] == 'K') {
						blockedYIndex = i;
						if(y2 >= blockedYIndex && blockedYIndex != -1)
							isValidInput = false;
						i = 9;
					} else if(board[x1][i] == 'p' || board[x1][i] == 'r' || board[x1][i] == 'n' ||
						board[x1][i] == 'b' || board[x1][i] == 'q' || board[x1][i] == 'k') {
						blockedYIndex = i;
						if(y2 > blockedYIndex && blockedYIndex != -1)
							isValidInput = false;
						i = 9;
					}
				}

				

			} else if(y2 < y1) {
				for(int i=y1-1; i > -1; i--) {
					if(board[x1][i] == 'P' || board[x1][i] == 'R' || board[x1][i] == 'N' ||
					board[x1][i] == 'B' || board[x1][i] == 'Q' || board[x1][i] == 'K') {
						blockedYIndex = i;
						
						if(y2 <= blockedYIndex && blockedYIndex != -1)
							isValidInput = false;
						i = -1;
					} else if(board[x1][i] == 'p' || board[x1][i] == 'r' || board[x1][i] == 'n' ||
						board[x1][i] == 'b' || board[x1][i] == 'q' || board[x1][i] == 'k') {
						blockedYIndex = i;
						
						if(y2 < blockedYIndex && blockedYIndex != -1)
							isValidInput = false;
						i = -1;
					}
				}
			}
		}

		//Done
		else if(y1 == y2) {
			int blockedXIndex = -1;
			if(x2 < x1) {
				for(int i=x1-1; i > -1; i--) {
					if(board[i][y1] == 'P' || board[i][y1] == 'R' || board[i][y1] == 'N' ||
					board[i][y1] == 'B' || board[i][y1] == 'Q' || board[i][y1] == 'K') {
						blockedXIndex = i;
						if(x2 <= blockedXIndex && blockedXIndex != -1)
							isValidInput = false;
						i=-1;
					} else if(board[i][y1] == 'p' || board[i][y1] == 'r' || board[i][y1] == 'n' ||
					board[i][y1] == 'b' || board[i][y1] == 'q' || board[i][y1] == 'k') {
						blockedXIndex = i;
						if(x2 < blockedXIndex && blockedXIndex != -1)
							isValidInput = false;
						i=-1;
					}
				}

			} else if(x2 > x1) {
				
				for(int i=x1+1; i < 8; i++) {
					if(board[i][y1] == 'P' || board[i][y1] == 'R' || board[i][y1] == 'N' ||
					board[i][y1] == 'B' || board[i][y1] == 'Q' || board[i][y1] == 'K') {
						blockedXIndex = i;
						if(x2 >= blockedXIndex && blockedXIndex != -1)
							isValidInput = false;
						i = 9;
					} else if(board[i][y1] == 'p' || board[i][y1] == 'r' || board[i][y1] == 'n' ||
					board[i][y1] == 'b' || board[i][y1] == 'q' || board[i][y1] == 'k') {
						blockedXIndex = i;
						if(x2 > blockedXIndex && blockedXIndex != -1)
							isValidInput = false;
						i = 9;
					}
				}
			}
		}
		else
			isValidInput = false;

		//Player 2
	} else {
		if(x1 == x2 && y1 == y2)
			isValidInput = false;
		else if(x1 == x2) {
			int blockedYIndex = -1;
			if(y2 > y1) {
				for(int i=y1+1; i < 8; i++) {
					if(board[x1][i] == 'p' || board[x1][i] == 'r' || board[x1][i] == 'n' ||
					board[x1][i] == 'b' || board[x1][i] == 'q' || board[x1][i] == 'k') {
						blockedYIndex = i;
						if(y2 >= blockedYIndex && blockedYIndex != -1)
							isValidInput = false;
						i = 9;
					} else if(board[x1][i] == 'P' || board[x1][i] == 'R' || board[x1][i] == 'N' ||
					board[x1][i] == 'B' || board[x1][i] == 'Q' || board[x1][i] == 'K') {
						blockedYIndex = i;
						if(y2 > blockedYIndex && blockedYIndex != -1)
							isValidInput = false;
						i = 9;
					}
				}

			} else if(y2 < y1) {
				for(int i=y1-1; i > -1; i--) {
					if(board[x1][i] == 'p' || board[x1][i] == 'r' || board[x1][i] == 'n' ||
					board[x1][i] == 'b' || board[x1][i] == 'q' || board[x1][i] == 'k') {
						blockedYIndex = i;
						if(y2 <= blockedYIndex && blockedYIndex != -1)
							isValidInput = false;
						i = -1;
					} else if(board[x1][i] == 'P' || board[x1][i] == 'R' || board[x1][i] == 'N' ||
					board[x1][i] == 'B' || board[x1][i] == 'Q' || board[x1][i] == 'K') {
						blockedYIndex = i;
						if(y2 < blockedYIndex && blockedYIndex != -1)
							isValidInput = false;
						i = -1;
					}
				}
			}
		}

		//Done
		else if(y1 == y2) {
			int blockedXIndex = -1;
			if(x2 < x1) {
				for(int i=x1-1; i > -1; i--) {
					if(board[i][y1] == 'p' || board[i][y1] == 'r' || board[i][y1] == 'n' ||
					board[i][y1] == 'b' || board[i][y1] == 'q' || board[i][y1] == 'k') {
						blockedXIndex = i;
						if(x2 <= blockedXIndex && blockedXIndex != -1)
							isValidInput = false;
						i=-1;
					} else if(board[i][y1] == 'P' || board[i][y1] == 'R' || board[i][y1] == 'N' ||
					board[i][y1] == 'B' || board[i][y1] == 'Q' || board[i][y1] == 'K') {
						blockedXIndex = i;
						if(x2 < blockedXIndex && blockedXIndex != -1)
							isValidInput = false;
						i=-1;
					}
				}

			} else if(x2 > x1) {
				
				for(int i=x1+1; i < 8; i++) {
					if(board[i][y1] == 'p' || board[i][y1] == 'r' || board[i][y1] == 'n' ||
					board[i][y1] == 'b' || board[i][y1] == 'q' || board[i][y1] == 'k') {
						blockedXIndex = i;
						if(x2 >= blockedXIndex && blockedXIndex != -1)
							isValidInput = false;
						i = 9;
					} else if(board[i][y1] == 'P' || board[i][y1] == 'R' || board[i][y1] == 'N' ||
					board[i][y1] == 'B' || board[i][y1] == 'Q' || board[i][y1] == 'K') {
						blockedXIndex = i;
						if(x2 > blockedXIndex && blockedXIndex != -1)
							isValidInput = false;
						i = 9;
					}
				}
			}
		}
		else
			isValidInput = false;

	}
	return isValidInput;
}

bool knightMove(char board[][8], int x1, int y1, int x2, int y2, int player) {
	bool isValidInput = true;
	if(player == 1) {
		

		if(x2 == x1+2 && (y2 == y1+1 || y2 == y1-1)) {
				if(board[x2][y2] == 'P' || board[x2][y2] == 'R' || board[x2][y2] == 'N' ||
					board[x2][y2] == 'B' || board[x2][y2] == 'Q' || board[x2][y2] == 'K')
					isValidInput = false;
		}

		else if(x2 == x1-2 && (y2 == y1+1 || y2 == y1-1)) {
			if(board[x2][y2] == 'P' || board[x2][y2] == 'R' || board[x2][y2] == 'N' ||
					board[x2][y2] == 'B' || board[x2][y2] == 'Q' || board[x2][y2] == 'K')
					isValidInput = false;
		}
		else if(y2 == y1-2 && (x2 == x1+1 || x2 ==  x1-1)) {
			if(board[x2][y2] == 'P' || board[x2][y2] == 'R' || board[x2][y2] == 'N' ||
					board[x2][y2] == 'B' || board[x2][y2] == 'Q' || board[x2][y2] == 'K')
					isValidInput = false;
		} 
		else if(y2 == y1+2 && (x2 == x1+1 || x2 ==  x1-1)) {
			if(board[x2][y2] == 'P' || board[x2][y2] == 'R' || board[x2][y2] == 'N' ||
					board[x2][y2] == 'B' || board[x2][y2] == 'Q' || board[x2][y2] == 'K')
					isValidInput = false;
		}
		else
			isValidInput = false;

		if(isValidInput)
			if(board[x2][y2] == 'k')
				isValidInput = false;

	} 
	//BLACK PLAYER
	else {
	



		if(x2 == x1+2 && (y2 == y1+1 || y2 == y1-1)) {
				if(board[x2][y2] == 'p' || board[x2][y2] == 'r' || board[x2][y2] == 'n' ||
					board[x2][y2] == 'b' || board[x2][y2] == 'q' || board[x2][y2] == 'k')
					isValidInput = false;
		}

		else if(x2 == x1-2 && (y2 == y1+1 || y2 == y1-1)) {
			if(board[x2][y2] == 'p' || board[x2][y2] == 'r' || board[x2][y2] == 'n' ||
					board[x2][y2] == 'b' || board[x2][y2] == 'q' || board[x2][y2] == 'k')
					isValidInput = false;
		}
		else if(y2 == y1-2 && (x2 == x1+1 || x2 ==  x1-1)) {
			if(board[x2][y2] == 'p' || board[x2][y2] == 'r' || board[x2][y2] == 'n' ||
					board[x2][y2] == 'b' || board[x2][y2] == 'q' || board[x2][y2] == 'k')
					isValidInput = false;
		} 
		else if(y2 == y1+2 && (x2 == x1+1 || x2 ==  x1-1)) {
			if(board[x2][y2] == 'p' || board[x2][y2] == 'r' || board[x2][y2] == 'n' ||
					board[x2][y2] == 'b' || board[x2][y2] == 'q' || board[x2][y2] == 'k')
					isValidInput = false;
		}
		else
			isValidInput = false;

		if(isValidInput)
			if(board[x2][y2] == 'K')
				isValidInput = false;


	}
	return isValidInput;
}

bool bishopMove(char board[][8], int x1, int y1, int x2, int y2, int player) {
	bool isValidInput = true;
	int blockedXIndex = -1;
	int blockedYIndex = -1;
	if(player == 1) {
		if((x1+y1) == (x2+y2) || x1-y1 == x2-y2) {
			//Checking diagonally left up
			if(x2 < x1 && y2 < y1) {
				int y = y1-1;
				for(int i=x1-1; i > -1 && y > -1; i--) {
				
					if(board[i][y] == 'P' || board[i][y] == 'R' || board[i][y] == 'N' ||
						board[i][y] == 'B' || board[i][y] == 'Q' || board[i][y] == 'K') {
							blockedXIndex = i;
							blockedYIndex = y;
							
							if(x2 <= blockedXIndex && y2 <= blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = -1;
					} else if(board[i][y] == 'p' || board[i][y] == 'r' || board[i][y] == 'n' ||
						board[i][y] == 'b' || board[i][y] == 'q' || board[i][y] == 'k') {
							blockedXIndex = i;
							blockedYIndex = y;
							
							if(x2 < blockedXIndex && y2 < blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = -1;
					}

					y--;
				}
			}
			//Checking diagonally right up

			else if(x2 < x1 && y2 > y1) {
				int y = y1+1;
				for(int i=x1-1; i > -1 && y < 8; i--) {
					
					if(board[i][y] == 'P' || board[i][y] == 'R' || board[i][y] == 'N' ||
						board[i][y] == 'B' || board[i][y] == 'Q' || board[i][y] == 'K') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 <= blockedXIndex && y2 >= blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = -1;
					} else if(board[i][y] == 'p' || board[i][y] == 'r' || board[i][y] == 'n' ||
						board[i][y] == 'b' || board[i][y] == 'q' || board[i][y] == 'k') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 < blockedXIndex && y2 > blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
							isValidInput = false;
							i = -1;
					}
					y++;
				}
			}

			//Checking diagonally left down

			else if(x2 > x1 && y2 < y1) {
				int y = y1-1;
				for(int i=x1+1; i<8 && y > -1; i++) {
					
					if(board[i][y] == 'P' || board[i][y] == 'R' || board[i][y] == 'N' ||
						board[i][y] == 'B' || board[i][y] == 'Q' || board[i][y] == 'K') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 >= blockedXIndex && y2 <= blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = 8;
							y = -1;
					} else if(board[i][y] == 'p' || board[i][y] == 'r' || board[i][y] == 'n' ||
						board[i][y] == 'b' || board[i][y] == 'q' || board[i][y] == 'k') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 > blockedXIndex && y2 < blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = 8;
							y = -1;
					}
					y--;
				}
			}

			else if(x2 > x1 && y2 > y1) {
				int y = y1+1;
				for(int i=x1+1; i < 8 && y < 8; i++) {
					if(board[i][y] == 'P' || board[i][y] == 'R' || board[i][y] == 'N' ||
						board[i][y] == 'B' || board[i][y] == 'Q' || board[i][y] == 'K') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 >= blockedXIndex && y2 >= blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = 8;
							y = 8;
					} else if(board[i][y] == 'p' || board[i][y] == 'r' || board[i][y] == 'n' ||
						board[i][y] == 'b' || board[i][y] == 'q' || board[i][y] == 'k') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 > blockedXIndex && y2 > blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = 8;
							y = 8;
					}
					y++;
				}
			} 
			else
				isValidInput = false;


		} else
			isValidInput = false;
	}

	//BLACK PLAYER
	else {

		if((x1+y1) == (x2+y2) || x1-y1 == x2-y2) {
			//Checking diagonally left up
			if(x2 < x1 && y2 < y1) {
				int y = y1-1;
				for(int i=x1-1; i > -1 && y > -1; i--) {
				
					if(board[i][y] == 'p' || board[i][y] == 'r' || board[i][y] == 'n' ||
						board[i][y] == 'b' || board[i][y] == 'q' || board[i][y] == 'k') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 <= blockedXIndex && y2 <= blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = -1;
					} else if(board[i][y] == 'P' || board[i][y] == 'R' || board[i][y] == 'N' ||
						board[i][y] == 'B' || board[i][y] == 'Q' || board[i][y] == 'K') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 < blockedXIndex && y2 < blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = -1;
					}
					y--;
				}
			}
			//Checking diagonally right up

			else if(x2 < x1 && y2 > y1) {
				int y = y1+1;
				for(int i=x1-1; i > -1 && y < 8; i--) {
					
					if(board[i][y] == 'p' || board[i][y] == 'r' || board[i][y] == 'n' ||
						board[i][y] == 'b' || board[i][y] == 'q' || board[i][y] == 'k') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 <= blockedXIndex && y2 >= blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = -1;
					} else if(board[i][y] == 'P' || board[i][y] == 'R' || board[i][y] == 'N' ||
						board[i][y] == 'B' || board[i][y] == 'Q' || board[i][y] == 'K') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 < blockedXIndex && y2 > blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = -1;
					}
					y++;
				}
			}

			//Checking diagonally left down

			else if(x2 > x1 && y2 < y1) {
				int y = y1-1;
				for(int i=x1+1; i<8 && y > -1; i++) {
					
					if(board[i][y] == 'p' || board[i][y] == 'r' || board[i][y] == 'n' ||
						board[i][y] == 'b' || board[i][y] == 'q' || board[i][y] == 'k') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 >= blockedXIndex && y2 <= blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = 8;
							y = -1;
					} else if(board[i][y] == 'P' || board[i][y] == 'R' || board[i][y] == 'N' ||
						board[i][y] == 'B' || board[i][y] == 'Q' || board[i][y] == 'K') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 > blockedXIndex && y2 < blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = 8;
							y = -1;
					}
					y--;
				}
			}

			else if(x2 > x1 && y2 > y1) {
				int y = y1+1;
				for(int i=x1+1; i < 8 && y < 8; i++) {
					if(board[i][y] == 'p' || board[i][y] == 'r' || board[i][y] == 'n' ||
						board[i][y] == 'b' || board[i][y] == 'q' || board[i][y] == 'k') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 >= blockedXIndex && y2 >= blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = 8;
							y = 8;
					} else if(board[i][y] == 'P' || board[i][y] == 'R' || board[i][y] == 'N' ||
						board[i][y] == 'B' || board[i][y] == 'Q' || board[i][y] == 'K') {
							blockedXIndex = i;
							blockedYIndex = y;
							if(x2 > blockedXIndex && y2 > blockedYIndex && blockedXIndex != -1 && blockedYIndex != -1)
								isValidInput = false;
							i = 8;
							y = 8;
					}
					y++;
				}
			}
			else
				isValidInput = false;


		} else
			isValidInput = false;
	}

	return isValidInput;
}

bool queenMove(char board[][8], int x1, int y1, int x2, int y2, int player) {
	bool isValidInput = true;
	if(!rookMove(board, x1,y1,x2,y2,player))
		isValidInput = bishopMove(board,x1,y1,x2,y2,player);
	return isValidInput;
}

bool kingMove(char board[][8], int x1, int y1, int x2, int y2, int player) {
	bool isValidInput = true;
	if(player == 1) {
		if(x2 > x1+1)
			isValidInput = false;
		else if(x2 < x1-1)
			isValidInput = false;
		else if(y2 > y1+1)
			isValidInput = false;
		else if(y2 < y1-1)
			isValidInput  = false;
		else if(board[x2][y2] == 'P' || board[x2][y2] == 'R' || board[x2][y2] == 'N' ||
				board[x2][y2] == 'B' || board[x2][y2] == 'Q' || board[x2][y2] == 'K' || board[x2][y2] == 'k')
				isValidInput = false;
		if(isValidInput) {

			if(x2 < x1 && y1==y2) {
				if(board[x1-2][y1] == 'k')
					isValidInput = false;
				else if(board[x1-2][y1+2] == 'k')
					isValidInput = false;
				else if(board[x1-2][y1-2] == 'k')
					isValidInput = false;
			} 

			else if(x2 < x1 && y2 > y1) {
				if(board[x1-2][y1] == 'k')
					isValidInput = false;
			else if(board[x1-2][y1-2] == 'k')
				isValidInput = false;
			else if(board[x1-2][y1+2] == 'k')
				isValidInput = false;
			else if(board[x1][y1+2] == 'k')
				isValidInput = false;
			else if(board[x1+2][y1+2] == 'k')
				isValidInput = false;
			}

			else if(x2 == x1 && y2 > y1) {
				if(board[x1-2][y1+2] == 'k')
					isValidInput = false;
				else if(board[x1][y1+2] == 'k')
					isValidInput = false;
				else if(board[x1+2][y1+2] == 'k')
					isValidInput = false;
			}

			else if(x2 > x1 && y2 > y1) {
				if(board[x1-2][y1+2] == 'k')
					isValidInput = false;
				else if(board[x1][y1+2] == 'k')
					isValidInput = false;
				else if(board[x1+2][y1+2] == 'k')
					isValidInput = false;
				else if(board[x1+2][y1] == 'k')
					isValidInput = false;
				else if(board[x1+2][y1-2] == 'k')
					isValidInput = false;
			}

			else if(x2 > x1 && y1==y2) {
				if(board[x1+2][y1] == 'k')
					isValidInput = false;
				else if(board[x1+2][y1+2] == 'k')
					isValidInput = false;
				else if(board[x1+2][y1-2] == 'k')
					isValidInput = false;
			}

			else if(x2 > x1 && y2 < y1) {
				if(board[x1+2][y1+2] == 'k')
					isValidInput = false;
				else if(board[x1+2][y1] == 'k')
					isValidInput = false;
				else if(board[x1+2][y1-2] == 'k')
					isValidInput = false;
				else if(board[x1][y1-2] == 'k')
					isValidInput = false;
				else if(board[x1-2][y1-2] == 'k')
					isValidInput = false;
			}

			else if(x2 == x1 && y2 < y1) {
				if(board[x1][y1-2] == 'k')
					isValidInput = false;
				else if(board[x1+2][y1-2] == 'k')
					isValidInput = false;
				else if(board[x1-2][y1-2] == 'k')
					isValidInput = false;
			}

			else if(x2 < x1 && y2 < y1) {
				if(board[x1+2][y1-2] == 'k')
					isValidInput = false;
				else if(board[x1][y1-2] == 'k')
					isValidInput = false;
				else if(board[x1-2][y1-2] == 'k')
					isValidInput = false;
				else if(board[x1-2][y1] == 'k')
					isValidInput = false;
				else if(board[x1-2][y1+2] == 'k')
					isValidInput = false;
			}
		}
	} 
	else {
		if(x2 > x1+1)
			isValidInput = false;
		else if(x2 < x1-1)
			isValidInput = false;
		else if(y2 > y1+1)
			isValidInput = false;
		else if(y2 < y1-1)
			isValidInput  = false;
		else if(board[x2][y2] == 'p' || board[x2][y2] == 'r' || board[x2][y2] == 'n' ||
				board[x2][y2] == 'b' || board[x2][y2] == 'q' || board[x2][y2] == 'k' || board[x2][y2] == 'K')
				isValidInput = false;

		if(isValidInput) {
			if(x2 < x1 && y1==y2) {
				if(board[x1-2][y1] == 'K')
					isValidInput = false;
				else if(board[x1-2][y1+2] == 'K')
					isValidInput = false;
				else if(board[x1-2][y1-2] == 'K')
					isValidInput = false;
			} 

			else if(x2 < x1 && y2 > y1) {
				if(board[x1-2][y1] == 'K')
					isValidInput = false;
			else if(board[x1-2][y1-2] == 'K')
				isValidInput = false;
			else if(board[x1-2][y1+2] == 'K')
				isValidInput = false;
			else if(board[x1][y1+2] == 'K')
				isValidInput = false;
			else if(board[x1+2][y1+2] == 'K')
				isValidInput = false;
			}

			else if(x2 == x1 && y2 > y1) {
				if(board[x1-2][y1+2] == 'K')
					isValidInput = false;
				else if(board[x1][y1+2] == 'K')
					isValidInput = false;
				else if(board[x1+2][y1+2] == 'K')
					isValidInput = false;
			}

			else if(x2 > x1 && y2 > y1) {
				if(board[x1-2][y1+2] == 'K')
					isValidInput = false;
				else if(board[x1][y1+2] == 'K')
					isValidInput = false;
				else if(board[x1+2][y1+2] == 'K')
					isValidInput = false;
				else if(board[x1+2][y1] == 'K')
					isValidInput = false;
				else if(board[x1+2][y1-2] == 'K')
					isValidInput = false;
			}

			else if(x2 > x1 && y1==y2) {
				if(board[x1+2][y1] == 'K')
					isValidInput = false;
				else if(board[x1+2][y1+2] == 'K')
					isValidInput = false;
				else if(board[x1+2][y1-2] == 'K')
					isValidInput = false;
			}

			else if(x2 > x1 && y2 < y1) {
				if(board[x1+2][y1+2] == 'K')
					isValidInput = false;
				else if(board[x1+2][y1] == 'K')
					isValidInput = false;
				else if(board[x1+2][y1-2] == 'K')
					isValidInput = false;
				else if(board[x1][y1-2] == 'K')
					isValidInput = false;
				else if(board[x1-2][y1-2] == 'K')
					isValidInput = false;
			}

			else if(x2 == x1 && y2 < y1) {
				if(board[x1][y1-2] == 'K')
					isValidInput = false;
				else if(board[x1+2][y1-2] == 'K')
					isValidInput = false;
				else if(board[x1-2][y1-2] == 'K')
					isValidInput = false;
			}

			else if(x2 < x1 && y2 < y1) {
				if(board[x1+2][y1-2] == 'K')
					isValidInput = false;
				else if(board[x1][y1-2] == 'K')
					isValidInput = false;
				else if(board[x1-2][y1-2] == 'K')
					isValidInput = false;
				else if(board[x1-2][y1] == 'K')
					isValidInput = false;
				else if(board[x1-2][y1+2] == 'K')
					isValidInput = false;
			}
		}
	}

	return isValidInput;
}

bool whiteKingCheck(char board[][8]) {
	bool isKingUnderCheck = false;
	int x,y;
	for(int i=7; i > -1; i--) {
		for(int j=0; j < 8; j++) {
			if(board[i][j] == 'K') {
				x = i;
				y = j;

				j = 8;
				i = -1;
			}
		}
	}

	for(int i=0; i < 8; i++) {
		for(int j=0; j<8;j++) {
			if(!isKingUnderCheck) {
				if(board[i][j] == 'r') {
					isKingUnderCheck = rookMove(board, i,j,x,y,2);
				} else if(board[i][j] == 'p') {
					isKingUnderCheck = pawnMove(board,i,j,x,y,2);
				} else if(board[i][j] == 'b') {
					isKingUnderCheck = bishopMove(board, i,j,x,y,2);
				} else if(board[i][j] == 'n') {
					isKingUnderCheck = knightMove(board, i,j,x,y,2);
				} else if(board[i][j] == 'k') {
					isKingUnderCheck = kingMove(board, i,j,x,y,2);
				} else if(board[i][j] == 'q') {
					isKingUnderCheck = queenMove(board, i,j,x,y,2);
				}
			}
		}
	}
	  
	return isKingUnderCheck;

}

bool blackKingCheck(char board[][8]) {
	bool isKingUnderCheck = false;
	int x,y;
	for(int i=0; i < 8; i++) {
		for(int j=0; j < 8; j++) {
			if(board[i][j] == 'k') {
				x = i;
				y = j;

				j = 8;
				i = 8;
			}
		}
	}

	for(int i=0; i < 8; i++) {
		for(int j=0; j<8;j++) {
			if(!isKingUnderCheck){
				if(board[i][j] == 'R') {
					isKingUnderCheck = rookMove(board, i,j,x,y,1);
				} else if(board[i][j] == 'P') {
					isKingUnderCheck = pawnMove(board, i,j,x,y,1);
				} else if(board[i][j] == 'B') {
					isKingUnderCheck = bishopMove(board, i,j,x,y,1);
				} else if(board[i][j] == 'N') {
					isKingUnderCheck = knightMove(board, i,j,x,y,1);
				} else if(board[i][j] == 'K') {
					isKingUnderCheck = kingMove(board, i,j,x,y,1);
				} else if(board[i][j] == 'Q') {
					isKingUnderCheck = queenMove(board, i,j,x,y,1);
				}
			}
		}

	}

	return isKingUnderCheck;

}

bool whiteKingCheckMate(char board[][8], bool temp[8]) {
	bool isNotCheckMate = true;
	bool locations[8] = {false, false, false, false, false, false, false, false};

	int kingPositonX,kingPositonY, x, y;
	for(int i=7; i > -1; i--) {
		for(int j=0; j < 8; j++) {
			if(board[i][j] == 'K') {
				kingPositonX = i;
				kingPositonY = j;

				j = 8;
				i = -1;
			}
		}
	}

	//One Step up
		x = kingPositonX - 1;
		y = kingPositonY;
		if(x > -1) {

			if(board[x][y] == 'P' || board[x][y] == 'R' || board[x][y] == 'N' || board[x][y] == 'B' || board[x][y] == 'Q')
				locations[0] = true;
			else {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[0] == false) {
							if(board[i][j] == 'r') {
								locations[0] = rookMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'p') {
								if(board[x][y] == ' ')
									board[x][y] = 'k';
								locations[0] = pawnMove(board,i,j,x,y,2);
								board[x][y] = ' ';
							} else if(board[i][j] == 'b') {
								locations[0] = bishopMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'n') {
								locations[0] = knightMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'k') {
								locations[0] = kingMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'q') {
								locations[0] = queenMove(board, i,j,x,y,2);
							}
						}
					}
				}

			}if(!locations[0]) {
				if(board[x-1][y] == 'k')
					locations[0] = true;
				else if(board[x-1][y+1] == 'k')
					locations[0] = true;
				else if(board[x-1][y-1] == 'k')
					locations[0] = true;
			}
		} else
			locations[0] = true;

		//One Step Right Up
		x = kingPositonX -1;
		y = kingPositonY +1;

		if(x > -1 && y < 8) {
		
		
		if(board[x][y] == 'P' || board[x][y] == 'R' || board[x][y] == 'N' || board[x][y] == 'B' || board[x][y] == 'Q')
				locations[1] = true;
			else {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[1] == false) {
							if(board[i][j] == 'r') {
								locations[1] = rookMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'p') {
								if(board[x][y] == ' ')
									board[x][y] = 'k';
								locations[1] = pawnMove(board,i,j,x,y,2);
								board[x][y] = ' ';
							} else if(board[i][j] == 'b') {
								locations[1] = bishopMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'n') {
								locations[1] = knightMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'k') {
								locations[1] = kingMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'q') {
								locations[1] = queenMove(board, i,j,x,y,2);
							}
						}
					}
				}

			} if(!locations[1]) {
			if(board[x-1][y] == 'k')
				locations[1] = true;
			else if(board[x-1][y-1] == 'k')
				locations[1] = true;
			else if(board[x-1][y+1] == 'k')
				locations[1] = true;
			else if(board[x][y+1] == 'k') {
				locations[1] = true;
			}
			else if(board[x+1][y+1] == 'k')
				locations[1] = true;
		}
		} else
			locations[1] = true;


		// One Step Right

		x = kingPositonX;
		y = kingPositonY +1;
		if(y < 8) {
		
			if(board[x][y] == 'P' || board[x][y] == 'R' || board[x][y] == 'N' || board[x][y] == 'B' || board[x][y] == 'Q')
				locations[2] = true;
			else {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[2] == false) {
							if(board[i][j] == 'r') {
								locations[2] = rookMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'p') {
								if(board[x][y] = ' ')
									board[x][y] = 'k';
								locations[2] = pawnMove(board,i,j,x,y,2);
								board[x][y] = ' ';
							} else if(board[i][j] == 'b') {
								locations[2] = bishopMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'n') {
								locations[2] = knightMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'k') {
								locations[2] = kingMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'q') {
								locations[2] = queenMove(board, i,j,x,y,2);
							}
						}
					}
				}

			} if(!locations[2]) {
				if(board[x-1][y+1] == 'k')
					locations[2] = true;
				else if(board[x][y+1] == 'k')
					locations[2] = true;
				else if(board[x+1][y+1] == 'k')
					locations[2] = true;
			}
		} else 
			locations[2] = true;

		// Right Bottom
		x = kingPositonX + 1;
		y = kingPositonY + 1;

		if(x < 8 && y < 8) {
		


			if(board[x][y] == 'P' || board[x][y] == 'R' || board[x][y] == 'N' || board[x][y] == 'B' || board[x][y] == 'Q')
				locations[3] = true;
			else {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[3] == false) {
							if(board[i][j] == 'r') {
								locations[3] = rookMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'p') {
								if(board[x][y] == ' ')
									board[x][y] = 'k';
								locations[3] = pawnMove(board,i,j,x,y,2);
								board[x][y] = ' '; 
							} else if(board[i][j] == 'b') {
								locations[3] = bishopMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'n') {
								locations[3] = knightMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'k') {
								locations[3] = kingMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'q') {
								locations[3] = queenMove(board, i,j,x,y,2);
							}
						}
					}
				}

			} if(!locations[3]){
				if(board[x-1][y+1] == 'k')
					locations[3] = true;
				else if(board[x][y+1] == 'k')
					locations[3] = true;
				else if(board[x+1][y+1] == 'k')
					locations[3] = true;
				else if(board[x+1][y] == 'k')
					locations[3] = true;
				else if(board[x+1][y-1] == 'k')
					locations[3] = true;
			}
		} else
			locations[3] = true;

		// One Step Bottom

		x = kingPositonX + 1;
		y = kingPositonY;

		if(x < 8) {
			if(board[x][y] == 'P' || board[x][y] == 'R' || board[x][y] == 'N' || board[x][y] == 'B' || board[x][y] == 'Q')
				locations[4] = true;
			else {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[4] == false) {
							if(board[i][j] == 'r') {
								locations[4] = rookMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'p') {
								if(board[x][y] == ' ')
									board[x][y] = 'k';
								locations[4] = pawnMove(board,i,j,x,y,2);
								board[x][y] = ' ';
							} else if(board[i][j] == 'b') {
								locations[4] = bishopMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'n') {
								locations[4] = knightMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'k') {
								locations[4] = kingMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'q') {
								locations[4] = queenMove(board, i,j,x,y,2);
							}
						}
					}
				}

			} if(!locations[4]){
				if(board[x+1][y] == 'k')
					locations[4] = true;
				else if(board[x+1][y+1] == 'k')
					locations[4] = true;
				else if(board[x+1][y-1] == 'k')
					locations[4] = true;
			}
		} else
			locations[4] = true;

		//One Step Left Bottom
		x = kingPositonX + 1;
		y = kingPositonY - 1;

		if(x < 8 && y > -1) {
			
			if(board[x][y] == 'P' || board[x][y] == 'R' || board[x][y] == 'N' || board[x][y] == 'B' || board[x][y] == 'Q')
				locations[5] = true;
			else {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[5] == false) {
							if(board[i][j] == 'r') {
								locations[5] = rookMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'p') {
								if(board[x][y] = ' ')
									board[x][y] = 'k';
								locations[5] = pawnMove(board,i,j,x,y,2);
								board[x][y] = ' ';
							} else if(board[i][j] == 'b') {
								locations[5] = bishopMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'n') {
								locations[5] = knightMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'k') {
								locations[5] = kingMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'q') {
								locations[5] = queenMove(board, i,j,x,y,2);
							}
						}
					}
				}

			} if(!locations[5]) {
				if(board[x+1][y+1] == 'k')
					locations[5] = true;
				else if(board[x+1][y] == 'k')
					locations[5] = true;
				else if(board[x+1][y-1] == 'k')
					locations[5] = true;
				else if(board[x][y-1] == 'k')
					locations[5] = true;
				else if(board[x-1][y-1] == 'k')
					locations[5] = true;
			}

		} else
			locations[5] = true;

		// One step left
		x = kingPositonX;
		y = kingPositonY - 1;

		if(y > -1) {
		
			if(board[x][y] == 'P' || board[x][y] == 'R' || board[x][y] == 'N' || board[x][y] == 'B' || board[x][y] == 'Q')
				locations[6] = true;
			else {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[6] == false) {
							if(board[i][j] == 'r') {
								locations[6] = rookMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'p') {
								if(board[x][y] == ' ')
									board[x][y] = 'k';
								locations[6] = pawnMove(board,i,j,x,y,2);
								board[x][y] = ' ';
							} else if(board[i][j] == 'b') {
								locations[6] = bishopMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'n') {
								locations[6] = knightMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'k') {
								locations[6] = kingMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'q') {
								locations[6] = queenMove(board, i,j,x,y,2);
							}
						}
					}
				}

			} if(!locations[6]){
				if(board[x][y-1] == 'k')
					locations[6] = true;
				else if(board[x+1][y-1] == 'k')
					locations[6] = true;
				else if(board[x-1][y-1] == 'k')
					locations[6] = true;
			}

		} else
			locations[6] = true;

		//One Step Left Up
		x = kingPositonX - 1;
		y = kingPositonY - 1;

		if(x > -1 && y > -1) {
			if(board[x][y] == 'P' || board[x][y] == 'R' || board[x][y] == 'N' || board[x][y] == 'B' || board[x][y] == 'Q')
				locations[7] = true;
			else {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[7] == false) {
							if(board[i][j] == 'r') {
								locations[7] = rookMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'p') {
								if(board[x][y] == ' ')
									board[x][y] = 'k';
								locations[7] = pawnMove(board,i,j,x,y,2);
								board[x][y] = ' ';
							} else if(board[i][j] == 'b') {
								locations[7] = bishopMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'n') {
								locations[7] = knightMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'k') {
								locations[7] = kingMove(board, i,j,x,y,2);
							} else if(board[i][j] == 'q') {
								locations[7] = queenMove(board, i,j,x,y,2);
							}
						}
					}
				}

			} if(!locations[7]){
				if(board[x+1][y-1] == 'k')
					locations[7] = true;
				else if(board[x][y-1] == 'k')
					locations[7] = true;
				else if(board[x-1][y-1] == 'k')
					locations[7] = true;
				else if(board[x-1][y] == 'k')
					locations[7] = true;
				else if(board[x-1][y+1] == 'k')
					locations[7] = true;
			}
		} else
			locations[7] = true;


		for(int i=0; i < 8; i++) {
			temp[i] = locations[i];
			if(locations[i] == false)
				isNotCheckMate = false;
		}

	return isNotCheckMate;


}

bool blackKingCheckMate(char board[][8], bool temp[8]) {
	bool isNotCheckMate = true;
	bool locations[8] = {false, false, false, false, false, false, false, false};

	int kingPositonX,kingPositonY, x, y;
	for(int i=7; i > -1; i--) {
		for(int j=0; j < 8; j++) {
			if(board[i][j] == 'k') {
				kingPositonX = i;
				kingPositonY = j;

				j = 8;
				i = -1;
			}
		}
	}

	//One Step up
		x = kingPositonX - 1;
		y = kingPositonY;
		if(x > -1) {

			if(board[x][y] == 'p' || board[x][y] == 'r' || board[x][y] == 'n' || board[x][y] == 'b' || board[x][y] == 'q')
				locations[0] = true;
			
			else if(!locations[0]) {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[0] == false) {
							if(board[i][j] == 'R') {
								locations[0] = rookMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'P') {
								if(board[x][y] == ' ')
									board[x][y] = 'K';
								locations[0] = pawnMove(board,i,j,x,y,1);
								board[x][y] = ' ';
							} else if(board[i][j] == 'B') {
								locations[0] = bishopMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'N') {
								locations[0] = knightMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'K') {
								locations[0] = kingMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'Q') {
								locations[0] = queenMove(board, i,j,x,y,1);
							}
						}
					}
				}

			} if(!locations[0]) {
				if(board[x-1][y] == 'K')
					locations[0] = true;
				else if(board[x-1][y+1] == 'K')
					locations[0] = true;
				else if(board[x-1][y-1] == 'K')
					locations[0] = true;
			}
		} else
			locations[0] = true;

		//One Step Right Up
		x = kingPositonX -1;
		y = kingPositonY +1;

		if(x > -1 && y < 8) {
		
		
		if(board[x][y] == 'p' || board[x][y] == 'r' || board[x][y] == 'n' || board[x][y] == 'b' || board[x][y] == 'q')
				locations[1] = true;
		
			else if(!locations[1]) {
				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[1] == false) {
							if(board[i][j] == 'R') {
								locations[1] = rookMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'P') {
								if(board[x][y] == ' ')
									board[x][y] = 'K';
								locations[1] = pawnMove(board,i,j,x,y,1);
								board[x][y] = ' ';
							} else if(board[i][j] == 'B') {
								locations[1] = bishopMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'N') {
								locations[1] = knightMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'K') {
								locations[1] = kingMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'Q') {
								locations[1] = queenMove(board, i,j,x,y,1);
							}
						}
					}
				}

			} if(!locations[1]) {
			if(board[x-1][y] == 'K')
				locations[1] = true;
			else if(board[x-1][y-1] == 'K')
				locations[1] = true;
			else if(board[x-1][y+1] == 'K')
				locations[1] = true;
			else if(board[x][y+1] == 'K') {
				locations[1] = true;
			}
			else if(board[x+1][y+1] == 'K')
				locations[1] = true;
		}
		} else
			locations[1] = true;


		// One Step Right

		x = kingPositonX;
		y = kingPositonY +1;
		if(y < 8) {
		
			if(board[x][y] == 'p' || board[x][y] == 'r' || board[x][y] == 'n' || board[x][y] == 'b' || board[x][y] == 'q')
				locations[2] = true;
			
			else if(!locations[2]) {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[2] == false) {
							if(board[i][j] == 'R') {
								locations[2] = rookMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'P') {
								if(board[x][y] == ' ')
									board[x][y] = 'K';
								locations[2] = pawnMove(board,i,j,x,y,1);
								board[x][y] = ' ';
							} else if(board[i][j] == 'B') {
								locations[2] = bishopMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'N') {
								locations[2] = knightMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'K') {
								locations[2] = kingMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'Q') {
								locations[2] = queenMove(board, i,j,x,y,1);
							}
						}
					}
				}

			} if(!locations[2]) {
				if(board[x-1][y+1] == 'K')
					locations[2] = true;
				else if(board[x][y+1] == 'K')
					locations[2] = true;
				else if(board[x+1][y+1] == 'K')
					locations[2] = true;
			}
		} else 
			locations[2] = true;

		// Right Bottom
		x = kingPositonX + 1;
		y = kingPositonY + 1;

		if(x < 8 && y < 8) {
		


			if(board[x][y] == 'p' || board[x][y] == 'r' || board[x][y] == 'n' || board[x][y] == 'b' || board[x][y] == 'q')
				locations[3] = true;
			
			else if(!locations[3]) {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[3] == false) {
							if(board[i][j] == 'R') {
								locations[3] = rookMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'P') {
								if(board[x][y] == ' ')
									board[x][y] == 'K';
								locations[3] = pawnMove(board,i,j,x,y,1);
								board[x][y] = ' ';
							} else if(board[i][j] == 'B') {
								locations[3] = bishopMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'N') {
								locations[3] = knightMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'K') {
								locations[3] = kingMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'Q') {
								locations[3] = queenMove(board, i,j,x,y,1);
							}
						}
					}
				}
			} if(!locations[3]){
				if(board[x-1][y+1] == 'K')
					locations[3] = true;
				else if(board[x][y+1] == 'K')
					locations[3] = true;
				else if(board[x+1][y+1] == 'K')
					locations[3] = true;
				else if(board[x+1][y] == 'K')
					locations[3] = true;
				else if(board[x+1][y-1] == 'K')
					locations[3] = true;
			}
		} else
			locations[3] = true;

		// One Step Bottom

		x = kingPositonX + 1;
		y = kingPositonY;

		if(x < 8) {
			if(board[x][y] == 'p' || board[x][y] == 'r' || board[x][y] == 'n' || board[x][y] == 'b' || board[x][y] == 'q')
				locations[4] = true;
			
			else if(!locations[4]) {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[4] == false) {
							if(board[i][j] == 'R') {
								locations[4] = rookMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'P') {
								if(board[x][y] == ' ')
									board[x][y] = 'K';
								locations[4] = pawnMove(board,i,j,x,y,1);
								board[x][y] = ' ';
							} else if(board[i][j] == 'B') {
								locations[4] = bishopMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'N') {
								locations[4] = knightMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'K') {
								locations[4] = kingMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'Q') {
								locations[4] = queenMove(board, i,j,x,y,1);
							}
						}
					}
				}

			} if(!locations[4]){
				if(board[x+1][y] == 'K')
					locations[4] = true;
				else if(board[x+1][y+1] == 'K')
					locations[4] = true;
				else if(board[x+1][y-1] == 'K')
					locations[4] = true;
			}
		} else
			locations[4] = true;

		//One Step Left Bottom
		x = kingPositonX + 1;
		y = kingPositonY - 1;

		if(x < 8 && y > -1) {
			
			if(board[x][y] == 'p' || board[x][y] == 'r' || board[x][y] == 'n' || board[x][y] == 'b' || board[x][y] == 'q')
				locations[5] = true;
			
			else if(!locations[5]) {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[5] == false) {
							if(board[i][j] == 'R') {
								locations[5] = rookMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'P') {
								if(board[x][y] == ' ')
									board[x][y] = 'K';
								locations[5] = pawnMove(board,i,j,x,y,1);
								board[x][y] == ' ';
							} else if(board[i][j] == 'B') {
								locations[5] = bishopMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'N') {
								locations[5] = knightMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'K') {
								locations[5] = kingMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'Q') {
								locations[5] = queenMove(board, i,j,x,y,1);
							}
						}
					}
				}

			} if(!locations[5]) {
				if(board[x+1][y+1] == 'K')
					locations[5] = true;
				else if(board[x+1][y] == 'K')
					locations[5] = true;
				else if(board[x+1][y-1] == 'K')
					locations[5] = true;
				else if(board[x][y-1] == 'K')
					locations[5] = true;
				else if(board[x-1][y-1] == 'K')
					locations[5] = true;
			}

		} else
			locations[5] = true;

		// One step left
		x = kingPositonX;
		y = kingPositonY - 1;

		if(y > -1) {
		
			if(board[x][y] == 'p' || board[x][y] == 'r' || board[x][y] == 'n' || board[x][y] == 'b' || board[x][y] == 'q')
				locations[6] = true;
			
			else if(!locations[6]) {

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[6] == false) {
							if(board[i][j] == 'R') {
								locations[6] = rookMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'P') {
								if(board[x][y] == ' ')
									board[x][y] = 'K';
								locations[6] = pawnMove(board,i,j,x,y,1);
								board[x][y] = ' ';
							} else if(board[i][j] == 'B') {
								locations[6] = bishopMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'N') {
								locations[6] = knightMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'K') {
								locations[6] = kingMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'Q') {
								locations[6] = queenMove(board, i,j,x,y,1);
							}
						}
					}
				}

			} if(!locations[6]){
				if(board[x][y-1] == 'K')
					locations[6] = true;
				else if(board[x+1][y-1] == 'K')
					locations[6] = true;
				else if(board[x-1][y-1] == 'K')
					locations[6] = true;
			}

		} else
			locations[6] = true;

		//One Step Left Up
		x = kingPositonX - 1;
		y = kingPositonY - 1;

		if(x > -1 && y > -1) {
			if(board[x][y] == 'p' || board[x][y] == 'r' || board[x][y] == 'n' || board[x][y] == 'b' || board[x][y] == 'q')
				locations[7] = true;
			
			else if(!locations[7]){

				for(int i=0; i < 8; i++) {
					for(int j=0; j<8;j++) {
						if(locations[7] == false) {
							if(board[i][j] == 'R') {
								locations[7] = rookMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'P') {
								if(board[x][y] == ' ')
									board[x][y] = 'K';
								locations[7] = pawnMove(board,i,j,x,y,1);
							} else if(board[i][j] == 'B') {
								locations[7] = bishopMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'N') {
								locations[7] = knightMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'K') {
								locations[7] = kingMove(board, i,j,x,y,1);
							} else if(board[i][j] == 'Q') {
								locations[7] = queenMove(board, i,j,x,y,1);
							}
						}
					}
				}

			} if(!locations[7]){
				if(board[x+1][y-1] == 'K')
					locations[7] = true;
				else if(board[x][y-1] == 'K')
					locations[7] = true;
				else if(board[x-1][y-1] == 'K')
					locations[7] = true;
				else if(board[x-1][y] == 'K')
					locations[7] = true;
				else if(board[x-1][y+1] == 'K')
					locations[7] = true;
			}
		} else
			locations[7] = true;


		for(int i=0; i < 8; i++) {
			temp[i] = locations[i];
			if(locations[i] == false) {
				isNotCheckMate = false;
			}
		}


	return isNotCheckMate;

}

void initBoard(char board[][8]){
	board[0][0] = 'r';
	board[0][1] = 'n';
	board[0][2] = 'b';
	board[0][3] = 'q';
	board[0][4] = 'k';
	board[0][5] = 'b';
	board[0][6] = 'n';
	board[0][7] = 'r';

	for(int i=0; i<8; i++) {
		board[1][i] = 'p';
	}

	for(int i=2; i<6 ;i++){
		for(int j=0; j<8; j++) {

			board[i][j] =' ';
		}
	}

	

	for(int i=0; i<8; i++) {
		board[6][i] = 'P';
	}

	board[7][0] = 'R';
	board[7][1] = 'N';
	board[7][2] = 'B';
	board[7][3] = 'Q';
	board[7][4] = 'K';
	board[7][5] = 'B';
	board[7][6] = 'N';
	board[7][7] = 'R';

}
	
void printBoard(char board[][8])
{
	system("cls");
	cout << "Instructions: Commands"<<endl;
	cout << "To save the game type command: sav[slotno] for Example: sav1"<<endl;
	cout << "To draw the game type command: shak"<<endl;
	cout << "To Resign game type command:   resi"<<endl<<endl;
	cout << "       ";
	char j = 97;
	for (int i = 0; i < 8; i++) {
		cout << j++ << "   ";
	}
	cout << endl;
	cout << "     ---------------------------------" << endl;
	char a = '8';
	char b = '8';
	//cout << "   ";
	for (int i = 0; i < 8; i++) {
		cout << " " << a-- << "   | ";
		for (int j = 0; j < 8; j++) {
			cout << board[i][j] << " | ";
		}
		cout << "  " << b-- << "   " << endl;
		cout << "     ---------------------------------" << endl;
	}
	cout << "       ";
	j = 97;
	for (int i = 0; i < 8; i++) {
		cout << j++ << "   ";
	}

	cout << endl << endl;
}

void gotoxy(int x, int y)
{
   HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(h, 15);
    COORD scrn;

    scrn.X = y, scrn.Y = x;

    SetConsoleCursorPosition(h,scrn);
}

bool isPawnStale(char board[][8], char piece, int player) {
	int totalPawns = 0;
	bool isPawnStale = true;

	for(int i=0; i<8;i++) {
		for(int j=0; j<8; j++) {
			if(board[i][j] == piece && isPawnStale && player == 1) {
				int x1 = i;
				int y1 = j;
				if(pawnMove(board, x1,y1,x1-1,y1, player))
					isPawnStale = false; 
				else if(pawnMove(board,x1,y1,x1-1,y1+1,player))
					isPawnStale = false;
				else if(pawnMove(board,x1,y1,x1-1,y1-1,player))
					isPawnStale = false;
				else if(pawnMove(board, x1,y1,x1-2,y1, player))
					isPawnStale = false;
			} 
			
			else if(board[i][j] == piece && isPawnStale && player == 2) {
				int x1 = i;
				int y1 = j;
				if(pawnMove(board, x1,y1,x1+1,y1, player))
					isPawnStale = false; 
				else if(pawnMove(board,x1,y1,x1+1,y1+1,player))
					isPawnStale = false;
				else if(pawnMove(board,x1,y1,x1+1,y1-1,player))
					isPawnStale = false;
				else if(pawnMove(board, x1,y1,x1+2,y1, player))
					isPawnStale = false;
			}
		}
	}
	return isPawnStale;
}

bool isBishopStale(char board[][8], char piece, int player) {
	bool isBishopStale = true;



	for(int i=0; i<8;i++) {
		for(int j=0; j<8;j++) {
			if(board[i][j] == piece && isBishopStale) {
				int x1=i;
				int y1= j;
				int x2 = x1-1;
				int y2 = y1-1;
				while(x2 > -1 && y2 > -1 && isBishopStale) {
					if(bishopMove(board,x1,y1,x2,y2,player))
						isBishopStale = false;
					x2--;
					y2--;
				}

				x2 = x1 - 1;
				y2 = y1 + 1;
				while(x2 > -1 && y2 < 8 && isBishopStale) {
					if(bishopMove(board,x1,y1,x2,y2,player))
						isBishopStale = false;
					x2--;
					y2++;
				}

				x2 = x1+1;
				y2 = y1 - 1;

				while(x2 < 8 && y2 > -1 && isBishopStale) {
					if(bishopMove(board,x1,y1,x2,y2,player))
						isBishopStale = false;
					x2++;
					y2--;
				}

				x2 = x1 + 1;
				y2 = y1 + 1;
				while(x2 < 8 && y2 < 8 && isBishopStale) {
					if(bishopMove(board,x1,y1,x2,y2,player))
						isBishopStale = false;
					x2++;
					y2++;
				}

			}
		}
	}
	return isBishopStale;
}

bool isRookStale(char board[][8], char piece, int player) {
	bool isRookStale = true;

	for(int i=0; i<8;i++) {
		for(int j=0; j<8;j++) {
			if(board[i][j] == piece && isRookStale) {
				int x1 = i;
				int y1 = j;
				
				int x2 = i-1;
				int y2 = y1;
				while(x2 > -1 && isRookStale) {
					if(rookMove(board,x1,y1,x2,y2,player))
						isRookStale = false;
					x2--;
				}

				x2 = i+1;
				while(x2 < 8 && isRookStale) {
					if(rookMove(board,x1,y1,x2,y2,player))
						isRookStale = false;
					x2++;
				}

				x2 = x1;
				y2 = y1-1;
				while(y2 > -1 && isRookStale) {
					if(rookMove(board,x1,y1,x2,y2, player))
						isRookStale = false;
					y2--;
				}

				y2 = y1+1;

				while(y2 < 8 && isRookStale) {
					if(rookMove(board,x1,y1,x2,y2, player))
						isRookStale = false;
					y2++;
				}
			}
		}
	}

	return isRookStale;
}

bool isKnightStale(char board[][8], char piece, int player) {
	bool isKnightStale = true;

	for(int i=0; i<8;i++) {
		for(int j=0; j <8;j++) {
			if(board[i][j] == piece && isKnightStale) {
				int x1 = i;
				int y1 = j;

				//UP
				int x2 = x1-2;
				int y2 = y1-1;
				if(knightMove(board, x1,y1,x2,y2,player) && isKnightStale)
					isKnightStale = false;

				y2 = y1+1;
				if(knightMove(board, x1,y1,x2,y2,player) && isKnightStale)
					isKnightStale = false;

				// RIGHT

				x2 = x1-1;
				y2 = y1+2;

				if(knightMove(board, x1,y1,x2,y2,player) && isKnightStale)
					isKnightStale = false;

				x2 = x1+1;

				if(knightMove(board, x1,y1,x2,y2,player) && isKnightStale)
					isKnightStale = false;

				//DOWN

				x2 = x1+2;
				y2 = y1-1;

				if(knightMove(board, x1,y1,x2,y2,player) && isKnightStale)
					isKnightStale = false;

				y2 = y1+1;
				if(knightMove(board, x1,y1,x2,y2,player) && isKnightStale)
					isKnightStale = false;

				//LEFT

				x2 = x1-1;
				y2 = y1-2;

				if(knightMove(board, x1,y1,x2,y2,player) && isKnightStale)
					isKnightStale = false;

				x2 = x1+1;
				if(knightMove(board, x1,y1,x2,y2,player) && isKnightStale)
					isKnightStale = false;


			}
		}
	}

	return isKnightStale;
}

bool isQueenStale(char board[][8], char piece, int player) {
	bool isQueenStale = false;

	if(isRookStale(board, piece, player)) {
		if(isBishopStale(board, piece, player))
			isQueenStale = true;
	}

	return isQueenStale;
}

bool isKingSlate(char board[][8], char piece, int player) {
	bool isKingSlate = true;

	if(player == 1) {
	
		bool locations[8];
		whiteKingCheckMate(board, locations);
		for(int i=0; i<8; i++) {
			if(!locations[i])
				isKingSlate = false;
		}
	} else if (player == 2) {

			bool locations[8];
			blackKingCheckMate(board, locations);
			for(int i=0; i<8; i++) {
			if(!locations[i])
				isKingSlate = false;
		}
	}

	return isKingSlate;
}

bool whitePlayerSlateMate(char board[][8]) {
	bool isSlateMate = true;

	if(!isPawnStale(board, 'P', 1))
		isSlateMate  = false;
	else if(!isBishopStale(board,'B',1))
		isSlateMate = false;
	else if(!isRookStale(board,'R',1))
		isSlateMate = false;
	else if(!isKingSlate(board,'N',1))
		isSlateMate = false;
	else if(!isQueenStale(board,'Q',1))
		isSlateMate = false;
	else if(!isKingSlate(board,'K',1))
		isSlateMate = false;

	return isSlateMate;
}

bool blackPlayerSlateMate(char board[][8]) {
	bool isSlateMate = true;

	if(!isPawnStale(board, 'p', 2))
		isSlateMate  = false;
	else if(!isBishopStale(board,'b',2))
		isSlateMate = false;
	else if(!isRookStale(board,'r',2))
		isSlateMate = false;
	else if(!isKingSlate(board,'n',2))
		isSlateMate = false;
	else if(!isQueenStale(board,'q',2))
		isSlateMate = false;
	else if(!isKingSlate(board,'k',2))
		isSlateMate = false;

	return isSlateMate;
}


int asciiConverter(int num) {
	if(num == 48)
		return 0;
	else if(num == 49)
		return 1;
	else if(num == 50)
		return 2;

	else if(num == 51)
		return 3;

	else if(num == 52)
		return 4;

	else if(num == 53)
		return 5;

	else if(num == 54)
		return 6;

	else if(num == 55)
		return 7;
	else if(num == 56)
		return 8;
	else if(num == 57)
		return 9;

	else 
		return -1;


}

int charToNum(char ch) {
	if(ch == 'a')
		return 0;
	else if(ch == 'b')
		return 1;
	else if(ch == 'c')
		return 2;

	else if(ch == 'd')
		return 3;

	else if(ch == 'e')
		return 4;

	else if(ch == 'f')
		return 5;
	else if(ch == 'g')
		return 6;
	else if(ch == 'h')
		return 7;
	else 
		return -1;
}


