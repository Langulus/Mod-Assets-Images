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

public:
   Image(ImageLibrary*, Describe);

   void Refresh() {}
   void Compare(Verb&) const;

   bool Generate(TMeta, Offset = 0);

   NOD() Ref<A::Image> GetLOD(const LOD&) const;
   NOD() void* GetGPUHandle() const noexcept;

private:
   bool FromDescriptor(Describe);
   bool FromFile(Describe);

   ImageLibrary* mProducer;

   bool ReadPNG(const A::File&);
   bool WritePNG(const A::File&) const;
   bool CompareInner(const Image&) const;
};

