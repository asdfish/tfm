extern "C" {
  #include <menu.h>
  #include <utils.h>
}

int change_directory(struct Menu* menu, const char* path);
void handle_movement(struct Menu* menu, struct tb_event* event);
void handle_resize(struct Menu* menu);

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
  menu_init(&menu);

  change_directory(&menu, ".");

  char mode = 'n';

  while(true) {
    menu_draw(&menu);
    tb_present();

    struct tb_event event;
    tb_poll_event(&event);

    if(event.ch == 'q')
      break;
  }

  tb_shutdown();

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

void handle_resize(struct Menu* menu) {
  int terminal_width = tb_width();
  int terminal_height = tb_height();

  menu->width = terminal_width > 0 ? terminal_width : 0;
  menu->height = terminal_height > 0 ? terminal_height : 0;
}
