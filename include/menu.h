#ifndef MENU_H
#define MENU_H

#include <termbox2.h>

#include <stdbool.h>

struct MenuItem {
  uintattr_t foreground;
  uintattr_t foreground_reversed;
  const char* contents;
};

struct Menu {
  unsigned int x, y, width, height;   // must be set

  unsigned int camera;                // do not change

  unsigned int cursor;                // must be set
  uintattr_t cursor_background;       // must be set
  uintattr_t normal_background;       // must be set

  bool select;                        // do not change
  unsigned int selection;             // do not change

  struct MenuItem* items;             // must be set
  unsigned int items_length;          // must be set

  const char* filter;                 // do not change
  struct MenuItem** filtered_items;   // do not change
  unsigned int filtered_items_length; // do not change
};

extern void menu_draw(struct Menu*);
extern void menu_init(struct Menu*);
extern int menu_get_selected(struct Menu*, struct MenuItem*** output); // 0 success | -1 malloc
extern int menu_set_filter(struct Menu*, const char* filter);          // 0 success | -1 malloc
extern void menu_toggle_select(struct Menu*);

#endif
