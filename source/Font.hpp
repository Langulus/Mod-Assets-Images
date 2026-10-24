///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Export.hpp"


///                                                                           
///   Font asset                                                              
///                                                                           
struct Font final : Things::Image {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = ImageLibrary;
   using CTTI_Bases     = Things::Image;

public:
   Font(ImageLibrary*, Many const&);

   void Refresh() {}

   bool Generate(TMeta, size_t = 0) {
      TODO();
      return false;
   }

   auto GetLOD(Math::LOD const&) const -> Ref<Things::Image> override;
   auto GetGPUHandle() const noexcept -> void*;
};

