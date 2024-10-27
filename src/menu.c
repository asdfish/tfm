#include <macros.h>
#include <menu.h>

#include <stdlib.h>
#include <string.h>

int menu_change_filtered_items(struct Menu* menu, const char* filter) {
  menu_free_filtered_items(menu);

  if(filter == NULL || strlen(filter) == 0)
    return 0;

  int result = menu_set_filtered_items(menu, filter);
  if(result != 0)
    return result;

  if(menu->cursor > menu->filtered_items_length)
    menu->cursor = menu->filtered_items_length;
  if(menu->selection > menu->filtered_items_length)
    menu->selection = menu->filtered_items_length;

  return 0;
}

int menu_change_items(struct Menu* menu, struct MenuItem* items, unsigned int items_length) {
  menu_free_items(menu);
  return menu_set_items(menu, items, items_length);
}

void menu_draw(struct Menu* menu) {
  menu_verify_cursor_position(menu);

  if(menu->camera > menu->cursor)
    menu->camera = menu->cursor;
  else if(menu->camera + menu->height - 1 < menu->cursor)
    menu->camera = menu->cursor - menu->height + 1;

  struct MenuItem** menu_items = menu_get_current_items(menu);
  unsigned int menu_items_length = menu_get_current_items_length(menu);

  for(unsigned int i = 0; i < menu->height; i ++) {
    unsigned int item_y = i + menu->y;

    unsigned int char_y = menu-> y + i;
    if(item_y >= menu_items_length) {
      for(unsigned int j = 0; j < menu->width; j ++)
        tb_set_cell(menu->x + j, char_y, ' ', 0, menu->background);
      continue;
    }

    struct MenuItem* item = *(menu_items + item_y);

    const char* item_name = item->contents;
    unsigned int item_name_length = strlen(item_name);
 
    uintattr_t foreground;
    uintattr_t background; 

    if(
      (menu->mode == 'v' && item_y >= MIN(menu->cursor, menu->selection) && item_y <= MAX(menu->cursor, menu->selection)) ||
      item_y == menu->cursor
    ) {
      foreground = item->foreground_reversed;
      background = menu->background_reversed;
    } else {
      foreground = item->foreground;
      background = menu->background;
    }

    for(unsigned int j = 0; j < menu->width; j ++) {
      char next_char = ' ';
      if(j < item_name_length)
        next_char = item_name[j];
      
      tb_set_cell(menu->x + j, char_y, next_char, foreground, background);
    }
  }
}

void menu_free(struct Menu* menu) {
  o_string_uninit(&menu->strokes);

  menu_free_filtered_items(menu);
  menu_free_items(menu);
}

void menu_free_filtered_items(struct Menu* menu) {
  if(menu->filtered_items == NULL)
    return;

  free(menu->filtered_items);
  menu->filtered_items = NULL;
}

void menu_free_items(struct Menu* menu) {
  if(menu->items == NULL)
    return;

  free(menu->items);
  menu->items = NULL;
}

struct MenuItem** menu_get_current_items(struct Menu* menu) {
  return menu->filtered_items != NULL ? menu->filtered_items : menu->items;
}

unsigned int menu_get_current_items_length(struct Menu* menu) {
  return menu->filtered_items != NULL ? menu->filtered_items_length : menu->items_length;
}

int menu_get_selected(struct Menu* menu, struct MenuItem*** output, unsigned int* output_length) {
  menu_verify_cursor_position(menu);

  if(menu->mode != 'v')
    return 0;

  struct MenuItem** items = menu_get_current_items(menu);

  unsigned int min = MIN(menu->cursor, menu->selection);
  unsigned int max = MAX(menu->cursor, menu->selection);

  unsigned int selection_length = (max - min) + 1;

  *output = (struct MenuItem**) malloc(selection_length * sizeof(struct MenuItem*));
  if(output == NULL)
    return -1;

  for(unsigned int i = 0; i < selection_length; i ++)
    (*output)[i] = items[min + i];
  *output_length = selection_length;
  return 0;
}

int menu_init(struct Menu* menu) {
  if(o_string_init(&menu->strokes) != O_SUCCESS)
    return -1;

  menu->camera = 0;
  menu->cursor = 0;

  menu->mode = 'n';
  menu->selection = 0;

  menu->items = NULL;
  menu->items_length = 0;

  menu->filtered_items = NULL;
  menu->filtered_items_length = 0;

  return 0;
}

int menu_set_filtered_items(struct Menu* menu, const char* filter) {
  unsigned int filtered_items_length = 0;
  for(unsigned int i = 0; i < menu->items_length; i++)
    if(strstr(menu->items[i]->contents, filter) != NULL)
      filtered_items_length ++;

  struct MenuItem** filtered_items = (struct MenuItem**) malloc(filtered_items_length * sizeof(struct Menu*));
  if(filtered_items == NULL)
    return -1;

  for(unsigned int i = 0, j = 0; i < menu->items_length; i ++)
    if(strstr(menu->items[i]->contents, filter) != NULL) {
      filtered_items[j] = menu->items[i];
      j ++;
    }
      
  menu->filtered_items = filtered_items;
  menu->filtered_items_length = filtered_items_length;

  menu_verify_cursor_position(menu);
  return 0;
}

int menu_set_items(struct Menu* menu, struct MenuItem* items, unsigned int items_length) {
  menu->items = (struct MenuItem**) malloc(items_length * sizeof(struct MenuItem*));
  if(menu->items == NULL)
    return -1;
  for(unsigned int i = 0; i < items_length; i ++)
    menu->items[i] = &items[i];

  menu->items_length = items_length;

  menu_verify_cursor_position(menu);
  return 0;
}

void menu_move_cursor(struct Menu* menu, int step) {
  if((int) menu->cursor + step > 0)
    menu->cursor += step;
  else
    menu->cursor = 0;

  menu_verify_cursor_position(menu);
}

void menu_toggle_select(struct Menu* menu) {
  menu_verify_cursor_position(menu);

  menu->mode = menu->mode == 'v' ? 'n' : 'v';
  menu->selection = menu->cursor;
}

void menu_verify_cursor_position(struct Menu* menu) {

  unsigned int items_length = menu_get_current_items_length(menu);

  if(menu->cursor > items_length - 1)
    menu->cursor = items_length - 1;
}
