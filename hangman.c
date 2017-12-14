//Programming in C
//MSG Mike Simpson
//
//Hangman game accepts and displays special characters in wordfile
//accepts upper and lower case guesses from user, displays in uppercase.
//will ask the user to play again and store their statics in ~/.hangman
//Expects to find a word list ~/.words

#include <stdio.h>
#include <stdlib.h>//srand system
#include <time.h>//time
#include <string.h>//strdup
#include <ctype.h>//tolower

#define MAXWORDLENGTH 36//supercalifragilisticexpialidocious

int guess_processing(char word[MAXWORDLENGTH], char word_output[MAXWORDLENGTH], int reset);
char get_input(void);
int test_guess(char guess_char);
int my_gui(char input, int wrong);
void gallows(int right_wrong);
void read_hist(int played[4]);
void write_hist(int played[4]);

int main(int argc, char* argv[])
{
	char word[MAXWORDLENGTH]= {'\0'}, word_output[MAXWORDLENGTH] = {'\0'};
	srand (time(NULL));
	char path[MAXWORDLENGTH];
    FILE *file_pointer = NULL;
    static int wrong_sum=0;
    int line_count =0, rand_index=0, end =0, no_blank_lines = 0;
    int win =0, lose=0, play = 0, test = 0, wguess = 0, wsum2 = 0;
	int played[4] = {0};
	read_hist(played);
	play = played[0];
	win = played[1];
	lose = played[2];
	wguess = played[3];
    wsum2 = wguess;
START:

	if (argc == 2){
         file_pointer = fopen(argv[1], "r");//if wordfile provided
    }else if (argc == 1){
		 if ( test == 0){
		 	test = 1;
			printf("No argument detected:  Using ~/.words\n");
		}
		 sprintf( path, "%s/%s", getenv("HOME"), ".words" );
		 file_pointer = fopen( path ,"r");//else default .words file
	}else {
         fprintf(stderr, "ERROR: wrong number of arguments\n"
                         "USEAGE: %s wordlist\n", argv[0]);
         return EXIT_FAILURE;
    }if (!file_pointer) {
         fprintf(stderr, "error: could not open textfile: %s\n", argv[1]);
         return EXIT_FAILURE;
    }
	line_count  = rand_index = end =0;
	no_blank_lines = 1;
	while(no_blank_lines == 1){	
		while(!feof(file_pointer)){//count lines in file
			fgets(word, MAXWORDLENGTH, file_pointer);
			line_count++;
		}//while
		rand_index = (rand() % line_count) + 1;
		fseek(file_pointer, 0, SEEK_SET);
		for(int i = 0; i < rand_index; ++i){
			fgets(word, MAXWORDLENGTH, file_pointer);
		}//for
	
		for(int i = 0 ; i < (signed)strlen(word); i++){//build word output blanks replace specual characters for blanks
			if ((word[i] >= 'a') && (word[i] <= 'z')){ //make the secret word uppercase
				word[i]+= 'A' - 'a';
			}

			if (word[i] >=0 && word[i] <= 64){//look for special chars and replace blank in word to guess
				word_output[i] = word[i];
			}else if(word[i] >= 91 && word[i] <= 96){
				word_output[i] = word[i];
			}else if(word[i]==126){
				word_output[i] = word[i];
			}else{
				word_output[i] = '_';
			}
		}//for
		fseek(file_pointer, 0, SEEK_SET);

		no_blank_lines = strlen(word);
		if(no_blank_lines>=2){//cant guess blank line in wordfile
			no_blank_lines=2;
		}
	}//while
	
	fclose(file_pointer);
	int reset = 0;
	end = guess_processing(word, word_output, reset);
	
	if (end == 1006){
		lose++;
        printf("%s\n", word_output);
        printf("%s\t\t\t YOU LOST!\n", word);
		wrong_sum += 6;//wguess is wrong guesses
//		printf("%d wrong_sum in lose fn\n", wrong_sum);
	}else{
		if (wrong_sum == end){//guessed the answer no missed guesses
		}
		else{
			wrong_sum += end;//wguess is wrong guesses
		}
		win++;
        printf("%s\n", word_output);
        printf("%s\t\tCONGRATULATIONS! YOU WIN!\n", word);
	}
	play = win + lose;
	double average = 0.0;
	wrong_sum += wsum2;
	wguess = wrong_sum;
	average = (wguess / play);
	if ((win!=1) && (lose!=1)){
	printf("Times Played: %d\t\tWins: %d\tLosses: %d\tAverage %0.1lf\n",play,win,lose,average);
	}if ((win==1) && (lose!=1)){
	printf("Times Played: %d\t\tWin: %d\tLosses: %d\tAverage %0.1lf\n",play,win,lose,average);
	}if ((win!=1) && (lose==1)){
	printf("Times Played: %d\t\tWins: %d\tLoss: %d\t\tAverage %0.1lf\n",play,win,lose,average);
	}if ((win==1) && (lose==1)){
	printf("Times Played: %d\t\tWin: %d\tLoss: %d\t\tAverage %0.1lf\n",play,win,lose,average);
	}
	char yes_no = '\0';
	while(1){ 
		printf("Would you like to play again? y/n: ");
		yes_no = getchar();
		while((getchar())!='\n'); //clean buffer
		if ((yes_no == 'y')||(yes_no == 'Y')){
			memset(word,'\0',MAXWORDLENGTH);//reset value of char array when game is over
			memset(word_output,'\0',MAXWORDLENGTH);
			my_gui('\0', 999);
			reset = 999;
			system("clear");
			goto START;
		}else if((yes_no == 'n')||(yes_no == 'N')){
			played[0]=play;
			played[1]=win;
			played[2]=lose;
			played[3]=wguess;
			write_hist(played);
			break;
		}else{
			printf("Invalid character entered!\nPlease enter y/Y or n/N\n");
		}//else
	}//while
}//main

char get_input(void){//needs work
	char guess_char = '\0';
	while(1){ 
		printf("Enter your guess: ");
		guess_char = getchar();
		while((getchar())!='\n'); //clean buffer
		if ((guess_char >='a'&& guess_char <='z')||(guess_char >= 'A' && guess_char <= 'Z')){
			break;
		}else{
			printf("Invalid character entered!\n");
		}//else
	}//while
	
	if(test_guess(guess_char)){
		printf("CHARACTER VALID: %c\n", guess_char);		
		return guess_char;
	}else{
		printf("CHARACTER INVALID: %c\n", guess_char);
		return '\0';
	}//else
}//get_input

int guess_processing(char word[MAXWORDLENGTH], char word_output[MAXWORDLENGTH], int reset){
    static char guess;
    static int wrong_check = 0, found = 0, wrong = 0; 
    static int end;
    my_gui('\0', wrong_check);//initial picture no wrong answers   
    while((found < (signed)strlen(word)-1)&&(wrong < 6)){//do it till we win or lose
        printf("%s\n", word_output);
        wrong_check = 1;
		guess = get_input();
		if(guess != '\0'){
			if((guess >= 'a') && (guess <= 'z')){//make guess capital
				guess += 'A' - 'a';
			}//if
            for( int i = 0; i < (signed)strlen(word) - 1; ++i){
                if(guess == word[i] && word[i] != word_output[i]){
                    word_output[i] = guess;
                    found++;
                    wrong_check = 0;
                }//if
			}//for
        }//if
		system("clear");
		if (reset == 999){
			reset = 0;//game reset no inc on wrong
		}
		else if((wrong_check == 1)&& (guess != '\0')){//if not valid dont inc wrong
			wrong++;
			reset = 0;
		}
		wrong_check = my_gui(guess, wrong_check);
    }//while
    if (wrong == 6){//return to main
		wrong = wrong_check =  found = 0;
		end = 1006;
	}else if(found == ((signed)strlen(word)-1)){
		end = wrong;//returning wrong plus 1 to account for bad guesses
		wrong = wrong_check =  found = 0;
	}
	return end;
}//guess_processing

int test_guess(char guess){
	
	if (guess >='a'&& guess <='z'){//make guess uppercase
		guess += 'A' - 'a';
        return 1;
    }else if(guess >= 'A' && guess <= 'Z'){
        return 1;
    }
    else
    {
        return 0;
    }
}//test_guess

int my_gui(char guess, int wrong_check){
    static char wrong_guess[7] = {'\0'}, right_guess[7] = {'\0'}; //needs to be static to keep words
    static int wrong_count = 0, right_count = 0;
	
	if (wrong_check == 999){//reset if u want to play again
		memset(wrong_guess,'\0',7);//reset value of char array when game is over
		memset(right_guess,'\0',7);
		right_count = 0;
		wrong_count = 0;
		wrong_check = 0;
	}else{

//    test_guess(guess_char);
	
		if(!wrong_check){
			right_guess[right_count++] = guess;
		}else{
			wrong_guess[wrong_count++] = guess;
		}
//    right_wrong = strlen(right_guess) + strlen(wrong_guess);//use both right and wrong
    printf("Incorrect Guesses: %s\n", wrong_guess);
//	printf("Correct Guesses: %s\n", right_guess);
	gallows(wrong_count);
	}//else

	return wrong_check;
}//my_gui

void gallows(int right_wrong){
//    printf("this is value of right_wrong in gallows: %d",right_wrong);
	switch(right_wrong){
     case 0:
      printf("Number of wrong letters: %d\n\n", right_wrong);
      printf("  _______\n"		);
      printf("  |/\n"			);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("__|_________\n\n"	);
     break;
     case 1:
      printf("Number of wrong letters: %d\n\n", right_wrong);
      printf("  _______\n"		);
      printf("  |/   | \n"		);
      printf("  |    O \n"		);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("__|_________\n\n");
     break;
     case 2:
      printf("Number of wrong letters: %d\n\n", right_wrong);
      printf("  _______\n"		);
      printf("  |/   | \n"		);
      printf("  |    O \n"		);
      printf("  |    |\n"		);
      printf("  |    |\n"		);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("__|_________\n\n"	);
     break;
     case 3:
      printf("Number of wrong letters: %d\n\n", right_wrong);
      printf("  _______\n"		);
      printf("  |/   | \n"		);
      printf("  |    O \n"		);
      printf("  |   \\|\n"		);
      printf("  |    | \n"		);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("__|_________\n\n"	);
     break;
     case 4:
      printf("Number of wrong letters: %d\n\n", right_wrong);
      printf("  _______\n"		);
      printf("  |/   | \n"		);
      printf("  |    O \n"		);
      printf("  |   \\|/\n"		);
      printf("  |    | \n"		);
      printf("  |\n"			);
      printf("  |\n"			);
      printf("__|_________\n\n"	);
     break;
     case 5:
      printf("Number of wrong letters: %d\n\n", right_wrong);
      printf("  _______\n"		);
      printf("  |/   | \n"		);
      printf("  |    O \n"		);
      printf("  |   \\|/\n"		);
      printf("  |    | \n"		);
      printf("  |   /\n"		);
      printf("  |\n"			);
      printf("__|_________\n\n"	);
     break;
     case 6:
      printf("Number of wrong letters: %d\n\n", right_wrong);
      printf("  _______\n"		);
      printf("  |/   | \n"		);
      printf("  |    | \n"		);
      printf("  |    X \n"		);
      printf("  |   \\|/\n"		);
      printf("  |    | \n"		);
      printf("  |   / \\\n"		);
      printf("__|_________\n\n"	);
     break;
 }
}

void read_hist(int played[4]){
    FILE *file_pointer;
    char path[30];
    char temp[10] = {'\0'};
    snprintf(path, 30, "%s/.hangman",getenv("HOME"));
    file_pointer = fopen(path, "r");
    if(file_pointer == NULL){
        file_pointer = fopen(path, "w");
        fprintf(file_pointer, "0\n0\n0\n0\n");
        fseek(file_pointer, 0, SEEK_SET);
    }
    fgets(temp, 10, file_pointer);
    played[0] = strtol(temp, NULL, 10);//play
    fgets(temp, 10, file_pointer);
    played[1] = strtol(temp, NULL, 10);//win
    fgets(temp, 10, file_pointer);
    played[2] = strtol(temp, NULL, 10);//lose
    fgets(temp, 10, file_pointer);
    played[3] = strtol(temp, NULL, 10);//wguess
    fclose(file_pointer);
}

void write_hist(int played[4]){
    FILE *file_pointer;
    char path[30];
    snprintf(path, 30, "%s/.hangman",getenv("HOME"));
    file_pointer = fopen(path, "w");
    fprintf(file_pointer, "%d\n%d\n%d\n%d\n", played[0], played[1], played[2], played[3]);
    fclose(file_pointer);
}
