#define CURL_STATICLIB
#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_color.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\mouse.h>
#include <allegro5\keyboard.h>
#include <allegro5\allegro_primitives.h>
#include <curl\curl.h>
#include <stdlib.h>
//#include <curl\curlbuild.h>
#include <config-win32.h>
#include "curl.h"
//#include "curl\curlver.h"
//#include "curlrules.h"

//#include <curl.7.30.0.2\tools\native\v110\Win32\Release\dynamic\libcurl.lib>





void print_welcome_screen(ALLEGRO_DISPLAY *display);
void print_meal_selection(ALLEGRO_DISPLAY *second_display); 
void print_meal_description(); //TODO
void print_textbox();
void split_string(char* input_text, char** ingridients);

int main() {

	CURL *curl = curl_easy_init();
	
	curl_easy_cleanup(curl);


	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_image_addon();

	ALLEGRO_DISPLAY *display = al_create_display(800, 600);
	if (!display)
	{
		printf("Failed to create display.\n");
		return 1;
	}

	print_welcome_screen(display);
	print_meal_selection(display);
	print_meal_description(display);


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

	ALLEGRO_BITMAP *background = al_load_bitmap("firstScreenBack.png");
	al_init_image_addon();
	al_draw_bitmap(background, 0, 0, 0);

	ALLEGRO_FONT *font = al_load_font("Cambay.AH.ttf", 45, NULL); // from https://fontlibrary.org/pl/font/cambay
	al_draw_text(font, al_map_rgb(253, 143, 0), 350, 600 / 8, ALLEGRO_ALIGN_CENTRE, "Hello! Welcome to emptyfridge!");
	ALLEGRO_FONT *font1 = al_load_font("Cambay.AH.ttf", 26, NULL);
	al_draw_text(font1, al_map_rgb(253, 143, 0), 10, 200, ALLEGRO_ALIGN_LEFT, "It is a programme made to help you with yours daily struggle - cooking");
	al_draw_text(font1, al_map_rgb(253, 143, 0), 10, 240, ALLEGRO_ALIGN_LEFT, "It is based on searching recipes by ingridients");

	al_draw_text(font1, al_map_rgb(144, 144, 144), 10, 360, ALLEGRO_ALIGN_LEFT, "Please insert at least three products and separate them with comma:");

	ALLEGRO_FONT *font2 = al_load_font("Cambay.AH.ttf", 22, NULL);
	al_draw_text(font2, al_map_rgb(144, 144, 144), 571, 415, ALLEGRO_ALIGN_LEFT, "ok");

	char input_text[100];

	print_textbox(input_text, display);

	char ingridients[10][10];
	split_string(input_text,ingridients);
	//al_draw_text(font1, al_map_rgb(253, 143, 0), 500, 490, ALLEGRO_ALIGN_LEFT, ingridients[1]);

	al_destroy_font(font);
	al_destroy_font(font1);
	al_destroy_font(font2);
}
int copyArray(char *from, char *to, int size) {

	for (int i = 0; i < size; i++) {
		to[i] = from[i];
	}
}

char *reverse_string(char *str) {
	char tmp, *src, *dst;
	size_t length;
	if (str != NULL)
	{
		length = strlen(str);
		if (length > 1) {
			src = str;
			dst = src + length - 1;
			while (src < dst) {
				tmp = *src;
				*src++ = *dst;
				*dst-- = tmp;
			}
		}
	}
	return str;
}

void print_textbox(char* input_text, ALLEGRO_DISPLAY *display) {
	int char_size = 28;
	int carriage_position = 12;
	printf("%c", input_text);
	al_draw_rectangle(10, 410, 550, 450, al_map_rgb(144, 144, 144), 2); // textbox frame
	al_draw_rectangle(560, 410, 600, 450, al_map_rgb(144, 144, 144), 2); //button

	ALLEGRO_FONT *font2 = al_load_font("Cambay.AH.ttf", char_size, NULL);


	int i = 0;
	int mouse_x;
	int mouse_y;
	int click_t = false;
	int click_b = false;

	bool start = false; //to start insert and to click button
	bool mouse_button = false;
	bool mouse_textbox = false;
	bool live = true; //to insert products

	ALLEGRO_EVENT_QUEUE *insert_queue = al_create_event_queue();
	al_register_event_source(insert_queue, al_get_mouse_event_source());
	al_register_event_source(insert_queue, al_get_keyboard_event_source());

	while (!start && live)
	{


		ALLEGRO_EVENT insert_event;
		al_wait_for_event(insert_queue, &insert_event);
		al_flip_display(display);


		if (insert_event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse_x = insert_event.mouse.x;
			mouse_y = insert_event.mouse.y;

			printf("x: %i\n", mouse_x);
			printf("y: %i\n", mouse_y);

			if ((mouse_x >= 10) && (mouse_y >= 410) && (mouse_x <= 550) && (mouse_y <= 450))
			{
				mouse_textbox = true;

			}
			else
			{
				mouse_textbox = false;
			}

			if ((mouse_x >= 560) && (mouse_y >= 410) && (mouse_x <= 600) && (mouse_y <= 450))
			{
				mouse_button = true;

			}
			else
			{
				mouse_button = false;
			}
		}
		if (insert_event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (mouse_textbox == true)
			{
				click_t = true; //means click at textbox
			}
			else
			{
				click_t = false;
			}
			if (mouse_button == true)
			{
				click_b = true; //means click at button
				start = true;
			}
			else
			{
				click_b = false;
			}
		}

		insert_event.keyboard.unichar = NULL;
		al_get_next_event(insert_queue, &insert_event);
		al_flip_display(display);

		if (click_t == true) {

			if (insert_event.keyboard.unichar != 0) { //0 is null in ascii
				if (insert_event.keyboard.unichar != 8) { //8 is backspace

					input_text[i] = insert_event.keyboard.unichar;
					int width = al_get_text_width(font2, input_text);
					if (width < 540)
					{
						al_draw_text(font2, al_map_rgb(144, 144, 144), carriage_position, 410, ALLEGRO_ALIGN_LEFT, input_text);
					}
					else
					{
						char partOfInput_text[100];
						copyArray(input_text, partOfInput_text, i);
						reverse_string(partOfInput_text);
						int newSize = 0;
						int j = i;
						while (true)
						{
							char tempArray[100];
							copyArray(partOfInput_text, tempArray, j);
							int newWidth = al_get_text_width(font2, tempArray);
							if (newWidth < 540)
							{
								break;
							}
							j--; //partogInput in every itreation is decreased
						}
						char tempArray2[100];
						reverse_string(partOfInput_text);
						copyArray(partOfInput_text, tempArray2, j);
						al_draw_text(font2, al_map_rgb(144, 144, 144), carriage_position, 410, ALLEGRO_ALIGN_LEFT, tempArray2);
					}

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
						al_draw_filled_rectangle(10, 410, 550, 450, al_map_rgb(255, 255, 255), 2); // textbox field
						al_draw_rectangle(10, 410, 550, 450, al_map_rgb(144, 144, 144), 2);// textbox frame
						al_draw_text(font2, al_map_rgb(144, 144, 144), carriage_position, 410, ALLEGRO_ALIGN_LEFT, input_text);
						al_flip_display(display);
					}
				}
			}

			if (insert_event.keyboard.unichar == 13) live = false; //13 is enter in ascii

			if (i > 99)live = false;
			insert_event.keyboard.unichar = NULL;

		}
	}
	
	}


	void print_meal_selection(ALLEGRO_DISPLAY *display){

	al_clear_to_color(al_map_rgb(255, 255, 255));


	ALLEGRO_BITMAP *background = al_load_bitmap("secondScreenBack.png");
	al_init_image_addon();
	al_draw_bitmap(background, 0, 0, 0);

	al_draw_rectangle(50, 40, 750, 560, al_map_rgb(144, 144, 144), 2);

	ALLEGRO_FONT *font = al_load_font("Cambay.AH.ttf", 30, NULL);
	al_draw_text(font, al_map_rgb(253, 143, 0), 60, 40, ALLEGRO_ALIGN_LEFT, "There are three ideas of meals based on your products,");
	al_draw_text(font, al_map_rgb(253, 143, 0), 60, 77, ALLEGRO_ALIGN_LEFT, "please select one of them:");

	ALLEGRO_FONT *font1 = al_load_font("Cambay.AH.ttf", 38, NULL);
	al_draw_rectangle(100, 180, 120, 200, al_map_rgb(144, 144, 144), 2);
	al_draw_text(font1, al_map_rgb(160, 160, 160), 130, 164, ALLEGRO_ALIGN_LEFT, "first");
	al_draw_rectangle(100, 250, 120, 270, al_map_rgb(144, 144, 144), 2);
	al_draw_text(font1, al_map_rgb(95, 95, 95), 130, 234, ALLEGRO_ALIGN_LEFT, "second");
	al_draw_rectangle(100, 320, 120, 340, al_map_rgb(144, 144, 144), 2);
	al_draw_text(font1, al_map_rgb(65, 65, 65), 130, 304, ALLEGRO_ALIGN_LEFT, "third");

	
	int mouse_x;
	int mouse_y;
	int click_f = false; // click first recipe
	int click_s = false; //click second recipe
	int click_t = false; //click third recipe

	bool mouse_first = false;
	bool mouse_second = false;
	bool mouse_third = false;
	bool chosen = false; //if recipe is chosen


		while(!chosen){

			ALLEGRO_EVENT_QUEUE *select_queue = al_create_event_queue();
			al_register_event_source(select_queue, al_get_mouse_event_source());

			ALLEGRO_EVENT select_event;
			al_wait_for_event(select_queue, &select_event);
			al_flip_display(display);

	if (select_event.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		mouse_x = select_event.mouse.x;
		mouse_y = select_event.mouse.y;

		printf("x: %i\n", mouse_x);
		printf("y: %i\n", mouse_y);

		if ((mouse_x >= 100) && (mouse_y >= 180) && (mouse_x <= 120) && (mouse_y <= 200))
		{
			mouse_first= true;
			al_draw_filled_rectangle(100, 180, 120, 200, al_map_rgb(144, 144, 144));
		}
		else
		{
			mouse_first = false;
			al_draw_filled_rectangle(100, 180, 120, 200, al_map_rgb(255, 255, 255));
			al_draw_rectangle(100, 180, 120, 200, al_map_rgb(144, 144, 144), 2);
		}

		if ((mouse_x >= 100) && (mouse_y >= 250) && (mouse_x <= 120) && (mouse_y <= 270))
		{
			mouse_second = true;
			al_draw_filled_rectangle(100, 250, 120, 270, al_map_rgb(144, 144, 144));
		}
		else
		{
			mouse_second = false;
			al_draw_filled_rectangle(100, 250, 120, 270, al_map_rgb(255, 255, 255));
			al_draw_rectangle(100, 250, 120, 270, al_map_rgb(144, 144, 144), 2);
		}

		if ((mouse_x >= 100) && (mouse_y >= 320) && (mouse_x <= 120) && (mouse_y <= 340))
		{
			mouse_third = true;
			al_draw_filled_rectangle(100, 320, 120, 340, al_map_rgb(144, 144, 144));
		}
		else
		{
			mouse_third = false;
			al_draw_filled_rectangle(100, 320, 120, 340, al_map_rgb(255, 255, 255));
			al_draw_rectangle(100, 320, 120, 340, al_map_rgb(144, 144, 144), 2);
		}
	}
	if (select_event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (mouse_first == true)
		{
			click_f = true; //means click at textbox
			chosen = true;
		}
		if (mouse_second == true)
		{
			click_s = true; //means click at button
			chosen = true;
		}
		if (mouse_third == true)
		{
			click_t = true; //means click at button
			chosen = true;
		}
	}
	}

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
// cliparts included in bitmaps from: https://www.1001freedownloads.com, https://openclipart.org/user-detail/Gerald_G


	void print_meal_description(ALLEGRO_DISPLAY *display) {

		al_clear_to_color(al_map_rgb(255, 255, 255));
		ALLEGRO_BITMAP *background = al_load_bitmap("thirdScreenBack.png");
		al_init_image_addon();
		al_draw_bitmap(background, 0, 0, 0);
	
	}