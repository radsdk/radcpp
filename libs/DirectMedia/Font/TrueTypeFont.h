#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/RefCounted.h"
#include "rad/Core/Span.h"
#include "rad/Core/String.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

namespace sdl
{

class TrueTypeLibrary;
class Surface;

class TrueTypeFont : public rad::RefCounted<TrueTypeFont>
{
public:
    TrueTypeFont(rad::Ref<TrueTypeLibrary> library);
    ~TrueTypeFont();

    TTF_Font* GetHandle() { return m_font; }

    bool Open(std::string_view filePath, int fontPointSize);
    void Close();

    int GetStyle();
    // TTF_STYLE_BOLD
    // TTF_STYLE_ITALIC
    // TTF_STYLE_UNDERLINE
    // TTF_STYLE_STRIKETHROUGH
    void SetStyle(int style);
    int GetOutlineSize();
    void SetOutlineSize(int sizeInPixels);
    int GetHinting();
    // TTF_HINTING_NORMAL
    // TTF_HINTING_LIGHT
    // TTF_HINTING_MONO
    // TTF_HINTING_NONE
    void SetHinting(int hinting);
    // @return 0(zero) if kerning is disabled. A non-zero value is returned when enabled.
    int GetKerning();
    void SetKerning(int allowed);
    // Get the maximum pixel height of all glyphs of the loaded font.
    int GetHeight();
    // Get the maximum pixel ascent of all glyphs of the loaded font.
    int GetAscent();
    // Get the maximum pixel descent of all glyphs of the loaded font.
    int GetDescent();
    // The maximum pixel height of all glyphs in the font.
    int GetLineSkip();
    // Get the number of faces ("sub-fonts") available in the loaded font.
    long GetFontFaceCount();

    bool IsFontFaceFixedWidth();
    const char* GetFontFaceFamilyName();
    const char* GetFontFaceStyleName();
    // Returns: the index of the glyph for ch in font, or 0 for an undefined character code.
    int HasGlyph(Uint16 ch);

    struct GlyphMetrices
    {
        int minx;
        int maxx;
        int miny;
        int maxy;
        int advance;
    };

    int GetGlyphMetrices(Uint16 ch, GlyphMetrices* metrices);
    // Calculate the resulting surface size of the LATIN1 encoded text rendered using font.
    int GetTextSizeLatin1(const char* text, int* w, int* h);
    // Calculate the resulting surface size of the UTF8 encoded text rendered using font.
    int GetTextSizeUTF8(const char* text, int* w, int* h);
    int GetTextSizeUTF16(const Uint16* text, int* w, int* h);

    // Render
    // There are three modes of rendering:
    // Solid: Quick and Dirty
    // Shaded: Slow and Nice, but with a Solid Box
    // Blended: Slow Slow Slow, but Ultra Nice over another image

    rad::Ref<Surface> RenderLatin1Solid(const char* text, SDL_Color color);
    rad::Ref<Surface> RenderUTF8Solid(const char* text, SDL_Color color);
    rad::Ref<Surface> RenderUTF16Solid(const Uint16* text, SDL_Color color);
    rad::Ref<Surface> RenderGlyphSolid(Uint16 ch, SDL_Color color);

    rad::Ref<Surface> RenderLatin1Shaded(const char* text, SDL_Color fg, SDL_Color bg);
    rad::Ref<Surface> RenderUTF8Shaded(const char* text, SDL_Color fg, SDL_Color bg);
    rad::Ref<Surface> RenderUTF16Shaded(const Uint16* text, SDL_Color fg, SDL_Color bg);
    rad::Ref<Surface> RenderGlyphShaded(Uint16 ch, SDL_Color fg, SDL_Color bg);

    rad::Ref<Surface> RenderLatin1Blended(const char* text, SDL_Color fg);
    rad::Ref<Surface> RenderUTF8Blended(const char* text, SDL_Color fg);
    rad::Ref<Surface> RenderUTF16Blended(const Uint16* text, SDL_Color fg);
    rad::Ref<Surface> RenderGlyphBlended(Uint16 ch, SDL_Color fg);

private:
    rad::Ref<TrueTypeLibrary> m_library;
    TTF_Font* m_font = nullptr;

}; // class TrueTypeFont

} // namespace sdl
