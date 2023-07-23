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

#if LANGULUS_FEATURE(MEMORY_STATISTICS)
static bool statistics_provided = false;
static Anyness::Allocator::Statistics memory_statistics;
#endif

/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("Image creation", "[images]") {
   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         Thing root;
         root.SetName("ROOT"_text);

         // Create runtime at the root                                  
         root.CreateRuntime();

         // Load modules                                                
         root.LoadMod("GLFW");
         root.LoadMod("Vulkan");
         root.LoadMod("FileSystem");
         root.LoadMod("AssetsImages");

         WHEN("The texture is created via tokens") {
            auto producedWindow = root.CreateUnitToken("Window", Traits::Size(640, 480));
            auto producedRenderer = root.CreateUnitToken("Renderer");
            auto producedTexture = root.CreateUnitToken("Texture", "pattern.png");

            // Update once                                              
            root.Update(Time::zero());
            
            THEN("Various traits change") {
               root.DumpHierarchy();

               REQUIRE(producedWindow.GetCount() == 1);
               REQUIRE(producedWindow.CastsTo<A::Window>(1));
               REQUIRE(producedWindow.IsSparse());

               REQUIRE(producedRenderer.GetCount() == 1);
               REQUIRE(producedRenderer.CastsTo<A::Renderer>(1));
               REQUIRE(producedRenderer.IsSparse());

               REQUIRE(producedTexture.GetCount() == 1);
               REQUIRE(producedTexture.CastsTo<A::Texture>(1));
               REQUIRE(producedTexture.IsSparse());
            }
         }

         WHEN("The texture is created via abstractions") {
            auto producedWindow = root.CreateUnit<A::Window>(Traits::Size(640, 480));
            auto producedRenderer = root.CreateUnit<A::Renderer>();
            auto producedTexture = root.CreateUnit<A::Texture>("pattern.png");

            // Update once                                              
            root.Update(Time::zero());

            THEN("Various traits change") {
               root.DumpHierarchy();

               REQUIRE(producedWindow.GetCount() == 1);
               REQUIRE(producedWindow.CastsTo<A::Window>(1));
               REQUIRE(producedWindow.IsSparse());

               REQUIRE(producedRenderer.GetCount() == 1);
               REQUIRE(producedRenderer.CastsTo<A::Renderer>(1));
               REQUIRE(producedRenderer.IsSparse());

               REQUIRE(producedTexture.GetCount() == 1);
               REQUIRE(producedTexture.CastsTo<A::Texture>(1));
               REQUIRE(producedTexture.IsSparse());
            }
         }

         #if LANGULUS_FEATURE(MEMORY_STATISTICS)
            Fractalloc.CollectGarbage();

            // Detect memory leaks                                      
            if (statistics_provided) {
               if (memory_statistics != Fractalloc.GetStatistics()) {
                  Fractalloc.DumpPools();
                  memory_statistics = Fractalloc.GetStatistics();
                  FAIL("Memory leak detected");
               }
            }

            memory_statistics = Fractalloc.GetStatistics();
            statistics_provided = true;
         #endif
      }
   }
}

