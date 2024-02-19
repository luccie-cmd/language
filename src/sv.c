#include "sv.h"

SVDEF String sv_from_parts(const char *data, size_t count)
{
    String sv;
    sv.count = count;
    sv.data = data;
    return sv;
}

SVDEF String sv_from_cstr(const char *cstr)
{
    return sv_from_parts(cstr, strlen(cstr));
}

SVDEF String sv_trim_left(String sv)
{
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[i])) {
        i += 1;
    }

    return sv_from_parts(sv.data + i, sv.count - i);
}

SVDEF String sv_trim_right(String sv)
{
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[sv.count - 1 - i])) {
        i += 1;
    }

    return sv_from_parts(sv.data, sv.count - i);
}

SVDEF String sv_trim(String sv)
{
    return sv_trim_right(sv_trim_left(sv));
}

SVDEF String sv_chop_left(String *sv, size_t n)
{
    if (n > sv->count) {
        n = sv->count;
    }

    String result = sv_from_parts(sv->data, n);

    sv->data  += n;
    sv->count -= n;

    return result;
}

SVDEF String sv_chop_right(String *sv, size_t n)
{
    if (n > sv->count) {
        n = sv->count;
    }

    String result = sv_from_parts(sv->data + sv->count - n, n);

    sv->count -= n;

    return result;
}

SVDEF bool sv_index_of(String sv, char c, size_t *index)
{
    size_t i = 0;
    while (i < sv.count && sv.data[i] != c) {
        i += 1;
    }

    if (i < sv.count) {
        if (index) {
            *index = i;
        }
        return true;
    } else {
        return false;
    }
}

SVDEF bool sv_try_chop_by_delim(String *sv, char delim, String *chunk)
{
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delim) {
        i += 1;
    }

    String result = sv_from_parts(sv->data, i);

    if (i < sv->count) {
        sv->count -= i + 1;
        sv->data  += i + 1;
        if (chunk) {
            *chunk = result;
        }
        return true;
    }

    return false;
}

SVDEF String sv_chop_by_delim(String *sv, char delim)
{
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delim) {
        i += 1;
    }

    String result = sv_from_parts(sv->data, i);

    if (i < sv->count) {
        sv->count -= i + 1;
        sv->data  += i + 1;
    } else {
        sv->count -= i;
        sv->data  += i;
    }

    return result;
}

SVDEF String sv_chop_by_sv(String *sv, String thicc_delim)
{
    String window = sv_from_parts(sv->data, thicc_delim.count);
    size_t i = 0;
    while (i + thicc_delim.count < sv->count
        && !(sv_eq(window, thicc_delim)))
    {
        i++;
        window.data++;
    }

    String result = sv_from_parts(sv->data, i);

    if (i + thicc_delim.count == sv->count) {
        // include last <thicc_delim.count> characters if they aren't
        //  equal to thicc_delim
        result.count += thicc_delim.count;
    }

    // Chop!
    sv->data  += i + thicc_delim.count;
    sv->count -= i + thicc_delim.count;

    return result;
}

SVDEF bool sv_starts_with(String sv, String expected_prefix)
{
    if (expected_prefix.count <= sv.count) {
        String actual_prefix = sv_from_parts(sv.data, expected_prefix.count);
        return sv_eq(expected_prefix, actual_prefix);
    }

    return false;
}

SVDEF bool sv_ends_with(String sv, String expected_suffix)
{
    if (expected_suffix.count <= sv.count) {
        String actual_suffix = sv_from_parts(sv.data + sv.count - expected_suffix.count, expected_suffix.count);
        return sv_eq(expected_suffix, actual_suffix);
    }

    return false;
}

SVDEF bool sv_eq(String a, String b)
{
    if (a.count != b.count) {
        return false;
    } else {
        return memcmp(a.data, b.data, a.count) == 0;
    }
}

SVDEF bool sv_eq_ignorecase(String a, String b)
{
    if (a.count != b.count) {
        return false;
    }

    char x, y;
    for (size_t i = 0; i < a.count; i++) {
        x = 'A' <= a.data[i] && a.data[i] <= 'Z'
              ? a.data[i] + 32
              : a.data[i];

        y = 'A' <= b.data[i] && b.data[i] <= 'Z'
              ? b.data[i] + 32
              : b.data[i];

        if (x != y) return false;
    }
    return true;
}

SVDEF uint64_t sv_to_u64(String sv)
{
    uint64_t result = 0;

    for (size_t i = 0; i < sv.count && isdigit(sv.data[i]); ++i) {
        result = result * 10 + (uint64_t) sv.data[i] - '0';
    }

    return result;
}

uint64_t sv_chop_u64(String *sv)
{
    uint64_t result = 0;
    while (sv->count > 0 && isdigit(*sv->data)) {
        result = result*10 + *sv->data - '0';
        sv->count -= 1;
        sv->data += 1;
    }
    return result;
}

SVDEF String sv_chop_left_while(String *sv, bool (*predicate)(char x))
{
    size_t i = 0;
    while (i < sv->count && predicate(sv->data[i])) {
        i += 1;
    }
    return sv_chop_left(sv, i);
}

SVDEF String sv_take_left_while(String sv, bool (*predicate)(char x))
{
    size_t i = 0;
    while (i < sv.count && predicate(sv.data[i])) {
        i += 1;
    }
    return sv_from_parts(sv.data, i);
}