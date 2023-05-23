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
///   Image asset                                                             
///                                                                           
struct Image final : A::Texture {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) ImageLibrary;
   LANGULUS_BASES(A::Texture);

public:
   Image(ImageLibrary*, const Descriptor&);

   void Refresh() {}

   NOD() const A::Texture* GetLOD(const Math::LOD&) const;
   NOD() void* GetGPUHandle() const noexcept;
};

