#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include "item.h"

void Item__Red__INIT(Item* item, ...);
void Item__Green__INIT(Item* item, ...);
void Item__Purple__INIT(Item* item, ...);
void Item__Brick__INIT(Item* item, ...);

void Item__Block__USE(Item* item, ...);

#endif