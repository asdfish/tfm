#ifndef TFM_H
#define TFM_H

#include <menu.h>

extern struct dirent** tfm_dirents;
extern unsigned int tfm_dirents_length;

extern struct MenuItem* tfm_menu_items;

extern int tfm(void);
extern int tfm_change_directory(struct Menu* menu, const char* path);
extern int tfm_handle_events(struct Menu* menu, struct tb_event* event);
extern void tfm_handle_resize(struct Menu* menu);

#endif
