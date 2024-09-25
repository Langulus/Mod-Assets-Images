///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Common.hpp"


///                                                                           
///   Font asset                                                              
///                                                                           
struct Font final : A::Image {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) ImageLibrary;
   LANGULUS_BASES(A::Image);

public:
   Font(ImageLibrary*, Describe);

   void Refresh() {}
   bool Generate(TMeta, Offset = 0) {
      TODO();
      return false;
   }

   NOD() Ref<A::Image> GetLOD(const Math::LOD&) const;
   NOD() void* GetGPUHandle() const noexcept;
};

