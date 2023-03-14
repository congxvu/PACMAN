/*
Author: Cong Vu
Class : ECE4122 - A
Last Date Modified : 10 / 31 / 2022

Description:


This is for Lab 3 _ Pacman
*/

#include <vector>
#include <ctime>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <random>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#define WIDTH 29
#define HEIGHT 32
#define PACMAN_SPEED 3
#define GHOST_SPEED 3

using namespace sf;
using namespace std;

unsigned int score = 0;
unsigned int lives = 1;
unsigned int snacks_count = 230;


unsigned int snacks_limiter = 0;
unsigned int power_limiter = 0;
unsigned int wall_limiter = 0;


//self explainatory variables
bool red_death, blue_death, orange_death, pink_death;
bool paused;
bool power = false;
bool Mleft, Mright, Mup, Mdown{ false };
bool Gleft, Gright, Gup, Gdown{ false };
bool Bleft, Bright, Bup, Bdown{ false };
bool Oleft, Oright, Oup, Odown{ false };
bool Pleft, Pright, Pup, Pdown{ false };
bool start = false;
bool acceptInput = false;
bool game_over;
bool RchangeDir, BchangeDir, OchangeDir, PchangeDir{ false }; // use to change directions of ghost when hit walls



// define FUNCTIONS

bool wall_collision(Sprite& pac, Vector2f& pos, char direction);
bool pathfinder(Sprite& ghost, Vector2f& pos, char direction); // direction of Ghost if hitting walls
void ghost_collision(Sprite& pac, Sprite& ghost, char color);
void eat_collision(Sprite& pac);


// vectors that store map elements
vector <CircleShape> power_vector;
vector <RectangleShape> wall_vector;
vector <CircleShape> snacks_vector;


// Stack MAP vector
std::vector <std::string> game_map
{
	"-----------------------------",
	"-............ -............ -",
	"-.    .     . -.     .    . -",
	"-0 ---. ----. -. ----. ---0 -",
	"-. ---. ----. -. ----. ---. -",
	"-.......................... -",
	"-.    .              .    . -",
	"-. ---. -------------. ---. -",
	"-...... -     -     -...... -",
	"-     . -     -     -.      -",
	"------. ----  -  ----. ------",
	"------. -     rrrr  -. ------",
	"------. -           -. ------",
	"------. -  -------  -. ------",
	"i     .    -------   .      o",
	"      .    -------   .       ",
	"------. -  -------  -. ------",
	"------. -           -. ------",
	"------. -           -. ------",
	"------. -  -------  -. ------",
	"-............ -............ -",
	"-.    .     . -.     .    . -",
	"-. ---. ----. -. ----. ---. -",
	"-0.. -................ -..0 -",
	"-  . -.  .        .  . -.   -",
	"---. -. -. -------. -. -. ---",
	"-...... -.... -.... -...... -",
	"-.      -   . -.    -     . -",
	"-. ---------. -. ---------. -",
	"-.......................... -",
	"-                           -",
	"-----------------------------"
};



int main()
{
	// Seed that helps determine ghost's direction
	srand(time(NULL));
	// Create a video mode pac
	VideoMode vm(641, 728);

	// Create and open a window for the game
	RenderWindow window(vm, "Pacman");
	//window.setFramerateLimit(60);
	window.clear();

	// Load background
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/maze.bmp");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Press ENTER to start
	Text messageText;
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	messageText.setFont(font);
	messageText.setString("PRESS ENTER TO START");
	messageText.setCharacterSize(30);
	messageText.setFillColor(Color::White);
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);
	messageText.setPosition(300, 350);


	// Ghosts' sprites
	Texture orange_ghost;
	orange_ghost.loadFromFile("graphics/orange_ghost.bmp");
	Sprite orange;
	orange.setTexture(orange_ghost);
	orange.setPosition(300, 245);

	Texture pink_ghost;
	pink_ghost.loadFromFile("graphics/pink_ghost.bmp");
	Sprite pink;
	pink.setTexture(pink_ghost);
	pink.setPosition(300, 245);

	Texture blue_ghost;
	blue_ghost.loadFromFile("graphics/blue_ghost.bmp");
	Sprite blue;
	blue.setTexture(blue_ghost);
	blue.setPosition(300, 245);

	Texture red_ghost;
	red_ghost.loadFromFile("graphics/red_ghost.bmp");
	Sprite red;
	red.setTexture(red_ghost);
	red.setPosition(300, 245);

	// PACMAN Sprite
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/pacman.bmp");
	Sprite PAC_MAN;
	PAC_MAN.setTexture(texturePlayer);
	PAC_MAN.setPosition(300, 492);

	// Tunnel

	RectangleShape portal_in(Vector2f(19, 19));
	portal_in.setPosition(32, 309);
	portal_in.setFillColor(Color::Transparent);
	RectangleShape portal_out(Vector2f(19, 19));
	portal_out.setPosition(574, 309);
	portal_out.setFillColor(Color::Transparent);

	paused = true;
	int ghost_direction = rand() % 2;

	// Here is our clock for timing everything
	Clock clock;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{

			if (event.type == Event::KeyReleased && !paused)
			{
				// Listen for key presses again
				acceptInput = true;
			}

		}

		/*
		****************************************
		Handle the players input
		****************************************
		*/

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (score == 228)	// pacman eat all snacks
		{
			window.close();
		}

		// Press Enter to start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			window.clear();

			paused = false;
			score = 0; // reset scores
			acceptInput = true;
			start = true;

		}
		if (!paused)
		{
			// pacman CONTROL
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{

				Mleft = false;
				Mdown = false;
				Mup = false;
				Mright = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				Mup = false;
				Mright = false;
				Mdown = false;
				Mleft = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				Mleft = false;
				Mright = false;
				Mdown = false;
				Mup = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				Mleft = false;
				Mright = false;
				Mup = false;
				Mdown = true;
			}

			// GHOSTS MOVING			

			int random = rand()%4 + 1; // random directions 1=L, 2 =R, 3 = U, 4 = D 
			
			Gleft = true;
			Bleft = true;
			Oright = true;
			Pright = true;
			
				// red ghost moving randomly
				while (RchangeDir == true)
				{
					switch (rand() % 4 + 1)
					{
					case 1: {
						Gright = true;
						Gleft = false;
						Gup = false;
						Gdown = false;
						RchangeDir = false;
						break; }
					case 2: {
						Gright = false;
						Gleft = true;
						Gup = false;
						Gdown = false;
						RchangeDir = false;
						break; }
					case 3: {
						Gright = false;
						Gleft = false;
						Gup = true;
						Gdown = false;
						RchangeDir = false;
						break; }
					case 4: {
						Gright = false;
						Gleft = false;
						Gup = false;
						Gdown = true;
						RchangeDir = false;
						break; }
					}
					
				}

				// blue ghost moving randomly
				while (BchangeDir == true)
				{
					switch (rand() % 4 + 1)
					{
					case 1: {
						Bright = true;
						Bleft = false;
						Bup = false;
						Bdown = false;
						BchangeDir = false;
						break; }
					case 2: {
						Bright = false;
						Bleft = true;
						Bup = false;
						Bdown = false;
						BchangeDir = false;
						break; }
					case 3: {
						Bright = false;
						Bleft = false;
						Bup = true;
						Bdown = false; 
						BchangeDir = false;
						break; }
					case 4: {
						Bright = false;
						Bleft = false;
						Bup = false;
						Bdown = true;
						BchangeDir = false;
						break; }
					}
				}
				
				// Orange ghost moving randomly
				while (OchangeDir == true)
				{
					switch (rand() % 4 + 1)
					{
					case 1: {
						Oright = true;
						Oleft = false;
						Oup = false;
						Odown = false;
						OchangeDir = false;
						break; }
					case 2: {
						Oright = false;
						Oleft = true;
						Oup = false;
						Odown = false;
						OchangeDir = false;
						break; }
					case 3: {
						Oright = false;
						Oleft = false;
						Oup = true;
						Odown = false;
						OchangeDir = false;
						break; }
					case 4: {
						Oright = false;
						Oleft = false;
						Oup = false;
						Odown = true;
						OchangeDir = false;
						break; }
					}
				}

				// pink ghost moving randomly
				while (PchangeDir == true)
				{
					switch (rand() % 4 + 1)
					{
					case 1: {
						Pright = true;
						Pleft = false;
						Pup = false;
						Pdown = false;
						PchangeDir = false;
						break; }
					case 2: {
						Pright = false;
						Pleft = true;
						Pup = false;
						Pdown = false;
						PchangeDir = false;
						break; }
					case 3: {
						Pright = false;
						Pleft = false;
						Pup = true;
						Pdown = false;
						PchangeDir = false;
						break; }
					case 4: {
						Pright = false;
						Pleft = false;
						Pup = false;
						Pdown = true;
						PchangeDir = false;
						break; }
					}
				}
		}
		// END HANDLING INPUTS


		/*
		****************************************
		// START DRAWING SPRITES
		****************************************
		*/

		// SCORES update
		std::ostringstream score_text;
		score_text << score;
		Text scores;
		scores.setString(score_text.str());
		scores.setPosition(168, 680);
		scores.setFont(font);
		scores.setCharacterSize(30);
		scores.setFillColor(Color::White);

		// PACMAN LIVES setup to 1
		std::ostringstream lives_text;
		lives_text << lives;
		Text live_;
		live_.setString(lives_text.str());
		live_.setPosition(445, 680);
		live_.setFont(font);
		live_.setCharacterSize(30);
		live_.setFillColor(Color::White);


		window.draw(spriteBackground);
		window.draw(PAC_MAN);
		window.draw(portal_in);	// Middle tunnel
		window.draw(portal_out);	// Middle tunnel
		window.draw(scores);
		window.draw(live_);

		// check if ghosts die
		if (!blue_death)
			window.draw(blue);
		if (!orange_death)
			window.draw(orange);
		if (!pink_death)
			window.draw(pink);
		if (!red_death)
			window.draw(red);


		// START DRAWING STATIONARY OBJECTS
		for (int i = 0; i < WIDTH; i++)
		{

			for (int j = 0; j < HEIGHT; j++)
			{

				switch (game_map[j][i])
				{
				case '-': // Invisible walls
				{
					RectangleShape wall(Vector2f(16, 16));
					wall.setPosition(28 + 20 * i, 23 + 20 * j);
					wall.setFillColor(Color::Transparent);
					window.draw(wall);
					if (wall_limiter < 370)
					{
						wall_vector.push_back(wall);
						wall_limiter++;
					}
					break;
				}
				case '.': // snacks
				{

					CircleShape snacks;
					snacks.setRadius(3);
					snacks.setPosition(45 + 20 * i, 38 + 20 * j);
					snacks.setFillColor(Color(255, 255, 255));
					window.draw(snacks);
					if (snacks_limiter < 230)
					{
						snacks_vector.push_back(snacks);
						snacks_limiter++;
					}
					break;
				}
				case '0': // power-ups
				{
					CircleShape power;
					power.setRadius(7);
					power.setPosition(40 + 20 * i, 35 + 20 * j);
					power.setFillColor(Color(255, 255, 255));
					window.draw(power);
					if (power_limiter < 4)
					{
						power_limiter++;
						power_vector.push_back(power);
					}
					break;
				}
				default:
				{
					break;
				}

				}
			}
		}

		// END DRAWING

		
		/*
		****************************************
		// START HANDLING GAME'S LOGIC
		****************************************
		*/

		// Moving Pacman LEFT RIGHT UP DOWN
		Vector2f pos = PAC_MAN.getPosition();
		if (Mright)
		{
			PAC_MAN.setPosition(pos.x + PACMAN_SPEED , pos.y + 0);
			wall_collision(PAC_MAN, pos, 'r');
		}
		if (Mleft)
		{

			PAC_MAN.setPosition(pos.x - PACMAN_SPEED , pos.y + 0);
			wall_collision(PAC_MAN, pos, 'l');
		}
		if (Mup)
		{
			PAC_MAN.setPosition(pos.x + 0, pos.y - PACMAN_SPEED );
			wall_collision(PAC_MAN, pos, 'u');

		}
		if (Mdown)
		{
			PAC_MAN.setPosition(pos.x + 0, pos.y + PACMAN_SPEED );
			wall_collision(PAC_MAN, pos, 'd');
		}

		//Tunnel Middle In-Out
		if (PAC_MAN.getGlobalBounds().intersects(portal_in.getGlobalBounds()))
		{
			PAC_MAN.setPosition(530, 309);
		}
		if (PAC_MAN.getGlobalBounds().intersects(portal_out.getGlobalBounds()))
		{
			PAC_MAN.setPosition(78, 309);
		}


		// red ghose
		//Tunnel Middle In-Out
		if (red.getGlobalBounds().intersects(portal_in.getGlobalBounds()))
		{
			red.setPosition(530, 309);
		}
		if (red.getGlobalBounds().intersects(portal_out.getGlobalBounds()))
		{
			red.setPosition(78, 309);
		}

		// Blue ghose
		//Tunnel Middle In-Out
		if (blue.getGlobalBounds().intersects(portal_in.getGlobalBounds()))
		{
			blue.setPosition(530, 309);
		}
		if (blue.getGlobalBounds().intersects(portal_out.getGlobalBounds()))
		{
			blue.setPosition(78, 309);
		}

		// Orange ghose
		//Tunnel Middle In-Out
		if (orange.getGlobalBounds().intersects(portal_in.getGlobalBounds()))
		{
			orange.setPosition(530, 309);
		}
		if (orange.getGlobalBounds().intersects(portal_out.getGlobalBounds()))
		{
			orange.setPosition(78, 309);
		}

		// pink ghose
		//Tunnel Middle In-Out
		if (pink.getGlobalBounds().intersects(portal_in.getGlobalBounds()))
		{
			pink.setPosition(530, 309);
		}
		if (pink.getGlobalBounds().intersects(portal_out.getGlobalBounds()))
		{
			pink.setPosition(78, 309);
		}



		// Define Pacman collide with ghosts
		ghost_collision(PAC_MAN, red, 'r');
		ghost_collision(PAC_MAN, pink, 'p');
		ghost_collision(PAC_MAN, orange, 'o');
		ghost_collision(PAC_MAN, blue, 'b');

		//edible handler
		if (!paused)
			eat_collision(PAC_MAN);

		// red ghosts position
		Vector2f rpos = red.getPosition();
		if (Gright)
		{
			red.setPosition(rpos.x + GHOST_SPEED, rpos.y + 0);
			pathfinder(red, rpos, 'r');
		}
		if (Gleft)
		{
			red.setPosition(rpos.x - GHOST_SPEED,rpos.y + 0);
			pathfinder(red, rpos, 'l');
		}
		if (Gup)
		{
			red.setPosition(rpos.x + 0, rpos.y - GHOST_SPEED);
			pathfinder(red, rpos, 'u');
		}
		if (Gdown)
		{
			red.setPosition(rpos.x + 0, rpos.y + GHOST_SPEED);
			pathfinder(red, rpos, 'd');
		}


		// blue ghosts position
		Vector2f bpos = blue.getPosition();
		if (Bright)
		{
			blue.setPosition(bpos.x + GHOST_SPEED, bpos.y + 0);
			pathfinder(blue, bpos, 'r');
		}
		if (Bleft)
		{
			blue.setPosition(bpos.x - GHOST_SPEED, bpos.y + 0);
			pathfinder(blue, bpos, 'l');
		}
		if (Bup)
		{
			blue.setPosition(bpos.x + 0, bpos.y - GHOST_SPEED);
			pathfinder(blue, bpos, 'u');
		}
		if (Bdown)
		{
			blue.setPosition(bpos.x + 0, bpos.y + GHOST_SPEED);
			pathfinder(blue, bpos, 'd');
		}

		// orange ghosts position
		Vector2f opos = orange.getPosition();
		if (Oright)
		{
			orange.setPosition(opos.x + GHOST_SPEED, opos.y + 0);
			pathfinder(orange, opos, 'r');
		}
		if (Oleft)
		{
			orange.setPosition(opos.x - GHOST_SPEED, opos.y + 0);
			pathfinder(orange, opos, 'l');
		}
		if (Oup)
		{
			orange.setPosition(opos.x + 0, opos.y - GHOST_SPEED);
			pathfinder(orange, opos, 'u');
		}
		if (Odown)
		{
			orange.setPosition(opos.x + 0, opos.y + GHOST_SPEED);
			pathfinder(orange, opos, 'd');
		}

		// pink ghosts position
		Vector2f ppos = pink.getPosition();
		if (Pright)
		{
			pink.setPosition(ppos.x + GHOST_SPEED, ppos.y + 0);
			pathfinder(pink, ppos, 'r');
		}
		if (Pleft)
		{
			pink.setPosition(ppos.x - GHOST_SPEED, ppos.y + 0);
			pathfinder(pink, ppos, 'l');
		}
		if (Pup)
		{
			pink.setPosition(ppos.x + 0, ppos.y - GHOST_SPEED);
			pathfinder(pink, ppos, 'u');
		}
		if (Pdown)
		{
			pink.setPosition(ppos.x + 0, ppos.y + GHOST_SPEED);
			pathfinder(pink, ppos, 'd');
		}

		//*******************//

		if (paused)
			window.draw(messageText);

		if (snacks_count == 0 || lives == 0)
			game_over = true;

		window.display();

	}
	return 0;
}

/*
****************************************
// END MAIN FUNCTION
*/

// Wall collision handler function

bool wall_collision(Sprite& pac, Vector2f& pos, char direction)
{
	for (int i = 0; i < wall_vector.size(); i++)
	{

		if (pac.getGlobalBounds().intersects(wall_vector[i].getGlobalBounds()))
		{
			switch (direction)
			{
			case 'r':
				//right = false;
				pac.setPosition(pac.getPosition().x - PACMAN_SPEED , pac.getPosition().y);
				break;
			case 'l':
				//left = false;
				pac.setPosition(pac.getPosition().x + PACMAN_SPEED , pac.getPosition().y);
				break;
			case 'u':
				//up = false;
				pac.setPosition(pac.getPosition().x, pac.getPosition().y + PACMAN_SPEED );
				break;
			case 'd':
				//down = false;
				pac.setPosition(pac.getPosition().x, pac.getPosition().y - PACMAN_SPEED );
				break;
			default:
				break;
			}
			return true;
		}
	}

	return false;

}

// Eat FUNCTION ( POWER AND snacks)
//rewrite the bitmap if the snacks are eaten

void eat_collision(Sprite& pac)
{
	for (int i = 0; i < snacks_vector.size(); i++)
	{
		if (pac.getGlobalBounds().intersects(snacks_vector[i].getGlobalBounds()))
		{
			game_map[(snacks_vector[i].getPosition().y - 38) / 20][(snacks_vector[i].getPosition().x - 45) / 20] = ' ';
			snacks_vector.erase(snacks_vector.begin() + i);
			snacks_count--;
			score++;
		}

	}
	for (int i = 0; i < power_vector.size(); i++)
	{
		if (pac.getGlobalBounds().intersects(power_vector[i].getGlobalBounds()))
		{
			game_map[(power_vector[i].getPosition().y - 35) / 20][(power_vector[i].getPosition().x - 40) / 20] = ' ';
			power = true;
		}
	}

}


void ghost_collision(Sprite& pac, Sprite& ghost, char color)
{
	Clock clock;
	Time dt = clock.restart();
	if (dt.asSeconds() < 6) power = false;

	if (pac.getGlobalBounds().intersects(ghost.getGlobalBounds()) && !power)
	{
		pac.setPosition(306, 492);
		score = 228; // end game
		//lives--;
	}
	if (pac.getGlobalBounds().intersects(ghost.getGlobalBounds()) && power && (dt.asSeconds() < 6))
	{
		if (color == 'r')
			red_death = 1;
		if (color == 'p')
			pink_death = 1;
		if (color == 'o')
			orange_death = 1;
		if (color == 'b')
			blue_death = 1;
		ghost.setPosition(52, 696); // move ghost out side
	}
}

// ghosts moving
void ghost_moving()
{
	switch (rand() % 4 + 1)
	{
	case 1: Gright = true;
	case 2: Gleft = true;
	case 3: Gup = true;
	case 4: Gdown = true;
	}
}

// find position of ghosts
bool pathfinder(Sprite& ghost, Vector2f& rpos, char direction)
{

	for (int i = 0; i < wall_vector.size(); i++)
	{

		if (ghost.getGlobalBounds().intersects(wall_vector[i].getGlobalBounds()))
		{
			RchangeDir = true;
			BchangeDir = true;
			OchangeDir = true;
			PchangeDir = true;
			switch (direction)
			{
			case 'r':
				Gright = false;
				Bright = false;
				Oright = false;
				Pright = false;
				ghost.setPosition(ghost.getPosition().x - PACMAN_SPEED, ghost.getPosition().y);
				break;
				
			case 'l':
				Gleft = false;
				Bleft = false;
				Oleft = false;
				Pleft = false;
				ghost.setPosition(ghost.getPosition().x + PACMAN_SPEED, ghost.getPosition().y);
				break;
			case 'u':
				Gup = false;
				Bup = false;
				Oup = false;
				Pup = false;
				ghost.setPosition(ghost.getPosition().x, ghost.getPosition().y + PACMAN_SPEED);
				break;
			case 'd':
				ghost.setPosition(ghost.getPosition().x, ghost.getPosition().y - PACMAN_SPEED);
				Gdown = false;
				Bdown = false;
				Odown = false;
				Pdown = false;
				break;
			default:
				break;
			}
			//return true;
		}

	}

	return false;

}



