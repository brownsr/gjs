/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
/*
 * Copyright (c) 2008  litl, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <string.h>

#include "glib.h"

#include <config.h>

typedef struct {
    void *key;
    void *value;
} StoreOneData;

static gboolean
get_first_one_predicate(void  *key,
                        void  *value,
                        void  *data)
{
    StoreOneData *sod = (StoreOneData *) data;

    sod->key = key;
    sod->value = value;

    /* found it! */
    return true;
}

bool
gjs_g_hash_table_steal_one(GHashTable *hash,
                           void      **key_p,
                           void      **value_p)
{
    StoreOneData sod;

    sod.key = NULL;
    sod.value = NULL;
    g_hash_table_find(hash, get_first_one_predicate, &sod);

    if (sod.key == NULL)
        return false;

    if (key_p)
        *key_p = sod.key;
    if (value_p)
        *value_p = sod.value;

    g_hash_table_steal(hash, sod.key);

    return sod.value != NULL;
}

/** gjs_g_strv_concat:
 *
 * Concate an array of string arrays to one string array. The strings in each
 * array is copied to the resulting array.
 *
 * @strv_array: array of NULL-terminated arrays of strings. NULL elements are
 * allowed.
 * @len: number of arrays in @strv_array
 *
 * @return: a newly allocated NULL-terminated array of strings. Use
 * g_strfreev() to free it
 */
char**
gjs_g_strv_concat(char ***strv_array, int len)
{
    GPtrArray *array;
    int i;

    array = g_ptr_array_sized_new(16);

    for (i = 0; i < len; i++) {
        char **strv;
        int j;

        strv = strv_array[i];
        if (strv == NULL)
            continue;

        for (j = 0; strv[j] != NULL; ++j)
            g_ptr_array_add(array, g_strdup(strv[j]));
    }

    g_ptr_array_add(array, NULL);

    return (char**)g_ptr_array_free(array, false);
}
