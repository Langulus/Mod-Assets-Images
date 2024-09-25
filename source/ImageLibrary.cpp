///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "ImageLibrary.hpp"

LANGULUS_DEFINE_MODULE(
   ImageLibrary, 9, "AssetsImages",
   "Image reading, writing, generation and modification module", "",
   ImageLibrary, Image
)


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param desc - instructions for configuring the module                   
ImageLibrary::ImageLibrary(Runtime* runtime, const Neat& desc)
   : Resolvable {this}
   , Module     {runtime} {
   VERBOSE_IMAGES("Initializing...");

   // Extract mesh folder, if any                                       
   Path repo = "assets/images";
   if (not desc.ExtractTrait<Traits::Name, Traits::Path>(repo))
      desc.ExtractDataAs(repo);

   try { mFolder = GetRuntime()->GetFolder(repo); }
   catch (...) {
      Logger::Warning(Self(), 
         "Can't access image asset library folder `", desc,
         "` - either folder is missing, or there's probably "
         "no file system module available. "
         "Image reading/writing won't be available, "
         "but you can still generate images"
      );
   }

   VERBOSE_IMAGES("Initialized");
}

/// Create/Destroy image assets                                               
///   @param verb - the creation/destruction verb                             
void ImageLibrary::Create(Verb& verb) {
   mImages.Create(this, verb);
}