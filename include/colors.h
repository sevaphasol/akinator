#ifndef COLORS_H__
#define COLORS_H__

//————————————————————————————————————————————————//

#include <stdlib.h>

//————————————————————————————————————————————————//

const char* const WhiteColor     = "\033[0m";
const char* const RedColor       = "\033[31m";
const char* const GreenColor     = "\033[32m";
const char* const YellowColor    = "\033[33m";
const char* const BlueColor      = "\033[34m";
const char* const PurpleColor    = "\033[35m";
const char* const TurquoiseColor = "\033[36m";

//————————————————————————————————————————————————//

void  ColorPrint (const char* color_code, const char* str, ...);
void  SetColor   (const char* color_code);
void  ResetColor ();

//————————————————————————————————————————————————//

#endif // COLORS_H__
