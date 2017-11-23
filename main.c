#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_color.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\mouse.h>
#include <allegro5\keyboard.h>

void print_welcome_screen();
void print_meal_selection(); //TODO
void print_meal_description(); //TODO

int main() {

	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();

	ALLEGRO_DISPLAY *display = al_create_display(800, 600);
	if (!display)
	{
		printf("Failed to create display.\n");
		return 1;
	}

	print_welcome_screen();

	ALLEGRO_MOUSE *mouse;
	al_install_mouse();

	ALLEGRO_KEYBOARD_STATE *keyboard;
	al_install_keyboard();



	al_flip_display(display);
		
	al_rest(10.0);

	al_destroy_display(display);
	return 0;
}

void print_welcome_screen() {

	al_clear_to_color(al_map_rgb(255, 255, 255));

	ALLEGRO_FONT *font = al_load_font("FantasqueSansMono-Regular.ttf", 34, NULL); // from https://fontlibrary.org/pl/font/fantasque-sans-mono
	al_draw_text(font, al_map_rgb(253, 143, 0), 800 / 2, 600 / 8, ALLEGRO_ALIGN_CENTRE, "Hello! Welcome to emptyfridge!");
	ALLEGRO_FONT *font1 = al_load_font("FantasqueSansMono-Regular.ttf", 18, NULL);
	al_draw_text(font1, al_map_rgb(253, 143, 0), 10, 200, ALLEGRO_ALIGN_LEFT, "It is a programme made to help you with yours daily struggle - cooking");
	al_draw_text(font1, al_map_rgb(253, 143, 0), 10, 240, ALLEGRO_ALIGN_LEFT, "It is based on searching recipies by ingridients");

	al_draw_text(font1, al_map_rgb(144, 144, 144), 10, 370, ALLEGRO_ALIGN_LEFT, "Please insert at least three products and separate them with comma:");

	al_destroy_font(font);
	al_destroy_font(font1);

}