#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LightAndTextStates.h"

#include "PushStateObjects.h"

///////////////////////
//  PUBLIC FUNCTIONS //
///////////////////////

char initPushStateObject(PushStateObject * ps)
{
  memset(ps, 0, sizeof(PushStateObject));

  memset(&(ps->padStates), 0, sizeof(outputPadState) * 64);

  outputPadState * pad;
  for(int y = 0; y < 8; y++)
  {
    for(int x = 0; x < 8; x++)
    {
      pad = &(ps->padStates[x + (y * 8)]);
      pad->id = x + (y * 8);
      pad->x = x;
      pad->y = y;
      pad->blinkState = BlinkStates_BlinkOff;
      pad->color = ColorStates_BLACK;
      pad->status = 0x69;  //idk why, just let it be
    }
  }

  memset(&(ps->buttonStates), 0, sizeof(outputButtonState) * 120);
  for(int i = 0; i < 120; i++)
  {
    ps->buttonStates[i].id = i;
  }

  memset(&(ps->buttonPadStates), 0, sizeof(outputButtonState) * 120);
  for(int i = 0; i < 16; i++)
  {
    ps->buttonPadStates[i].id = i;
  }

  memset(ps->text, 0x20, TEXT_BUFFER_SIZE);

  return 1;
}
