#define INCLUDE_CONFIG_BINDINGS
#include <config.h>
#include <macros.h>
#include <menu.h>
#include <utils.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int change_directory(struct Menu* menu, const char* path);

struct dirent** dirents = NULL;
unsigned int dirents_length = 0;

struct MenuItem* menu_items = NULL;

int main(void) {
  tb_init();

  struct Menu menu;
  menu.x = 0;
  menu.y = 0;
  menu.background = TB_BLACK;
  menu.background_reversed = TB_WHITE;
  handle_resize(&menu);

  if(menu_init(&menu) != 0) {
    tb_shutdown();
    perror("menu_init");
    return -1;
  }

  if(change_directory(&menu, ".") != 0) {
    tb_shutdown();
    perror("change_directory");
    return -1;
  }

  {
    unsigned int largest_stroke = 0;
    for(unsigned int i = 0; i < ARRAY_LENGTH(bindings); i ++)
      unsigned int stroke_length = MAX(largest_stroke, strlen(bindings[i].strokes));

    o_string_reserve(&menu.strokes, largest_stroke);
  }


  while(true) {
    menu_draw(&menu);
    tb_present();

    struct tb_event event;
    tb_poll_event(&event);

    if(event.ch == 'q')
      break;

    if(handle_events(&menu, &event) != 0)
      break;
  }

  tb_shutdown();
  menu_free(&menu);

  return 0;
}

int change_directory(struct Menu* menu, const char* path) {
  if(dirents != NULL) {
    for(unsigned int i = 0; i < dirents_length; i ++) {
      free(dirents[i]);
      dirents[i] = NULL;
    }
    free(dirents);
    dirents = NULL;
  }
  if(menu_items != NULL) {
    free(menu_items);
    menu_items = NULL;
  }

  if(directory_dirents(path, &dirents, &dirents_length) != 0)
    goto exit_failure;

  if(create_menu_items(dirents, dirents_length, &menu_items) != 0)
    goto free_dirents;

  menu_free_items(menu);
  if(menu_set_items(menu, menu_items, dirents_length) != 0)
    goto free_menu_items;

  return 0;

free_menu_items:
  menu_free_items(menu);

free_dirents:
  for(unsigned int i = 0; i < dirents_length; i ++) { 
    free(dirents[i]);
    dirents[i] = NULL;
  }
  free(dirents);
  dirents = NULL;
exit_failure:
  return -1;
}
