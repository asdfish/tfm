#ifndef TFM_H
#define TFM_H
#include <menu.h>

extern struct dirent** tfm_dirents;
extern unsigned int tfm_dirents_length;

extern struct MenuItem* tfm_menu_items;

extern int tfm(void);
extern int tfm_change_directory(struct Menu* menu, const char* path);

#endif
