#ifndef MORPION_H
#define MORPION_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <vector>

#define N 3
#define BLOC 32
using namespace std;

enum {blue,red,draw};
enum {player1,player2,ai};

typedef struct
{
	int x,y;
}Move;

class Morpion
{
	private:
       int winner;
	   SDL_Surface *ecran;
	   vector < vector <int> > tab;
	   SDL_Surface *empty,*cross, *circle, *curseur_p1, *curseur_p2;
       int now_turn;
       bool PvP;
	public:
	   Morpion();
	   void run();
	   void launch();
	   void player1_turn();
	   void player2_turn();
	   void ai_turn();
       void show();
       bool check_full();
       bool check_end();
       bool check_line(int i,int n);
       bool check_diag(int n);
       bool check_row(int j,int n);
       void play();
       void reset();
       int evaluate();
       int minimax(int branch,bool ai_choice);
       Move best_choice();
};

#endif // MORPION_H
