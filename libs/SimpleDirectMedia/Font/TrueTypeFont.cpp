#include "TrueTypeFont.h"
#include "TrueTypeLibrary.h"
#include "Core/Surface.h"
#include "rad/IO/Logging.h"

namespace sdl
{

TrueTypeFont::TrueTypeFont(rad::Ref<TrueTypeLibrary> library) :
    m_library(std::move(library))
{
}

TrueTypeFont::~TrueTypeFont()
{
}

bool TrueTypeFont::Open(std::string_view filePath, int fontPointSize)
{
    m_font = TTF_OpenFont(filePath.data(), fontPointSize);
    if (m_font)
    {
        LogGlobal(Info, "Font opened successfully: File=%s; PointSize=%d;",
            filePath.data(), fontPointSize);
        return true;
    }
    else
    {
        LogGlobal(Info, "Cannot open font: File=%s; PointSize=%d;",
            filePath.data(), fontPointSize);
        return false;
    }
}

void TrueTypeFont::Close()
{
    TTF_CloseFont(m_font);
}

int TrueTypeFont::GetStyle()
{
    return TTF_GetFontStyle(m_font);
}

void TrueTypeFont::SetStyle(int style)
{
    TTF_SetFontStyle(m_font, style);
}

int TrueTypeFont::GetOutlineSize()
{
    return TTF_GetFontOutline(m_font);
}

void TrueTypeFont::SetOutlineSize(int sizeInPixels)
{
    TTF_SetFontOutline(m_font, sizeInPixels);
}

int TrueTypeFont::GetHinting()
{
    return TTF_GetFontHinting(m_font);
}

void TrueTypeFont::SetHinting(int hinting)
{
    TTF_SetFontHinting(m_font, hinting);
}

int TrueTypeFont::GetKerning()
{
    return TTF_GetFontKerning(m_font);
}

void TrueTypeFont::SetKerning(int allowed)
{
    TTF_SetFontKerning(m_font, allowed);
}

int TrueTypeFont::GetHeight()
{
    return TTF_FontHeight(m_font);
}

int TrueTypeFont::GetAscent()
{
    return TTF_FontAscent(m_font);
}

int TrueTypeFont::GetDescent()
{
    return TTF_FontDescent(m_font);
}

int TrueTypeFont::GetLineSkip()
{
    return TTF_FontLineSkip(m_font);
}

long TrueTypeFont::GetFontFaceCount()
{
    return TTF_FontFaces(m_font);
}

bool TrueTypeFont::IsFontFaceFixedWidth()
{
    return TTF_FontFaceIsFixedWidth(m_font);
}

const char* TrueTypeFont::GetFontFaceFamilyName()
{
    return TTF_FontFaceFamilyName(m_font);
}

const char* TrueTypeFont::GetFontFaceStyleName()
{
    return TTF_FontFaceStyleName(m_font);
}

int TrueTypeFont::HasGlyph(Uint16 ch)
{
    return TTF_GlyphIsProvided(m_font, ch);
}

int TrueTypeFont::GetGlyphMetrices(Uint16 ch, GlyphMetrices* metrices)
{
    return TTF_GlyphMetrics(m_font, ch,
        &metrices->minx, &metrices->maxx, &metrices->miny, &metrices->maxy, &metrices->advance);
}

int TrueTypeFont::GetTextSizeLatin1(const char* text, int* w, int* h)
{
    return TTF_SizeText(m_font, text, w, h);
}

int TrueTypeFont::GetTextSizeUTF8(const char* text, int* w, int* h)
{
    return TTF_SizeUTF8(m_font, text, w, h);
}

int TrueTypeFont::GetTextSizeUTF16(const Uint16* text, int* w, int* h)
{
    return TTF_SizeUNICODE(m_font, text, w, h);
}

rad::Ref<Surface> TrueTypeFont::RenderLatin1Solid(const char* text, SDL_Color color)
{
    SDL_Surface* surfaceHandle = TTF_RenderText_Solid(m_font, text, color);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderText_Solid failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderUTF8Solid(const char* text, SDL_Color color)
{
    SDL_Surface* surfaceHandle = TTF_RenderUTF8_Solid(m_font, text, color);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderUTF8_Solid failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderUTF16Solid(const Uint16* text, SDL_Color color)
{
    SDL_Surface* surfaceHandle = TTF_RenderUNICODE_Solid(m_font, text, color);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderUNICODE_Solid failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderGlyphSolid(Uint16 ch, SDL_Color color)
{
    SDL_Surface* surfaceHandle = TTF_RenderGlyph_Solid(m_font, ch, color);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderGlyph_Solid failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderLatin1Shaded(const char* text, SDL_Color fg, SDL_Color bg)
{
    SDL_Surface* surfaceHandle = TTF_RenderText_Shaded(m_font, text, fg, bg);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderText_Shaded failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderUTF8Shaded(const char* text, SDL_Color fg, SDL_Color bg)
{
    SDL_Surface* surfaceHandle = TTF_RenderUTF8_Shaded(m_font, text, fg, bg);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderUTF8_Shaded failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderUTF16Shaded(const Uint16* text, SDL_Color fg, SDL_Color bg)
{
    SDL_Surface* surfaceHandle = TTF_RenderUNICODE_Shaded(m_font, text, fg, bg);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderUNICODE_Shaded failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderGlyphShaded(Uint16 ch, SDL_Color fg, SDL_Color bg)
{
    SDL_Surface* surfaceHandle = TTF_RenderGlyph_Shaded(m_font, ch, fg, bg);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderGlyph_Shaded failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderLatin1Blended(const char* text, SDL_Color fg)
{
    SDL_Surface* surfaceHandle = TTF_RenderText_Blended(m_font, text, fg);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderText_Blended failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderUTF8Blended(const char* text, SDL_Color fg)
{
    SDL_Surface* surfaceHandle = TTF_RenderUTF8_Blended(m_font, text, fg);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderUTF8_Blended failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderUTF16Blended(const Uint16* text, SDL_Color fg)
{
    SDL_Surface* surfaceHandle = TTF_RenderUNICODE_Blended(m_font, text, fg);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderUNICODE_Blended failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

rad::Ref<Surface> TrueTypeFont::RenderGlyphBlended(Uint16 ch, SDL_Color fg)
{
    SDL_Surface* surfaceHandle = TTF_RenderGlyph_Blended(m_font, ch, fg);
    if (!surfaceHandle)
    {
        LogGlobal(Error, "TTF_RenderGlyph_Blended failed: %s", TTF_GetError());
    }
    return new Surface(surfaceHandle);
}

} // namespace sdl
