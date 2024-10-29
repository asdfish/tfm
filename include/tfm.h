#ifndef TFM_H
#define TFM_H

#include <menu.h>
#include <stdbool.h>

extern struct dirent** tfm_dirents;
extern unsigned int tfm_dirents_length;

extern struct CommandLine command_line;

extern struct Menu menu;
extern struct MenuItem* tfm_menu_items;

extern bool run;

extern int tfm(void);
extern int tfm_change_directory(const char* path);
extern int tfm_handle_events(struct tb_event* event);
extern void tfm_handle_resize(void);

#endif
