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
	al_init_image_addon();

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

	ALLEGRO_BITMAP *test = al_load_bitmap("firstScreenBack.png");
	al_init_image_addon();
	al_draw_bitmap(test, 0, 0, 0);

	ALLEGRO_FONT *font = al_load_font("Cambay.AH.ttf", 45, NULL); // from https://fontlibrary.org/pl/font/cambay
	al_draw_text(font, al_map_rgb(253, 143, 0), 350, 600 / 8, ALLEGRO_ALIGN_CENTRE, "Hello! Welcome to emptyfridge!");
	ALLEGRO_FONT *font1 = al_load_font("Cambay.AH.ttf", 26, NULL);
	al_draw_text(font1, al_map_rgb(253, 143, 0), 10, 200, ALLEGRO_ALIGN_LEFT, "It is a programme made to help you with yours daily struggle - cooking");
	al_draw_text(font1, al_map_rgb(253, 143, 0), 10, 240, ALLEGRO_ALIGN_LEFT, "It is based on searching recipes by ingridients");

	al_draw_text(font1, al_map_rgb(144, 144, 144), 10, 360, ALLEGRO_ALIGN_LEFT, "Please insert at least three products and separate them with comma:");



	ALLEGRO_FONT *font2 = al_load_font("Cambay.AH.ttf", 22, NULL);
	al_draw_text(font2, al_map_rgb(144, 144, 144), 571, 415, ALLEGRO_ALIGN_LEFT, "ok");

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
	bool live = true;

	ALLEGRO_EVENT_QUEUE * queue = al_create_event_queue();
	ALLEGRO_EVENT event;

	al_register_event_source(queue, al_get_keyboard_event_source()); //register keyboard

	//al_register_event_source(queue, al_get_mouse_event_source());


	event.keyboard.unichar = NULL;

	while (live) {
		al_get_next_event(queue, &event);
		al_flip_display(display);

		if (event.keyboard.unichar != 0) { //0 is null in ascii
			if (event.keyboard.unichar != 8) { //8 is backspace

				input_text[i] = event.keyboard.unichar;
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
					/*char tempArray2[100];
					reverse_string(partOfInput_text);
					copyArray(partOfInput_text, tempArray2, j);
					al_draw_text(font2, al_map_rgb(144, 144, 144), carriage_position, 410, ALLEGRO_ALIGN_LEFT, tempArray2);*/
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

		if (event.keyboard.unichar == 13) live = false; //13 is enter in ascii

		//i++;
		if (i > 99)live = false;
		event.keyboard.unichar = NULL;

	}

}



void print_meal_selection(ALLEGRO_DISPLAY *display)
{
	al_clear_to_color(al_map_rgb(255, 255, 255));
	ALLEGRO_BITMAP *test = al_load_bitmap("secondScreenBack.png");
	al_init_image_addon();
	al_draw_bitmap(test, 0, 0, 0);

	al_draw_rectangle(50, 40, 750, 560, al_map_rgb(144, 144, 144), 2);

	ALLEGRO_FONT *font = al_load_font("Cambay.AH.ttf", 30, NULL);
	al_draw_text(font, al_map_rgb(253, 143, 0), 60, 40, ALLEGRO_ALIGN_LEFT, "There are three ideas of meals based on your products,");
	al_draw_text(font, al_map_rgb(253, 143, 0), 60, 77, ALLEGRO_ALIGN_LEFT, "please select one of them:");

	ALLEGRO_FONT *font1 = al_load_font("Cambay.AH.ttf", 38, NULL);
	al_draw_text(font1, al_map_rgb(160, 160, 160), 100, 180, ALLEGRO_ALIGN_LEFT, "first");
	al_draw_text(font1, al_map_rgb(95, 95, 95), 100, 260, ALLEGRO_ALIGN_LEFT, "second");
	al_draw_text(font1, al_map_rgb(65, 65, 65), 100, 340, ALLEGRO_ALIGN_LEFT, "third");

	ALLEGRO_EVENT_QUEUE * queue = al_create_event_queue();
	ALLEGRO_EVENT event;

	al_register_event_source(queue, al_get_keyboard_event_source());



}
/*void split_string(char* input_text, char** ingridients) {

	int i = 0;
	char * pch;
	pch = strtok(input_text, ",");
	while (pch != NULL)    
	{
		pch = strtok(NULL, ",");
		ingridients[i] = pch;
		i++;
	}

}*/
// cliparts included in bitmaps from: https://www.1001freedownloads.com