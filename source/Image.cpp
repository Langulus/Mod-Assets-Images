///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Image.hpp"
#include "ImageLibrary.hpp"
#include <Math/Colors.hpp>


/// Image construction                                                        
///   @param producer - the image producer                                    
///   @param descriptor - instructions for configuring the image              
Image::Image(ImageLibrary* producer, const Neat& descriptor)
   : A::Image {MetaOf<::Image>(), producer, descriptor} {
   VERBOSE_IMAGES("Initializing...");
   
   // Get a path from the descriptor                                    
   Path filename;
   if (not descriptor.ExtractTrait<Traits::Name, Traits::Path>(filename))
      descriptor.ExtractDataAs(filename);

   if (filename) {
      // Load a filename if such was provided                           
      auto fileInterface = producer->GetFolder()->GetFile(filename);
      if (fileInterface)
         ReadPNG(*fileInterface);
   }
   else {
      // Consider all provided data                                     
      if (not mDescriptor.ExtractData(mView))
         LANGULUS_THROW(Image, "No image view available for custom texture");

      // Upload raw data if any                                         
      Bytes rawData;
      if (mDescriptor.ExtractData(rawData))
         Upload(Copy(rawData));
   }

   VERBOSE_IMAGES("Initialized");
}

/// Compare image to another image/uniform color, etc.                        
///   @param verb - the comparison verb                                       
void Image::Compare(Verb& verb) {
   if (verb.CastsTo<A::Color>()) {
      // Compare against colors                                         
      if (verb.GetCount() == 1) {
         // Check if texture is filled with a uniform color             
         const auto cast = verb.AsCast<RGBA>();
         const auto color = mView.mReverseFormat
            ? RGBA {cast[2], cast[1], cast[0], cast[3]}
            : cast;

         const auto matches = ForEachPixel([&color](const RGBA& pixel) {
            if (pixel == color)
               return Flow::Continue;
            return Flow::Break;
         });

         verb << (matches == mView.GetPixelCount()
               ? Compared::Equal
               : Compared::Unequal);
      }
   }
   else if (verb.CastsTo<A::Image>()) {
      // Compare against other images                                   
      TODO();
   }
}

/// Generate color data                                                       
///   @param trait - the trait to generate                                    
///   @param index - trait group to generate                                  
///   @return true if data was generated                                      
bool Image::Generate(TMeta trait, Offset index) {
   if (trait->Is<Traits::Color>() and index == 0)
      return true;
   return false;
}

/// Get a level of detail (mip level)                                         
///   @param lod - the LOD state                                              
///   @return the level of detail image                                       
Ref<A::Image> Image::GetLOD(const LOD& lod) const {
   TODO();
}

/// Returns nullptr, because this is not a GPU image                          
///   @return nullptr                                                         
void* Image::GetGPUHandle() const noexcept {
   return nullptr;
}

/// Load image via filename/file interface                                    
///   @param descriptor - the file to load                                    
void Image::LoadFile(const Any& descriptor) {
   descriptor.ForEach(
      [&](const A::File& file) {
			ReadPNG(file);
		},
      [&](const Text& path) {
			auto file = GetRuntime()->GetFile(path);
			if (file)
            ReadPNG(*file);
		}
   );
}
