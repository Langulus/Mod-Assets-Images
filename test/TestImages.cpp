///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright(C) 2016 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Main.hpp"
#include <Entity/Thing.hpp>
#include <catch2/catch.hpp>

/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("Image creation", "[images]") {
   Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         Thing root;
         root.SetName("ROOT");

         // Create runtime at the root                                  
         root.CreateRuntime();

         // Load modules                                                
         root.LoadMod("FileSystem");
         root.LoadMod("AssetsImages");
         
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The texture is created via tokens") {
            auto producedTexture = root.CreateUnitToken("Image", "pattern.png");

            // Update once                                              
            root.Update(Time::zero());
            
            THEN("Various traits change") {
               root.DumpHierarchy();

               REQUIRE(producedTexture.GetCount() == 1);
               REQUIRE(producedTexture.CastsTo<A::Image>(1));
               REQUIRE(producedTexture.IsSparse());
            }
         }
      #endif

         WHEN("The texture is created via abstractions") {
            auto producedTexture = root.CreateUnit<A::Image>("pattern.png");

            // Update once                                              
            root.Update(Time::zero());

            THEN("Various traits change") {
               root.DumpHierarchy();

               REQUIRE(producedTexture.GetCount() == 1);
               REQUIRE(producedTexture.CastsTo<A::Image>(1));
               REQUIRE(producedTexture.IsSparse());
            }
         }

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

