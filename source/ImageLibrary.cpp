///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "ImageLibrary.hpp"


LANGULUS_DEFINE_MODULE(
   ImageLibrary, 9, "AssetsImages",
   "Image reading, writing, generation and modification module", "",
   ImageLibrary, Image
)

/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
ImageLibrary::ImageLibrary(Runtime* runtime, const Neat&)
   : A::AssetModule {MetaOf<ImageLibrary>(), runtime}
   , mImages {this} {
   VERBOSE_IMAGES("Initializing...");
   // Extract image folder, if any                                      
   //TODO configure mFolder from descriptor

   try {
      mImageFolder = Path {"assets"} / "images";
      mFolder = GetRuntime()->GetFolder(mImageFolder);
   }
   catch (...) {
      Logger::Warning(Self(), 
         "Can't access image asset library folder `", mImageFolder,
         "` - either folder is missing, or there's probably "
         "no file system module available. "
         "Image reading/writing won't be available, "
         "but you can still generate images"
      );
   }

   VERBOSE_IMAGES("Initialized");
}

/// Module update routine                                                     
///   @param dt - time from last update                                       
void ImageLibrary::Update(Time) {

}

/// Create/Destroy image assets                                               
///   @param verb - the creation/destruction verb                             
void ImageLibrary::Create(Verb& verb) {
   mImages.Create(verb);
}

/// Get the image library folder                                              
///   @return the folder interface                                            
const A::Folder* ImageLibrary::GetFolder() const noexcept {
   return *mFolder;
}
