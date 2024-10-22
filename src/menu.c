#include <menu.h>

#include <string.h>

void menu_draw(struct Menu* menu) {
  for(unsigned int i = 0; i < menu->height; i ++) {
    unsigned int item_y = i + menu->y;

    unsigned int char_y = menu-> y + i;
    if(item_y >= menu->items_length) {
      for(unsigned int j = 0; j < menu->width; j ++)
        tb_set_cell(menu->x + j, char_y, ' ', 0, menu->normal_background);
      continue;
    }

    const char* item = menu->items[item_y].contents;
    unsigned int item_length = strlen(item);

    uintattr_t foreground = 0;
    uintattr_t background = 0;
    /*uintattr_t foreground = menu->items[item_y].foreground;*/
    /*uintattr_t background = item_y == menu->cursor ? menu->cursor_background : menu->normal_background;*/

    for(unsigned int j = 0; j < menu->width; j ++) {
      unsigned int char_x = menu->x + j;
      if(j < item_length)
        tb_set_cell(char_x, char_y, item[j], foreground, background);
      else
        tb_set_cell(char_x, char_y, ' ', foreground, background);
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
