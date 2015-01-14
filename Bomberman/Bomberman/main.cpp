#include "Header.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_FONT *font_big = NULL;
ALLEGRO_EVENT ev;
int winXsize = 700;
int winYsize = 700;

// TODO ; kolejne bomby nie wybuchaja, zanikanie, smierc

//typedef struct coord
//{
//	int x;
//	int y;
//};
//
int allegro_initialization(int widht, int height)
{
	if (!al_init())
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	else
		printf("Allegro initialized\n");

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

	display = al_create_display(widht, height);
	if (!display)
	{
		al_show_native_message_box(display, "Error", "Error", "failed to create display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	else
		printf("Display created\n");

	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		al_show_native_message_box(display, "Error", "Error", "failed to create event_queue!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return -1;
	}
	else
		printf("Event queue created\n");
	al_flush_event_queue(event_queue);

	al_init_font_addon();
	al_init_ttf_addon();
	font = al_load_ttf_font("C:\\Windows\\Fonts\\cour.ttf", TEXT_SIZE, 0);
	font_big = al_load_ttf_font("C:\\Windows\\Fonts\\cour.ttf", 30, 0);
	if (!font || !font_big)
	{
		al_destroy_event_queue(event_queue);
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize font!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return -1;
	}
	else
		printf("Fonts installed\n");

	al_init_image_addon();

	al_install_keyboard();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_init_primitives_addon();
	al_clear_to_color(al_map_rgb(0, 0, 20));
	al_flip_display();
	return 0;
}



#define COORDS MARGIN+grid+i*grid, MARGIN+grid+j*grid, MARGIN+grid+(i+1)*grid, MARGIN+grid+(j+1)*grid

void draw_static_world(World *world)
{
	static ALLEGRO_BITMAP *image_wall = al_load_bitmap("Images\\wall.jpg");
	static ALLEGRO_BITMAP *image_nondestroyable = al_load_bitmap("Images\\stone.png");
	static ALLEGRO_BITMAP *image_nothing = al_load_bitmap("Images\\grass4.png");
	static ALLEGRO_BITMAP *image_door = al_load_bitmap("Images\\door.jpg");
	static ALLEGRO_BITMAP *image_key = al_load_bitmap("Images\\key.png");
	//int MARGIN = 0;
#define MARGIN 0
	static ALLEGRO_COLOR grey, black, orange, brown;
	grey = al_map_rgb(100, 100, 100);
	black = al_map_rgb(0, 0, 0);
	orange = al_map_rgb(120, 75, 0);
	brown = al_map_rgb(50, 10, 0);
	al_clear_to_color(grey);
	static int grid = (winXsize - 2 * MARGIN) / (WORLD_SIZE + 2);
	//draw borders
	for (int i = 0; i < world->world_size_in_blocks + 2; i++)
	{
		al_draw_filled_rectangle(MARGIN + i*grid, MARGIN, MARGIN + (i + 1)*grid, MARGIN + grid, black);
		al_draw_filled_rectangle(MARGIN + i*grid, MARGIN + (WORLD_SIZE + 1) * grid, MARGIN + (i + 1)*grid, MARGIN + ((WORLD_SIZE + 1) + 1)*grid, black);
	}
	for (int i = 0; i < world->world_size_in_blocks + 2; i++)
	{
		al_draw_filled_rectangle(MARGIN, MARGIN + (i)*grid, MARGIN + grid, MARGIN + (i + 1)*grid, black);
		al_draw_filled_rectangle(MARGIN + (WORLD_SIZE + 1) * grid, MARGIN + (i)*grid, MARGIN + (WORLD_SIZE + 2) * grid, MARGIN + (i + 1)*grid, black);
	}
	//	draw internal non-movable walls and other static things (door, keys)
	for (int i = 0; i < world->world_size_in_blocks; i++)
	for (int j = 0; j < world->world_size_in_blocks; j++)
	{
		if (world->get_from_map(i, j) == OBSTACLE_NONDESTROYABLE)
		{
#define COLOR_MULTIPLIER1 255
			if (image_nondestroyable != 0)
				al_draw_tinted_scaled_bitmap(image_nondestroyable, al_map_rgb(COLOR_MULTIPLIER1, COLOR_MULTIPLIER1, COLOR_MULTIPLIER1),
				0, 0, al_get_bitmap_width(image_nondestroyable), al_get_bitmap_height(image_nondestroyable),
				MARGIN + grid + i*grid, MARGIN + grid + j*grid, grid, grid, 0);
			else
				al_draw_filled_rectangle(COORDS, black);
		}
		else if (world->get_from_map(i, j) == OBSTACLE_NOTHING)
		{
#define COLOR_MULTIPLIER2 200
			if (image_nothing != 0)
				al_draw_tinted_scaled_bitmap(image_nothing, al_map_rgb(COLOR_MULTIPLIER2, COLOR_MULTIPLIER2, COLOR_MULTIPLIER2),
				139, 19, 64, 64,
				MARGIN + grid + i*grid, MARGIN + grid + j*grid, grid, grid, 0);
			else
				al_draw_filled_rectangle(COORDS, grey);
		}
		else if (world->get_from_map(i, j) == OBSTACLE_DESTROYABLE)
		{
			if (image_wall != 0)
				al_draw_scaled_bitmap(image_wall, 0, 0, al_get_bitmap_width(image_wall), al_get_bitmap_height(image_wall),
				MARGIN + grid + i*grid, MARGIN + grid + j*grid, grid, grid, 0);
			else
				al_draw_filled_rectangle(COORDS, orange);
		}
		else if (world->get_from_map(i, j) == OBSTACLE_FINISH_DOOR)
		{
			if (image_door != 0)
				al_draw_scaled_bitmap(image_door, 0, 0, al_get_bitmap_width(image_door), al_get_bitmap_height(image_door),
				MARGIN + grid + i*grid, MARGIN + grid + j*grid, grid, grid, 0);
			else
				al_draw_filled_rectangle(COORDS, brown);
		}
		else if (world->get_from_map(i, j) == OBSTACLE_KEY)
		{
			if (image_key != 0)
				al_draw_scaled_bitmap(image_key, 0, 0, al_get_bitmap_width(image_key), al_get_bitmap_height(image_key),
				MARGIN + grid + i*grid, MARGIN + grid + j*grid, grid, grid, 0);
			else
				al_draw_filled_rectangle(COORDS, brown);
		}
	}

	//al_flip_display();
}

void draw_dynamic_things(World *world)
{
	static ALLEGRO_BITMAP *image_player = al_load_bitmap("Images\\smerf_alpha.png");
	static ALLEGRO_BITMAP *image_bomb = al_load_bitmap("Images\\bomb.png");
	static ALLEGRO_BITMAP *image_explosion = al_load_bitmap("Images\\explosion.png");
	static int grid = (winXsize - 2 * MARGIN) / (WORLD_SIZE + 2);
	if (image_player)
		al_draw_scaled_bitmap(image_player, 0, 0, al_get_bitmap_width(image_player), al_get_bitmap_height(image_player),
		MARGIN + grid + world->creatures_list[0].coord[0] * grid, MARGIN + grid + world->creatures_list[0].coord[1] * grid, grid, grid, 0);
	else
	{
		int x, y;
		al_draw_filled_rectangle(x = MARGIN + grid + world->creatures_list[0].coord[0] * grid, y = MARGIN + grid + world->creatures_list[0].coord[1] * grid, x + grid, y + grid, al_map_rgb(255, 255, 0));
	}
	//drawing bombs
	for (int i = 0; i < world->bombs_list.size(); i++)
	if (!world->bombs_list[i].did_it_explode())
	{
		if (image_bomb)
			al_draw_scaled_bitmap(image_bomb, 0, 0, al_get_bitmap_width(image_bomb), al_get_bitmap_height(image_bomb),
			MARGIN + grid + world->bombs_list[i].coord[0] * grid, MARGIN + grid + world->bombs_list[i].coord[1] * grid, grid, grid, 0);
		else
		{
			int x, y;
			al_draw_filled_rectangle(x = MARGIN + grid + world->creatures_list[0].coord[0] * grid, y = MARGIN + grid + world->creatures_list[0].coord[1] * grid, x + grid, y + grid, al_map_rgb(0, 0, 0));
		}
	}
	//drawing explosions
	for (int i = 0; i < world->bombs_list.size(); i++)
	{
		float transparency;
		if (world->bombs_list[i].did_it_explode())
		{
			transparency = cos(world->bombs_list[i].calc_vanish_ratio()*0.5*M_PI);
			if (transparency <= 0)
			{
				world->bombs_list.erase(world->bombs_list.begin() + i);
				//i = 0;
				i -= 1;
				continue;
			}
		}
		else
			transparency = 1.0;

		if (world->bombs_list[i].did_it_explode())
		for (int j = 0; j < world->bombs_list[i].explosion_coords.size(); j++)
		{
			if (image_explosion)
				al_draw_tinted_scaled_bitmap(image_explosion, al_map_rgba_f(transparency, transparency, transparency, transparency), 0, 0, al_get_bitmap_width(image_explosion), al_get_bitmap_height(image_explosion),
				MARGIN + grid + world->bombs_list[i].explosion_coords[j].coord[0] * grid, MARGIN + grid + world->bombs_list[i].explosion_coords[j].coord[1] * grid, grid, grid, 0);
			else
			{
				int x, y;
				al_draw_filled_rectangle(x = MARGIN + grid + world->bombs_list[i].explosion_coords[j].coord[0] * grid, y = MARGIN + grid + world->bombs_list[i].explosion_coords[j].coord[1] * grid,
										 x + grid, y + grid, al_map_rgb(255, 0, 0));
			}
		}
	}
	//al_flip_display();
}

void redraw_all(World *world)
{
	draw_static_world(world);
	draw_dynamic_things(world);
	//al_draw_textf(font, al_map_rgb(255, 255, 255), 5, 5, 0, "x:%2d y:%2d", world->creatures_list[0].coord[0], world->creatures_list[0].coord[1]);
	al_flip_display();
	return;
}


void draw_alert(string text1, string text2)
{
	al_draw_filled_rectangle(0.1*al_get_display_width(display), 0.4*al_get_display_height(display), 0.9*al_get_display_width(display), 0.6*al_get_display_height(display), al_map_rgba(255, 0, 0, 0.8));
	al_draw_text(font, al_map_rgb(TEXT_COLOR), 0.15*al_get_display_width(display), 0.45*al_get_display_height(display), 0, text1.c_str());
	al_draw_text(font, al_map_rgb(TEXT_COLOR), 0.15*al_get_display_width(display)+1, 0.45*al_get_display_height(display)+TEXT_SIZE+3, 0, text2.c_str());	//bold
	al_draw_text(font, al_map_rgb(TEXT_COLOR), 0.15*al_get_display_width(display), 0.45*al_get_display_height(display), 0, text1.c_str());
	al_draw_text(font, al_map_rgb(TEXT_COLOR), 0.15*al_get_display_width(display)+1, 0.45*al_get_display_height(display)+TEXT_SIZE+3, 0, text2.c_str());	//bold
}

int main(int argc = 0, char **argv = NULL)
{
	allegro_initialization(winXsize, winYsize);
	srand(time(NULL));
	while (1)
	{
		World *GameWorld = new World(WORLD_SIZE);
		GameWorld->creatures_list.push_back(*new Movable_creature);	//create player at index 0
		GameWorld->creatures_list[0].generate_starting_position(GameWorld);
		redraw_all(GameWorld);
		while (1)
		{

			try
			{
				if (!al_is_event_queue_empty(event_queue))
				{
					al_wait_for_event(event_queue, &ev);
					if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE || (ev.type == ALLEGRO_EVENT_KEY_DOWN && (ev.keyboard.keycode == ALLEGRO_KEY_Q || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)))
						return 0;  //exit program
					else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)

						switch (ev.keyboard.keycode)
					{
						case ALLEGRO_KEY_UP:	GameWorld->creatures_list[0].move_up(GameWorld);	break;
						case ALLEGRO_KEY_DOWN:	GameWorld->creatures_list[0].move_down(GameWorld);	break;
						case ALLEGRO_KEY_LEFT:	GameWorld->creatures_list[0].move_left(GameWorld);	break;
						case ALLEGRO_KEY_RIGHT:	GameWorld->creatures_list[0].move_right(GameWorld);	break;
						case ALLEGRO_KEY_SPACE:	GameWorld->bombs_list.push_back(*new Bomb(GameWorld->creatures_list[0].coord[0], GameWorld->creatures_list[0].coord[1])); break;
						default:
							continue;
					}
				}

				//check if player is standing on exit door or key
				if (GameWorld->get_from_map(GameWorld->creatures_list[0].coord[0], GameWorld->creatures_list[0].coord[1]) == OBSTACLE_FINISH_DOOR)
				{
					delete GameWorld;
					break;	// restart game
					return 0;
				}
				//switch (GameWorld->get_from_map(GameWorld->creatures_list[0].coord[0], GameWorld->creatures_list[0].coord[1] + 1))
				//{
				//case OBSTACLE_FINISH_DOOR:
				//	main(argc, argv);
				//	return 0;
				//};	
				//check if it's time for some bomb to explode - if yes -> explode
				for (int i = 0; i < GameWorld->bombs_list.size(); i++)
				if (GameWorld->bombs_list[i].should_bomb_expolode() && !GameWorld->bombs_list[i].did_it_explode())
					GameWorld->bombs_list[i].explode(GameWorld);
			}
			catch (int error_code)
			{
				redraw_all(GameWorld);
				switch (error_code)
				{
				case EXSC_PLAYER_ON_FIRE:
					draw_alert("You are dead.", "q - quit, space - play again"); break;
				case EXSC_PLAYER_KILLED:
					draw_alert("You were killed.", "q - quit, space - play again"); break;
				}
				al_flip_display();

				//al_flush_event_queue(event_queue);
				al_wait_for_event(event_queue, &ev);
				if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE || (ev.type == ALLEGRO_EVENT_KEY_DOWN && (ev.keyboard.keycode == ALLEGRO_KEY_Q || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)))
					return 0;  //exit program
				else if (ev.type == ALLEGRO_EVENT_KEY_DOWN/* && ev.keyboard.keycode == ALLEGRO_KEY_SPACE*/)
				{
					break;	//play again
				}

			}


			redraw_all(GameWorld);
		}
	}
	//system("pause");
	return 0;
}