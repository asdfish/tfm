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

void command_line_draw(struct CommandLine* command_line) {
  command_line_verify_cursor_position(command_line);

  unsigned int command_length = strlen(command_line->command.contents);

  if(command_line->camera > command_line->cursor)
    command_line->camera = command_line->cursor;
  else if(command_line->camera + command_length - 1 < command_line->cursor)
    command_line->camera = command_line->cursor - command_length + 1; 

  for(unsigned int i = 0; i < command_line->width; i ++) {
    unsigned int char_x = command_line->camera + i;

    uintattr_t foreground = command_line->foreground, background = command_line->background;

    if(char_x == command_line->cursor) {
      foreground = command_line->foreground_reversed;
      background = command_line->background_reversed;
    }

    char next_char = ' ';

    if(i >= command_length)
      goto draw_char;

    next_char = command_line->command.contents[char_x];

draw_char:
    tb_set_cell(i + command_line->x, command_line->y, next_char, foreground, background);
  }
}

int command_line_init(struct CommandLine* command_line) {
  int result = o_string_init(&command_line->command);
  if(result != O_SUCCESS)
    return result;

  command_line->camera = 0;
  command_line->cursor = 0;

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
