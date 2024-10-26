#ifndef CONFIG_H
#define CONFIG_H

#ifdef INCLUDE_FOREGROUNDS
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <menu.h>
#include <termbox2.h>
#include <dirent.h>

#ifdef __cplusplus
}
#endif // __cplusplus

static const uintattr_t foregrounds[] = {
  [DT_UNKNOWN] = TB_WHITE,
  [DT_FIFO] = TB_WHITE,
  [DT_CHR] = TB_WHITE,
  [DT_DIR] = TB_BLUE,
  [DT_BLK] = TB_WHITE,
  [DT_REG] = TB_WHITE,
  [DT_LNK] = TB_GREEN,
};

static const uintattr_t foregrounds_reversed[] = {
  [DT_UNKNOWN] = TB_BLACK,
  [DT_FIFO] = TB_BLACK,
  [DT_CHR] = TB_BLACK,
  [DT_DIR] = TB_GREEN,
  [DT_BLK] = TB_BLACK,
  [DT_REG] = TB_BLACK,
  [DT_LNK] = TB_BLUE,
};
#endif // INCLUDE_FOREGROUNDS

#endif
