///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Image.hpp"
#include <Flow/Verbs/Create.hpp>


///                                                                           
///   Image reading, writing, and generation module                           
///                                                                           
struct ImageLibrary final : A::AssetModule {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(A::AssetModule);
   LANGULUS_VERBS(Verbs::Create);

private:
   // Where images are read and written to                              
   Path mImageFolder;
   // Image library                                                     
   TFactoryUnique<::Image> mImages;
   // Data folder, where images will be saved or loaded from            
   Ref<A::Folder> mFolder;

public:
   ImageLibrary(Runtime*, const Neat&);

   void Create(Verb&);

   const A::Folder* GetFolder() const noexcept;
};

