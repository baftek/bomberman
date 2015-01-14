#include "Header.h"

void Movable_creature::generate_starting_position(World *world)
{
	int a, b;
	while (world->get_from_map(a = rand() % WORLD_SIZE, b = rand() % WORLD_SIZE) != OBSTACLE_NOTHING && world->is_move_possible(a, b) == true);
	coord[0] = a;
	coord[1] = b;
}

//move methods

World::World(int size)
{
	world_size_in_blocks = size;
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++)
	{
		world_map[i][j] = (i % 2 != 0 && j % 2 != 0 ? OBSTACLE_NONDESTROYABLE : OBSTACLE_NOTHING);
	}
	char a, b;
	for (int i = 0; i < HOW_MANY_BRICK_BLOCKS; i++)	// put destroyable
	{
		// here we randomly choose places to set destroyable blocks
		while (world_map[a = rand() % WORLD_SIZE][b = rand() % WORLD_SIZE] != OBSTACLE_NOTHING);	// semicolon is necessary here
		world_map[a][b] = OBSTACLE_DESTROYABLE;
	}

	//here we randomly choose under which destroyable block we decide to hide exit door
	while (world_map[a = rand() % WORLD_SIZE][b = rand() % WORLD_SIZE] != OBSTACLE_DESTROYABLE);	// semicolon is necessary here
	finish_point.coord[0] = a;
	finish_point.coord[1] = b;

	//here we randomly choose under which destroyable block we decide to hide key to exit door
	while (world_map[a = rand() % WORLD_SIZE][b = rand() % WORLD_SIZE] != OBSTACLE_DESTROYABLE && world_map[a = rand() % WORLD_SIZE][b = rand() % WORLD_SIZE] != OBSTACLE_FINISH_DOOR);	// semicolon is necessary here
	key.coord[0] = a;
	key.coord[1] = b;
}
void World::draw_console_world()
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
bool World::is_move_possible(int x, int y)	// checks if player can move in any of possible directions (used in position generation)
{
	if (world_map[x + 1][y] != OBSTACLE_NOTHING && world_map[x - 1][y] != OBSTACLE_NOTHING && world_map[x][y + 1] != OBSTACLE_NOTHING && world_map[x][y - 1] != OBSTACLE_NOTHING)
		return false;
	else
		return true;
}
int World::get_from_map(int x, int y)
{
	return world_map[x][y];
}

void World::destroy_brick_block(int x, int y)
{
	if (world_map[x][y] == OBSTACLE_DESTROYABLE)
	if (x == finish_point.coord[0] && y == finish_point.coord[1])
		world_map[x][y] = OBSTACLE_FINISH_DOOR;
	else if (x == key.coord[0] && y == key.coord[1])
		world_map[x][y] = OBSTACLE_KEY;
	else
		world_map[x][y] = OBSTACLE_NOTHING;
}

Something::Something() {}

Something::Something(int x, int y)
{
	coord[0] = x;
	coord[1] = y;
}

Movable_creature::Movable_creature()
{
	alive = true;
	is_moving = false;
}
int Movable_creature::move_up(World *world)
{
	int successful = false;
	if (successful = world->creatures_list[0].coord[1] != 0
		&& ((world->get_from_map(world->creatures_list[0].coord[0], world->creatures_list[0].coord[1] - 1) == OBSTACLE_NOTHING)
		|| (world->get_from_map(world->creatures_list[0].coord[0], world->creatures_list[0].coord[1] - 1) == OBSTACLE_FINISH_DOOR)
		|| (world->get_from_map(world->creatures_list[0].coord[0], world->creatures_list[0].coord[1] - 1) == OBSTACLE_KEY)))
		world->creatures_list[0].coord[1] -= 1;
	return successful;
}
int Movable_creature::move_left(World *world)
{
	int successful = false;
	if (successful = world->creatures_list[0].coord[0] != 0
		&& ((world->get_from_map(world->creatures_list[0].coord[0] - 1, world->creatures_list[0].coord[1]) == OBSTACLE_NOTHING)
		|| (world->get_from_map(world->creatures_list[0].coord[0] - 1, world->creatures_list[0].coord[1]) == OBSTACLE_FINISH_DOOR)
		|| (world->get_from_map(world->creatures_list[0].coord[0] - 1, world->creatures_list[0].coord[1]) == OBSTACLE_KEY)))
		world->creatures_list[0].coord[0] -= 1;
	return successful;
}
int Movable_creature::move_down(World *world)
{
	int successful = false;
	if (successful = world->creatures_list[0].coord[1] < WORLD_SIZE - 1
		&& ((world->get_from_map(world->creatures_list[0].coord[0], world->creatures_list[0].coord[1] + 1) == OBSTACLE_NOTHING)
		|| (world->get_from_map(world->creatures_list[0].coord[0], world->creatures_list[0].coord[1] + 1) == OBSTACLE_FINISH_DOOR)
		|| (world->get_from_map(world->creatures_list[0].coord[0], world->creatures_list[0].coord[1] + 1) == OBSTACLE_KEY)))
		world->creatures_list[0].coord[1] += 1;
	return successful;
}
int Movable_creature::move_right(World *world)
{
	int successful = false;
	if (successful = world->creatures_list[0].coord[0] < WORLD_SIZE - 1
		&& ((world->get_from_map(world->creatures_list[0].coord[0] + 1, world->creatures_list[0].coord[1]) == OBSTACLE_NOTHING)
		|| (world->get_from_map(world->creatures_list[0].coord[0] + 1, world->creatures_list[0].coord[1]) == OBSTACLE_FINISH_DOOR)
		|| (world->get_from_map(world->creatures_list[0].coord[0] + 1, world->creatures_list[0].coord[1]) == OBSTACLE_KEY)))
		world->creatures_list[0].coord[0] += 1;
	return successful;
}

Bomb::Bomb()
{
	time_of_bomb_set = al_get_time();
	it_exploded = false;
}
Bomb::Bomb(int _x, int _y)
{
	time_of_bomb_set = al_get_time();
	coord[0] = _x;
	coord[1] = _y;
	it_exploded = false;
}
bool Bomb::should_bomb_expolode()
{
	if (al_get_time() - time_of_bomb_set > BOMB_TIME)
		return 1;
	else
		return 0;
}
void Bomb::explode(World *world)
{
	//explosion in place of bomb
	explosion_coords.push_back(*new Something(coord[0], coord[1]));
	bool player_on_fire = 0;
	if (world->creatures_list[0].coord[0] == coord[0] && world->creatures_list[0].coord[1] == coord[1])
		player_on_fire = true;

	//exploring up
	for (int i = 1; world->get_from_map(coord[0], coord[1] - i) < 2 && coord[1] - i >= 0; i++)	//condition <2 checks whether there is nothing or destroyable (considers both values)
	{
		explosion_coords.push_back(*new Something(coord[0], coord[1] - i));
		if (world->get_from_map(coord[0], coord[1] - i) == OBSTACLE_DESTROYABLE)
		{
			world->destroy_brick_block(coord[0], coord[1] - i);
			break;
		}
		if (world->creatures_list[0].coord[0] == explosion_coords.back().coord[0] && world->creatures_list[0].coord[1] == explosion_coords.back().coord[1])
			player_on_fire = true;
	}
	//exploring down
	for (int i = 1; world->get_from_map(coord[0], coord[1] + i) < 2 && coord[1] + i < WORLD_SIZE; i++)
	{
		explosion_coords.push_back(*new Something(coord[0], coord[1] + i));
		if (world->get_from_map(coord[0], coord[1] + i) == OBSTACLE_DESTROYABLE)
		{
			world->destroy_brick_block(coord[0], coord[1] + i);
			break;
		}
		if (world->creatures_list[0].coord[0] == explosion_coords.back().coord[0] && world->creatures_list[0].coord[1] == explosion_coords.back().coord[1])
			player_on_fire = true;

	}
	//exploring left
	for (int i = 1; world->get_from_map(coord[0] - i, coord[1]) < 2 && coord[0] - i >= 0; i++)
	{
		explosion_coords.push_back(*new Something(coord[0] - i, coord[1]));
		if (world->get_from_map(coord[0] - i, coord[1]) == OBSTACLE_DESTROYABLE)
		{
			world->destroy_brick_block(coord[0] - i, coord[1]);
			break;
		}
		if (world->creatures_list[0].coord[0] == explosion_coords.back().coord[0] && world->creatures_list[0].coord[1] == explosion_coords.back().coord[1])
			player_on_fire = true;

	}
	//exploring right
	for (int i = 1; world->get_from_map(coord[0] + i, coord[1]) < 2 && coord[0] + i < WORLD_SIZE; i++)
	{
		explosion_coords.push_back(*new Something(coord[0] + i, coord[1]));
		if (world->get_from_map(coord[0] + i, coord[1]) == OBSTACLE_DESTROYABLE)
		{
			world->destroy_brick_block(coord[0] + i, coord[1]);
			break;
		}
		if (world->creatures_list[0].coord[0] == explosion_coords.back().coord[0] && world->creatures_list[0].coord[1] == explosion_coords.back().coord[1])
			player_on_fire = true;

	}

	it_exploded = true;
	if (player_on_fire)
		throw EXSC_PLAYER_ON_FIRE;
}

bool Bomb::did_it_explode()
{
	return it_exploded;
}

double Bomb::calc_vanish_ratio()
{
	double a = al_get_time();
	double b = ((a - time_of_bomb_set) - BOMB_TIME) / BOMB_VANISH_TIME;
	return b;
}