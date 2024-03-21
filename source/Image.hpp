///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
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
   Image(ImageLibrary*, const Neat&);

   void Refresh() {}
   void Compare(Verb&) const;

   bool Generate(TMeta, Offset = 0);

   NOD() Ref<A::Image> GetLOD(const LOD&) const;
   NOD() void* GetGPUHandle() const noexcept;

private:
   bool FromDescriptor(const Neat&);
   bool FromFile(const Neat&);

   ImageLibrary* mProducer;

   bool ReadPNG(const A::File&);
   bool WritePNG(const A::File&) const;
   bool CompareInner(const Image&) const;
};

