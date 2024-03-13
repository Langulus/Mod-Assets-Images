///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Langulus.hpp>
#include <Langulus/Image.hpp>

using namespace Langulus;
using namespace Math;

struct ImageLibrary;
struct Image;

#define VERBOSE_IMAGES(...)      Logger::Verbose(Self(), __VA_ARGS__)
#define VERBOSE_IMAGES_TAB(...)  const auto tab = Logger::Verbose(Self(), __VA_ARGS__, Logger::Tabs {})
