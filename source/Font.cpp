///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Font.hpp"
#include "ImageLibrary.hpp"
#include <Math/Colors.hpp>


/// Font descriptor-constructor                                               
///   @param producer - the image producer                                    
///   @param descriptor - instructions for configuring the image              
Font::Font(ImageLibrary* producer, const Descriptor& descriptor)
   : A::Texture {MetaOf<Font>(), producer, descriptor} {}

const A::Texture* Font::GetLOD(const Math::LOD&) const {
   TODO();
}

void* Font::GetGPUHandle() const noexcept {
   TODO();
}
