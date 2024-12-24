#include <iostream>
#include <ctime>
#define GODS_NUMBER 12 //set the maximum number of moves of the algorithm
#define MAX_ITERATION 1e7 //set the maximum number of iteration allowed
using namespace std;


int ratio[4] = {1, 1, 1, 1}; //probability of R showing up to L to U to B


int move_count; //store the number of moves of the algorithm found
int iteration; //store the number of iteration used to find the algorithm


//define skewb's state
int centres[6]; // position of centres
int corners_p[7]; //position of corners
int corners_o[7]; //define the orientation of corners, for white or yellow colour, 0:face up/down, 1:face left/right, 2:face front/back
int centres_copy[6];
int corners_p_copy[7];
int corners_o_copy[7];


int track_moves[GODS_NUMBER]; //array to store moves
string notation[8] = {"R", "R'", "L", "L'", "U", "U'", "B", "B'"}; //define allowed moves (0 map to R, 1 map to R', and so on)


//function to turn the cube
void R(); //turn right clockwise
void Rp(); //turn right counterclockwise
void L(); //turn left clockwise
void Lp(); //turn left counterclockwise
void U(); //turn top clockwise
void Up(); //turn top counterclockwise
void B(); //turn back clockwise
void Bp(); //turn back counterclockwise
void (*turn[8])() = {R, Rp, L, Lp, U, Up, B, Bp}; //pointers pointing to functions, same order as in notation array defined above


int find_next_move(int last_move); //find next move
int weighted_monte_carlo();
int check(); //check if the cube is solved
void reset_cube(); //reset cube to inputted scrambled state
void set_cube(); //user input the scrambled state of the cube
int solve_cube(); //perform monte carlo to solve the cube and return 1 if solution was found
void print_solution(int solved); //print the solution


int main(){
	set_cube(); //input the scrambled cube
	srand(time(NULL)); //set seed for latter monte carlo simulation
	int solved = solve_cube();
	print_solution(solved);
	return 0;
}


void set_cube(){ //input the scrambled cube
	char colour[6][5]; //array for storing characters representing colours

	//user input colours
	for(int i=0; i<6; i++){
		for(int j=0; j<5; j++){
			cin >> colour[i][j];
		}
	}

	//input position of centres into centres array
	for(int i=0; i<6; i++){
		switch (colour[i][4]) {
			case 'w': centres_copy[i] = 0; break;
			case 'g': centres_copy[i] = 1; break;
			case 'r': centres_copy[i] = 2; break;
			case 'b': centres_copy[i] = 3; break;
			case 'o': centres_copy[i] = 4; break;
			case 'y': centres_copy[i] = 5; break;
		}
	}

	int corners_multiplied[7] = {colour[0][0]*colour[3][1]*colour[4][0], 
		colour[0][1]*colour[2][1]*colour[3][0],
		colour[0][3]*colour[1][0]*colour[4][1],
		colour[5][1]*colour[1][2]*colour[2][3],
		colour[5][2]*colour[2][2]*colour[3][3],
		colour[5][3]*colour[4][3]*colour[3][2],
		colour[5][0]*colour[1][3]*colour[4][2]};
	//input position of corners into corners_p array
	for(int i=0; i<7; i++){
		switch(corners_multiplied[i]) {
			case 1294482: corners_p_copy[i] = 0; break;
			case 1329468: corners_p_copy[i] = 1; break;
			case 1360527: corners_p_copy[i] = 2; break;
			case 1420782: corners_p_copy[i] = 3; break;
			case 1351812: corners_p_copy[i] = 4; break;
			case 1316238: corners_p_copy[i] = 5; break;
			case 1383393: corners_p_copy[i] = 6; break;
		}
	}

	//input orientation of corners into corners_o array
	//check corner 0
	if(colour[0][0]=='w'||colour[0][0]=='y'){corners_o_copy[corners_p_copy[0]] = 0;}
	else if(colour[4][0]=='w'||colour[4][0]=='y'){corners_o_copy[corners_p_copy[0]] = 1;}
	else if(colour[3][1]=='w'||colour[3][1]=='y'){corners_o_copy[corners_p_copy[0]] = 2;}

	//check corner 1
	if(colour[0][1]=='w'||colour[0][1]=='y'){corners_o_copy[corners_p_copy[1]] = 0;}
	else if(colour[2][1]=='w'||colour[2][1]=='y'){corners_o_copy[corners_p_copy[1]] = 1;}
	else if(colour[3][0]=='w'||colour[3][0]=='y'){corners_o_copy[corners_p_copy[1]] = 2;}

	//check corner 2
	if(colour[0][3]=='w'||colour[0][3]=='y'){corners_o_copy[corners_p_copy[2]] = 0;}
	else if(colour[4][1]=='w'||colour[4][1]=='y'){corners_o_copy[corners_p_copy[2]] = 1;}
	else if(colour[1][0]=='w'||colour[1][0]=='y'){corners_o_copy[corners_p_copy[2]] = 2;}

	//check corner 3
	if(colour[5][1]=='w'||colour[5][1]=='y'){corners_o_copy[corners_p_copy[3]] = 0;}
	else if(colour[2][3]=='w'||colour[2][3]=='y'){corners_o_copy[corners_p_copy[3]] = 1;}
	else if(colour[1][2]=='w'||colour[1][2]=='y'){corners_o_copy[corners_p_copy[3]] = 2;}

	//check corner 4
	if(colour[5][2]=='w'||colour[5][2]=='y'){corners_o_copy[corners_p[4]] = 0;}
	else if(colour[2][2]=='w'||colour[2][2]=='y'){corners_o_copy[corners_p_copy[4]] = 1;}
	else if(colour[3][3]=='w'||colour[3][3]=='y'){corners_o_copy[corners_p_copy[4]] = 2;}

	//check corner 5
	if(colour[5][3]=='w'||colour[5][3]=='y'){corners_o_copy[corners_p_copy[5]] = 0;}
	else if(colour[4][3]=='w'||colour[4][3]=='y'){corners_o_copy[corners_p_copy[5]] = 1;}
	else if(colour[3][2]=='w'||colour[3][2]=='y'){corners_o_copy[corners_p_copy[5]] = 2;}

	//check corner 6
	if(colour[5][0]=='w'||colour[5][0]=='y'){corners_o_copy[corners_p_copy[6]] = 0;}
	else if(colour[4][2]=='w'||colour[4][2]=='y'){corners_o_copy[corners_p_copy[6]] = 1;}
	else if(colour[1][3]=='w'||colour[1][3]=='y'){corners_o_copy[corners_p_copy[6]] = 2;}
}


void reset_cube(){//reset the cube back to the original scrambled state
	for(int i=0; i<6; i++){
		centres[i] = centres_copy[i];
	}
	for(int i=0; i<7; i++){
		corners_p[i] = corners_p_copy[i];
		corners_o[i] = corners_o_copy[i];
	}
}


int solve_cube(){
	int solved = 0; //0: not yet solved, 1: solved
	while(!solved&&iteration<MAX_ITERATION){
		int last_move = rand()%8; //initialise last_move to feed into find_next_move(), a random number is initialised so that the first move can be any moves
		int current_move;
		reset_cube(); //reset the cube back to the original scrambled state
		for(int i=0; i<GODS_NUMBER; i++){
			current_move = find_next_move(last_move); //find next move that is different from previous move
			last_move = current_move; //store current move
			track_moves[i] = current_move; //record the current move
			turn[current_move](); //turn the cube
			solved = check(); //check if the cube is solved
			if(solved==1){
				move_count=i+1; //if the cube is solved, store the number of moves of the algorithm
				break; //then stop the iteration
			}
		}
		iteration += 1;
	}
	return solved;
}


void print_solution(int solved){
	string link = "https://alpha.twizzle.net/edit/?puzzle=skewb&alg="; //web link to visualising solution
	string alg_inverse = "&setup-alg=";

	cout << "Number of iteration: " << iteration << endl; //print the iteration used
	if(solved==0){
		cout << "No solution found." << endl; //print if no solution found after reaching maximum number of iteration
		return;
	}
	cout << "Found a " << move_count << " moves solution!" << endl; //print the number of moves of the algorithm
	for(int i=0; i<move_count; i++){
		cout << notation[track_moves[i]] << " "; //print out the algorithm
		link += notation[track_moves[i]] + "+";
		//find the inverse of the algorithm found
		if (track_moves[move_count-i-1]%2==0){
			alg_inverse += notation[track_moves[move_count-i-1]+1] + "+";
		}
		else{
			alg_inverse += notation[track_moves[move_count-i-1]-1] + "+";
		}
	}
	cout << endl << "Solution visualiser: " << link.substr(0, link.length()-1) << alg_inverse.substr(0, alg_inverse.length()-1) << endl; //print the web link
}


int weighted_monte_carlo(){
	int current_move;
	int side = rand()%(ratio[0]+ratio[1]+ratio[2]+ratio[3]); //choose which side to turn
	int clockwise = rand()%2; //0: counterclockwise, 1: clockwise

	if(side<ratio[0]){ //if right side is chosen
		if(clockwise==1){current_move = 0;} //set current move to R
		else{current_move = 1;} //set current move to R'
	}
	else if(side>=ratio[0]&&side<ratio[0]+ratio[1]){ //if left side is chosen
		if(clockwise==1){current_move = 2;} //set current move to L
		else{current_move = 3;} //set current move to L'
	}
	else if(side>=ratio[0]+ratio[1]&&side<ratio[0]+ratio[1]+ratio[2]){ //if top side is chosen
		if(clockwise==1){current_move = 4;} //set current move to U
		else{current_move = 5;} //set current move to U'
	}
	else if(side>=ratio[0]+ratio[1]+ratio[2]&&side<ratio[0]+ratio[1]+ratio[2]+ratio[3]){ //if top side is chosen
		if(clockwise==1){current_move = 6;} //set current move to B
		else{current_move = 7;} //set current move to B'
	}
	return current_move;
}


int find_next_move(int last_move){
	int current_move;
	if(last_move==0 || last_move==1){//if the last move was R or R', the next move should not contain R
		do{
			current_move = weighted_monte_carlo();
		}while(current_move==0 || current_move==1);
	}
	if(last_move==2 || last_move==3){//if the last move was L or L', the next move should not contain L
		do{
			current_move = weighted_monte_carlo();
		}while(current_move==2 || current_move==3);
	}
	if(last_move==4 || last_move==5){//if the last move was U or U', the next move should not contain U
		do{
			current_move = weighted_monte_carlo();
		}while(current_move==4 || current_move==5);
	}
	if(last_move==6 || last_move==7){//if the last move was B or B', the next move should not contain B
		do{
			current_move = weighted_monte_carlo();
		}while(current_move==6 || current_move==7);
	}
	return current_move;
}


int check(){
	for(int i=0; i<6; i++){ //check for centres
		if(centres[i]!=i){ //if any of the centre is not in the correct position, return 0 (not yet solved), then stop the iteration
			return 0;
			break;
		}
	}
	for(int i=0; i<7; i++){ //check for corners
		if(corners_p[i]!=i){ //if any of the corner is not in the correct position, return 0 (not yet solved), then stop the iteration
			return 0;
			break;
		}
		if(corners_o[i]!=0){ //if any of the corner is not oriented correctly, return 0 (not yet solved), then stop the iteration
			return 0;
			break;
		}
	}
	return 1; //if all are in correct position, return 1 (solved)
}


void R(){
	//permutation (doing R move would change the position of the centres as listed below)
	int temp = centres[2];
	centres[2] = centres[5];
	centres[5] = centres[3];
	centres[3] = temp;

	//permutation (doing R move would change the position of the corners as listed below)
	temp = corners_p[1];
	corners_p[1] = corners_p[3];
	corners_p[3] = corners_p[5];
	corners_p[5] = temp;

	//orientation (doing R move would change the orientation of the corners as listed below)
	for(int i=1; i<6; i++){
		if(i==2){continue;}
		if(corners_o[corners_p[i]]==0){corners_o[corners_p[i]]=1;}
		else if(corners_o[corners_p[i]]==1){corners_o[corners_p[i]]=2;}
		else if(corners_o[corners_p[i]]==2){corners_o[corners_p[i]]=0;}
	}
}


void Rp(){
	//permutation (doing R' move would change the position of the centres as listed below)
	int temp = centres[2];
	centres[2] = centres[3];
	centres[3] = centres[5];
	centres[5] = temp;

	//permutation (doing R' move would change the position of the corners as listed below)
	temp = corners_p[1];
	corners_p[1] = corners_p[5];
	corners_p[5] = corners_p[3];
	corners_p[3] = temp;

	//orientation (doing R' move would change the orientation of the corners as listed below)
	for(int i=1; i<6; i++){
		if(i==2){continue;}
		if(corners_o[corners_p[i]]==0){corners_o[corners_p[i]]=2;}
		else if(corners_o[corners_p[i]]==1){corners_o[corners_p[i]]=0;}
		else if(corners_o[corners_p[i]]==2){corners_o[corners_p[i]]=1;}
	}
}


void L(){
	//permutation (doing L move would change the position of the centres as listed below)
	int temp = centres[1];
	centres[1] = centres[4];
	centres[4] = centres[5];
	centres[5] = temp;

	//permutation (doing L move would change the position of the corners as listed below)
	temp = corners_p[2];
	corners_p[2] = corners_p[5];
	corners_p[5] = corners_p[3];
	corners_p[3] = temp;

	//orientation (doing L move would change the orientation of the corners as listed below)
	for(int i=2; i<7; i++){
		if(i==4){continue;}
		if(corners_o[corners_p[i]]==0){corners_o[corners_p[i]]=1;}
		else if(corners_o[corners_p[i]]==1){corners_o[corners_p[i]]=2;}
		else if(corners_o[corners_p[i]]==2){corners_o[corners_p[i]]=0;}
	}
}


void Lp(){
	//permutation (doing L' move would change the position of the centres as listed below)
	int temp = centres[1];
	centres[1] = centres[5];
	centres[5] = centres[4];
	centres[4] = temp;

	//permutation (doing L' move would change the position of the corners as listed below)
	temp = corners_p[2];
	corners_p[2] = corners_p[3];
	corners_p[3] = corners_p[5];
	corners_p[5] = temp;

	//orientation (doing L' move would change the orientation of the corners as listed below)
	for(int i=2; i<7; i++){
		if(i==4){continue;}
		if(corners_o[corners_p[i]]==0){corners_o[corners_p[i]]=2;}
		else if(corners_o[corners_p[i]]==1){corners_o[corners_p[i]]=0;}
		else if(corners_o[corners_p[i]]==2){corners_o[corners_p[i]]=1;}
	}
}


void U(){
	//permutation (doing U move would change the position of the centres as listed below)
	int temp = centres[0];
	centres[0] = centres[3];
	centres[3] = centres[4];
	centres[4] = temp;

	//permutation (doing U move would change the position of the corners as listed below)
	temp = corners_p[1];
	corners_p[1] = corners_p[5];
	corners_p[5] = corners_p[2];
	corners_p[2] = temp;

	//orientation (doing U move would change the orientation of the corners as listed below)
	for(int i=0; i<6; i++){
		if(i==3||i==4){continue;}
		if(corners_o[corners_p[i]]==0){corners_o[corners_p[i]]=1;}
		else if(corners_o[corners_p[i]]==1){corners_o[corners_p[i]]=2;}
		else if(corners_o[corners_p[i]]==2){corners_o[corners_p[i]]=0;}
	}
}


void Up(){
	//permutation (doing U' move would change the position of the centres as listed below)
	int temp = centres[0];
	centres[0] = centres[4];
	centres[4] = centres[3];
	centres[3] = temp;

	//permutation (doing U' move would change the position of the corners as listed below)
	temp = corners_p[1];
	corners_p[1] = corners_p[2];
	corners_p[2] = corners_p[5];
	corners_p[5] = temp;

	//orientation (doing U' move would change the orientation of the corners as listed below)
	for(int i=0; i<6; i++){
		if(i==3||i==4){continue;}
		if(corners_o[corners_p[i]]==0){corners_o[corners_p[i]]=2;}
		else if(corners_o[corners_p[i]]==1){corners_o[corners_p[i]]=0;}
		else if(corners_o[corners_p[i]]==2){corners_o[corners_p[i]]=1;}
	}
}


void B(){
	//permutation (doing B move would change the position of the centres as listed below)
	int temp = centres[3];
	centres[3] = centres[5];
	centres[5] = centres[4];
	centres[4] = temp;

	//permutation (doing B move would change the position of the corners as listed below)
	temp = corners_p[0];
	corners_p[0] = corners_p[4];
	corners_p[4] = corners_p[6];
	corners_p[6] = temp;

	//orientation (doing B move would change the orientation of the corners as listed below)
	for(int i=0; i<7; i++){
		if(i==1||i==2||i==3){continue;}
		if(corners_o[corners_p[i]]==0){corners_o[corners_p[i]]=2;}
		else if(corners_o[corners_p[i]]==1){corners_o[corners_p[i]]=0;}
		else if(corners_o[corners_p[i]]==2){corners_o[corners_p[i]]=1;}
	}
}


void Bp(){
	//permutation (doing B' move would change the position of the centres as listed below)
	int temp = centres[3];
	centres[3] = centres[4];
	centres[4] = centres[5];
	centres[5] = temp;

	//permutation (doing B' move would change the position of the corners as listed below)
	temp = corners_p[0];
	corners_p[0] = corners_p[6];
	corners_p[6] = corners_p[4];
	corners_p[4] = temp;

	//orientation (doing B' move would change the orientation of the corners as listed below)
	for(int i=0; i<7; i++){
		if(i==1||i==2||i==3){continue;}
		if(corners_o[corners_p[i]]==0){corners_o[corners_p[i]]=1;}
		else if(corners_o[corners_p[i]]==1){corners_o[corners_p[i]]=2;}
		else if(corners_o[corners_p[i]]==2){corners_o[corners_p[i]]=0;}
	}
}
