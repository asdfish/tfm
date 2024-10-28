#include <command_line.h>

int command_line_add_char(struct CommandLine* command_line, char new_char) {
  char insert[2] = " \0";
  insert[0] = new_char;

  command_line_verify_cursor_position(command_line);
  int result = o_string_insert(&command_line->command, command_line->cursor, insert);
  if(result != O_SUCCESS)
    return -1;

  command_line_move_cursor(command_line, 1);

  return 0;
}

int command_line_delete_char(struct CommandLine* command_line) {
  if(command_line->cursor > 0) {
    int result = o_string_delete(&command_line->command, command_line->cursor - 1, 1);
    if(result != O_SUCCESS)
      return result;
  }

  return 0;
}

void command_line_draw(struct CommandLine* command_line) {
  if(command_line->mode == ' ') {
    for(unsigned int i = 0; i < command_line->width; i ++) {
      char next_char = ' ';

      if(command_line->message != NULL && i < strlen(command_line->message))
        next_char = command_line->message[i];

      tb_set_cell(command_line->x + i, command_line->y, next_char, command_line->foreground, command_line->background);
    }

    return;
  }

  command_line_verify_cursor_position(command_line);

  unsigned int command_length = strlen(command_line->command.contents);

  if(command_line->camera > command_line->cursor)
    command_line->camera = command_line->cursor;
  else if(command_line->camera + command_line->width - 1 < command_line->cursor)
    command_line->camera = command_line->cursor - command_line->width + 1;

  for(unsigned int i = 0; i < (command_line->width == 0 ? 0 : command_line->width - 1); i ++) {
    unsigned int char_x = command_line->camera + i;

    uintattr_t foreground = command_line->foreground;
    uintattr_t background = command_line->background;

    if(char_x == command_line->cursor) {
      foreground = command_line->foreground_reversed;
      background = command_line->background_reversed;
    }

    char next_char = ' ';

    if(char_x < command_length)
      next_char = command_line->command.contents[char_x];

    tb_set_cell(command_line->x + i + 1, command_line->y, next_char, foreground, background);
  }

  tb_set_cell(command_line->x, command_line->y, command_line->mode, command_line->foreground, command_line->background);
}

int command_line_init(struct CommandLine* command_line) {
  int result = o_string_init(&command_line->command);
  if(result != O_SUCCESS)
    return result;

  command_line->camera = 0;
  command_line->cursor = 0;

  command_line->message = NULL;
  command_line->mode = ':';

  return 0;
}

void command_line_move_cursor(struct CommandLine* command_line, int step) {
  if((int) command_line->cursor + step > 0)
    command_line->cursor += step;
  else
    command_line->cursor = 0;

  command_line_verify_cursor_position(command_line);
}

void command_line_uninit(struct CommandLine* command_line) {
  o_string_uninit(&command_line->command);
}

void command_line_verify_cursor_position(struct CommandLine* command_line) {
  unsigned int command_contents_length = strlen(command_line->command.contents);

  if(command_line->cursor > command_contents_length)
    command_line->cursor = command_contents_length;
}
