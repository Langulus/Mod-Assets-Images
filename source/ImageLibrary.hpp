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
   // Image library                                                     
   TFactoryUnique<Image> mImages;
   // Data folder, where images will be saved or loaded from            
   Ptr<A::Folder> mFolder;
   // Default place to search for images, can be configured on mod load 
   static constexpr auto DefaultTextureFolder = "assets/images";

public:
   ImageLibrary(Runtime*, const Descriptor&);

   void Update(Time);
   void Create(Verb&);
};

