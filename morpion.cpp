#include "morpion.h"
#include <SDL/SDL.h>

Morpion::Morpion()
{
	winner = draw;
	now_turn = player1;
	PvP = false;
	ecran = SDL_SetVideoMode(N * BLOC, N * BLOC, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	for (int i = 0; i < N; i++)
		tab.push_back(vector < int >(N));
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			tab[i][j] = 0;
	empty = IMG_Load("./data/images/empty.png");
	cross = IMG_Load("./data/images/cross.png");
	circle = IMG_Load("./data/images/circle.png");
	curseur_p1 = IMG_Load("./data/images/curseur_p1.png");
	curseur_p2 = IMG_Load("./data/images/curseur_p2.png");
}

void Morpion::show()
{
	SDL_Rect pos;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			pos.x = j * BLOC;
			pos.y = i * BLOC;
			if (tab[i][j] == 0)
				SDL_BlitSurface(empty, NULL, ecran, &pos);
			if (tab[i][j] == 1)
				SDL_BlitSurface(cross, NULL, ecran, &pos);
			if (tab[i][j] == 2)
				SDL_BlitSurface(circle, NULL, ecran, &pos);
		}
}

void Morpion::player1_turn()
{
	SDL_Rect pos_curseur;
	pos_curseur ={0, 0};
	bool end_turn = false;
	SDL_Event event;
	while (!end_turn)
	{
		show();
		SDL_BlitSurface(curseur_p1, NULL, ecran, &pos_curseur);
		SDL_Flip(ecran);
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				exit(0);
				case SDLK_UP:
					{
						if (pos_curseur.y > 0)
							pos_curseur.y -= BLOC;
						break;
					}
				case SDLK_DOWN:
					{
						if (pos_curseur.y < BLOC * (N - 1))
							pos_curseur.y += BLOC;
						break;
					}
				case SDLK_LEFT:
					{
						if (pos_curseur.x > 0)
							pos_curseur.x -= BLOC;
						break;
					}
				case SDLK_RIGHT:
					{
						if (pos_curseur.x < BLOC * (N - 1))
							pos_curseur.x += BLOC;
						break;
					}
				case SDLK_SPACE:
					{
						if (tab[pos_curseur.y / BLOC][pos_curseur.x / BLOC] == 0)
						{
							tab[pos_curseur.y / BLOC][pos_curseur.x / BLOC] = 1;
							end_turn = true;
						}
						break;
					}
				}
				break;
			}
		}
	}
}

void Morpion::player2_turn()
{
	SDL_Rect pos_curseur;
	pos_curseur =
	{
	(N - 1) * BLOC, (N - 1) * BLOC};
	bool end_turn = false;
	SDL_Event event;
	while (!end_turn)
	{
		show();
		SDL_BlitSurface(curseur_p2, NULL, ecran, &pos_curseur);
		SDL_Flip(ecran);
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				exit(0);
				case SDLK_UP:
					{
						if (pos_curseur.y > 0)
							pos_curseur.y -= BLOC;
						break;
					}
				case SDLK_DOWN:
					{
						if (pos_curseur.y < BLOC * (N - 1))
							pos_curseur.y += BLOC;
						break;
					}
				case SDLK_LEFT:
					{
						if (pos_curseur.x > 0)
							pos_curseur.x -= BLOC;
						break;
					}
				case SDLK_RIGHT:
					{
						if (pos_curseur.x < BLOC * (N - 1))
							pos_curseur.x += BLOC;
						break;
					}
				case SDLK_SPACE:
					{
						if (tab[pos_curseur.y / BLOC][pos_curseur.x / BLOC] == 0)
						{
							tab[pos_curseur.y / BLOC][pos_curseur.x / BLOC] = 2;
							end_turn = true;
						}
						break;
					}
				}
				break;
			}
		}
	}
}

bool Morpion::check_line(int i, int n)
{
	for (int k = 0; k < N; k++)
		if (tab[i][k] != n)
			return false;
	return true;
}

bool Morpion::check_row(int j, int n)
{
	for (int k = 0; k < N; k++)
		if (tab[k][j] != n)
			return false;
	return true;
}

bool Morpion::check_diag(int n)
{
	if (tab[2][0]==tab[1][1] && tab[1][1]==tab[0][2] && tab[2][0] == n)
		return true;
	for (int k = 0; k < N; k++)
		if (tab[k][k] != n)
			return false;
	return true;
}

bool Morpion::check_full()
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (tab[i][j] == 0)
				return false;
		}
	winner = draw;
	return true;
}

bool Morpion::check_end()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (tab[i][j] == 1 && (check_line(i, 1) || check_row(j, 1) || check_diag(1)))
			{
				winner = blue;
				return true;
			}
			if (tab[i][j] == 2 && (check_line(i, 2) || check_row(j, 2) || check_diag(2)))
			{
				winner = red;
				return true;
			}
		}
		return false;
	}
}

void Morpion::ai_turn()
{
	tab[best_choice().x][best_choice().y] = 2;
	show();
	SDL_Flip(ecran);
}

int Morpion::evaluate()
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (tab[i][j] == 1 && (check_line(i, 1) || check_row(j, 1) || check_diag(1)))
				return -10;
			else if (tab[i][j] == 2 && (check_line(i, 2) || check_row(j, 2) || check_diag(2)))
				return 10;
		}
	return 0;
}

int Morpion::minimax(int branch, bool ai_choice)
{
	int score = evaluate();
	if (score == 10)
		return score;
	if (score == -10)
		return score;
    if (check_full())
        return 0;
	if (ai_choice)
	{
		int best = -1000;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
			{
				if (tab[i][j] == 0)
				{
					tab[i][j] = 2;
					best = max(best, minimax(branch + 1, !ai_choice));
					tab[i][j] = 0;
				}
			}
		return best;
	}
	else
	{
		int best = 1000;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
			{
				if (tab[i][j] == 0)
				{
					tab[i][j] = 1;
					best = min(best, minimax(branch + 1, !ai_choice));
					tab[i][j] = 0;
				}
			}
		return best;
	}
}

Move Morpion::best_choice()
{
	int bestVal = -1000;
	Move bestMove;
	bestMove.x = -1;
	bestMove.y = -1;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (tab[i][j] == 0)
			{
				tab[i][j] = 2;
				int moveVal = minimax(0, false);
				tab[i][j] = 0;

				if (moveVal > bestVal)
				{
					bestMove.x = i;
					bestMove.y = j;
					bestVal = moveVal;
				}
			}
		}
	return bestMove;
}

void Morpion::play()
{
	while (1)
	{
		if (PvP)
		{
			if (check_end() || check_full())
				break;
			if (now_turn == player1)
			{
				player1_turn();
				now_turn = player2;
			}
			else if (now_turn == player2)
			{
				player2_turn();
				now_turn = player1;
			}
		}
		else if (!PvP)
		{
			if (check_end() || check_full())
					break;
			if (now_turn == player1)
			{
				player1_turn();
				now_turn = ai;
			}
			else if (now_turn == ai)
			{
				ai_turn();
				now_turn = player1;
			}
		}
	}
	SDL_Delay(500);
	if (winner == red)
		SDL_BlitSurface(IMG_Load("./data/images/end_red.png"), NULL, ecran, NULL);
	else if (winner == blue)
		SDL_BlitSurface(IMG_Load("./data/images/end_blue.png"), NULL, ecran, NULL);
	else if (winner == draw)
		SDL_BlitSurface(IMG_Load("./data/images/end_draw.png"), NULL, ecran, NULL);
	SDL_Flip(ecran);
	SDL_Delay(500);
}

void Morpion::reset()
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			tab[i][j] = 0;
}

void Morpion::launch()
{
	int anim_state_pva = 1,anim_state_pvp = 1;
	bool begin = false;
	SDL_Event event;
	while(!begin)
	{
		if (PvP)
		{
			if (anim_state_pvp == 1)
			{
			   SDL_BlitSurface(IMG_Load("./data/images/start_pvp.png"),NULL,ecran,NULL);
			   anim_state_pvp = 2;
			}
        	else if (anim_state_pvp == 2)
        	{
			   SDL_BlitSurface(IMG_Load("./data/images/start_pvp_2.png"),NULL,ecran,NULL);
			   anim_state_pvp = 3;
        	}
			else if (anim_state_pvp == 3)
			{
			   SDL_BlitSurface(IMG_Load("./data/images/start_pvp_3.png"),NULL,ecran,NULL);
			   anim_state_pvp = 1;
			}
		}
		if (!PvP)
		{
			if (anim_state_pva == 1)
			{
			   SDL_BlitSurface(IMG_Load("./data/images/start_pva.png"),NULL,ecran,NULL);
			   anim_state_pva = 2;
			}
        	else if (anim_state_pva == 2)
        	{
			   SDL_BlitSurface(IMG_Load("./data/images/start_pva_2.png"),NULL,ecran,NULL);
			   anim_state_pva = 3;
        	}
			else if (anim_state_pva == 3)
			{
			   SDL_BlitSurface(IMG_Load("./data/images/start_pva_3.png"),NULL,ecran,NULL);
			   anim_state_pva = 1;
			}
		}
		SDL_Flip(ecran);
		SDL_Delay(300);
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
							exit(0);
					case SDLK_UP:
						if (PvP)
						{
							anim_state_pva = 1;
							PvP = false;
						}
					break;
					case SDLK_DOWN:
						if (!PvP)
						{
							anim_state_pvp = 1;
							PvP = true;
						}
						break;
					case SDLK_SPACE:
						{
							begin = true;
						}
					break;
				}
			}
		}
	}
}

void Morpion::run()
{
	launch();
	bool play_again = true;
	show();
	SDL_Flip(ecran);
	while (play_again)
	{
		reset();
		show();
		SDL_Flip(ecran);
		play();
	}
}
