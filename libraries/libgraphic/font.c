/* Copyright © 2018-2019 N. Van Bossuyt.                                      */
/* This code is licensed under the MIT License.                               */
/* See: LICENSE.md                                                            */

#include <libsystem/path.h>
#include <libgraphic/font.h>
#include <libsystem/cstring.h>
#include <libsystem/iostream.h>
#include <libsystem/logger.h>
#include <libsystem/error.h>

void font_delete(font_t *this);

glyph_t *font_load_glyph(const char *name)
{
    char glyph_path[PATH_LENGHT];
    snprintf(glyph_path, PATH_LENGHT, "/res/font/%s.glyph", name);

    iostream_t *glyph_file = iostream_open(glyph_path, IOSTREAM_READ);

    if (glyph_path == NULL)
    {
        logger_error("Failled to load glyph from %s: %s", glyph_path, error_to_string(error_get()));

        return NULL;
    }

    iostream_stat_t stat;
    iostream_fstat(glyph_file, &stat);
    if (stat.type == IOSTREAM_TYPE_REGULAR && stat.size == 0)
    {
        logger_error("Failled to load font glyph from %s: The glyph file isn't a regular file!", glyph_path);

        iostream_close(glyph_file);
        return NULL;
    }

    glyph_t *glyph = malloc(stat.size);
    iostream_read(glyph_file, glyph, stat.size);

    iostream_close(glyph_file);

    return glyph;
}

bitmap_t *font_load_bitmap(const char *name)
{
    char bitmap_path[PATH_LENGHT];
    snprintf(bitmap_path, PATH_LENGHT, "/res/font/%s.png", name);

    bitmap_t *bmp = bitmap_load_from(bitmap_path);

    if (bmp == NULL)
    {
        logger_error("Failled to load font bitmap from %s: %s", bitmap_path, error_to_string(error_get()));
        return NULL;
    }
    else
    {
        bmp->filtering = BITMAP_FILTERING_LINEAR;
        return bmp;
    }
}

font_t *font(const char *name)
{
    glyph_t *glyph = font_load_glyph(name);

    if (glyph == NULL)
    {
        return NULL;
    }

    bitmap_t *bmp = font_load_bitmap(name);

    if (bmp == NULL)
    {
        free(glyph);
        return NULL;
    }

    font_t *this = OBJECT(font);

    this->bitmap = bmp;
    this->glyph = glyph;
    this->default_glyph = *font_glyph(this, '?');

    return this;
}

void font_delete(font_t *this)
{
    free(this->glyph);
    object_release(this->bitmap);
}

glyph_t *font_glyph(font_t *this, int codepoint)
{
    for (int i = 0; this->glyph[i].codepoint != 0; i++)
    {
        if (this->glyph[i].codepoint == codepoint)
        {
            return &this->glyph[i];
        }
    }

    return &this->default_glyph;
}

int font_measure_width(font_t *this, float font_size, const char *str, int str_size)
{
    int width = 0;

    for (int i = 0; i < str_size; i++)
    {
        glyph_t *g = font_glyph(this, str[i]);
        width += g->advance * (font_size / 16.0);
    }

    return width;
}