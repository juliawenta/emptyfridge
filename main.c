#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_color.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\mouse.h>
#include <allegro5\keyboard.h>
#include <allegro5\allegro_primitives.h>
#include <Windows.h>

const char * print_welcome_screen(ALLEGRO_DISPLAY *display);
const char *create_python_command(int i, char *type);
void load_file(char *file_name, char *result);
int print_meal_selection(ALLEGRO_DISPLAY *second_display); 
void print_meal_description(ALLEGRO_DISPLAY *display, int selected_meal); //TODO
void print_textbox(char* input_text, ALLEGRO_DISPLAY *display);

int main(void) {

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

	FILE *file= fopen("ingr.txt", "w");
	const char * ingridients = print_welcome_screen(display);
	
	fprintf(file,  ingridients);
	
	fclose(file);

	system("python.exe RestHandler/RestHandler.py recipe");



	int selected_meal = print_meal_selection(display); 
	print_meal_description(display,selected_meal);

	ALLEGRO_MOUSE *mouse;
	al_install_mouse();

	ALLEGRO_KEYBOARD_STATE *keyboard;
	al_install_keyboard();

	al_flip_display(display);

	al_rest(40.0);

	al_destroy_display(display);
	
	return 0;
}

const char * print_welcome_screen(ALLEGRO_DISPLAY *display) {

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
	

	al_destroy_font(font);
	al_destroy_font(font1);
	al_destroy_font(font2);

	char cleaned_input_text[100]; // to clean  file from unnecessary symbols

	int i = 0, c = 0;
	for (; i < 100; i++)
	{
		if (isalpha(input_text[i]) || (input_text[i]==','))
		{	
			cleaned_input_text[c] = input_text[i];
			c++;
		}
	}
	cleaned_input_text[c] = '\0';

	return cleaned_input_text;
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

			//printf("x: %i\n", mouse_x);
			//printf("y: %i\n", mouse_y);

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
				if (insert_event.keyboard.unichar != 8) { //8 is backspace in ascii

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
							if (newWidth > 540)
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

int print_meal_selection(ALLEGRO_DISPLAY *display){

	int result = -1;
	al_clear_to_color(al_map_rgb(255, 255, 255));

	ALLEGRO_BITMAP *background = al_load_bitmap("secondScreenBack2.png");
	al_init_image_addon();
	al_draw_bitmap(background, 0, 0, 0);

	al_draw_rectangle(50, 40, 750, 560, al_map_rgb(144, 144, 144), 2);

	ALLEGRO_FONT *font = al_load_font("Cambay.AH.ttf", 30, NULL);
	al_draw_text(font, al_map_rgb(253, 143, 0), 60, 40, ALLEGRO_ALIGN_LEFT, "There are three ideas of meals based on your products,");
	al_draw_text(font, al_map_rgb(253, 143, 0), 60, 77, ALLEGRO_ALIGN_LEFT, "please select one of them:");

	/*
	We run python to obtain next label value. 
	Every execution overwrites result.txt file with new label. 
	*/
	system("python.exe RestHandler/RestHandler.py label 0");
	char label_0[1000];
	load_file("C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/result.txt", label_0);
	//printf("%s", label_0);
	system("python.exe RestHandler/RestHandler.py label 1");
	char label_1[1000];
	load_file("C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/result.txt",label_1);
	system("python.exe RestHandler/RestHandler.py label 2");
	char label_2[1000];
	load_file("C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/result.txt",label_2);

	/*
	We use labels created above to show recipes on second screen.
	*/
	ALLEGRO_FONT *font1 = al_load_font("Cambay.AH.ttf", 18, NULL);
	al_draw_rectangle(100, 180, 120, 200, al_map_rgb(144, 144, 144), 2);
	al_draw_text(font1, al_map_rgb(160, 160, 160), 130, 177, ALLEGRO_ALIGN_LEFT, label_0);
	al_draw_rectangle(100, 250, 120, 270, al_map_rgb(144, 144, 144), 2);
	al_draw_text(font1, al_map_rgb(95, 95, 95), 130, 248, ALLEGRO_ALIGN_LEFT, label_1);
	al_draw_rectangle(100, 320, 120, 340, al_map_rgb(144, 144, 144), 2);
	al_draw_text(font1, al_map_rgb(65, 65, 65), 130, 318, ALLEGRO_ALIGN_LEFT, label_2);

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

		//printf("x: %i\n", mouse_x);
		//printf("y: %i\n", mouse_y);

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
			result= 0;
			break;
		}
		if (mouse_second == true)
		{
			click_s = true; //means click at button
			chosen = true;
			result = 1;
			break;
		}
		if (mouse_third == true)
		{
			click_t = true; //means click at button
			chosen = true;
			result = 2;
			break;
		}
		
	}
	al_destroy_event_queue(select_queue);
	}
		return result;
		
		al_destroy_display(display);
		
}

// cliparts included in bitmaps from: https://www.1001freedownloads.com, https://openclipart.org/user-detail/Gerald_G


void load_file(char *file_name, char *result)
{
	FILE *file;
	file = fopen(file_name, "r");
	fgets(result, 200, file);
	fclose(file);
}

const char *create_python_command(int i, char *type)
{
	char selected_meal_as_string[5];
	char execute_python[50];
	sprintf(selected_meal_as_string, "%d", i);
	strcpy(execute_python, "python.exe RestHandler/RestHandler.py ");
	strcat(execute_python, type);
	strcat(execute_python, " ");

	strcat(execute_python, selected_meal_as_string);
	printf("%s", execute_python);
	
	int size = 0;
	for (int j = 0; !isdigit(execute_python[j]); j++)
	{
		size++;
	}
	//char cleaned_input_text[100];
	char *cleaned_input_text = malloc(size * sizeof(char));
	int j = 0, c = 0;
	for (; j < size +1 ; j++)
	{
		if ((isalpha(execute_python[j]) || (execute_python[j] == '/') || (execute_python[j] == '.') || (execute_python[j] == ' ') || (isdigit(execute_python[j]))))
		{
			cleaned_input_text[c] = execute_python[j];
			c++;
			if (isdigit(execute_python[j]))
			{
				break;
			}
		}
	}
	cleaned_input_text[c] = '\0';

	return cleaned_input_text;

	//return execute_python;
}

void print_meal_description(ALLEGRO_DISPLAY *display, int selected_meal) {

		al_clear_to_color(al_map_rgb(255, 255, 255));
		ALLEGRO_BITMAP *background = al_load_bitmap("thirdScreenBack.png");
		al_init_image_addon();
		al_draw_bitmap(background, 0, 0, 0);
	
		ALLEGRO_FONT *font = al_load_font("Cambay.AH.ttf", 30, NULL);
		al_draw_text(font, al_map_rgb(253, 143, 0), 45, 22, ALLEGRO_ALIGN_LEFT, "That is a very good choice,");
		al_draw_text(font, al_map_rgb(253, 143, 0), 45, 50, ALLEGRO_ALIGN_LEFT, "here are some information about yours recipe:");

		ALLEGRO_FONT *font1 = al_load_font("Cambay.AH.ttf", 25, NULL);
		al_draw_text(font1, al_map_rgb(253, 143, 0), 45, 440, ALLEGRO_ALIGN_LEFT, "Click HERE to see recipe.");

		int x=0;
		int y=0;
		bool third_screen = false; //if recipe is chosen
		bool click = false;
		bool mouse_over_here = false;
		bool click_link = false;


		system(create_python_command(selected_meal, "image"));


		//system
		//char cmd[100];
		//*cmd = create_python_command(selected_meal, "url");
		system(create_python_command(selected_meal, "url"));
		char url[1000];
		load_file("C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/result.txt",url);
		
		printf("%s", url);

		char execute_cmd[100];
		strcpy(execute_cmd, "cmd /c start ");
		strcat(execute_cmd, url);

		
		

		while (!third_screen) {

			ALLEGRO_EVENT_QUEUE *select_queue = al_create_event_queue();
			al_register_event_source(select_queue, al_get_mouse_event_source());

			ALLEGRO_EVENT select_event;
			al_wait_for_event(select_queue, &select_event);
			al_flip_display(display);


			if (select_event.type == ALLEGRO_EVENT_MOUSE_AXES)
			{
				x = select_event.mouse.x;
				y = select_event.mouse.y;

				//printf("x: %i\n", x);
				//printf("y: %i\n", y);
			}
			if ((y >= 449) && (y <= 463) && (x >= 96) && (x <= 139))
			{
				al_draw_text(font1, al_map_rgb(255, 68, 0), 94, 440, ALLEGRO_ALIGN_LEFT, "HERE");
				al_flip_display(display);
				mouse_over_here = true;
			}
			else 
			{
				al_draw_text(font1, al_map_rgb(255, 255, 255), 45, 440, ALLEGRO_ALIGN_LEFT, "Click HERE to see recipe.");
				al_draw_text(font1, al_map_rgb(253, 143, 0), 45, 440, ALLEGRO_ALIGN_LEFT, "Click HERE to see recipe.");
				mouse_over_here = false;
			}

			if (select_event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (mouse_over_here == true)
				{
					click = true; //means click at the link
					click_link = true;
					system(execute_cmd);
				}
			}
		}

	}
