///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Image.hpp"
#include <Langulus/Factory.hpp>


///                                                                           
///   Image reading, writing, and generation module                           
///                                                                           
struct ImageLibrary final : Things::AssetModule {
   using CTTI_Abstract = No;
   LANGULUS_BASES(A::AssetModule);
   LANGULUS_VERBS(Verbs::Create);

private:
   // Image library                                                     
   TFactoryUnique<::Image> mImages;

public:
   ImageLibrary(Runtime*, Many const&);

   void Create(Verb&);
   void Teardown();
   void RequestGarbageCollection() {}
};

