///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Image.hpp"
#include "ImageLibrary.hpp"
#include <Langulus/IO.hpp>


/// Image construction                                                        
///   @param producer - the image producer                                    
///   @param descriptor - instructions for configuring the image              
Image::Image(ImageLibrary* producer, const Neat& descriptor)
   : A::Image {MetaOf<::Image>(), producer, descriptor}
   , mProducer {producer} {
   VERBOSE_IMAGES("Initializing...");
   
   // Get a path from the descriptor                                    
   Path filename;
   if (not descriptor.ExtractTrait<Traits::Name, Traits::Path>(filename))
      descriptor.ExtractDataAs(filename);

   if (filename) {
      // Load a filename if such was provided                           
      auto fileInterface = producer->GetFolder()->RelativeFile(filename);
      if (fileInterface)
         ReadPNG(*fileInterface);
   }
   else {
      // Consider all provided data                                     
      if (not descriptor.ExtractData(mView)) {
         LANGULUS_OOPS(Image, 
            "No image view available for custom texture with descriptor: ",
            descriptor
         );
      }

      // Upload raw data if any                                         
      Bytes rawData;
      if (descriptor.ExtractData(rawData))
         Upload(Copy(rawData));
   }

   Couple(descriptor);
   VERBOSE_IMAGES("Initialized");
}

/// Compare image to another image/uniform color, etc.                        
///   @param verb - the comparison verb                                       
void Image::Compare(Verb& verb) const {
   if (verb.CastsTo<A::Color>()) {
      // Compare against colors                                         
      if (verb.GetCount() == 1) {
         // Check if texture is filled with a uniform color             
         const auto cast = verb.AsCast<RGBA>();
         const auto color = mView.mReverseFormat
            ? RGBA {cast[2], cast[1], cast[0], cast[3]}
            : cast;

         const auto matches = ForEachPixel(
            [&color](const RGBA& pixel) noexcept -> bool {
               return pixel == color;
            });

         verb << (matches == mView.GetPixelCount()
            ? Compared::Equal
            : Compared::Unequal);
      }
   }
   else if (verb.CastsTo<A::Image>()) {
      // Compare against other images                                   
      verb << (CompareInner(verb->As<Image>())
         ? Compared::Equal
         : Compared::Unequal);
   }
   else if (verb.CastsTo<A::Text>()) {
      // Compare against other images                                   
      Verbs::Create rhsCreator {Construct::From<Image>(verb.GetArgument())};
      mProducer->Create(rhsCreator);
      verb << (CompareInner(rhsCreator->As<Image>())
         ? Compared::Equal
         : Compared::Unequal);
   }
}

/// Inner pixel-by-pixel comparison                                           
/// Accounts for inversed pixel formats                                       
///   @param rhs - the image to compare against                               
///   @return true if both images match exactly                               
bool Image::CompareInner(const Image& rhs) const {
   if (rhs.GetView() == GetView()
   and rhs.GetView().mReverseFormat == rhs.GetView().mReverseFormat) {
      // We can batch-compare                                           
      return GetDataListMap() == rhs.GetDataListMap();
   }
   else if (rhs.GetView() == GetView()
   and rhs.GetView().mReverseFormat == rhs.GetView().mReverseFormat) {
      // We have to compare pixel-by-pixel, because one of the pixel    
      // formats is flipped                                             
      // We have to compare pixel-by-pixel, because formats differ      
      auto lit = begin();
      auto rit = rhs.begin();
      while (lit != end()) {
         if (lit.As<RGBA>() != rit.As<RGBA>()) //TODO SIMD the crap out of this using shuffles
            return false;
         ++lit;
         ++rit;
      }
      return true;
   }
   else if (rhs.GetView().mHeight == GetView().mHeight
   and rhs.GetView().mWidth  == GetView().mWidth
   and rhs.GetView().mDepth  == GetView().mDepth
   and rhs.GetView().mFrames == GetView().mFrames) {
      // We have to compare pixel-by-pixel, because formats differ      
      auto lit = begin();
      auto rit = rhs.begin();
      while (lit != end()) {
         if (lit.As<RGBA>() != rit.As<RGBA>())
            return false;
         ++lit;
         ++rit;
      }
      return true;
   }

   return false;
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
Ref<A::Image> Image::GetLOD(const LOD&) const {
   TODO();
   return {};
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
