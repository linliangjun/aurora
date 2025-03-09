/**
 * Copyright 2025 linliangjun
 */

#include "stdio.h"
#include "ctype.h"
#include "string.h"

struct format_flags
{
    unsigned char left : 1;    // 左对齐
    unsigned char plus : 1;    // 0 或正数显示正号
    unsigned char space : 1;   // 正号用空格代替
    unsigned char special : 1; // 带前缀
    unsigned char zeropad : 1; // 填充 0
    unsigned char sign : 1;    // 有符号
    unsigned char small : 1;   // 小写
};

static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

/**
 * 字符串（0-9）转换整数，并将指针前移
 */
static unsigned int skip_atoi(const char **s)
{
    unsigned int i = 0;
    while (isdigit(**s))
        i = i * 10 + *((*s)++) - '0';
    return i;
}

static char *number(char *buf,
                    unsigned int num,
                    unsigned char base,
                    int width,
                    int precision,
                    struct format_flags flags)
{
    char sign = 0;
    /* 如果是有符号数，且 num < 0，则 sign = '-'，并使 num 取绝对值 */
    if (flags.sign && (int)num < 0)
    {
        sign = '-';
        num = -(int)num;
    }
    /* 对于无符号数，或者是有符号的非负数，若要求带上正号，则 sign = '+' */
    else if (flags.plus)
    {
        sign = '+';
    }

    /* 将数字转换为字符串 */
    unsigned char sup, i = 0;
    char str[36];
    do
    {
        sup = num % base;
        str[i++] = digits[sup];
        num /= base;
    } while (num != 0);

    /* 设置精度 */
    if (precision == -1)
    {
        precision = i;
    }

    /* 如果有符号，则宽度减 1 */
    if (sign)
    {
        width--;
    }

    /* 如果有特殊符号，则宽度要减去 1（8 进制的 0） 或 2（16 进制的 0x） */
    if (flags.special)
    {
        if (base == 8)
        {
            width--;
        }
        else if (base == 16)
        {
            width -= 2;
        }
    }

    /* 宽度再减去精度 */
    width -= precision;

    /****************** 开始填充缓冲区 *******************/

    /* 如果是右对齐，且填空格，则填入相应数量的空格 */
    if (!flags.left && !flags.zeropad)
    {
        while (width-- > 0)
        {
            *buf++ = ' ';
        }
    }

    /* 如果需带符号位，则填入符号 */
    if (sign)
    {
        *buf++ = sign;
    }

    /* 如果有特殊符号，则填入特殊符号 */
    if (flags.special)
    {
        if (base == 8)
        {
            *buf++ = '0';
        }
        else if (base == 16)
        {
            *buf++ = '0';
            *buf++ = flags.small ? 'x' : 'X';
        }
    }

    /* 如果是右对齐，且填 0，则填入相应数量的 0 */
    if (!flags.left && flags.zeropad)
    {
        while (width-- > 0)
        {
            *buf++ = '0';
        }
    }

    /* 数字字符串长度小于精度，左侧填 0 */
    while (i < precision--)
    {
        *buf++ = '0';
    }

    /* 填充数字字符串 */
    while (i-- > 0)
    {
        *buf++ = flags.small ? str[i] : toupper(str[i]);
    }

    /* 如果宽度还不为 0（左对齐），则填空格 */
    while (width-- > 0)
    {
        *buf++ = ' ';
    }
    return buf;
}

unsigned int vsprintf(char *buf, const char *fmt, va_list args)
{
    char *buffer = buf;
    /* 遍历格式字符串 */
    while (*fmt)
    {
        /* 如果当前字符不是 %，则直接写入缓冲区，并进入下一次循环 */
        if (*fmt != '%')
        {
            *buf++ = *fmt++;
            continue;
        }

        /* 如果下一个字符还是 %，则写入 % 到缓冲区，并进入下一次循环 */
        if (*(fmt + 1) == '%')
        {
            *buf++ = '%';
            fmt += 2;
            continue;
        }

        /* 处理标志域，结果存放在 flags 中 */
        struct format_flags field_flags;
        memset(&field_flags, 0, sizeof(struct format_flags));
    repeat:
        fmt++;
        switch (*fmt)
        {
        case '-':
            field_flags.left = 1;
            goto repeat;
        case '+':
            field_flags.plus = 1;
            goto repeat;
        case ' ':
            field_flags.space = 1;
            goto repeat;
        case '#':
            field_flags.special = 1;
            goto repeat;
        case '0':
            field_flags.zeropad = 1;
            goto repeat;
        }

        /* 处理宽度域，结果存放在 field_width 中 */
        int field_width = -1;
        if (*fmt == '*')
        {
            field_width = va_arg(args, int);
            if (field_width < 0)
            {
                field_flags.left = 1;
                field_width = -field_width;
            }
            fmt++;
        }
        else if (isdigit(*fmt))
        {
            field_width = skip_atoi(&fmt);
        }

        /* 处理精度域，结果存放在 field_precision 中 */
        int field_precision = -1;
        if (*fmt == '.')
        {
            fmt++;
            if (*fmt == '*')
            {
                field_precision = va_arg(args, int);
                fmt++;
            }
            else
            {
                field_precision = skip_atoi(&fmt);
            }
            if (field_precision < 0)
            {
                field_precision = 0;
            }
        }

        /* 处理转换指示符 */
        switch (*fmt)
        {
        case 'c': // 字符
            if (!field_flags.left)
            {
                while (--field_width > 0)
                {
                    *buf++ = ' ';
                }
            }
            *buf++ = va_arg(args, char);
            while (--field_width > 0)
            {
                *buf++ = ' ';
            }
            break;
        case 's': // 字符串
            char *str = va_arg(args, char *);
            int len = strlen(str);
            if (field_precision == -1)
            {
                field_precision = len;
            }
            else if (field_precision < len)
            {
                len = field_precision;
            }
            if (!field_flags.left)
            {
                while (len < field_width--)
                {
                    *buf++ = ' ';
                }
            }
            for (int i = 0; i < len; ++i)
            {
                *buf++ = *str++;
            }
            while (len < field_width--)
            {
                *buf++ = ' ';
            }
            break;
        case 'b': // 二进制数
            unsigned int num = va_arg(args, unsigned int);
            buf = number(buf, num, 2, field_width, field_precision, field_flags);
            break;
        case 'o': // 八进制数
            num = va_arg(args, unsigned int);
            buf = number(buf, num, 8, field_width, field_precision, field_flags);
            break;
        case 'd':
        case 'i': // 有符号十进制数
            field_flags.sign = 1;
        case 'u': // 无符号十进制数
            num = va_arg(args, unsigned int);
            buf = number(buf, num, 10, field_width, field_precision, field_flags);
            break;
        case 'x': // 16 进制数，小写
            field_flags.small = 1;
        case 'X': // 16 进制数
            num = va_arg(args, unsigned int);
            buf = number(buf, num, 16, field_width, field_precision, field_flags);
            break;
        case 'p': // 指针
            if (field_width == -1)
            {
                field_width = 8;
                field_flags.zeropad = 1;
            }
            num = va_arg(args, unsigned int);
            buf = number(buf, num, 16, field_width, field_precision, field_flags);
            break;

        default:
            *buf++ = *fmt;
            break;
        }
        fmt++;
    }
    *buf++ = 0;
    return buf - buffer;
}
