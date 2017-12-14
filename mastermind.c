//MasterMind
//MSG Mike Simpson

#include <stdio.h>
#include <stdlib.h>//srand
#include <time.h>//time
#include <string.h>//strlen

const int FOUR = 4;
int win_chk(int red, int guess_count);
int score_calc(int red, int white);
int test_guess(char *guess);
int test_again(char *guess);
int auto_solve (char *secret);
int redwht_cnt(char *secret, char *input);
void peg_counter(char *peg);
void correct_check(char *guess, char *result, char *code);

int main(void)
{
	char guess[8]={'\0'}, secret[5]={'\0'};
	int red = 0, guess_count = 0, done = 1, user_test = 0, u_again=1;

START:
	srand(time(NULL));
	for(int i=0; i < FOUR; i++){
		secret[i] = (rand() % 10) + '0';//generate random secret number + 48 ASCII
	}
//show secret number
	printf("***  %4s  *** SECRET NUMBER ***  %4s  *** \n", secret, secret);


	while(done){//done will be 0 when complete
		printf("Enter 4 digit guess or auto: ");
		scanf("%7s", guess);
		while (getchar()!='\n');//clear cache
		user_test = (test_guess(guess));//test user input 
						//0 = bad user
		if (user_test == 0){		//1 = good
			goto START;		//2 = auto
		}else if (user_test == 2){
			guess_count = auto_solve(secret);//call auto_solve and return number of guesses
			red = FOUR;
			done = win_chk(red, guess_count);
			break;
		}else{//good user guess
			guess_count++;//+1 valid guess
			red = redwht_cnt(secret,guess);
			done = win_chk(red, guess_count);//chk for winner
		}
	}

	while (u_again){
		printf("\nWould you like to play again? y/n: ");
		scanf("%7s", guess);
		guess_count = 0;
		while (getchar()!='\n');//clear cache
		u_again = (test_again(guess));//test user input
	  	if (u_again == 5){
			done = 1;
			goto START;
	  	}
	}
	return 0;
}

int test_again(char *guess){
	if (guess[0] == 'y'){
		return 5;
	}else{
		printf("Thank you for playing MasterMind!\n");
		return 0;
	}
	return 0;
}

int test_guess(char *guess){
	if (strlen(guess)==4){//4 chars entered
		if (guess[0] == 'a'&& guess[1] == 'u'&& guess[2] == 't'&& guess[3] == 'o'){
			return 2;
		}
		for (int i = 0; i < FOUR; i ++){
			if ((guess[i]<'0'||guess[i]>'9')){//check for valid input
				printf("Invalid character detected\n");
				printf("please enter 4 digit number\n");
				return 0;//bad user data 
			}
		}
	}else{//!4 characters entered
		printf("Incorrect number of digits:\n");
		printf("please enter 4 digits.\n");
		return 0;//bad user data 
	}
	return 1;//good user input
}

int win_chk(int red, int guess_count){//have I won?!
	int done = 1;
	if (red==FOUR){
		printf("MasterMind Solution Found ");
		if (guess_count == 1){
			printf("in %d guess!\n", guess_count);
			done = 0;
		}else{
			printf("in %d guesses.\n", guess_count);
			done = 0;
		}
	}
	return done;
}

int redwht_cnt(char *secret, char *guess){
	char num[5]={'\0'};//copy of secret
	int red = 0, white =0;//reset values for each guess

	for (int i = 0;i < FOUR; i++){//copy secret number for each guess, changed in fn
		num[i] = secret[i];
	}
	for (int j=0; j<FOUR; j++){
		if (num[j]==guess[j]){//chk red
			num[j] = 'S';
			guess[j] = 'R';//set to another value so it wont flag white in another place
			red++;//inc red count
		}
	}
	for (int j=0; j<FOUR; j++){
		for (int k=0; k<FOUR; k++){
			if ((num[j]==guess[k])){
				guess[k]='X';//set to another value so it wont flag white in another place
				num[j]='W';//set to another value so it wont flag white in another place
				white++;//inc white count
			}
		}
	}
  	printf("%d red, %d white\n", red, white);
	return red;
}

int auto_solve(char *secret){
	char auto_secret[4] = {'\0'}, guess1[4] = {'\0'}, guess2[4] = {'\0'};
	char answer_list[10][4], score_list[10] = {'\0'}, result[] = {0, 0};
	int guesses = 0, solutions = 0, test_solution = 0, test_score = 0, score = 0;

	for (int i = 0;i<4;i++){//reassign secret number to auto_secret
		auto_secret[i] = (int)secret[i]- '0';//reassigning to pointer
	}

	guess1[0] = 0, guess1[1] = 0, guess1[2] = 1, guess1[3] = 1;
	guess2[0] = 0, guess2[1] = 0, guess2[2] = 1, guess2[3] = 1;
	printf("Peg/Codes\t\tGuess\n");
	printf("==============================\n");

	while(1){//until break
		solutions = test_solution = 1;

		guesses++;//inc guesses
		correct_check(guess1, result, auto_secret);//process guess
		score = score_calc(result[0],result[1]);//score gets value

		for (int i = 0;i<FOUR;i++){
			answer_list[guesses-1][i] = guess1[i];
		}//for i
		score_list[guesses-1] = score;

		if(result[0] == FOUR){//got 4 done
		  guesses--;
		  break;//win
		}//if result

		while(solutions){
			test_solution = 1;
			for(int i=0; i<guesses; i++){
				correct_check(guess2, result, answer_list[i]);
				test_score = score_calc(result[0],result[1]);
		      		if(test_score != score_list[i]){
					test_solution = 0;
					break;
		      		}//if
			}//for i
        		if(test_solution){
				break;
	    		}//if test_solution = 1

			peg_counter(guess2);//call peg_counter on guess
		}//while solutions

		for (int i = 0;i<FOUR;i++){
			guess1[i] = guess2[i];//copy
		}

		printf("[%d %d]\t\t\t%d%d%d%d\n", result[0],result[1],guess2[0],guess2[1],guess2[2],guess2[3]);
	}//while true..

    	if(result[0] == FOUR){//found it
//        	printf("\t\n-- %d %d %d %d -- \n\n",auto_secret[0],auto_secret[1],auto_secret[2],auto_secret[3]);
		return guesses;
    	}
	return 0;
}

void correct_check(char *guess, char *result, char *code){
    result[0] = 0, result[1] = 0;
    char reds[4] = {0}, whites[4] = {0};

    for(int i=0; i<FOUR; i++){//detect reds
        if(guess[i] == code[i]){
            reds[i]=1;
        }else{
            reds[i]=0;
	}
    }//for i

    for(int i=0; i<FOUR; i++){//red&white_cnt
        if(reds[i]!=1){
            for(int f=0; f<FOUR; f++){
                if(f==i){
			continue;
		}
                if((guess[i] == code[f] && reds[f] != 1)){//not red find position
			if(whites[f] == 0){
				whites[f]=1;
				break;
			}
		}
             }//for f
        }
    }//for i
    for(int i=0; i<4; i++){//for each element
        result[0] += reds[i];//inc red
        result[1] += whites[i];//inc white
    }//for i
}

int score_calc(int red, int white){
    if(red == 0 && white == 0) return 0;
    if(red == 0 && white == 1) return 1;
    if(red == 1 && white == 0) return 2;
    if(red == 0 && white == 2) return 3;
    if(red == 1 && white == 1) return 4;
    if(red == 2 && white == 0) return 5;
    if(red == 0 && white == 3) return 6;
    if(red == 1 && white == 2) return 7;
    if(red == 2 && white == 1) return 8;
    if(red == 3 && white == 0) return 9;
    if(red == 0 && white == 4) return 10;
    if(red == 1 && white == 3) return 11;
    if(red == 2 && white == 2) return 12;
    if(red == 4 && white == 0) return 13;
    return 0;
}

void peg_counter(char *peg){//check guess 4 pegs
    peg[3]++;
    if(peg[3] == 10){
        peg[2]++;
        if(peg[2] == 10){
            peg[1]++;
            if(peg[1] == 10){
                peg[0]++;
                if(peg[0] == 10){ 
					peg[0] = 0;
				}
			
			peg[1] = 0;
			}
		
            peg[2] = 0;
        }
        peg[3] = 0;
    }
}
