/**
 * Copyright 2025 linliangjun
 */

#include "string.h"
#include "ctype.h"
#include "math.h"

/**
 * 格式化标识符
 */
typedef struct
{
    bool left;    // 左对齐
    bool plus;    // 0 或正数显示正号
    bool space;   // 正号用空格代替
    bool special; // 带前缀
    bool zeropad; // 填充 0
    bool sign;    // 有符号
    bool small;   // 小写
} format_flags_t;

static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

static char *string(char *buf, const char *str, u32 width, i32 precision, bool left)
{
    // 计算出实际展示的字符串长度
    size_t len = strlen(str);
    if (precision >= 0)
        len = MIN(len, precision);

    // 计算出需要填充的空格数量
    size_t padding = width <= len ? 0 : width - len;

    /****************** 开始填充缓冲区 *******************/
    if (left)
    {
        memcpy(buf, str, len);
        buf += len;
        memset(buf, ' ', padding);
        buf += padding;
    }
    else
    {
        memset(buf, ' ', padding);
        buf += padding;
        memcpy(buf, str, len);
        buf += len;
    }
    return buf;
}

/**
 * 字符串（'0' - '9'）转换为无符号整数，并将指针前移
 */
static u32 skip_atoi(const char **s)
{
    u32 i = 0;
    while (isdigit(**s))
        i = i * 10 + *((*s)++) - '0';
    return i;
}

/**
 * 将指定的数字转换为字符串
 *
 * @param buf 输出缓冲区
 * @param num 数字
 * @param base 进制
 * @param width 宽度
 * @param precision 精度
 * @param flags 格式化标志
 * @return 返回指向缓冲区末尾的指针
 */
static char *number(char *buf, u32 num, u8 base, u32 width, i32 precision, format_flags_t flags)
{
    char sign = 0;

    // 如果是有符号数，且 num > I32_MAX，则 sign = '-'，并使 num 取绝对值
    if (flags.sign && num > I32_MAX)
    {
        sign = '-';
        num = ~num + 1;
    }

    // 对于无符号数，或者是有符号的非负数，若要求带上正号，则 sign = '+'
    else if (flags.plus)
        sign = '+';

    // 将数字转换为字符串
    u8 sup, i = 0;
    char str[36];
    do
    {
        sup = num % base;
        str[i++] = digits[sup];
        num /= base;
    } while (num);

    // 如果精度未指定，则使用当前位数做为精度
    if (precision < 0)
        precision = i;

    // 计算出需要填充的空格或 0 数量
    u32 padding = width <= precision ? 0 : width - precision;

    // 如果有符号，则减 1
    if (sign)
        padding--;

    // 如果有特殊符号，则要再减去 1（8 进制的 0） 或 2（16 进制的 0x）
    if (flags.special)
    {
        if (base == 8)
            padding--;
        else if (base == 16)
            padding -= 2;
    }

    // 如果向下溢出，则置为 0
    if (padding > I32_MAX)
        padding = 0;

    /****************** 开始填充缓冲区 *******************/
    // 如果是右对齐，且填空格，则填入相应数量的空格
    if (!flags.left && !flags.zeropad)
    {
        memset(buf, ' ', padding);
        buf += padding;
        padding = 0;
    }

    // 如果需带符号位，则填入符号
    if (sign)
        *buf++ = sign;

    // 如果有特殊符号，则填入特殊符号
    if (flags.special)
    {
        if (base == 8)
            *buf++ = '0';
        else if (base == 16)
        {
            *buf++ = '0';
            *buf++ = flags.small ? 'x' : 'X';
        }
    }

    // 如果是右对齐，且填 0，则填入相应数量的 0
    if (!flags.left && flags.zeropad)
    {
        memset(buf, '0', padding);
        buf += padding;
        padding = 0;
    }

    // 填入数字字符串，若长度小于精度，在左侧补 0
    u32 gap = i < precision ? precision - i : 0;
    memset(buf, '0', gap);
    buf += gap;
    while (i--)
        *buf++ = flags.small ? str[i] : toupper(str[i]);

    // 如果是右对齐，则填空格
    if (flags.left)
    {
        memset(buf, ' ', padding);
        buf += padding;
    }

    return buf;
}

size_t vsprintf(char *buf, const char *fmt, va_list args)
{
    char *old_buf = buf;
    while (*fmt)
    {
        // 如果当前字符不是 %，则直接写入缓冲区，并进入下一次循环
        if (*fmt != '%')
        {
            *buf++ = *fmt++;
            continue;
        }

        // 如果下一个字符还是 %，则写入 % 到缓冲区，并进入下一次循环
        if (*(fmt + 1) == '%')
        {
            *buf++ = '%';
            fmt += 2;
            continue;
        }

        // 标识
        format_flags_t flags;
        memset(&flags, 0, sizeof(format_flags_t));
    repeat:
        fmt++;
        switch (*fmt)
        {
        case '-': // 左对齐
            flags.left = true;
            goto repeat;
        case '+': // 显示正号
            flags.plus = true;
            goto repeat;
        case ' ': // 正号用空格代替
            flags.space = true;
            goto repeat;
        case '#': // 带前缀
            flags.special = true;
            goto repeat;
        case '0': // 填充 0
            flags.zeropad = true;
            goto repeat;
        }

        // 宽度，0 表示不限制宽度
        u32 width = 0;
        if (*fmt == '*')
        {
            fmt++;
            i32 w = va_arg(args, i32);

            // 负数表示左对齐，宽度取绝对值
            if (w < 0)
            {
                flags.left = true;
                width = ~(u32)w + 1;
            }
            else
                width = w;
        }
        else if (isdigit(*fmt))
            width = skip_atoi(&fmt);

        // 精度，负数表示不限制精度
        i32 precision = -1;
        if (*fmt == '.')
        {
            fmt++;
            if (*fmt == '*')
            {
                fmt++;
                precision = va_arg(args, i32);
            }
            else
                precision = skip_atoi(&fmt);
        }

        // 转换指示符
        switch (*fmt)
        {
        case 'c': // 字符
            char cstr[2] = {va_arg(args, char), 0};
            buf = string(buf, cstr, width, 1, flags.left); // 字符的精度固定为 1
            break;
        case 's': // 字符串
            char *str = va_arg(args, char *);
            buf = string(buf, str, width, precision, flags.left);
            break;
        case 'b': // 二进制数
            u32 num = va_arg(args, u32);
            buf = number(buf, num, 2, width, precision, flags);
            break;
        case 'o': // 八进制数
            num = va_arg(args, u32);
            buf = number(buf, num, 8, width, precision, flags);
            break;
        case 'd':
        case 'i': // 有符号十进制数
            flags.sign = true;
        case 'u': // 无符号十进制数
            num = va_arg(args, u32);
            buf = number(buf, num, 10, width, precision, flags);
            break;
        case 'x': // 16 进制数，小写
            flags.small = true;
        case 'X': // 16 进制数
            num = va_arg(args, u32);
            buf = number(buf, num, 16, width, precision, flags);
            break;
        case 'p': // 指针
            if (width == 0)
            {
                width = 8;
                flags.zeropad = true;
            }
            num = va_arg(args, u32);
            buf = number(buf, num, 16, width, precision, flags);
            break;
        default:
            *buf++ = *fmt;
            break;
        }
        fmt++;
    }
    *buf++ = 0;
    return buf - old_buf;
}
