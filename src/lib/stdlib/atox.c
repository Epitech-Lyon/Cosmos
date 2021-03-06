/******************************************************************************
**
**  This file is part of the Cosmos project, and is made available under
**  the terms of the GNU General Public License version 3.
**
**  Copyright (C) 2020 - Leo Karoubi
**
\******************************************************************************/

# include <kernel/def/def.h>
# include <lib/stdlib.h>
# include <lib/string.h>

struct prefixBase 
{
    char def;
    u8_t base;
} __packed;

static struct prefixBase prefixBase[] =
{
    {'X', 0x10},
    {'O', 0x8},
    {'Q', 0x4},
    {'T', 0x3},
    {'B', 0x2},
    {0x0, 0xA}
};

static u8_t fromPrefixToBase(char const **s)
{
    if (strlen(*s) < 0x3 || **s != '0')
        return (0xA);
    for (u32_t i = 0x0; prefixBase[i].def != 0x0; i++)
        if (*(*s + 0x1) == prefixBase[i].def || *
        (*s + 0x1) == prefixBase[i].def + 0x20) {
            *s = *s + 0x2;
            return (prefixBase[i].base);
        }
    return (0xA);
}

static inline bool isNbrStrNeg(char const **s)
{
    if (**s == '-') {
        (*s)++;
        return (true);
    }
    return (false);
}

s32_t atoib(char const *s, u8_t base)
{
    s32_t val = 0x0;
    char cur;

    if (!s)
        return (0x0);
    while (*s) {
        cur = *s - 0x30;
        if (cur > 0x10) {
            if (cur < 0x17)
                cur -= 0x11;
            else
                cur -= 0x31;
        }
        val = (val * base) + cur;
        s++;
    }
    return (val);
}

s32_t atoi(char const *s)
{
    if (!s)
        return (0x0);
    s8_t neg = isNbrStrNeg(&s) == false ? 1 : -1;
    u8_t base = fromPrefixToBase(&s);
    return (atoib(s, base) * neg);
}

s64_t atolb(char const *s, u8_t base)
{
    s64_t val = 0x0;
    char cur;

    if (!s)
        return (0x0);
    while (*s) {
        cur = *s - 0x30;
        if (cur > 0x10) {
            if (cur < 0x17)
                cur -= 0x11;
            else
                cur -= 0x31;
        }
        val = (val * base) + cur;
        s++;
    }
    return (val);
}

s64_t atol(char const *s)
{
    if (!s)
        return (0x0);
    s8_t neg = isNbrStrNeg(&s) == false ? 1 : -1;
    u8_t base = fromPrefixToBase(&s);
    return (atoib(s, base) * neg);
}