///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"


///                                                                           
///   Font asset                                                              
///                                                                           
struct Font final : A::Texture {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) ImageLibrary;
   LANGULUS_BASES(A::Texture);

public:
   Font(ImageLibrary*, const Descriptor&);

   void Refresh() {}
};

