///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Font.hpp"
#include "ImageLibrary.hpp"


/// Font descriptor-constructor                                               
///   @param producer - the image producer                                    
///   @param descriptor - instructions for configuring the image              
Font::Font(ImageLibrary* producer, const Neat& descriptor)
   : Resolvable {this}
   , ProducedFrom {producer, descriptor} {}

Ref<A::Image> Font::GetLOD(const Math::LOD&) const {
   TODO();
   return {};
}

void* Font::GetGPUHandle() const noexcept {
   TODO();
   return {};
}
