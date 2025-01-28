///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Image.hpp>

using namespace Langulus;
using namespace Math;

struct ImageLibrary;
struct Image;

#if 0
   #define VERBOSE_IMAGES(...)      Logger::Verbose(Self(), __VA_ARGS__)
   #define VERBOSE_IMAGES_TAB(...)  const auto tab = Logger::VerboseTab(Self(), __VA_ARGS__)
#else
   #define VERBOSE_IMAGES(...)      LANGULUS(NOOP)
   #define VERBOSE_IMAGES_TAB(...)  LANGULUS(NOOP)
#endif
