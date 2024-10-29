#define INCLUDE_CONFIG_BINDINGS
#include <config.h>
#include <macros.h>
#include <tfm.h>
#include <utils.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct dirent** tfm_dirents = NULL;
unsigned int tfm_dirents_length = 0;

struct CommandLine command_line;

struct Menu menu;
struct MenuItem* tfm_menu_items = NULL;

int tfm_change_directory(const char* path) {
  if(tfm_dirents != NULL) {
    for(unsigned int i = 0; i < tfm_dirents_length; i ++) {
      free(tfm_dirents[i]);
      tfm_dirents[i] = NULL;
    }
    free(tfm_dirents);
    tfm_dirents = NULL;
  }
  if(tfm_menu_items != NULL) {
    free(tfm_menu_items);
    tfm_menu_items = NULL;
  }

  if(get_dirents(path, &tfm_dirents, &tfm_dirents_length) != 0)
    goto exit_failure;

  if(dirents_to_menu_items(tfm_dirents, tfm_dirents_length, &tfm_menu_items) != 0)
    goto free_tfm_dirents;

  menu_free_items(&menu);
  if(menu_set_items(&menu, tfm_menu_items, tfm_dirents_length) != 0)
    goto free_tfm_menu_items;

  return 0;

free_tfm_menu_items:
  menu_free_items(&menu);

free_tfm_dirents:
  for(unsigned int i = 0; i < tfm_dirents_length; i ++) { 
    free(tfm_dirents[i]);
    tfm_dirents[i] = NULL;
  }
  free(tfm_dirents);
  tfm_dirents = NULL;
exit_failure:
  return -1;
}

int tfm(void) {
  const char* error= NULL;

  tb_init();

  command_line.x = 0;
  command_line.foreground = TB_WHITE;
  command_line.foreground_reversed = TB_BLACK;
  command_line.background = TB_BLACK;
  command_line.background_reversed = TB_WHITE;

  menu.x = 0;
  menu.y = 0;
  menu.background = TB_BLACK;
  menu.background_reversed = TB_WHITE;
  tfm_handle_resize();

  if(menu_init(&menu) != 0) {
    error = "menu_init";
    goto tb_shutdown;
  }

  if(tfm_change_directory(".") != 0) {
    error = "tfm_change_directory";
    goto menu_free;
  }

  if(command_line_init(&command_line) != 0) {
    error = "command_line_init";
    goto menu_free;
  }

  {
    unsigned int largest_stroke = 0;
    for(unsigned int i = 0; i < ARRAY_LENGTH(strokes); i ++)
      largest_stroke = MAX(largest_stroke, strlen(strokes[i].chars));

    o_string_reserve(&menu.strokes, largest_stroke);
  }

  while(true) {
    command_line_draw(&command_line);
    menu_draw(&menu);
    tb_present();

    struct tb_event event;
    tb_poll_event(&event);

    if(event.ch == 'q')
      break;

    if(tfm_handle_events(&event) != 0)
      break;
  }

  menu_free(&menu);
  tb_shutdown();

  return 0;

menu_free:
  menu_free(&menu);
tb_shutdown:
  tb_shutdown();
  if(error != NULL)
    perror(error);

  return -1;
}

int tfm_handle_events(struct tb_event* event) {
  if(event->type == TB_EVENT_RESIZE)
    tfm_handle_resize();

  if(event->ch < UCHAR_MAX) {
    char cat[2];
    cat[0] = event->ch;
    cat[1] = '\0';
    if(o_string_cat(&menu.strokes, cat) != O_SUCCESS)
      return -1;

    if(menu.mode == ':')
      if(command_line_add_char(&command_line, event->ch) != 0)
        return -1;
  }

  bool clear_stroke = false;
  bool matches_stroke = false;
  for(unsigned int i = 0; i < ARRAY_LENGTH(strokes); i ++)
    if(strokes[i].mode == ' ' || menu.mode == strokes[i].mode) {
      if(strncmp(menu.strokes.contents, strokes[i].chars, strlen(menu.strokes.contents)) == 0)
        matches_stroke = true;

      if(strcmp(menu.strokes.contents, strokes[i].chars) == 0) {
        if(strokes[i].function(&menu, &command_line, &strokes[i].argument) != 0)
          return -1;
        clear_stroke = true;
      }
    }

  if(!matches_stroke)
    clear_stroke = true;

  if(clear_stroke)
    o_string_clear(&menu.strokes);

  return 0;
}

void tfm_handle_resize(void) {
  int terminal_width = tb_width();
  int terminal_height = tb_height();

  unsigned int width = terminal_width > 0 ? terminal_width : 0;

  command_line.y = terminal_height > 0 ? terminal_height - 1 : 0;
  command_line.width = width;

  menu.width = width;
  menu.height = terminal_height > 0 ? terminal_height - 1 : 0;
}
