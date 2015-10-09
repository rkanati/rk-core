workspace "rk-core"
  configurations { "debug", "release" }

project "rk-core"
  kind "StaticLib"
  language "C++"
  targetdir "%{cfg.buildcfg}/lib"

  includedirs { "include" }
  files { "include/Rk/**.hpp", "src/**.hpp", "src/**.cpp" }

  filter "configurations:debug"
    defines { "DEBUG" }
    flags { "Symbols" }

  filter "configurations:release"
    defines { "NDEBUG" }
    optimize "On"

  filter "action:gmake" -- FIXME: this should be toolset:gcc, but toolset: is broken in premake5 as of 2015-09-01
    buildoptions { "-std=c++14" }

