///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Verbs/Compare.hpp>


///                                                                           
///   Image asset                                                             
///                                                                           
struct Image final : A::Image {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) ImageLibrary;
   LANGULUS(FILES) "png";
   LANGULUS_BASES(A::Image);
   LANGULUS_VERBS(Verbs::Compare);

private:
   bool FromDescriptor(const Many&);
   bool FromFile(const Many&);
   bool ReadPNG(const A::File&);
   bool WritePNG(const A::File&) const;
   bool CompareInner(const Image&) const;

public:
   Image(ImageLibrary*, const Many&);

   void Refresh() {}
   void Compare(Verb&) const;
   bool Generate(TMeta, Offset = 0);

   auto GetLOD(const LOD&) const -> Ref<A::Image>;
   auto GetGPUHandle() const noexcept -> void*;
   auto GetLibrary() const -> ImageLibrary*;
};

