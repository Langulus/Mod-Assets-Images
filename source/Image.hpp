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
struct Image final : A::Image {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) ImageLibrary;
   LANGULUS(FILES) "png";
   LANGULUS_BASES(A::Image);

public:
   Image(ImageLibrary*, const Descriptor&);

   void Refresh() {}

   void Upload(const Any&);
   void Upload(Any&&);
   NOD() Ref<A::Image> GetLOD(const LOD&) const;
   NOD() void* GetGPUHandle() const noexcept;

private:
   void LoadFile(const Any&);
};

