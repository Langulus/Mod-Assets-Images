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
Image::Image(ImageLibrary* producer, const Descriptor& descriptor)
   : A::Texture {MetaOf<Image>(), producer, descriptor} {
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
