#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[50];
    int wins;
    int losses;
    int draws;
} Player;

void showBoard(char board[3][3]);
int checkWinner(char board[3][3]);
int boardFull(char board[3][3]);
int getScore(char board[3][3]);
int bestMoveValue(char board[3][3], int depth, int isMaximizing);
void calculateBestMove(char board[3][3], int* bestRow, int* bestCol);
void thankYouScreen(char* name);
int pullPlayerRecord(char* name, Player* player);
void storePlayerRecord(Player* player);
void showPlayerRecord(Player* player);

int main() {
    char board[3][3];
    char symbol;
    int pos, repeat;
    int row, col;
    int gameMode;
    char player2nameInput[50]; 
    char playAgainChoice; 
    
    Player currentPlayer;
    Player player2Stats;
    char playername[50];

    SetConsoleOutputCP(65001);

    system("cls");
    printf("\n\n\n");
    printf("\033[1;36m"); 
    printf("        ========================================\n");
    printf("\n");
    printf("\033[1;33m"); 
    printf("              T I C   T A C   T O E\n");
    printf("\n");
    printf("                    GAME\n");
    printf("\n");
    printf("\033[1;32m"); 
    printf("                  WELCOME TO THE GAME\n");
    printf("\033[1;36m"); 
    printf("        ========================================\n");
    printf("\n\n");
    printf("\033[1;37m");
    printf("        =========================================================\n");
    printf("          Submitted By      : Shivansh Upadhyay\n");
    printf("          Sap Id            : 59002474\n");
    printf("          Batch             : B10\n");
    printf("          College           : UPES, School of Computer Science\n");
    printf("          Subject Teacher   : Professor Pankaj Badoni\n");
    printf("\n");
    printf("        ==========================================================\n");
    printf("\033[0m\n\n");

    
    printf("            Please tell me your name (Player X): ");
    scanf("%s", playername);
    
    // --- Load Player 1 Stats ---
    int isExisting = pullPlayerRecord(playername, &currentPlayer);
    if (!isExisting) {
        strcpy(currentPlayer.name, playername);
    }
    printf("\033[1;32m\n    Welcome back, %s! \033[0m", playername);
    showPlayerRecord(&currentPlayer);
    
    do {
        
        printf("\033[1;32m");
        printf("\n    Select Game Mode:\n");
        printf("    1. Player vs Player\n");
        printf("    2. Player vs AI (Unbeatable)\n");
        printf("    Enter choice (1 or 2): ");
        printf("\033[0m");
        
        if (playAgainChoice != 0) while(getchar() != '\n');
        
        while (scanf("%d", &gameMode) != 1 || (gameMode != 1 && gameMode != 2)) {
            printf("\033[1;31mInvalid choice. Please enter 1 or 2: \033[0m");
            while(getchar() != '\n');
        }

        if (gameMode == 1) {
            printf("    Enter Player 2's name (Player O): ");
            scanf("%s", player2nameInput);
            
            // --- Load Player 2 Stats ---
            int isP2Existing = pullPlayerRecord(player2nameInput, &player2Stats);
            if (!isP2Existing) {
                strcpy(player2Stats.name, player2nameInput);
            }
            showPlayerRecord(&player2Stats);
        } else {
            strcpy(player2Stats.name, "AI");
        }
        
        printf("\n    Press the Enter key when you are ready to start...");
        
        getchar();
        getchar();
        
        system("cls");

        
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++){
                board[i][j] = ' ';
            }
        }
        
        symbol = 'X';
        repeat = 1;
        
       
        do {
            system("cls");
            showBoard(board);
            
            if(symbol == 'X') {
                printf("\n\033[1;31m%s's turn (X).\033[0m Choose a spot (1-9): ", currentPlayer.name);
                while (scanf("%d", &pos) != 1 || pos < 1 || pos > 9) {
                    printf("\033[1;31mThat's not a spot! Choose a number between 1 and 9: \033[0m");
                    while(getchar() != '\n');
                }
                
                row = (pos - 1) / 3;
                col = (pos - 1) % 3;
            } 
            else {
                if (gameMode == 1) {
                    printf("\n\033[1;34m%s's turn (O).\033[0m Choose a spot (1-9): ", player2Stats.name);
                     while (scanf("%d", &pos) != 1 || pos < 1 || pos > 9) {
                        printf("\033[1;31mThat's not a spot! Choose a number between 1 and 9: \033[0m");
                        while(getchar() != '\n');
                    }
                    
                    row = (pos - 1) / 3;
                    col = (pos - 1) % 3;
                } else {
                    printf("\nAI is thinking... (O)\n");
                    Sleep(1000);
                    
                    calculateBestMove(board, &row, &col);
                    pos = row * 3 + col + 1;
                    
                    printf("AI chose spot %d\n", pos);
                    Sleep(1000);
                }
            }

            if(pos >= 1 && pos <= 9 && board[row][col] == ' ') {
                board[row][col] = symbol;
                
                if(checkWinner(board) == 1) {
                    system("cls");
                    showBoard(board);
                    
                    if(symbol == 'X') {
                        printf("\n \033[1;32m CONGRATULATIONS! %s WINS! \033[0m\n", currentPlayer.name);
                        currentPlayer.wins++;
                        player2Stats.losses++;
                    } else {
                        printf("\n \033[1;31m%s WINS! \033[0m\n", (gameMode == 1) ? player2Stats.name : "🤖 THE AI");
                        currentPlayer.losses++;
                        if (gameMode == 1) player2Stats.wins++;
                    }
                    repeat = 0;
                } 
                else if (boardFull(board)) {
                    system("cls");
                    showBoard(board);
                    printf("\n \033[1;33m IT'S A DRAW! \033[0m\n");
                    currentPlayer.draws++;
                    if (gameMode == 1) player2Stats.draws++;
                    repeat = 0;
                }

                if(repeat) {
                    symbol = (symbol == 'X') ? 'O' : 'X';
                }
            } else {
                if (symbol == 'X' || (symbol == 'O' && gameMode == 1)) {
                    printf("\033[1;31mThat spot is taken or not valid! Try again.\033[0m\n");
                    printf("Press Enter to continue...");
                    getchar();
                    getchar();
                }
            }
        } while(repeat);

       
        storePlayerRecord(&currentPlayer);
        if (gameMode == 1) storePlayerRecord(&player2Stats);
        
        showPlayerRecord(&currentPlayer);
        if (gameMode == 1) showPlayerRecord(&player2Stats);
        
        printf("\nPress \033[1;32mENTER\033[0m to continue...\n");
        getchar();
        getchar();

        system("cls");
        printf("\033[1;33m\n\n");
        printf("        =============================================\n");
        printf("        ||                                         ||\n");
        printf("        || \033[1;37mTHANK YOU FOR PLAYING, %s!\033[1;33m||\n", currentPlayer.name);
        printf("        ||                                         ||\n");
        printf("        =============================================\n");
        printf("\033[0m");

        printf("\n\n");
        printf("        \033[1;37mPlayer Records Updated! Ready for next game.\033[0m\n");
        
        printf("\033[1;35m\nDo you want to play another match? (y/n): \033[0m");
        scanf(" %c", &playAgainChoice);
        
        while (playAgainChoice != 'y' && playAgainChoice != 'Y' && playAgainChoice != 'n' && playAgainChoice != 'N') {
            printf("\033[1;31mInvalid input. Please enter 'y' or 'n': \033[0m");
            scanf(" %c", &playAgainChoice);
        }

    } while (playAgainChoice == 'y' || playAgainChoice == 'Y');

    system("cls");
    printf("\033[1;33m\n\nGoodbye, %s! Your records are safe.\033[0m\n", currentPlayer.name);

    return 0;
}


int boardFull(char board[3][3]) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(board[i][j] == ' ') {
                return 0;
            }
        }
    }
    return 1;
}

void showBoard(char board[3][3]) {
    
    char* superscript[] = {"⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹"};
    
    printf("\033[1;37m\n        "); 
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            
            if(board[i][j] == ' ') {
                int pos = i * 3 + j + 1;
                printf(" \033[1;33m%s\033[1;37m ", superscript[pos]);
            } 
            else {
                if (board[i][j] == 'X') {
                    printf(" \033[1;31m%c\033[1;37m ", board[i][j]);
                } else {
                    printf(" \033[1;34m%c\033[1;37m ", board[i][j]);
                }
            }
            
            if(j < 2) {
                printf("|");
            }
        }
        printf("\n");
        
        if(i < 2) {
            printf("        -----------\n        ");
        }
    }
    printf("\033[0m\n");
}

int checkWinner(char board[3][3]) {
    
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) return 1;
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) return 1;
    }
    
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) return 1;
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) return 1;
    
    return 0;
}


int getScore(char board[3][3]) {
    
    for(int i = 0; i < 3; i++) {
        
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            return (board[i][0] == 'O') ? +10 : -10;
        }
        
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            return (board[0][i] == 'O') ? +10 : -10;
        }
    }
    
    if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') return (board[0][0] == 'O') ? +10 : -10;
    if(board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') return (board[0][2] == 'O') ? +10 : -10;
    
    return 0;
}

int bestMoveValue(char board[3][3], int depth, int isMaximizing) {
    int score = getScore(board);
    
    if(score == 10) return score - depth;
    if(score == -10) return score + depth;
    if(boardFull(board)) return 0;
    
    if(isMaximizing) {
        int best = -1000;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(board[i][j] == ' ') {
                    board[i][j] = 'O';
                    int val = bestMoveValue(board, depth + 1, 0);
                    best = (val > best) ? val : best;
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
    else {
        int best = 1000;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(board[i][j] == ' ') {
                    board[i][j] = 'X';
                    int val = bestMoveValue(board, depth + 1, 1);
                    best = (val < best) ? val : best;
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

void calculateBestMove(char board[3][3], int* bestRow, int* bestCol) {
    int bestVal = -1000;
    *bestRow = -1;
    *bestCol = -1;
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(board[i][j] == ' ') {
                board[i][j] = 'O';
                
                int moveVal = bestMoveValue(board, 0, 0);
                
                board[i][j] = ' ';
                
                if(moveVal > bestVal) {
                    *bestRow = i;
                    *bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }
}

int pullPlayerRecord(char* name, Player* player) {
    FILE *file = fopen("scoreboard.txt", "r");
    
    player->wins = 0;
    player->losses = 0;
    player->draws = 0;
    
    if (file == NULL) return 0;

    char tempName[50];
    int tempWins, tempLosses, tempDraws;
    
    while (fscanf(file, "%[^,],%d,%d,%d\n", tempName, &tempWins, &tempLosses, &tempDraws) == 4) {
        if (strcmp(tempName, name) == 0) {
            strcpy(player->name, tempName);
            player->wins = tempWins;
            player->losses = tempLosses;
            player->draws = tempDraws;
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

void storePlayerRecord(Player* player) {
    if (strcmp(player->name, "AI") == 0) return; 

    FILE *file = fopen("scoreboard.txt", "r");
    FILE *temp = fopen("temp_score.txt", "w");
    
    if (temp == NULL) {
        if (file != NULL) fclose(file);
        return;
    }

    char tempName[50];
    int tempWins, tempLosses, tempDraws;
    int found = 0;
    
    if (file != NULL) {
        while (fscanf(file, "%[^,],%d,%d,%d\n", tempName, &tempWins, &tempLosses, &tempDraws) == 4) {
            if (strcmp(tempName, player->name) == 0) {
                fprintf(temp, "%s,%d,%d,%d\n", player->name, player->wins, player->losses, player->draws);
                found = 1;
            } else {
                fprintf(temp, "%s,%d,%d,%d\n", tempName, tempWins, tempLosses, tempDraws);
            }
        }
        fclose(file);
    }
    
    if (!found) {
        fprintf(temp, "%s,%d,%d,%d\n", player->name, player->wins, player->losses, player->draws);
    }
    
    fclose(temp);
    
    remove("scoreboard.txt");
    rename("temp_score.txt", "scoreboard.txt");
}

void showPlayerRecord(Player* player) {
    if (strcmp(player->name, "AI") == 0) return;

    int totalGames = player->wins + player->losses + player->draws;
    
    printf("\n");
    printf("\033[1;36m        ========================================\n");
    printf("                  %s's SCOREBOARD\n", player->name); 
    printf("        ========================================\033[0m\n");
    printf("\033[1;37m\n");
    printf("          Player: \033[1;32m%s\033[1;37m\n", player->name);
    printf("          Total Games: %d\n", totalGames);
    printf("          Wins:    \033[1;32m%d\033[1;37m\n", player->wins);
    printf("          Losses:  \033[1;31m%d\033[1;37m\n", player->losses);
    printf("          Draws:   \033[1;33m%d\033[1;37m\n", player->draws);
    
    if (totalGames > 0) {
        float winRate = (player->wins * 100.0) / totalGames;
        printf("          Win Rate: %.1f%%\n", winRate);
    }
    
    printf("        ========================================\n\033[0m\n");
}