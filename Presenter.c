#include <time.h>
#include <stdlib.h>
#include "View.h"
#include "data_structures.h"
#include <windows.h>

FieldProperties (*cards)[FIELD_SIZE];
//FieldProperties cards[FIELD_SIZE][FIELD_SIZE];
int player;
int *points;
int pairsleft;

void validateGameInput();
void generateCards();
void compareCards();

int main() {
	int again;
	while (1) {
		again = 0;
		player = 0;
		points = malloc (2 * sizeof(int));
		points[0] = points[1] = 0;
		pairsleft = FIELD_SIZE * FIELD_SIZE / 2;
		cards = malloc (FIELD_SIZE * FIELD_SIZE * sizeof(FieldProperties));
		generateCards();
		printField(cards, points, player);
		int c_v1; 
		int c_h1; 
		int c_v2; 
		int c_h2;
		while(pairsleft) {
			validateGameInput(&c_v1, &c_h1);
			validateGameInput(&c_v2, &c_h2);
			compareCards(&c_v1, &c_h1, &c_v2, &c_h2, *cards);
		}
		again = gameOver(points);
		free(points);
		free(cards);
		if (again == 0)
		{
			return 0;
		}
	}
	return 0;
}

void compareCards(int *card1_x, int *card1_y, int *card2_x, int *card2_y, FieldProperties (*cards)[FIELD_SIZE]){
	if (cards[*card1_x][*card1_y].image == cards[*card2_x][*card2_y].image){
		Sleep(800);
		points[player] += 1;
		cards[*card1_x][*card1_y].animationState = -1;
		cards[*card2_x][*card2_y].animationState = -1;
		pairsleft -= 1;
		printField(cards, points, player);
	
	}
	else{
		Sleep(800);
		if (player == 1)
			player = 0;
		else
			player = 1;
		animate(*card1_x, *card1_y, cards, points, player);
		animate(*card2_x, *card2_y, cards, points, player);
	}
}

void validateGameInput(int *c_v,int *c_h) {
	while (1) {
		getGameInput(c_v, c_h);
		if (*c_v < 0 || *c_v > 3 || *c_h < 0 || *c_h > 3 ||
			cards[*c_v][*c_h].animationState == -1)
		{
			printf("\nEingabe ung\x81ltig! (W\x84hle zwei Koordinaten zwischen 1 und 4 von einer der sichtbaren Karten)\n");
		}
		else if (cards[*c_v][*c_h].animationState != 0)
		{
			printf("\nEingabe ung\x81ltig! (Es m\x81ssen zwei verschiedene Karten gew\x84hlt werden)\n");
		}
		else
		{
			animate(*c_v, *c_h, cards, points, player);
			return;
		}
	}
}

void generateCards() {
	//initialize cards with defaults
	for (int i = 0; i < FIELD_SIZE; ++i) {
		for (int j = 0; j < FIELD_SIZE; ++j)
		{
			cards[i][j].image = -1;
			cards[i][j].animationState = 0;
		}
	}
	//generate random numbers
	srand(time(NULL));
	int numChars = FIELD_SIZE * FIELD_SIZE / 2;
	int *chars = (int*) malloc (numChars * sizeof(int));
	int i = 0;
	while(1) {	//generatorLoop
		for (; i < numChars; ++i)
		{
			int ascii = (rand() % 25) + 65;
			chars[i] = ascii;
		}
		//check if unique
		for (int j = 0; j < numChars; ++j) //outer checkLoop
		{
			for (int k = j + 1; k < numChars; ++k) //inner checkLoop
			{
				if(chars[j] == chars[k]) {
					i = j;
					j = k = numChars; //break out of both checkLoops
					break;
				}
			}
		}
		if (i >= numChars)
		{
			break; //break out of generatorLoop
		}
	}
	//put numbers into cards
	for (i = 0; i < numChars; ++i)
	{
		for (int j = 0; j < 2; ++j) //2 cards per image
		{
			int position = rand() % FIELD_SIZE * FIELD_SIZE - 1;
			int x = position/FIELD_SIZE;
			int y = position%FIELD_SIZE;
			while(cards[x][y].image != -1) { //iterate until "empty" card is found
				position++;
				if (position > FIELD_SIZE * FIELD_SIZE - 1) {
					position = 0;
				}
				x = position/FIELD_SIZE;
				y = position%FIELD_SIZE;
			}
			cards[x][y].image = chars[i];
		}
	}
}