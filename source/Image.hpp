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
///   Image art                                                               
struct Image final : Things::Image {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = ImageLibrary;
   using CTTI_Files     = Yes<"png">;
   using CTTI_Bases     = Things::Image;
   using CTTI_Ability   = Verbs::Compare;

private:
   bool FromDescriptor(Many const&);
   bool FromFile(Many const&);
   bool ReadPNG(Things::File const&);
   bool WritePNG(Things::File const&) const;
   bool CompareInner(const Image&) const;

public:
   Image(ImageLibrary*, Many const&);

   void Refresh() override {}
   bool Generate(TMeta, size_t = 0) override;
   auto GetGPUHandle() const noexcept -> void* override;
   auto GetLOD(LOD const&) const -> Ref<Things::Image> override;

   void Compare(Verb&) const;

   auto GetLibrary() const -> ImageLibrary*;
};

