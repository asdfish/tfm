#include <bind_functions.h>
#include <tfm.h>
#include <sys/stat.h>

int bind_function_change_command_line_mode(const struct Argument* argument) {
  menu.mode = argument->c;
  command_line_change_mode(&command_line, argument->c);
  return 0;
}

int bind_function_change_command_line_mode_with(const struct Argument* argument) {
  bind_function_change_command_line_mode(argument);
  o_string_set(&command_line.command, argument->str);
  command_line.cursor = strlen(command_line.command.contents);
  return 0;
}

int bind_function_change_command_line_mode_with_menu_selection(const struct Argument* argument) {
  int exit_code = 0;

  struct MenuItem** selected_items = NULL;
  unsigned int selected_items_length = 0;

  if(menu_get_selected(&menu, &selected_items, &selected_items_length) != 0) {
    exit_code = -1;
    goto exit;
  }

  if(o_string_set(&command_line.command, argument->str) != O_SUCCESS) {
    exit_code = -1;
    goto free_selected_items;
  }

  for(unsigned int i = 0; i < selected_items_length; i ++) {
    if(o_string_cat(&command_line.command, " ") != O_SUCCESS) {
      exit_code = -1;
      goto free_selected_items;
    }
    if(o_string_cat(&command_line.command, selected_items[i]->contents) != O_SUCCESS) {
      exit_code = -1;
      goto free_selected_items;
    }
  }

  bind_function_change_command_line_mode(argument);
  command_line.cursor = strlen(command_line.command.contents);

free_selected_items:
  free(selected_items);
  selected_items = NULL;
exit:
  return exit_code;
}

int bind_function_exit_command_mode(const struct Argument* argument) {
  menu.mode = 'n';

  command_line.mode = ' ';
  o_string_clear(&command_line.command);
  return 0;
}

int bind_function_command_line_cursor_move(const struct Argument* argument) {
  command_line_move_cursor(&command_line, argument->i);
  return 0;
}

int bind_function_command_line_delete_char(const struct Argument* argument) {
  return command_line_delete_char(&command_line);
}

int bind_function_command_line_execute(const struct Argument* argument) {
  menu.mode = 'n';
  return command_line_execute(&command_line);
}

int bind_function_menu_change_directory(const struct Argument* argument) {
  struct MenuItem** menu_items = menu_get_current_items(&menu);
  struct MenuItem* item = *(menu_items + menu.cursor);

  struct stat st;
  if(stat(item->contents, &st) != 0)
    return -1;

  if(st.st_mode & S_IFDIR) {
    if(chdir(item->contents) != 0)
      return -1;
    return tfm_change_directory(".");
  }

  return 0;
}

int bind_function_menu_cursor_move(const struct Argument* argument) {
  menu_move_cursor(&menu, argument->i);
  return 0;
}

int bind_function_menu_cursor_move_top(const struct Argument* argument) {
  menu.cursor = 0;
  return 0;
}

int bind_function_menu_cursor_move_bottom(const struct Argument* argument) {
  unsigned int menu_length = menu_get_current_items_length(&menu);
  menu.cursor = menu_length;
  return 0;
}

int bind_function_menu_switch_cursor_with_selection(const struct Argument* argument) {
  if(menu.mode != 'v')
    return 0;

  unsigned int buffer = menu.cursor;
  menu.cursor = menu.selection;
  menu.selection = buffer;

  return 0;
}

int bind_function_menu_toggle_visual_mode(const struct Argument* argument) {
  menu_toggle_select(&menu);
  
  return 0;
}
