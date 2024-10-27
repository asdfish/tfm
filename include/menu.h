#ifndef MENU_H
#define MENU_H

#include <orchestra.h>
#include <termbox2.h>

struct MenuItem {
  uintattr_t foreground;
  uintattr_t foreground_reversed;
  const char* contents;
};

struct Menu {
  char mode;                                  // must be set
  o_string strokes;                           // do not change
 
  unsigned int x, y, width, height;           // must be set

  unsigned int camera, cursor;                // do not change
  uintattr_t background, background_reversed; // must be set

  unsigned int selection;                     // do not change

  struct MenuItem** items;                    // do not change
  unsigned int items_length;                  // do not change

  struct MenuItem** filtered_items;           // do not change
  unsigned int filtered_items_length;         // do not change
};

extern int menu_change_filtered_items(struct Menu* menu, const char* filter);
extern int menu_change_items(struct Menu* menu, struct MenuItem* items, unsigned int items_length);
extern void menu_draw(struct Menu* menu);
extern void menu_free(struct Menu* menu);
extern void menu_free_filtered_items(struct Menu* menu);
extern void menu_free_items(struct Menu* menu);
extern int menu_init(struct Menu* menu);
extern struct MenuItem** menu_get_current_items(struct Menu* menu);
extern unsigned int menu_get_current_items_length(struct Menu* menu);
extern int menu_get_selected(struct Menu* menu, struct MenuItem*** output, unsigned int* output_length);
extern void menu_move_cursor(struct Menu* menu, int step);
extern int menu_set_filtered_items(struct Menu* menu, const char* filter);
extern int menu_set_items(struct Menu* menu, struct MenuItem* items, unsigned int items_length);
extern void menu_toggle_select(struct Menu* menu);
extern void menu_verify_cursor_position(struct Menu* menu);

#endif
