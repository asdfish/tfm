#include <menu.h>

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
    .cursor = 0, .cursor_background = TB_WHITE, .normal_background = TB_BLACK,
    .items = menu_items, .items_length = ARRAY_LENGTH(menu_items),
  };

  menu_init(&menu);

  tb_init();

  menu_draw(&menu);
  tb_present();

  struct tb_event event;
  tb_poll_event(&event);

  tb_shutdown();
  return 0;
}
