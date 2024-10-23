/*#include <menu.h>*/
/*#include <stdio.h>*/
/**/
/*#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))*/
/**/
/*int main(void) {*/
/*  struct MenuItem menu_items[] = {*/
/*    { TB_WHITE, TB_BLACK, "lorem ipsum" },*/
/*    { TB_WHITE, TB_BLACK, "the quick brown fox jumps over the lazy dog" },*/
/*    { TB_WHITE, TB_BLACK, "asdakfsadfkjslsakdhjjflakshgjkahwejghawejkghalwh" },*/
/*  };*/
/**/
/*  struct Menu menu = {*/
/*    .x = 0, .y = 0, .width = 10, .height = 10,*/
/*    .cursor = 0, .background_reversed = TB_WHITE, .background = TB_BLACK,*/
/*  };*/
/**/
/*  menu_init(&menu);*/
/*  if(menu_change_items(&menu, menu_items, ARRAY_LENGTH(menu_items)) != 0)*/
/*    goto exit;*/
/**/
/*  menu_change_filtered_items(&menu, "um");*/
/*  menu_change_filtered_items(&menu, "");*/
/**/
/*  tb_init();*/
/**/
/*  while(true) {*/
/*    menu_draw(&menu);*/
/*    tb_present();*/
/**/
/*    struct tb_event event;*/
/*    tb_poll_event(&event);*/
/**/
/*    switch(event.ch) {*/
/*      case 'q':*/
/*        goto tb_shutdown;*/
/*      case 'k':*/
/*        menu_move_cursor(&menu, -1);*/
/*        break;*/
/*      case 'j':*/
/*        menu_move_cursor(&menu, 1);*/
/*        break;*/
/*      case ' ':*/
/*        menu_toggle_select(&menu);*/
/*        break;*/
/*    }*/
/*  }*/
/**/
/*tb_shutdown:*/
/*  menu_free(&menu);*/
/*  tb_shutdown();*/
/*exit:*/
/*  return 0;*/
/*}*/

#include <utils.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  struct dirent** dirents = NULL;
  unsigned int length = 0;

  if(directory_dirents(".", &dirents, &length) != 0)
    printf("a error occured\n");

  if(dirents) {
    for(unsigned int i = 0; i < length; i ++) {
      printf("%s\n", dirents[i]->d_name);
      free(dirents[i]);
      dirents[i] = NULL;
    }
    free(dirents);
    dirents = NULL;
  }
  return 0;
}
