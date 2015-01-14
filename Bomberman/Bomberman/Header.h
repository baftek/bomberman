#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace std;
#include <allegro5\allegro5.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>

#define M_PI       3.14159265358979323846
#define TEXT_COLOR 255, 255, 255
#define TEXT_SIZE 18

#define WORLD_SIZE 15
#define HOW_MANY_BRICK_BLOCKS 5
#define OBSTACLE_NOTHING 0
#define OBSTACLE_DESTROYABLE 1
#define OBSTACLE_NONDESTROYABLE 2
#define OBSTACLE_KEY 4
#define OBSTACLE_FINISH_DOOR 5
#define BOMB_TIME 2.0
#define BOMB_VANISH_TIME 2.0

#define EXSC_PLAYER_ON_FIRE 1
#define EXSC_PLAYER_KILLED 2

class World;
class Something;
class Movable_creature;
class Bomb;

class Something
{
public:
	float coord[2]; 
	Something();
	Something(int x, int y);
};

class World
{
private:
	char world_map[WORLD_SIZE][WORLD_SIZE];
public:
	vector<Bomb> bombs_list;
	vector<Movable_creature> creatures_list;	// player is at [0]
	int world_size_in_blocks;
	Something finish_point;
	Something key;
	World(int size);
	void draw_console_world();
	bool is_move_possible(int x, int y);
	int get_from_map(int x, int y);
	void destroy_brick_block(int x, int y);
};

class Movable_creature : public Something
{
private:
	//double time_of_movement_start;
	bool is_moving;
public:
	bool alive;
	Movable_creature();
	void generate_starting_position(World *world);

	int move_up(World *world);
	int move_left(World *world);
	int move_down(World *world);
	int move_right(World *world);
};

class Bomb : public Something
{
private:
	double time_of_bomb_set;
	bool it_exploded;
public:
	Bomb();
	Bomb(int _x, int _y);
	vector<Something> explosion_coords;
	bool did_it_explode();
	bool should_bomb_expolode();	//countdown check
	void explode(World *world);
	double calc_vanish_ratio();
};


void draw_static_world(World *world);
void draw_dynamic_things(World *world);
