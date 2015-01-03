#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;
#include <allegro5\allegro5.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>

class World;
class Something;
class Movable_creature;
class Bomb;

#define WORLD_SIZE 13
#define HOW_MANY_BRICK_BLOCKS 50
#define OBSTACLE_NOTHING 0
#define OBSTACLE_NONDESTROYABLE 1
#define OBSTACLE_DESTROYABLE 2
