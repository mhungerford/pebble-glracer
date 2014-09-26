#pragma once

#include "common.h"

struct invader_s {
  point position;
  void (*draw)(struct invader_s*);
  void (*move)(struct invader_s*, point);
};

void invader_init(struct invader_s *inv);

