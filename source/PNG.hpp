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
   bool Read(const A::File&, A::Image&);
   bool Write(const A::File&, const A::Image&);
};
