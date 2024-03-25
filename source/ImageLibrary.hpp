///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Image.hpp"


///                                                                           
///   Image reading, writing, and generation module                           
///                                                                           
struct ImageLibrary final : A::AssetModule {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(A::AssetModule);
   LANGULUS_VERBS(Verbs::Create);

private:
   // Image library                                                     
   TFactoryUnique<::Image> mImages;

public:
   ImageLibrary(Runtime*, const Neat&);

   void Create(Verb&);
};

