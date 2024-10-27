#include <bind_functions.h>
#include <tfm.h>
#include <sys/stat.h>

int change_directory(struct Menu* menu, const struct Argument* unused) {
  struct MenuItem** menu_items = menu_get_current_items(menu);
  struct MenuItem* item = *(menu_items + menu->cursor);

  struct stat st;
  if(stat(item->contents, &st) != 0)
    return -1;

  if(st.st_mode & S_IFDIR) {
    if(chdir(item->contents) != 0)
      return -1;
    return tfm_change_directory(menu, ".");
  }

  return 0;
}

int cursor_top(struct Menu* menu, const struct Argument* unused) {
  menu->cursor = 0;

  return 0;
}

int cursor_bottom(struct Menu* menu, const struct Argument* unused) {
  unsigned int menu_length = menu_get_current_items_length(menu);

  menu->cursor = menu_length;

  return 0;
}

int move(struct Menu* menu, const struct Argument* argument) {
  menu_move_cursor(menu, argument->i);
  return 0;
}

int switch_cursor_with_selection(struct Menu* menu, const struct Argument* unused) {
  if(menu->mode != 'v')
    return 0;

  unsigned int buffer = menu->cursor;
  menu->cursor = menu->selection;
  menu->selection = buffer;

  return 0;
}

int toggle_visual_mode(struct Menu* menu, const struct Argument* unused) {
  menu_toggle_select(menu);
  
  return 0;
}
