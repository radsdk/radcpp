cmake_minimum_required(VERSION 3.10)

project(stb
    VERSION 0.0.0.0
    DESCRIPTION "single-file public domain (or MIT licensed) libraries for C/C++"
    HOMEPAGE_URL "https://github.com/nothings/stb"
    LANGUAGES C CXX
)

set(stb_header_files
    stb_connected_components.h
    stb_c_lexer.h
    stb_divide.h
    stb_ds.h
    stb_dxt.h
    stb_easy_font.h
    stb_herringbone_wang_tile.h
    stb_hexwave.h
    stb_image.h
    stb_image_resize.h
    stb_image_write.h
    stb_include.h
    stb_leakcheck.h
    stb_perlin.h
    stb_rect_pack.h
    stb_sprintf.h
    stb_textedit.h
    stb_tilemap_editor.h
    stb_truetype.h
    stb_voxel_render.h
)

set(stb_source_files
    ${stb_header_files}
    stb_image.c
    stb_vorbis.c
)

add_library(stb
    ${stb_source_files}
)
