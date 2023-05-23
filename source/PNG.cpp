///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "PNG.hpp"
#include <png.h>

/// Intent of file                                                            
enum class Intent {
   Read, Write
};

/// PNG file struct helper and destructor                                     
template<Intent INTENT>
struct PNGFileHelper {
   png_structp png_ptr = nullptr;
   png_infop info_ptr = nullptr;

   ~PNGFileHelper() {
      if (png_ptr) {
         if constexpr (INTENT == Intent::Read)
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
         else if constexpr (INTENT == Intent::Write)
            png_destroy_write_struct(&png_ptr, &info_ptr);
         png_ptr = nullptr;
      }
   }

   /// A helper callback function to read a chunk from a PNG file             
   static void Read(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
      png_voidp io_ptr = png_get_io_ptr(png_ptr);
      if (!io_ptr)
         return;

      auto inputStream = static_cast<A::File::StreamIn*>(io_ptr);
      auto outputBlock = Block::From(outBytes, byteCountToRead);
      inputStream->Read(outputBlock);
   }

   /// A helper callback function to write a chunk to a PNG file              
   static void Write(png_structp png_ptr, png_bytep inBytes, png_size_t byteCountToWrite) {
      png_voidp io_ptr = png_get_io_ptr(png_ptr);
      if (!io_ptr)
         return;

      auto outputStream = static_cast<A::File::StreamOut*>(io_ptr);
      auto inputBlock = Block::From(inBytes, byteCountToWrite);
      outputStream->Write(inputBlock);
   }
};

/// Load PNG from file                                                        
///   @param file - [in/out] the file to load from                            
///   @param destination - [out] the texture to load to                       
///   @return true if image was loaded without any problems                   
bool PNG::Read(A::File& file, A::Texture& destination) {
   auto loadTime = SteadyClock::now();
   auto stream = file.NewStreamIn();
   if (!stream) {
      Logger::Error("Cannot open file: ", file);
      return false;
   }

   // Check if file is PNG by reading the header                        
   Bytes png_header;
   png_header.Null(8);

   auto marker = stream->Read(png_header);
   if (marker == 0) {
      Logger::Error("Cannot read png_header from file: ", file);
      return false;
   }

   if (0 != png_sig_cmp(
      reinterpret_cast<const png_const_bytep>(png_header.GetRaw()), 
      0, png_header.GetByteSize())
   ) {
      Logger::Error("File is not PNG: ", file);
      return false;
   }

   const auto warning_fn = [](png_structp, png_const_charp txt) {
      Logger::Warning("PNG warning: ", txt);
   };

   PNGFileHelper<Intent::Read> fileReader;
   fileReader.png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, warning_fn);
   if (!fileReader.png_ptr) {
      Logger::Error("Cannot create PNG pointer: ", file);
      return false;
   }

   png_set_read_fn(fileReader.png_ptr, stream, fileReader.Read);
   png_set_sig_bytes(fileReader.png_ptr, 8);
   fileReader.info_ptr = png_create_info_struct(fileReader.png_ptr);
   if (!fileReader.info_ptr) {
      Logger::Error("Cannot create PNG info struct: ", file);
      return false;
   }

   // Read file info                                                    
   auto& view = destination.GetView();
   png_read_info(fileReader.png_ptr, fileReader.info_ptr);
   view.mWidth = png_get_image_width(fileReader.png_ptr, fileReader.info_ptr);
   view.mHeight = png_get_image_height(fileReader.png_ptr, fileReader.info_ptr);
   view.mDepth = 1;
   view.mFrames = 1;

   auto color_type = png_get_color_type(fileReader.png_ptr, fileReader.info_ptr);
   auto bit_depth = png_get_bit_depth(fileReader.png_ptr, fileReader.info_ptr);
   if (bit_depth == 16)   {
      // Set strip from 16 bits to 8 bits                               
      // We need only 8 bits per color channel                          
      png_set_strip_16(fileReader.png_ptr);
   }

   // Pick the correct color format                                     
   switch (color_type) {
   case PNG_COLOR_TYPE_GRAY:
      if (bit_depth < 8)
         png_set_expand_gray_1_2_4_to_8(fileReader.png_ptr);
      view.mFormat = MetaOf<Red8>();
      break;

   case PNG_COLOR_TYPE_GRAY_ALPHA:
      // This type is always 8 or 16 bits but we already strip 16 bits  
      png_set_gray_to_rgb(fileReader.png_ptr);
      if (0 != png_get_valid(fileReader.png_ptr, fileReader.info_ptr, PNG_INFO_tRNS))
         png_set_tRNS_to_alpha(fileReader.png_ptr);
      view.mFormat = MetaOf<RGBA>();
      break;

   case PNG_COLOR_TYPE_RGB:
      view.mFormat = MetaOf<RGB>();
      break;

   case PNG_COLOR_TYPE_RGBA:
      view.mFormat = MetaOf<RGBA>();
      break;

   case PNG_COLOR_TYPE_PALETTE:
      png_set_palette_to_rgb(fileReader.png_ptr);
      if (0 != png_get_valid(fileReader.png_ptr, fileReader.info_ptr, PNG_INFO_tRNS))
         png_set_tRNS_to_alpha(fileReader.png_ptr);
      else
         png_set_filler(fileReader.png_ptr, 0xFF, PNG_FILLER_AFTER);
      view.mFormat = MetaOf<RGBA>();
      break;

   default:
      Logger::Error("Unknown color type for PNG: ", file);
      return false;
   }

   png_read_update_info(fileReader.png_ptr, fileReader.info_ptr);

   // Read pixels                                                       
   std::vector<uint8_t*> row_pointers;
   row_pointers.reserve(view.mHeight);

   auto pitch = png_get_rowbytes(fileReader.png_ptr, fileReader.info_ptr);
   TAny<uint8_t> rawData;
   rawData.New(pitch * view.mHeight);
   for (uint32_t i = 0; i < view.mHeight; i++)
      row_pointers.push_back(rawData.GetRaw() + i * pitch);
   png_read_image(fileReader.png_ptr, row_pointers.data());

   // Save the resulting memory inside content descriptor               
   destination.template Commit<Traits::Color>(Abandon(rawData));

   Logger::Verbose(Logger::Green, "File ", file.GetFilePath(), 
      " loaded in ", SteadyClock::now() - loadTime);
   return true;
}

/// Save to PNG file                                                          
///   @param file - [in/out] the file to write to                             
///   @param source - the texture to save                                     
///   @return true if image was saved without any problems                    
bool PNG::Write(A::File& file, const A::Texture& source) {
   auto rawData = source.GetData<Traits::Color>();
   if (!rawData || rawData->IsEmpty())
      return false;

   auto writeTime = SteadyClock::now();
   auto stream = file.NewStreamOut();
   if (!stream) {
      Logger::Error("Cannot open file: ", file);
      return false;
   }

   PNGFileHelper<Intent::Write> fileWriter;
   fileWriter.png_ptr = png_create_write_struct(
      PNG_LIBPNG_VER_STRING,
      nullptr, nullptr, nullptr
   );

   if (!fileWriter.png_ptr) {
      Logger::Error("Cannot allocate PNG write-struct for file: ", file);
      return false;
   }

   fileWriter.info_ptr = png_create_info_struct(fileWriter.png_ptr);
   if (!fileWriter.info_ptr) {
      Logger::Error("Cannot create PNG info-struct for file: ", file);
      return false;
   }

   png_set_write_fn(fileWriter.png_ptr, stream, fileWriter.Write, nullptr);

   // Pick the correct color format                                     
   auto& view = source.GetView();
   int pngFormat;
   if (view.mFormat->IsExact<Red8>())
      pngFormat = PNG_COLOR_TYPE_GRAY;
   else if (view.mFormat->IsExact<RGBA>())
      pngFormat = PNG_COLOR_TYPE_RGBA;
   else if (view.mFormat->IsExact<RGB>())
      pngFormat = PNG_COLOR_TYPE_RGB;
   else
      LANGULUS_THROW(Write, "Unknown color type");

   png_set_IHDR(
      fileWriter.png_ptr, fileWriter.info_ptr, 
      static_cast<png_uint_32>(view.mWidth), 
      static_cast<png_uint_32>(view.mHeight),
      8, pngFormat,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_BASE,
      PNG_FILTER_TYPE_BASE
   );

   png_write_info(fileWriter.png_ptr, fileWriter.info_ptr);

   // Write data                                                        
   auto rawDataPtr = rawData->GetRawAs<const uint8_t*>();
   const auto pitch = view.mWidth * view.mFormat->mSize;
   std::vector<const uint8_t*> rows;
   rows.reserve(view.mHeight);
   for (Offset i = 0; i < view.mHeight; i++)
      rows.push_back(rawDataPtr + i * pitch);

   png_write_image(fileWriter.png_ptr, rows.data());
   png_write_end(fileWriter.png_ptr, nullptr);
   return true;
}