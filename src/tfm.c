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
  tfm_handle_resize(&menu);

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

    if(tfm_handle_events(&menu, &event) != 0)
      break;
  }

  tb_shutdown();
  menu_free(&menu);

  return 0;
}

int tfm_handle_events(struct Menu* menu, struct tb_event* event) {
  if(event->type == TB_EVENT_RESIZE)
    tfm_handle_resize(menu);

  if(event->ch < UCHAR_MAX) {
    char cat[2];
    cat[0] = event->ch;
    cat[1] = '\0';
    if(o_string_cat(&menu->strokes, cat) != O_SUCCESS)
      return -1;
  }

  for(unsigned int i = 0; i < ARRAY_LENGTH(bindings); i ++)
    if((bindings[i].mode == ' ' || menu->mode == bindings[i].mode) && strcmp(menu->strokes.contents, bindings[i].strokes) == 0) {
      bindings[i].function(menu, &bindings[i].argument);
      o_string_clear(&menu->strokes);
    }

  return 0;
}

void tfm_handle_resize(struct Menu* menu) {
  int terminal_width = tb_width();
  int terminal_height = tb_height();

  menu->width = terminal_width > 0 ? terminal_width : 0;
  menu->height = terminal_height > 0 ? terminal_height : 0;
}
