#include <iostream>
#include <string>
using namespace std;

string input;
int decide, pos, pos2, width, height, rowToPutEnemy, putEnemy, rowToPutPlayer, putPlayer, newRowPutPlayer, newPutPlayer;
bool inputNow = false;


void draw(int width, int height){
	rowToPutEnemy = 1;
	rowToPutPlayer = 1;
	putEnemy = -2;
	putPlayer = -1;
	if(inputNow == false){
		for(int checker = 0; checker <= pos; checker++){
			if(checker % width == 0 && checker > 0){
				rowToPutPlayer = rowToPutPlayer + 2;
				putPlayer = 2;
			}else putPlayer = putPlayer + 3;
		}
	}else{
		rowToPutPlayer = newRowPutPlayer;
		putPlayer = newPutPlayer;
	}
	for(int checker2 = 0; checker2 <= pos2; checker2++){
		if(checker2 % width == 0 && checker2 > 0){
			rowToPutEnemy = rowToPutEnemy + 2;
			putEnemy = 1;
		}else putEnemy = putEnemy + 3;
	}
	for(int row = 0; row <= height*2; row++){
		if(row % 2 == 0){
			for(int putChar1 = 0; putChar1 <= width*3; putChar1++){
				if(putChar1 % 3 == 0) cout << "+";
				else cout << "-";
			}
		}else if(row % 2 == 1 && row == rowToPutEnemy && row != rowToPutPlayer){
			for(int spec1 = 0; spec1 <= width*3; spec1++){
				if(spec1 % 3 == 0) cout << "|";
				else if(spec1 == putEnemy) cout << "0";
				else cout << " ";
			}
		}else if(row % 2 == 1 && row != rowToPutEnemy && row == rowToPutPlayer){
			for(int spec2 = 0; spec2 <= width*3; spec2++){
				if(spec2 % 3 == 0) cout << "|";
				else if(spec2 == putPlayer) cout << "X";
				else cout << " ";
			}
		}else if(row % 2 == 1 && row == rowToPutEnemy && row == rowToPutPlayer){
			for(int spec3 = 0; spec3 <= width*3; spec3++){
				if(spec3 % 3 == 0) cout << "|";
				else if(spec3 == putEnemy) cout << "0";
				else if(spec3 == putPlayer) cout << "X";
				else cout << " ";
			}
		}else if(row % 2 == 1 && row != rowToPutEnemy && row != rowToPutPlayer){
			for(int putChar2 =0; putChar2 <= width*3; putChar2++){
				if(putChar2 % 3 == 0) cout << "|";
				else cout << " ";
			}
		}
		cout << "\n";
	}
}

void move(string dir){
	if(dir == "n" || dir == "north"){
		if(pos < width){
			pos = width*(height-1) + pos;
			newRowPutPlayer = height*2 - 1;
			newPutPlayer = putPlayer;
		}else{
			pos = pos-width;
			newRowPutPlayer = rowToPutPlayer - 2;
			newPutPlayer = putPlayer;
		}
	}else if(dir == "e" || dir == "east"){
		if(pos % width == (width - 1)){
			pos = pos - (width - 1);
			newPutPlayer = 2;
			newRowPutPlayer = rowToPutPlayer;
		}else{
			pos = ++pos;
			newPutPlayer = putPlayer + 3;
			newRowPutPlayer = rowToPutPlayer;
		}
	}else if(dir == "s" || dir == "south"){
		if(pos >= width*(height-1)){
			pos = pos % width;
			newRowPutPlayer = 1;
			newPutPlayer = putPlayer;
		}else{
			pos = pos+width;
			newRowPutPlayer = rowToPutPlayer + 2;
			newPutPlayer = putPlayer;
		}
	}else if(dir == "w" || dir == "west"){
		if(pos % width == 0){
			pos = pos + (width - 1);
			newPutPlayer = width*3 - 1;
			newRowPutPlayer = rowToPutPlayer;
		}else{
			pos = --pos;
			newPutPlayer = putPlayer - 3;
			newRowPutPlayer = rowToPutPlayer;
		}
	}
}

void attack(){
	if(pos == pos2){
		cout << "Enemy destroyed! Thank you for playing!";
		inputNow = false;
	}else{
		cout << "Nothing happened!\n";
	}
}

void help(){
	cout << "Available commands:\n";
	cout << "attack | a  =>    attack\n";
	cout << "north  | n  =>    move up\n";
	cout << "east   | e  =>    move right\n";
	cout << "south  | s  =>    move down\n";
	cout << "west   | w  =>    move left\n";
	cout << "exit   | x  =>    exit the game\n";
	cout << "help   | h  =>    display list of available commands\n";
}

void exit(){
	cout << "Goodbye!";
	inputNow = false;
}

int main(){
	do{
		cout << "Please input the height and width of your world.\n";
		cin >> width >> height;
		if(height <= 0 || width <= 0) cout << "Invalid dimensions. Please try again.\n";
	}while(height <= 0 || width <= 0);
	int area = height * width;
	do{
		cout << "Now, please input the starting positions of the target and the player respectively.\n" << "You can only choose between rooms 0 to " << area-1 << "\n"; 
		cin >> pos2 >> pos;
		if(pos2 < 0 || pos < 0 || pos > area - 1 || pos2 > area - 1) cout << "Invalid positions. Please try again.\n";
	}while(pos2 < 0 || pos < 0 || pos > area - 1 || pos2 > area - 1);
	do{
		cout << "Decide if you want to see the map or not. Choose 1 if you want, else choose 0 if you do not want to.\n";
		cin >> decide;
		if(decide > 1 || decide < 0) cout << "Choose properly!\n";
	}while(decide > 1 || decide < 0);
	if(decide == 1){
		draw(width, height);
		cout << "Position of Enemy is in Room: " << pos2 << "\n";
		cout << "Position of Player is in Room: " << pos << "\n";
	}else{
		cout << "Position of Enemy is in Room: " << pos2 << "\n";
		cout << "Position of Player is in Room: " << pos << "\n";
	}
	if(pos == pos2) cout << "Enemy is now in your sight!\n";
	inputNow = true;
	cin.sync();
	while(inputNow){
		cout << "Now what do you want to do?\n";
		getline(cin,input);
		if(input == "n" || input == "e" || input == "w" || input == "s" || input == "west" || input == "east" || input == "north" || input == "south"){
			move(input);
			if(decide == 1){
				draw(width, height);
				cout << "Position of Enemy is in Room: " << pos2 << "\n";
				cout << "Position of Player is in Room: " << pos << "\n";
			}else{
				cout << "Position of Enemy is in Room: " << pos2 << "\n";
				cout << "Position of Player is in Room: " << pos << "\n";
			}
			if(pos == pos2) cout << "Enemy is now in your sight!\n";
		}else if(input == "a" || input == "attack"){
			attack();
		}else if(input == "h" || input == "help"){
			help();
		}else if(input == "x" || input == "exit"){
			exit();
		}else if(input == "n n" || input == "north north" || input == "e e" || input == "east east" || input == "w w" || input == "west west" || 
			input == "s s" || input == "south south" || input == "a a" || input == "attack attack" || input == "h h" || input == "help help"
			|| input == "exit exit" || input == "x x"){
			cout << "Invalid command. Type 'h' or 'help' to show list of available commands.\n";
		}else{
			cout << "Invalid command. Type 'h' or 'help' to show list of available commands.\n";
		}
		if(!inputNow) break;
	}
}