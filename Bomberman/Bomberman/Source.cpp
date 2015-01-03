#include "Header.h"

#define TEXT_COLOR 255, 255, 255
#define TEXT_SIZE 18
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_FONT *font_big = NULL;
ALLEGRO_EVENT ev;
int winXsize = 700;
int winYsize = 700;

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
	//int MARGIN = 0;
#define MARGIN 0
	ALLEGRO_COLOR grey, black, orange;
	grey = al_map_rgb(100, 100, 100);
	black = al_map_rgb(0, 0, 0);
	orange = al_map_rgb(120, 75, 0);
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
	for (int i = 0; i < world->world_size_in_blocks; i++)
	for (int j = 0; j < world->world_size_in_blocks; j++)
	{
		if (world->get_from_map(i, j) == OBSTACLE_NONDESTROYABLE)
		{
#define COLOR_MULTIPLIER 255
			if (image_nondestroyable != 0)
				al_draw_tinted_scaled_bitmap(image_nondestroyable, al_map_rgb(COLOR_MULTIPLIER, COLOR_MULTIPLIER, COLOR_MULTIPLIER),
				0, 0, al_get_bitmap_width(image_nondestroyable), al_get_bitmap_height(image_nondestroyable),
				MARGIN + grid + i*grid, MARGIN + grid + j*grid, grid, grid, 0);
			else
				al_draw_filled_rectangle(COORDS, black);
		}
		else if (world->get_from_map(i, j) == OBSTACLE_NOTHING)
		{
#define COLOR_MULTIPLIER 200
			if (image_nothing != 0)
				al_draw_tinted_scaled_bitmap(image_nothing, al_map_rgb(COLOR_MULTIPLIER, COLOR_MULTIPLIER, COLOR_MULTIPLIER),
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
	}

	al_flip_display();
}

void draw_movable_things(World *world)
{
	static ALLEGRO_BITMAP *image_player = al_load_bitmap("Images\\smerf_alpha.png");
	static int grid = (winXsize - 2 * MARGIN) / (WORLD_SIZE + 2);
	al_draw_scaled_bitmap(image_player, 0, 0, al_get_bitmap_width(image_player), al_get_bitmap_height(image_player), 
						  MARGIN + grid + world->creatures_list[0].coord[0] * grid, MARGIN + grid + world->creatures_list[0].coord[1] * grid, grid, grid, 0);
}

int main()
{
	allegro_initialization(winXsize, winYsize);
	srand(time(NULL));
	World GameWorld(WORLD_SIZE);
	draw_static_world(&GameWorld);
	GameWorld.draw_console_world();
	FreeConsole();
	Movable_creature Player;
	GameWorld.creatures_list.push_back(Player);
	Player.generate_starting_position(&GameWorld);
	while (1)
	{
		if (!al_is_event_queue_empty(event_queue))
		{
			al_wait_for_event(event_queue, &ev);
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE || (ev.type == ALLEGRO_EVENT_KEY_DOWN && (ev.keyboard.keycode == ALLEGRO_KEY_Q || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)))
				return 0;  //exit program
			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				break;
		}
	}

	//system("pause");
	return 0;
}