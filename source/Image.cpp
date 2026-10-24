///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Image.hpp"
#include "ImageLibrary.hpp"
#include <Langulus/CppAPI/IO.hpp>


/// Image construction                                                        
///   @param producer the image producer                                      
///   @param desc instructions for configuring the image                      
Image::Image(ImageLibrary* producer, Many const& desc)
   : Downcastable {this}
   , ProducedFrom {producer, desc} {
   VERBOSE_IMAGES("Initializing...");
   
   if (not FromFile(desc)) {
      // Mesh isn't file-based, so inspect the descriptor more closely  
      if (desc.ExtractData(mView)) {
         // Upload raw data if any                                      
         Bytes rawData;
         if (desc.ExtractData(rawData))
            Upload(Copy(rawData));
      }
      else {
         // Configure a generator from descriptor                       
         LglsAssert(FromDescriptor(desc), "Couldn't create image generator");
      }
   }

   // If this was reached, then mesh was successfully initialized, so   
   // it is ready to be added to the hierarchy of Things                
   Couple(desc);
   VERBOSE_IMAGES("Initialized");
}

/// Compare image to another image/uniform color, etc.                        
///   @param verb the comparison verb                                         
void Image::Compare(Verb& verb) const {
   if (verb.CastsTo<Math::Color>()) {
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
   else if (verb.CastsTo<Things::Image>()) {
      // Compare against other images                                   
      verb << (CompareInner(verb->As<Image>())
         ? Compared::Equal
         : Compared::Unequal);
   }
   else if (verb.CastsTo<Text>()) {
      // Compare against other images                                   
      Verbs::Create rhsCreator {Recipe::From<Image>(verb.GetRHS())};
      GetLibrary()->Create(rhsCreator);
      verb << (CompareInner(rhsCreator->As<Image>())
         ? Compared::Equal
         : Compared::Unequal);
   }
}

/// Inner pixel-by-pixel comparison                                           
/// Accounts for inversed pixel formats                                       
///   @param rhs the image to compare against                                 
///   @return true if both images match exactly                               
bool Image::CompareInner(const Image& rhs) const {
   if (rhs.GetView() == GetView()
   and rhs.GetView().mReverseFormat == GetView().mReverseFormat) {
      // We can batch-compare                                           
      return GetDataListMap() == rhs.GetDataListMap();
   }
   else if (rhs.GetView() == GetView()
   and rhs.GetView().mReverseFormat != GetView().mReverseFormat) {
      // We have to compare pixel-by-pixel, because one of the pixel    
      // formats is flipped                                             
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
///   @param tag the tag to generate                                          
///   @param index tag group to generate                                      
///   @return true if data was generated                                      
bool Image::Generate(TMeta tag, size_t index) {
   if (tag == MetaTagOf<Tags::Color>() and index == 0)
      return true;
   return false;
}

/// Get a level of detail (mip level)                                         
///   @param lod the LOD state                                                
///   @return the level of detail image                                       
auto Image::GetLOD(LOD const&) const -> Ref<Things::Image> {
   TODO();
   return {};
}

/// Returns nullptr, because this is not a GPU image                          
///   @return nullptr                                                         
auto Image::GetGPUHandle() const noexcept -> void* {
   return nullptr;
}

/// Populate the image view and generator functions, by analyzing descriptor  
///   @param desc the descriptor to parse                                     
bool Image::FromDescriptor(Many const& desc) {
   const auto primitive = desc.FindType<Math::Primitive>();
   if (not primitive)
      return false;
   
   TODO();
   return false;
}

/// Load image via filename/file interface                                    
///   @param descriptor the file to load                                      
bool Image::FromFile(Many const& desc) {
   Path filename;
   if (not desc.ExtractTag<Tags::Name, Tags::Path>(filename))
      desc.ExtractDataAs(filename);

   if (filename) {
      // Load a filename if such was provided                           
      auto fileInterface = GetProducer()->GetFolder()->RelativeFile(filename);
      if (fileInterface)
         return ReadPNG(*fileInterface);
   }

   return false;
}

/// Get the image library                                                     
///   @return the library                                                     
auto Image::GetLibrary() const -> ImageLibrary* {
   return mProducer.template As<ImageLibrary>();
}
