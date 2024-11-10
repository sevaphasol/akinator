#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//------------------------------------------------//

#include "colors.h"
#include "custom_assert.h"

//————————————————————————————————————————————————//

void ColorPrintf(const char* color_code, const char *str, ...)
{
    ASSERT(color_code);
    ASSERT(str);

    //------------------------------------------------//

	va_list list;
    va_start(list, str);

    //------------------------------------------------//

	SetColor(color_code);
    vprintf(str, list);
	ResetColor();

    //------------------------------------------------//

    va_end(list);
}

//================================================//

void SetColor(const char* color_code)
{
    ASSERT(color_code);

    //------------------------------------------------//

	printf("%s", color_code);
}

//================================================//

void ResetColor()
{
	printf(WhiteColor);
}

//================================================//

int GetShortAnsColored(const char* color_code, const char* str, ...)
{
    ASSERT(color_code);
    ASSERT(str);

    //------------------------------------------------//

	va_list list;
    va_start(list, str);

    //------------------------------------------------//

	SetColor(color_code);
    vprintf(str, list);
	ResetColor();

    //------------------------------------------------//

    va_end(list);

    //------------------------------------------------//

    int ans = getchar();
    getchar();

    //------------------------------------------------//

    return ans;
}

//================================================//

char* GetLongAnsColored(const char* color_code, const char* str, ...)
{
    ASSERT(color_code);
    ASSERT(str);

    //------------------------------------------------//

	va_list list;
    va_start(list, str);

    //------------------------------------------------//

	SetColor(color_code);
    vprintf(str, list);
	ResetColor();

    //------------------------------------------------//

    va_end(list);

    //------------------------------------------------//

    char* ret_ans;
    scanf("%m[^\n]", &ret_ans);
    getchar();

    //------------------------------------------------//

    return ret_ans;
}

//————————————————————————————————————————————————//
