#include<iostream>
#include<conio.h>
#include<dos.h> 
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70 

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = { ' ','±','±',' ', 
					'±','±','±','±', 
					' ','±','±',' ',
					'±','±','±','±' }; 

int carPosX = WIN_WIDTH / 2;  // X position of the car
int carPosY = 22;  // Y position of the car (static starting position)
int score = 0;

void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder(){  
	for(int i=0; i<SCREEN_HEIGHT; i++){
		for(int j=0; j<17; j++){
			gotoxy(0+j,i); cout<<"±";
			gotoxy(WIN_WIDTH-j,i); cout<<"±";
		}
	} 
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(SCREEN_WIDTH,i); cout<<"±";
	} 
}

void genEnemy(int ind){
	enemyX[ind] = 17 + rand() % (33);  
}

void drawEnemy(int ind){
	if(enemyFlag[ind] == true){
		gotoxy(enemyX[ind], enemyY[ind]);   cout<<"****";  
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout<<" ** "; 
		gotoxy(enemyX[ind], enemyY[ind] + 2); cout<<"****"; 
		gotoxy(enemyX[ind], enemyY[ind] + 3); cout<<" ** ";  
	} 
}

void eraseEnemy(int ind){
	if(enemyFlag[ind] == true){
		gotoxy(enemyX[ind], enemyY[ind]); cout<<"    ";  
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind] + 2); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind] + 3); cout<<"    "; 
	} 
}

void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 1;
	genEnemy(ind);
}

void drawCar(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			gotoxy(j + carPosX, i + carPosY); cout << car[i][j];
		}
	}
}

void eraseCar(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			gotoxy(j + carPosX, i + carPosY); cout << " ";
		}
	}
}

int collision(){
	if(enemyY[0] + 4 >= carPosY && enemyY[0] <= carPosY + 3){
		if(enemyX[0] + 4 - carPosX >= 0 && enemyX[0] + 4 - carPosX < 9){
			return 1;
		}
	}
	return 0;
}

void gameover(){
	system("cls");
	cout<<endl;
	cout<<"\t\t--------------------------"<<endl;
	cout<<"\t\t-------- Game Over -------"<<endl;
	cout<<"\t\t--------------------------"<<endl<<endl;
	cout<<"\t\tPress any key to go back to menu.";
	getch();
}

void updateScore(){
	gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
}

void instructions(){
	system("cls");
	cout << "Instructions";
	cout << "\n----------------";
	cout << "\n Avoid Cars by moving left, right, up, or down.";
	cout << "\n\n Press 'a' to move left";
	cout << "\n Press 'd' to move right";
	cout << "\n Press 'w' to move up";
	cout << "\n Press 's' to move down";
	cout << "\n Press 'escape' to exit";
	cout << "\n\nPress any key to go back to menu";
	getch();
}

// Function to change text color based on the percentage of loading
void setColor(int percentage) {
    if (percentage <= 25) {
        SetConsoleTextAttribute(console, FOREGROUND_RED);  // Red
    } else if (percentage <= 35) {
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);  // Orange (Red + Green)
    } else if (percentage <= 60) {
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);  // Yellow (Red + Green)
    } else if (percentage <= 85) {
        SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);  // Yellow-Green
    } else {
        SetConsoleTextAttribute(console, FOREGROUND_GREEN);  // Green
    }
}

void loadingScreen() {
    system("cls");
    int progress = 0;
    int loadingBarWidth = 50; // Width of the progress bar
    int barStartX = 10, barStartY = 5;

    // Draw the box around the loading bar
    SetConsoleTextAttribute(console, 7);  // Set to default color
    
    
    // Loop to simulate loading
    for (int i = 0; i < loadingBarWidth; i++) {
        progress = (i + 1) * 2; // Calculate percentage (0% to 100%)

        // Set color based on progress
        setColor(progress);

        // Draw the progress bar segment
        gotoxy(barStartX + i, barStartY + 1);
        cout << "±";

        // Display percentage
        gotoxy(0, 2);
        SetConsoleTextAttribute(console, 7); // Reset text color
        cout << "Loading: " << progress << "%" << flush;
        Sleep(50);  // Delay to simulate loading time
    }
    
    // After loading, move cursor down and reset color
    cout << "]" << endl;
    SetConsoleTextAttribute(console, 7); // Reset text color to normal
    Sleep(500); // Wait before proceeding to the game
}

void play(){
	carPosX = -1 + WIN_WIDTH / 2;
	carPosY = 22;  // reset car's vertical position
	score = 0;
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	enemyY[0] = enemyY[1] = 1;
	
	system("cls"); 
	drawBorder(); 
	updateScore();
	genEnemy(0);
	genEnemy(1);
	
	gotoxy(WIN_WIDTH + 7, 2); cout << "Car Game";
	gotoxy(WIN_WIDTH + 6, 4); cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6); cout << "----------";
	gotoxy(WIN_WIDTH + 7, 12); cout << "Control ";
	gotoxy(WIN_WIDTH + 7, 13); cout << "-------- ";
	gotoxy(WIN_WIDTH + 2, 14); cout << " A Key - Left";
	gotoxy(WIN_WIDTH + 2, 15); cout << " D Key - Right"; 
	gotoxy(WIN_WIDTH + 2, 16); cout << " W Key - Up"; 
	gotoxy(WIN_WIDTH + 2, 17); cout << " S Key - Down"; 
	
	gotoxy(18, 5); cout << "Press any key to start";
	getch();
	gotoxy(18, 5); cout << "                      ";
	
	while(1){
		if(kbhit()){
			char ch = getch();
			
			if(ch == 'a' || ch == 'A'){
				if(carPosX > 18)
					carPosX -= 4;
			}
			if(ch == 'd' || ch == 'D'){
				if(carPosX < 50)
					carPosX += 4;
			}
			if(ch == 'w' || ch == 'W'){
				if(carPosY > 1)
					carPosY -= 1;
			}
			if(ch == 's' || ch == 'S'){
				if(carPosY < 22)
					carPosY += 1;
			}
			if(ch == 27){  // Escape key
				break;
			}
		} 
		
		drawCar(); 
		drawEnemy(0); 
		drawEnemy(1); 
		if(collision() == 1){
			gameover();
			return;
		}
		Sleep(50);
		eraseCar();
		eraseEnemy(0);
		eraseEnemy(1); 
		
		if(enemyY[0] == 10)
			if(enemyFlag[1] == 0)
				enemyFlag[1] = 1;
		
		if(enemyFlag[0] == 1)
			enemyY[0] += 1;
		
		if(enemyFlag[1] == 1)
			enemyY[1] += 1;
		 
		if(enemyY[0] > SCREEN_HEIGHT - 4){
			resetEnemy(0);
			score++;
			updateScore();
		}
		if(enemyY[1] > SCREEN_HEIGHT - 4){
			resetEnemy(1);
			score++;
			updateScore();
		}
	}
}

int main(){
	setcursor(0, 0); 
	srand((unsigned)time(NULL)); 
	
	// Show the loading screen before the menu
	loadingScreen();
	
	do{
		system("cls");
		gotoxy(10,5); cout << " -------------------------- "; 
		gotoxy(10,6); cout << " |        Car Game        | "; 
		gotoxy(10,7); cout << " --------------------------";
		gotoxy(10,9); cout << "1. Start Game";
		gotoxy(10,10); cout << "2. Instructions";	 
		gotoxy(10,11); cout << "3. Quit";
		gotoxy(10,13); cout << "Select option: ";
		char op = getche();
		
		if(op == '1') play();
		else if(op == '2') instructions();
		else if(op == '3') exit(0);
		
	}while(1);
	
	return 0;
}