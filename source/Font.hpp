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
struct Font final : A::Image {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) ImageLibrary;
   LANGULUS_BASES(A::Image);

public:
   Font(ImageLibrary*, const Neat&);

   void Refresh() {}

   NOD() Ref<A::Image> GetLOD(const Math::LOD&) const;
   NOD() void* GetGPUHandle() const noexcept;
};

