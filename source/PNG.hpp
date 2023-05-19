///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"


///                                                                           
///   PNG file handler                                                        
///                                                                           
namespace PNG
{
   bool Load(A::File&, A::Texture&);
   bool Write(A::File&, const A::Texture&);
};
