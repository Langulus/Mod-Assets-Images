///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Export.hpp"
#include <Langulus/Verbs/Compare.hpp>


///                                                                           
///   Image asset                                                             
///                                                                           
struct Image final : Things::Image {
   using CTTI_Abstract = No;
   using CTTI_Producer = ImageLibrary;
   LANGULUS(FILES) "png";
   LANGULUS_BASES(Things::Image);
   LANGULUS_VERBS(Verbs::Compare);

private:
   bool FromDescriptor(Many const&);
   bool FromFile(Many const&);
   bool ReadPNG(const A::File&);
   bool WritePNG(const A::File&) const;
   bool CompareInner(const Image&) const;

public:
   Image(ImageLibrary*, Many const&);

   void Refresh() {}
   void Compare(Verb&) const;
   bool Generate(TMeta, size_t = 0);

   auto GetLOD(const LOD&) const -> Ref<Things::Image>;
   auto GetGPUHandle() const noexcept -> void*;
   auto GetLibrary() const -> ImageLibrary*;
};

