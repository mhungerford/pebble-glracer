#pragma once

#include "common.h"

struct racer_s {
  point position;
  void (*draw)(struct racer_s*);
};

//external singleton
extern struct racer_s racer;
