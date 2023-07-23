///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Image.hpp"
#include "ImageLibrary.hpp"
#include "PNG.hpp"
#include <Math/Colors.hpp>


/// Image construction                                                        
///   @param producer - the image producer                                    
///   @param descriptor - instructions for configuring the image              
Image::Image(ImageLibrary* producer, const Descriptor& descriptor)
   : A::Texture {MetaOf<Image>(), producer, descriptor} {
   VERBOSE_IMAGES("Initializing...");
   
   // Parse the descriptor for a filename                               
   Path filename;
   descriptor.ForEachDeep(
      [&](const Text& text) {
         filename = text;
      },
      [&](const Trait& trait) {
         if (trait.TraitIs<Traits::Name, Traits::Path>())
            filename = trait.template AsCast<Text>();
      }
   );

   if (filename) {
      // Load a filename if such was provided                           
      auto fileInterface = producer->GetFolder()->GetFile(filename);
      if (fileInterface)
         PNG::Read(*fileInterface, *this);
   }
   
   // Consider all provided data                                        
   auto colorData = mDescriptor.template GetData<A::Color>();
   if (colorData) {
      // Create texture from raw colors                                 
      TODO();
   }

   VERBOSE_IMAGES("Initialized");
}

/// Get a level of detail (mip level)                                         
///   @param lod - the LOD state                                              
///   @return the level of detail image                                       
const A::Texture* Image::GetLOD(const LOD& lod) const {
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
			PNG::Read(file, *this);
		},
      [&](const Text& path) {
			auto file = GetRuntime()->GetFile(path);
			if (file)
            PNG::Read(*file, *this);
		}
   );
}
