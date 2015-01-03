#include "Header.h"

class Something
{
public:
	int coord[2];
	//float coord[2];	//float because it will move smoothly (probably)
};

class Movable_creature : public Something
{
private:
	//speed
public:
	void generate_starting_position(World *world)
	{
		int a, b;
		while (world->get_from_map(a = rand(), b = rand()) != OBSTACLE_NOTHING && world->is_move_possible(a, b) == true);
		coord[0] = a;
		coord[1] = b;
	}

	//move methods
};

class Bomb : Something
{
private:
	// detonation countdown
public:
	//explode
};
class World
{
private:
	char world_map[WORLD_SIZE][WORLD_SIZE];
public:
	vector<Something> bombs_list;
	vector<Movable_creature> creatures_list;	// first is always player
	int world_size_in_blocks;
	World(int size)
	{
		world_size_in_blocks = size;
		for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			world_map[i][j] = (i % 2 != 0 && j % 2 != 0 ? 1 : 0);
		}
		char a, b;
		for (int i = 0; i < HOW_MANY_BRICK_BLOCKS; i++)	// put destroyable
		{
			while (world_map[a = rand() % WORLD_SIZE][b = rand() % WORLD_SIZE] != OBSTACLE_NOTHING);	// semicolon is necessary here

			world_map[a][b] = OBSTACLE_DESTROYABLE;
		}
	}
	void draw_console_world()
	{
		for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
		{
			if (j == 0)
				printf("{");
			if (world_map[i][j] == OBSTACLE_NONDESTROYABLE)
				printf("#");
			else if (world_map[i][j] == OBSTACLE_NOTHING)
				printf(" ");
			else if (world_map[i][j] == OBSTACLE_DESTROYABLE)
				printf("@");
			if (j == WORLD_SIZE - 1)
				printf("}\n");
		}
	}
	bool is_move_possible(int x, int y)	// checks if player can move in any of possible directions (used in position generation)
	{
		if (world_map[x + 1][y] != OBSTACLE_NOTHING && world_map[x - 1][y] != OBSTACLE_NOTHING && world_map[x][y + 1] != OBSTACLE_NOTHING && world_map[x][y - 1] != OBSTACLE_NOTHING)
			return false;
		else
			return true;
	}
	int get_from_map(int x, int y)
	{
		return world_map[x][y];
	}
	// update_world()
};
