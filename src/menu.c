#include <menu.h>

#include <stdlib.h>
#include <string.h>

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

void menu_draw(struct Menu* menu) {
  if(menu->camera > menu->cursor)
    menu->camera = menu->cursor;
  else if(menu->camera + menu->height - 1 < menu->cursor)
    menu->camera = menu->cursor - menu->height + 1;

  struct MenuItem** menu_items = NULL;
  unsigned int menu_items_length = 0;

  if(menu->filter != NULL) {
    menu_items = menu->filtered_items;
    menu_items_length = menu->filtered_items_length;
  } else {
    menu_items = menu->items;
    menu_items_length = menu->items_length;
  }

  if(menu->filter != NULL) {
    menu_items = menu->filtered_items;
    menu_items_length = menu->filtered_items_length;
  } else {
    menu_items = menu->items;
    menu_items_length = menu->items_length;
  }

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
 
    uintattr_t foreground = 0;
    uintattr_t background = 0; 

    if(
      (menu->select && item_y >= MIN(menu->cursor, menu->selection) && item_y <= MAX(menu->cursor, menu->selection)) || // selected
      item_y == menu->cursor // cursor
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

void menu_init(struct Menu* menu) {
  menu->camera = 0;

  menu->select = false;
  menu->selection = 0;

  menu->filter = NULL;
  menu->filtered_items = NULL;
  menu->filtered_items_length = 0;
}

int menu_set_items(struct Menu* menu, struct MenuItem* items, unsigned int items_length) {
  menu->items = (struct MenuItem**) malloc(items_length * sizeof(struct MenuItem*));
  if(menu->items == NULL)
    return -1;
  for(unsigned int i = 0; i < items_length; i ++)
    menu->items[i] = &items[i];

  menu->items_length = items_length;
  return 0;
}

void menu_move_cursor(struct Menu* menu, int step) {
  if((int) menu->cursor + step > 0)
    menu->cursor += step;
  else
    menu->cursor = 0;

  if(menu->cursor > menu->items_length - 1)
    menu->cursor = menu->items_length - 1;
}

void menu_toggle_select(struct Menu* menu) {
  menu->select = !menu->select;
  if(menu->select)
    menu->selection = menu->cursor;
}

void menu_uninit(struct Menu* menu) {
  if(menu->items != NULL) {
    free(menu->items);
    menu->items = NULL;
  }
}
