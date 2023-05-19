#include "MContent.hpp"
#include "Textures/PNG.hpp"
#include <png.h>
#include <locale>

#define PC_VERBOSE_FONT(a) pcLogSelfVerbose << a

bool isPixelSet(FT_Bitmap&, pcu32, pcu32);
void addKerning(AFont::Glyph&, AFont::CodePoint, FT_Pos);
void Write(const AFont::Glyph&, AFile&);
void Read(AFont::Glyph&, AFile&);
float getSignedDistance(FT_Bitmap&, pcu32 scale, pcu32 spread, pcu32 xSD, pcu32 ySD);
bool testOrthogonalPoints(FT_Bitmap&, bool testBase, long xBase, long yBase, long offset);
bool testDiagonalPoints(FT_Bitmap&, bool testBase, long xBase, long yBase, long offset);
bool testSymmetricPoints(FT_Bitmap&, bool testBase, long xBase, long yBase, long offset1, long offset2);

FT_Library CGeneratorFont::mFtHandle = nullptr;
pcref CGeneratorFont::mFtHandleUsage = 0;

/// Font generator construction															   
///	@param producer - the producer module												
CGeneratorFont::CGeneratorFont(MContent* producer)
	: AFont {MetaData::Of<CGeneratorFont>()}
	, TProducedFrom {producer} {
   mConstruct = { ClassMeta() };
   ClassValidate();
}

/// Font destruction																			
CGeneratorFont::~CGeneratorFont() {
   Uninitialize();

   if (mFtHandle) {
      --mFtHandleUsage;
      if (0 == mFtHandleUsage) {
         FT_Done_FreeType(mFtHandle);
         mFtHandle = nullptr;
      }
   }

   if (mFtFace) {
      FT_Done_Face(mFtFace);
      mFtFace = nullptr;
   }
}

/// Add generators, traits and data to content											
///	@param verb - the creation verb														
void CGeneratorFont::Create(Verb& verb) {
   if (verb.GetArgument().IsEmpty())
      return;

   // Accumulate stuff into the content construct								
   mConstruct << verb.GetArgument();
   verb.Done();
}

/// Initialize the texture using the descriptor											
void CGeneratorFont::Generate() {
   if (mGenerated)
      return;

   PC_VERBOSE_FONT("Generating " << mConstruct);

   // Scan constructor																	
   mConstruct.GetAll().ForEachDeep([&](Block& group) {
      EitherDoThis
         group.ForEach([&](AFile& file) {
            if (file.GetFileExtension() == "ttf")
               LoadTTF(file);
            else if (file.GetFileExtension() == "pcf")
               LoadPCF(file);
            else pcLogSelfError 
               << "Unsupported font generator format: " << file.GetFilename();
         })
      OrThis
         group.ForEach([&](const Text& fontName) {
            // Try loading PCF first, if available                      
            auto pcfFile = mProducer->GetFontsFolder()->GetFile(fontName + ".pcf");
            if (!pcfFile->Exists()) {
               // Then try generating PCF via TTF                       
               auto ttfFile = mProducer->GetFontsFolder()->GetFile(fontName + ".ttf");
               if (!ttfFile->Exists())
                  throw Except::Content(pcLogSelfError
                     << "No PCF or TTF file available for font request: " << fontName);
               LoadTTF(*ttfFile);
               WritePCF(*pcfFile);
            }
            else LoadPCF(*pcfFile);
         });
   });

   if (GetDataList().IsEmpty())
      throw Except::Content(pcLogSelfError << "No content was generated");

   PC_VERBOSE_FONT(ccGreen << "Generated");
   mGenerated = true;
   ClassValidate();
}

/// Load TTF																						
///	@param file - the file to load from													
void CGeneratorFont::LoadTTF(AFile& file) {
   std::locale::global(std::locale(mLocale.GetRaw()));
   mLocale = std::locale().name().c_str();
   mFontName = file.GetFilenameWithoutExtension();
   mFontFile = file.GetFilename();
   InitializeFreeType();
   GetGlyphMetrics();
   GetKernings();
   GenerateGlyphBitmaps();
}

/// Load PCF																						
///	@param file - the file to load from													
void CGeneratorFont::LoadPCF(AFile& file) {
   mFontName = file.GetFilenameWithoutExtension();
   auto openedFile = file.Open(FState::BinaryRead);
   file.ReadHeader<CGeneratorFont>();
   file >> mLocale;
   file >> mFontFile;
   file >> mScale;
   file >> mSpread;
   for (auto& g : mGlyphs)
      Read(g, file);

   auto pngFile = mProducer->GetFontsFolder()->GetFile(mFontFile + ".png");
   PNG::LoadPNG(*pngFile, *this);
}

/// Write a PCF exported font                                                 
///   @param file - the file to write                                         
void CGeneratorFont::WritePCF(AFile& file) const {
   // Write the font atlas                                              
   auto pngFile = mProducer->GetFontsFolder()->GetFile(mFontFile + ".png");
   PNG::WritePNG(*pngFile, *this);

   // Write the font metrics file                                       
   auto openedFile = file.Open(FState::BinaryWrite);
   file.WriteHeader<CGeneratorFont>();
   file << mLocale;
   file << mFontFile;
   file << mScale;
   file << mSpread;
   for (auto& g : mGlyphs)
      Write(g, file);
}

/// Initialize the FreeType library                                           
void CGeneratorFont::InitializeFreeType() {
   if (!mFtHandle) {
      // Initialize free type only once per application                 
      auto ftError = FT_Init_FreeType(&mFtHandle);
      if (ftError != FT_Err_Ok)
         throw Except::Content(pcLogSelfError
            << "Can't initialize FreeType: " << ftError);
      mFtHandleUsage = 1;
   }
   else ++mFtHandleUsage;

   if (!mFtFace) {
      // Initialize free type face once per font                        
      auto ttfFile = mProducer->GetFontsFolder()->GetFile(mFontFile);
      if (!ttfFile->Exists())
         throw Except::Content(pcLogSelfError
            << "Missing font file: " << ttfFile->GetFilename());

      const auto absoluteFilename = ttfFile->GetAbsoluteFilename().Terminate();
      auto ftError = FT_New_Face(mFtHandle, absoluteFilename.GetRaw(), 0, &mFtFace.mValue);
      if (ftError != FT_Err_Ok)
         throw Except::Content(pcLogSelfError
            << "Can't create FreeType face: " << ftError);

      ftError = FT_Set_Pixel_Sizes(mFtFace, 0, DefaultScale);
      if (ftError != FT_Err_Ok)
         throw Except::Content(pcLogSelfError
            << "Can't set FreeType pixel sizes: " << ftError);
   }
}

/// Get kernings                                                              
void CGeneratorFont::GetKernings() {
   if (!FT_HAS_KERNING(mFtFace))
      return;

   for (auto& g1 : mGlyphs) {
      auto ci1 = FT_Get_Char_Index(mFtFace, g1.mCodePoint);
      for (auto& g2 : mGlyphs) {
         auto ci2 = FT_Get_Char_Index(mFtFace, g2.mCodePoint);
         FT_Vector kerning;
         FT_Get_Kerning(mFtFace, ci1, ci2, FT_KERNING_DEFAULT, &kerning);
         if (kerning.x != 0)
            addKerning(g1, g2.mCodePoint, kerning.x);
      }
   }
}

/// Generate glyph                                                            
void CGeneratorFont::GetGlyphMetrics() {
   for (pcptr i = 0; i < GlyphCount; i++) {
      auto ind = FT_Get_Char_Index(mFtFace, FT_ULong(i));
      auto ftError = FT_Load_Glyph(mFtFace, ind, FT_LOAD_DEFAULT);
      if (ftError != FT_Err_Ok)
         throw Except::Content(pcLogSelfError
            << "Can't load FreeType glyph: " << ftError);

      Glyph& g = mGlyphs[i];
      g.mCodePoint = static_cast<CodePoint>(i);
      g.mRect[2] = pci16(mSpread);
      g.mRect[3] = pci16(mSpread);
      g.mSize = {
         pci16(mFtFace->glyph->metrics.width / 64),
         pci16(mFtFace->glyph->metrics.height / 64)
      };
      g.mHorizontalBearingAndAdvance = {
         pci16(mFtFace->glyph->metrics.horiBearingX / 64),
         pci16(mFtFace->glyph->metrics.horiBearingY / 64),
         pci16(mFtFace->glyph->metrics.horiAdvance / 64)
      };
      g.mVerticalBearingAndAdvance = {
         pci16(mFtFace->glyph->metrics.vertBearingX / 64),
         pci16(mFtFace->glyph->metrics.vertBearingY / 64),
         pci16(mFtFace->glyph->metrics.vertAdvance / 64)
      };
   }
}

/// Generate glyph textures                                                   
void CGeneratorFont::GenerateGlyphBitmaps() {
   const auto numItemsPerRow = pcSqrt(GlyphCount);
   const auto atlasSide = mSpread * numItemsPerRow;
   
   PC_VERBOSE_FONT("Number of glyphs is " << numItemsPerRow << "x" << numItemsPerRow);
   PC_VERBOSE_FONT("Area is " << atlasSide << "x" << atlasSide);

   // Allocate the glyph atlas                                          
   TAny<uint8_t> content;
   content.Null(atlasSide * atlasSide);

   // Allocate a distance field for a single glyph                      
   TAny<float> temporary;

   // Start rendering glyphs                                            
   pcptr baseX = 0;
   pcptr baseY = 0;
   pcptr glyphNum = 0;
   pcptr numFontsProcessed = 1;
   for (auto& g : mGlyphs) {
      // Render the FreeType glyph                                      
      auto ind = FT_Get_Char_Index(mFtFace, g.mCodePoint);
      auto ftError = FT_Load_Glyph(mFtFace, ind, FT_LOAD_DEFAULT);
      if (ftError != FT_Err_Ok)
         throw Except::Content(pcLogSelfError
            << "Can't load FreeType glyph: " << ftError);

      ftError = FT_Render_Glyph(mFtFace->glyph, FT_RENDER_MODE_MONO);
      if (ftError != FT_Err_Ok)
         throw Except::Content(pcLogSelfError
            << "Can't render FreeType glyph: " << ftError);

      auto& bm = mFtFace->glyph->bitmap;
      temporary.Null(mSpread * mSpread);
      for (pcu32 i = 0; i < mSpread; i++) {
         for (pcu32 j = 0; j < mSpread; j++) {
            temporary[i * mSpread + j] = 
               getSignedDistance(bm, mScale, mSpread, j, i);
         }
      }

      PC_VERBOSE_FONT(numFontsProcessed 
         << "/" << GlyphCount << " at " << baseX << ", " << baseY);
      g.mRect[0] = pci16(baseX);
      g.mRect[1] = pci16(baseY);

      for (pcptr srcY = 0; srcY < mSpread; srcY++) {
         auto dstY = srcY + baseY;
         auto curRow = content.GetRaw() + atlasSide * dstY;
         for (pcptr srcX = 0; srcX < mSpread; srcX++) {
            const auto dist = temporary[srcY * mSpread + srcX] * 255.0f;
            curRow[srcX + baseX] = uint8_t(pcClamp(dist, 0, 255));
         }
      } 

      glyphNum++;

      if ((glyphNum % numItemsPerRow) == 0) {
         baseX = 0;
         baseY += mSpread;
      }
      else baseX += mSpread;

      numFontsProcessed++;
   }

   // Save the resulting memory inside content descriptor					
   SaveData<Traits::Color>(pcMove(content), rgba{0}, rgba{255});
   mView.mWidth = mView.mHeight = atlasSide;
   mView.mFormat = MetaData::Of<red8>();
}

void addKerning(AFont::Glyph& g, AFont::CodePoint followingCodePoint, FT_Pos v) {
   g.mKernings.Add(followingCodePoint, pci16(v));
}

/// Test if the point is 'in' or 'out' of glyph.                              
///   @param bm - FreeType bitmap info                                        
///   @param x - x position in the FreeType's bitmap coordinate space         
///   @param y - y position in the FreeType's bitmap coordinate space         
///   @return true if the point is in the glyph                               
bool isPixelSet(FT_Bitmap& bm, pcu32 x, pcu32 y) {
   if (x >= bm.width || y >= bm.rows)
      return false;

   FT_Byte* p = bm.buffer;
   FT_Byte* row = p + (bm.pitch * y);

   const auto byteOffset = x / 8u;
   const auto bitOffset = 7u - (x % 8u);

   FT_Byte b = *(row + byteOffset);
   FT_Byte bitPosTested = (FT_Byte)(1u << bitOffset);
   return (b & bitPosTested) != 0;
}

/// Test the points along the X and Y axes distant by 'offset'                
/// from the terget point. The points are:                                    
///             ( xBase,          yBase + offset )                            
///             ( xBase,          yBase - offset )                            
///             ( xBase + offset, yBase          )                            
///             ( xBase - offset, yBase          )                            
///   @param bm - FreeType bitmap info.                                       
///   @param testBase - True if the target point is 'in' the glyph.           
///   @param xBase - x position in the downsampled local coordinate           
///                  system for the glyph                                     
///   @param yBase - y position in the downsampled local coordinate           
///                  system for the glyph                                     
///   @param offset - distance from the target point.                         
///   @return true if at least one of the four points tested is different     
///           from the target point in the in/out categorization.             
bool testOrthogonalPoints(FT_Bitmap& bm, bool testBase, long xBase, long yBase, long offset) {
   bool test01 = isPixelSet(bm, xBase, yBase + offset);
   bool test02 = isPixelSet(bm, xBase, yBase - offset);
   bool test03 = isPixelSet(bm, xBase + offset, yBase);
   bool test04 = isPixelSet(bm, xBase - offset, yBase);

   if (test01 && test02 && test03 && test04)
      return !testBase;
   else if (!(test01 || test02 || test03 || test04))
      return testBase;
   else return true;
}

/// Test the 8 symmetric points from the target point by offset1              
/// and offset2.) from the terget point. The points are:                      
///             ( xBase + offset1, yBase + offset2 )                          
///             ( xBase + offset2, yBase + offset1 )                          
///             ( xBase + offset1, yBase - offset2 )                          
///             ( xBase + offset2, yBase - offset1 )                          
///             ( xBase - offset1, yBase + offset2 )                          
///             ( xBase - offset2, yBase + offset1 )                          
///             ( xBase - offset1, yBase - offset2 )                          
///             ( xBase - offset2, yBase - offset1 )                          
///   @param bm - FreeType bitmap info.                                       
///   @param testBase - True if the target point is 'in' the glyph.           
///   @param xBase - x position in the downsampled local coordinate           
///                  system for the glyph                                     
///   @param yBase - y position in the downsampled local coordinate           
///                  system for the glyph                                     
///   @param offset1 - distance from the target point.                        
///   @param offset2 - distance from the target point.                        
///   @return true if at least one of the 8 points tested is different        
///           from the target point in the in/out categorization.             
bool testSymmetricPoints(FT_Bitmap& bm, bool testBase, long xBase, long yBase, long offset1, long offset2) {
   bool test01 = isPixelSet(bm, xBase + offset1, yBase + offset2);
   bool test02 = isPixelSet(bm, xBase + offset1, yBase - offset2);
   bool test03 = isPixelSet(bm, xBase - offset1, yBase + offset2);
   bool test04 = isPixelSet(bm, xBase - offset1, yBase - offset2);

   bool test05 = isPixelSet(bm, xBase + offset2, yBase + offset1);
   bool test06 = isPixelSet(bm, xBase + offset2, yBase - offset1);
   bool test07 = isPixelSet(bm, xBase - offset2, yBase + offset1);
   bool test08 = isPixelSet(bm, xBase - offset2, yBase - offset1);

   if (test01 && test02 && test03 && test04 && test05 && test06 && test07 && test08)
      return !testBase;
   else if (!(test01 || test02 || test03 || test04 || test05 || test06 || test07 || test08))
      return testBase;
   else return true;
}

/// Test the points along the diagonal lines distant by 'offset'              
/// from the terget point. The points are:                                    
///             ( xBase + offset, yBase + offset )                            
///             ( xBase + offset, yBase - offset )                            
///             ( xBase - offset, yBase + offset )                            
///             ( xBase - offset, yBase - offset )                            
///   @param bm - FreeType bitmap info.                                       
///   @param testBase - True if the target point is 'in' the glyph.           
///   @param xBase - x position in the downsampled local coordinate           
///                  system for the glyph                                     
///   @param yBase - y position in the downsampled local coordinate           
///                  system for the glyph                                     
///   @param offset - distance from the target point.                         
///   @return true if at least one of the four points tested is different     
///           from the target point in the in/out categorization.             
bool testDiagonalPoints(FT_Bitmap& bm, bool testBase, long xBase, long yBase, long offset) {
   bool test01 = isPixelSet(bm, xBase + offset, yBase + offset);
   bool test02 = isPixelSet(bm, xBase + offset, yBase - offset);
   bool test03 = isPixelSet(bm, xBase - offset, yBase + offset);
   bool test04 = isPixelSet(bm, xBase - offset, yBase - offset);

   if (test01 && test02 && test03 && test04)
      return !testBase;
   else if (!(test01 || test02 || test03 || test04))
      return testBase;
   else return true;
}

/// Calculates the signed distance value from the current point               
///  @param bm - FreeType bitmap info                                         
///  @param scale - scale value for down sampling                             
///  @param spread - size of the extra region around the glyph that are       
///                  included in the signed distance calculation              
///  @param xSD - x position in the downsampled local coordinate system for   
///               the glyph                                                   
///  @param ySD - y position in the downsampled local coordinate system for   
///               the glyph                                                   
///  @return normalized signed distance                                       
///          < 0.5 : the point is not on the glyph                            
///                  the value 0.5 - rtn indicates the normalized             
///                  distance to the closest point 'in' the glyph.            
///            0.5 : the point is on the boundary.                            
///          > 0.5 : the point is not on the glyph                            
///                  the value rtn - 0.5 indicates the normalized             
///                  distance to the closest point 'out' of the glyph.        
float getSignedDistance(FT_Bitmap& bm, pcu32 bmSize, pcu32 glSize, pcu32 xSD, pcu32 ySD) {
   const auto spread = (glSize - bmSize) / 2;
   const auto xPix = xSD - spread;
   const auto yPix = ySD - spread;
   const bool curP = isPixelSet(bm, xPix, yPix);
   const float fSpread = float(spread);

   float minSqDist = fSpread * fSpread;
   pcu32 nextStartI = spread + 1;
   for (pcu32 i = 1; i <= spread; i++) {
      float fi = (float)i;
      if (testOrthogonalPoints(bm, curP, xPix, yPix, i)) {
         minSqDist = pcMin(minSqDist, fi * fi);
         nextStartI = i + 1;
         break;
      }

      bool breaking = false;
      for (pcu32 j = 1; j < i; j++) {
         if (testSymmetricPoints(bm, curP, xPix, yPix, i, j)) {
            float fj = (float)j;
            minSqDist = pcMin(minSqDist, fi * fi + fj * fj);
            nextStartI = i + 1;
            breaking = true;
            break;
         }
      }

      if (breaking)
         break;

      if (testDiagonalPoints(bm, curP, xPix, yPix, i)) {
         minSqDist = pcMin(minSqDist, 2 * fi * fi);
         nextStartI = i + 1;
         break;
      }
   }

   pcu32 maxI = pcMin(pcu32(sqrt(minSqDist)) + 1, spread);
   for (pcu32 i = nextStartI; i <= maxI; i++) {
      float fi = (float)i;
      if (testOrthogonalPoints(bm, curP, xPix, yPix, i)) {
         minSqDist = pcMin(minSqDist, fi * fi);
         break;
      }

      bool breaking = false;
      for (pcu32 j = 1; j < nextStartI; j++) {
         if (testSymmetricPoints(bm, curP, xPix, yPix, i, j)) {
            float fj = (float)j;
            minSqDist = pcMin(minSqDist, fi * fi + fj * fj);
            breaking = true;
            break;
         }
      }

      if (breaking)
         break;
   }

   auto normalizedMinDist = (sqrt(minSqDist) - 1.0f) / fSpread;
   if (curP)
      return 0.5f + (normalizedMinDist / 2.0f);
   else
      return 0.5f - (normalizedMinDist / 2.0f);
}

/// Write the glyph to a file                                                 
///   @param os - the file to write to                                        
void Write(const AFont::Glyph& g, AFile& file) {
   file << g.mCodePoint;
   file << g.mRect;
   file << g.mSize;
   file << g.mHorizontalBearingAndAdvance;
   file << g.mVerticalBearingAndAdvance;
   file << pcu32(g.mKernings.GetCount());
   if (!g.mKernings.IsEmpty())
      file << g.mKernings;
}

/// Read the glyph from a file                                                
///   @param os - the file to read from                                       
void Read(AFont::Glyph& g, AFile& file) {
   file >> g.mCodePoint;
   file >> g.mRect;
   file >> g.mSize;
   file >> g.mHorizontalBearingAndAdvance;
   file >> g.mVerticalBearingAndAdvance;
   pcu32 kerningCount = 0;
   file >> kerningCount;
   if (kerningCount)
      file >> g.mKernings;
}