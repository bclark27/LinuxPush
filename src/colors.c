
unsigned char hsb(float h, float s, float b){
  /*
  Convert a HSB to a Push Color
  param h: Hue from 0-1
  param s: Saturation from 0-1
  param b: Brightness from 0-1
  return: Push color that can be passed to pad_color functions
  */

  if(s < 0.2){  //grayscale
    return (unsigned char)(int)(4 * b);
  }
  h += (float)1 / (float)28;
  if(h > 1){
    h -= 1;
  }
  unsigned char color = (int)(h * 14) * 4 + 4;
  if(b < 0.5){
    color += 2;
    if(s < 0.5){
      color += 1;
    }
  } else if(s >= 0.5){
    color += 1;
  }
  return color;
}
