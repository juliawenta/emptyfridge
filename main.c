#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_color.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\mouse.h>
#include <allegro5\keyboard.h>
#include <allegro5\allegro_primitives.h>


void print_welcome_screen(ALLEGRO_DISPLAY *display);
void print_meal_selection(ALLEGRO_DISPLAY *second_display); //TODO
void print_meal_description(); //TODO

void print_textbox();
void split_string(char* input_text, char** ingridients);

int main() {

	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();

	ALLEGRO_DISPLAY *display = al_create_display(800, 600);
	if (!display)
	{
		printf("Failed to create display.\n");
		return 1;
	}

	print_welcome_screen(display);
	print_meal_selection(display);


	ALLEGRO_MOUSE *mouse;
	al_install_mouse();

	ALLEGRO_KEYBOARD_STATE *keyboard;
	al_install_keyboard();



	al_flip_display(display);
		
	al_rest(40.0);

	al_destroy_display(display);
	return 0;
}

void print_welcome_screen(ALLEGRO_DISPLAY *display) {

	al_clear_to_color(al_map_rgb(255, 255, 255));

	ALLEGRO_FONT *font = al_load_font("FantasqueSansMono-Regular.ttf", 34, NULL); // from https://fontlibrary.org/pl/font/fantasque-sans-mono
	al_draw_text(font, al_map_rgb(253, 143, 0), 800 / 2, 600 / 8, ALLEGRO_ALIGN_CENTRE, "Hello! Welcome to emptyfridge!");
	ALLEGRO_FONT *font1 = al_load_font("FantasqueSansMono-Regular.ttf", 18, NULL);
	al_draw_text(font1, al_map_rgb(253, 143, 0), 10, 200, ALLEGRO_ALIGN_LEFT, "It is a programme made to help you with yours daily struggle - cooking");
	al_draw_text(font1, al_map_rgb(253, 143, 0), 10, 240, ALLEGRO_ALIGN_LEFT, "It is based on searching recipies by ingridients");

	al_draw_text(font1, al_map_rgb(144, 144, 144), 10, 370, ALLEGRO_ALIGN_LEFT, "Please insert at least three products and separate them with comma:");

	

	ALLEGRO_FONT *font2 = al_load_font("FantasqueSansMono-Regular.ttf", 16, NULL);
	al_draw_text(font2, al_map_rgb(144,144,144), 380, 422, ALLEGRO_ALIGN_LEFT, "ok");

	//ALLEGRO_KEYBOARD_EVENT 
	char input_text[100];// = "asd";

	print_textbox(input_text, display);

	/*char ingridients[10][10];
	split_string(input_text,ingridients);
	al_draw_text(font1, al_map_rgb(253, 143, 0), 500, 490, ALLEGRO_ALIGN_LEFT, ingridients[1]);*/

	al_destroy_font(font);
	al_destroy_font(font1);
	al_destroy_font(font2);
}

void print_textbox(char* input_text, ALLEGRO_DISPLAY *display) {
	int char_size = 16;
	int carriage_position = 12;
	printf("%c", input_text);
	al_draw_rectangle(10, 410, 360, 450, al_map_rgb(144, 144, 144), 2); // textbox frame
	al_draw_rectangle(370, 410, 410, 450, al_map_rgb(144, 144, 144), 2); //button

	ALLEGRO_FONT *font2 = al_load_font("FantasqueSansMono-Regular.ttf", char_size, NULL);
	

	int i = 0;
	bool live = true;

	ALLEGRO_EVENT_QUEUE * queue = al_create_event_queue();
	ALLEGRO_EVENT event;
	
	al_register_event_source(queue, al_get_keyboard_event_source()); //register keyboard

	//al_register_event_source(queue, al_get_mouse_event_source());

	
	event.keyboard.unichar = NULL;

	while (live) {
		al_get_next_event(queue, & event);
		al_flip_display(display);

		if (event.keyboard.unichar != 0) { //0 is null in ascii
			if (event.keyboard.unichar != 8) { //8 is backspace

				input_text[i] = event.keyboard.unichar;
				al_draw_text(font2, al_map_rgb(144, 144, 144), carriage_position, 410, ALLEGRO_ALIGN_LEFT, input_text);
				i++;
				al_flip_display(display);
				//carriage_position = carriage_position + char_size;
			}
			else
			{
				if (i > 0)
				{

				input_text[i - 1] = ' ';
				i--; //if basckspace we want to move back
				al_draw_filled_rectangle(10, 410, 360, 450, al_map_rgb(255, 255, 255), 2); // textbox field
				al_draw_rectangle(10, 410, 360, 450, al_map_rgb(144, 144, 144), 2); // textbox frame
				al_draw_text(font2, al_map_rgb(144, 144, 144), carriage_position, 410, ALLEGRO_ALIGN_LEFT, input_text);
				al_flip_display(display);
			}
		}
		}
		if (event.keyboard.unichar == 13) live = false; //13 is enter in ascii
	
		//i++;
		if (i > 99)live = false;
		event.keyboard.unichar = NULL;

	}

}
void print_meal_selection(ALLEGRO_DISPLAY *display)
{
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_rectangle(50, 40, 750, 560, al_map_rgb(144, 144, 144), 2); //the big box
	al_draw_line(50, 120, 750, 120, al_map_rgb(144, 144, 144), 2); //line which separate title from recipies
	al_draw_line(50, 180, 750, 180, al_map_rgb(144, 144, 144), 1); //recipe 1
	al_draw_line(50, 260, 750, 260, al_map_rgb(144, 144, 144), 1); //recipe 1
	al_draw_line(50, 300, 750, 300, al_map_rgb(144, 144, 144), 1); //recipe 2
	al_draw_line(50, 380, 750, 380, al_map_rgb(144, 144, 144), 1); //recipe 2
	al_draw_line(50, 420, 750, 420, al_map_rgb(144, 144, 144), 1); //recipe 3
	al_draw_line(50, 500, 750, 500, al_map_rgb(144, 144, 144), 1); //recipe 3

	ALLEGRO_FONT *font = al_load_font("FantasqueSansMono-Regular.ttf", 22, NULL);
	al_draw_text(font, al_map_rgb(253, 143, 0), 60, 48, ALLEGRO_ALIGN_LEFT, "There are three ideas of meals based on your products,");
	al_draw_text(font, al_map_rgb(253, 143, 0), 60, 77, ALLEGRO_ALIGN_LEFT, "please select one of them:");

	ALLEGRO_FONT *font1 = al_load_font("FantasqueSansMono-Regular.ttf", 20, NULL);
	al_draw_text(font1, al_map_rgb(150, 150, 150), 60, 210, ALLEGRO_ALIGN_LEFT, "first");
	al_draw_text(font1, al_map_rgb(100, 100, 100), 60, 330, ALLEGRO_ALIGN_LEFT, "second");
	al_draw_text(font1, al_map_rgb(70, 70, 70), 60, 450, ALLEGRO_ALIGN_LEFT, "third");


}
void split_string(char* input_text, char** ingridients) {

	int i = 0;
	char * pch;
	pch = strtok(input_text, ",");
	while (pch != NULL)
	{
		pch = strtok(NULL, ",");
		ingridients[i] = pch;
		i++;
	}

}
