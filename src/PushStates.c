#include "PushStates.h"

#include "comm/IPC.h"

#define textLine(stateObject, lineNumber) (((unsigned char *)(&(stateObject->text))) + (lineNumber * TEXT_LINE_SIZE))

bool ps_btnIdIsBtnPad(unsigned char btnId);
unsigned char ps_btnIdToBtnPadIdx(unsigned char btnId);
unsigned char ps_btnPadIdxToBtnId(unsigned char btnPadIdx);

//////////////////////////////
//  FUNCTION DECELERATIONS  //
//////////////////////////////

void PushStates_initStateObj(pushStateObject* ps)
{
    
  memset(ps, 0, sizeof(pushStateObject));

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
    ps->buttonPadStates[i].id = ps_btnPadIdxToBtnId(i);
  }

  memset(ps->text, 0x20, TEXT_BUFFER_SIZE);
}

void PushStates_setPadState(pushStateObject* ps, int x, int y, unsigned char color, unsigned char blinkState)
{
  if(x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    ps->padStates[x + (y * 8)].color = color;
    ps->padStates[x + (y * 8)].blinkState = blinkState;
  }
}

void PushStates_setPadColor(pushStateObject* ps, int x, int y, unsigned char color)
{
  if(x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    ps->padStates[x + (y * 8)].color = color;
  }
}
void PushStates_setPadBlink(pushStateObject* ps, int x, int y, unsigned char blinkState)
{
  if(x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    ps->padStates[x + (y * 8)].blinkState = blinkState;
  }
}
void PushStates_setButtonBlink(pushStateObject* ps, unsigned char id, unsigned char blinkState)
{
  if(id > 0 && id < 120)
  {
    ps->buttonStates[id].blinkState = blinkState;
  }
}
void PushStates_setButtonPadState(pushStateObject* ps, unsigned char id, unsigned char color, unsigned char blinkState)
{
  if(ps_btnIdIsBtnPad(id))
  {
    unsigned char idx = ps_btnIdToBtnPadIdx(id);
    ps->buttonPadStates[idx].color = color;
    ps->buttonPadStates[idx].blinkState = blinkState;
  }
}
void PushStates_setButtonPadColor(pushStateObject* ps, unsigned char id, unsigned char color)
{
    if(ps_btnIdIsBtnPad(id))
    {
        unsigned char idx = ps_btnIdToBtnPadIdx(id);
        ps->buttonPadStates[idx].color = color;
    }
}
void PushStates_setButtonPadBlink(pushStateObject* ps, unsigned char id, unsigned char blinkState)
{
    if(ps_btnIdIsBtnPad(id))
    {
      unsigned char idx = ps_btnIdToBtnPadIdx(id);
      ps->buttonPadStates[idx].blinkState = blinkState;
    }
}
void PushStates_setText(pushStateObject* ps, int x, int y, char * text, unsigned int length)
{
    if(x >= 0 && x < 68 && y >= 0 && y < 4)
    {
        memcpy(textLine((ps), y) + x, text, x + length <= 68 ? length : 68 - x);
    }
}

bool ps_btnIdIsBtnPad(unsigned char btnId)
{
  return (btnId >= 20 && btnId <= 27) || (btnId >= 102 && btnId <= 109);
}
unsigned char ps_btnIdToBtnPadIdx(unsigned char btnId)
{
  if (btnId >= 20 && btnId <= 27)
    return btnId - 20;

  if (btnId >= 102 && btnId <= 109)
    return (btnId - 102) + 8;

  return 0;
}

unsigned char ps_btnPadIdxToBtnId(unsigned char btnPadIdx)
{
  if (btnPadIdx < 8)
    return btnPadIdx + 20;

  if (btnPadIdx < 16)
    return (btnPadIdx - 8) + 102;
  
  return 0;
}