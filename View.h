#pragma once

#include <stdio.h>
#include <windows.h>
#include "data_structures.h"

void getGameInput(int *c_v, int *c_h);
void animate(int c_v, int c_h, FieldProperties (*cards)[FIELD_SIZE]);
void printField(FieldProperties (*cards)[FIELD_SIZE]);
void printPart(int line, int state, int image);
