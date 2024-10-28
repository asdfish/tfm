#include <utils.h>
int main(void) {
  const char* sentence = "helloworld ";

  const char** words = NULL;
  unsigned int words_count = 0;

  sentence_separate(sentence, &words, &words_count);

  for(unsigned int i = 0; i < words_count; i ++) {
    printf("%s\n", words[i]);
    free((char*) words[i]);
  }
  free(words);

  return 0;
}

/*#include <tfm.h>*/
/**/
/*int main(void) {*/
/*  return tfm();*/
/*}*/
/**/
/*#include <command_line.h>*/
/*#include <limits.h>*/
/*int main(void) {*/
/*  tb_init();*/
/**/
/*  struct CommandLine command_line = {*/
/*    .x = 0, .y = 0, .width = 10,*/
/*    .foreground = TB_WHITE, .foreground_reversed = TB_BLACK,*/
/*    .background = TB_BLACK, .background_reversed = TB_WHITE,*/
/*  };*/
/**/
/*  if(command_line_init(&command_line) != 0)*/
/*    return -1;*/
/**/
/*  while(1) {*/
/*    command_line_draw(&command_line);*/
/*    tb_present();*/
/**/
/*    struct tb_event event;*/
/*    tb_poll_event(&event);*/
/**/
/*    if(event.ch == 'q')*/
/*      break;*/
/**/
/*    if(event.ch < CHAR_MAX)*/
/*      if(command_line_add_char(&command_line, (char) event.ch) != 0)*/
/*        break;*/
/**/
/*    if(event.key == TB_KEY_BACKSPACE2)*/
/*      command_line_delete_char(&command_line);*/
/*  }*/
/**/
/*  command_line_uninit(&command_line);*/
/*  tb_shutdown();*/
/**/
/*  return 0;*/
/*}*/
