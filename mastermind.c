#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define xstr(x) #x
#define str(x)  xstr(x)
#define CODE_SIZE 4
#define NUM_TRIES 10
#define NUM_INDENTATION 4

typedef int CODE[CODE_SIZE];

// removes a certain character from string
void remove_char(char * str, char charToRemmove){
    int i, j;
    int len = strlen(str);
    for(i=0; i<len; i++)
    {   
        if(str[i] == charToRemmove)
        {
            for(j=i; j<len; j++)
            {
                str[j] = str[j+1];
            }
            len--;
            i--;
        }
    }
    
}

// check if element is in a set or not
bool in_set(int set[], int element, int setSize){
    
    for (int i = 0; i < setSize; i++)
    {
       if(element == set[i]){
           return true;
       }
    }
    
    return false;
}

// black key == correct, white key == misplaced
void evaluate_guess(CODE guess, CODE secret, int wb[]){
    // wb[0] == white , wb[1] == black
    for (int i = 0; i < CODE_SIZE; i++)
    {   
        if(in_set(guess, secret[i], CODE_SIZE)){
            if(guess[i] == secret[i]){
                wb[1]++;
            } else {
                wb[0]++;
            }
        }
    } 
}

// 1:Vert(v), 2:Jaune(j), 3:Rouge(r), 4:Blue(b), 5:Blanc(w), 6:Noir(n)
void get_guess(CODE propositions[], int guess_count){

    printf("\nSaisissez une combinaison pour votre essai %d (V,J,R,B,W,N): ", guess_count+1);
    char str[CODE_SIZE]; 
    scanf("%" str(CODE_SIZE) "s", str);
    //clean string
    //char space = 'a';
    // remove_char(str, space);
    for (int i = 0; i < CODE_SIZE; i++)
    {
        switch (str[i])
        {
        case 'v':
        case 'V':
            propositions[guess_count][i] = 1;
            break;
        case 'j':
        case 'J':
            propositions[guess_count][i] = 2;

            break;
        case 'r':
        case 'R':
            propositions[guess_count][i] = 3;
            break;
        case 'b':
        case 'B':
            propositions[guess_count][i] = 4;
            break;
        case 'w':
        case 'W':
            propositions[guess_count][i] = 5;
            break;
        case 'n':
        case 'N':
            propositions[guess_count][i] = 6;
            break;
        default:
            propositions[guess_count][i] = 0;
            break;
        }
    }
}

// converts a number to its corresponding color character
char convert_int_to_color(int num){

    switch (num)
    {
    case 1:
        return 'V';
    case 2:
        return 'J';
    case 3:
        return 'R';
    case 4:
        return 'B';
    case 5:
        return 'W';
    case 6:
        return 'N';
    default:
        return ' ';
    }
}

// initiailizes all guesses to 0
void init_propostions(CODE propositions[]){
    for (int i = 0; i < NUM_TRIES; i++)
    {
        for (int j = 0; j < CODE_SIZE; j++)
        {
            propositions[i][j] = 0;
        }
        
    }
}

// helper function for quickly add indentation
void indent(int n)
{
    for (int i = 0; i < n; i++)
        putchar('\t');
}

// prints game start interface 
void init_interface(){
    system("cls");
    printf("Bienvenue au jeu MASTERMIND ^__^");
    printf("\n-----------------------------------------------------------------------------------");
    printf("\nLe code secret est compose de quatre couleurs parmi les six couleurs suivantes :");
    printf("\nVert (V), Jaune (J), Rouge (R), Bleu (B), Blanc (W), Noir (N)");
    printf("\nC'est parti ! ");
    printf("\n");
    printf("Essais");
    indent(NUM_INDENTATION);
    printf("Reponses");
    indent(NUM_INDENTATION);
    printf("(noir/blanc)");
    for (int i = 0; i < NUM_TRIES; i++){
        printf("\n");
        printf("  %d", i);
        indent(NUM_INDENTATION);
        printf("| | | | |");
        indent(NUM_INDENTATION);
        printf("  | | |");   
    }
}

// prints the interface of a certain round
void draw_interface(CODE propositions[], CODE secret, int current){
    printf("\nLe code secret est compose de quatre couleurs parmi les six couleurs suivantes :");
    printf("\nVert (V), Jaune (J), Rouge (R), Bleu (B), Blanc (W), Noir (N)");
    printf("\n");
    printf("Essais");
    indent(NUM_INDENTATION);
    printf("Reponses");
    indent(NUM_INDENTATION);
    printf("(noir/blanc)");
    for (int i = 0; i < NUM_TRIES; i++){
        printf("\n");
        printf("  %d", i);
        indent(NUM_INDENTATION);
        for (int j = 0; j < CODE_SIZE; j++){
            printf("|%c", convert_int_to_color(propositions[i][j]));
        }
        printf("|");
        indent(NUM_INDENTATION);
        if(i>current){
            printf("  | | |");
        } else {
            int wb[2] = {0};
            evaluate_guess(propositions[i], secret, wb);
            printf("  |%d|%d|", wb[1], wb[0]);

        }
    }
}

void generate_random_secret(CODE secret){
    srand(time(NULL));
    for (int i = 0; i < CODE_SIZE; i++)
    {   
        int random_color = rand()%4+1;
        while(in_set(secret, random_color, CODE_SIZE)){
            random_color = rand()%4+1;
        }

        secret[i] = random_color;
    }
}

int main(int argc, char **argv) {
    CODE secret = {0}; 
    CODE propositions[NUM_TRIES];
    generate_random_secret(secret);
    
    init_propostions(propositions);
    init_interface();
    int try_count = 0;

    
    while (try_count<NUM_TRIES)
    {   
        int wb[2] = {0};
        
        get_guess(propositions, try_count);
        evaluate_guess(propositions[try_count], secret, wb);
        
        if(wb[1] == CODE_SIZE){
            printf("\nFelicitation ! Tu as trouve notre secret, garde le tres bien ;)");
            exit(1);
        }
        
        // clear screen
        system("cls");

        // draw interface for next round
        draw_interface(propositions, secret, try_count);
        try_count++;
    }
    
    printf("\nOops ! Tu as perdu cette partie :( Mais tu peux toujours recommencer ;)");
    return 0;
}
