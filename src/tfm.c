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

struct MenuItem* tfm_menu_items = NULL;

int tfm_change_directory(struct Menu* menu, const char* path) {
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

  if(directory_dirents(path, &tfm_dirents, &tfm_dirents_length) != 0)
    goto exit_failure;

  if(create_menu_items(tfm_dirents, tfm_dirents_length, &tfm_menu_items) != 0)
    goto free_tfm_dirents;

  menu_free_items(menu);
  if(menu_set_items(menu, tfm_menu_items, tfm_dirents_length) != 0)
    goto free_tfm_menu_items;

  return 0;

free_tfm_menu_items:
  menu_free_items(menu);

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

  if(tfm_change_directory(&menu, ".") != 0) {
    tb_shutdown();
    perror("tfm_change_directory");
    return -1;
  }

  {
    unsigned int largest_stroke = 0;
    for(unsigned int i = 0; i < ARRAY_LENGTH(bindings); i ++)
      largest_stroke = MAX(largest_stroke, strlen(bindings[i].strokes));

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

