///                                                                           
/// Langulus::Module::Assets::Images                                          
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/Image.hpp>
#include <Langulus/Testing.hpp>


SCENARIO("Loading non-existent file", "[images]") {
   static Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         auto root = Thing::Root<false>(
            "FileSystem",
            "AssetsImages"
         );

         WHEN("The texture is created via abstractions") {
            REQUIRE_THROWS(root.CreateUnit<A::Image>("nonexistent.png"));
            REQUIRE(root.GetUnits().IsEmpty());
         }
         
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The texture is created via tokens") {
            REQUIRE_THROWS(root.CreateUnitToken("Image", "nonexistent.png"));
            REQUIRE(root.GetUnits().IsEmpty());
         }
      #endif

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

SCENARIO("Image creation", "[images]") {
   static Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         auto root = Thing::Root<false>(
            "FileSystem",
            "AssetsImages"
         );
       
         WHEN("The texture is created via abstractions") {
            auto producedTexture = root.CreateUnit<A::Image>("pattern.png");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedTexture.GetCount() == 1);
            REQUIRE(producedTexture.CastsTo<A::Image>(1));
            REQUIRE(producedTexture.IsSparse());
            REQUIRE(root.GetUnits().GetCount() == 1);
         }
  
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The texture is created via tokens") {
            auto producedTexture = root.CreateUnitToken("Image", "pattern.png");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedTexture.GetCount() == 1);
            REQUIRE(producedTexture.CastsTo<A::Image>(1));
            REQUIRE(producedTexture.IsSparse());
            REQUIRE(root.GetUnits().GetCount() == 1);
         }
      #endif

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

