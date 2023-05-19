#pragma once
#include "CGeneratorTexture.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <vector>

///																									
///	FONT GENERATOR																				
///																									
class CGeneratorFont : public AFont, public TProducedFrom<MContent> {
   REFLECT(CGeneratorFont);
public:
   CGeneratorFont(MContent*);
   CGeneratorFont(CGeneratorFont&&) noexcept = default;
   CGeneratorFont& operator = (CGeneratorFont&&) noexcept = default;
   ~CGeneratorFont();

public:
   PC_VERB(Create);

   void Generate() override;
   void LoadTTF(AFile&);
   void LoadPCF(AFile&);
   void WritePCF(AFile&) const;

protected:
   void InitializeFreeType();
   void GetGlyphMetrics();
   void GenerateGlyphBitmaps();
   void GetKernings();

protected:
   static FT_Library mFtHandle;
   static pcref mFtHandleUsage;

   Own<FT_Face> mFtFace;
   Path mFontFile = DefaultFont;
   pcu32 mScale = DefaultScale;
   pcu32 mSpread = DefaultSpreadInPixels;
};

LANGULUS_DECLARE_TRAIT(Text, "Text contents");
LANGULUS_DECLARE_TRAIT(Font, "Font name");
LANGULUS_DECLARE_TRAIT(Locale, "Locale name");
LANGULUS_DECLARE_TRAIT(Spacing, "Spacing");
