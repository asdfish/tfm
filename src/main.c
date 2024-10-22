#include <menu.h>
#include <stdio.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

int main(void) {
  struct MenuItem menu_items[] = {
    { TB_WHITE, TB_BLACK, "asdf" },
    { TB_WHITE, TB_BLACK, "asdf" },
    { TB_WHITE, TB_BLACK, "asdf" },
    { TB_WHITE, TB_BLACK, "asdf" },
  };

  struct Menu menu = {
    .x = 0, .y = 0, .width = 10, .height = 10,
    .cursor = 0, .background_reversed = TB_WHITE, .background = TB_BLACK,
  };

  menu_init(&menu);
  if(menu_set_items(&menu, menu_items, ARRAY_LENGTH(menu_items)) != 0)
    goto exit;

  tb_init();

  while(true) {
    menu_draw(&menu);
    tb_present();

    struct tb_event event;
    tb_poll_event(&event);

    switch(event.ch) {
      case 'q':
        goto tb_shutdown;
      case 'k':
        menu_move_cursor(&menu, -1);
        break;
      case 'j':
        menu_move_cursor(&menu, 1);
        break;
      case ' ':
        menu_toggle_select(&menu);
    }
  }

  menu_uninit(&menu);

tb_shutdown:
  tb_shutdown();
exit:
  return 0;
}
