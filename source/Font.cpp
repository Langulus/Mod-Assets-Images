///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Font.hpp"
#include "ImageLibrary.hpp"


/// Font descriptor-constructor                                               
///   @param producer - the image producer                                    
///   @param descriptor - instructions for configuring the image              
Font::Font(ImageLibrary* producer, const Many& descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor} {}

/// Generate a level of detail                                                
///   @param level-of-detail observation point                                
///   @return the generated LOD image                                         
auto Font::GetLOD(const Math::LOD&) const -> Ref<A::Image> {
   TODO();
   return {};
}

/// Get a GPU handle                                                          
///   @return an opaque pointer representing the handle                       
auto Font::GetGPUHandle() const noexcept -> void* {
   TODO();
   return {};
}
