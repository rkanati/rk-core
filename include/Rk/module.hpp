//
// Copyright (C) 2013 Rk
// Permission to use, copy, modify and distribute this file for any purpose is hereby granted without fee or other
// limitation, provided that
//  - this notice is reproduced verbatim in all copies and modifications of this file, and
//  - neither the names nor trademarks of the copyright holder are used to endorse any work which includes or is derived
//    from this file.
// No warranty applies to the use, either of this software, or any modification of it, and Rk disclaims all liabilities
// in relation to such use.
//

#pragma once

#include <Rk/unicode_convert.hpp>
#include <Rk/exception.hpp>
#include <Rk/memory.hpp>
#include <Rk/guard.hpp>
#include <Rk/types.hpp>

namespace Rk {
  namespace detail {
    typedef void (__stdcall *sysfn_t) ();

    extern "C" {
      #define fromdll(t) __declspec(dllimport) t __stdcall
      fromdll (void*)   __stdcall LoadLibraryW   (const wchar_t*);
      fromdll (sysfn_t) __stdcall GetProcAddress (void*, const char*);
      fromdll (i32)     __stdcall FreeLibrary    (void*);
      fromdll (u32)     __stdcall GetLastError   ();
      #undef fromdll
    }

    static void* load_library (u16string_ref path) {
      return LoadLibraryW ((const wchar*) to_string (path).c_str ());
    }

    static void* load_library (cstring_ref path) {
      return LoadLibraryW ((const wchar*) string_utf8_to_16 (path).c_str ());
    }

    class module_stub_base {
    public:
      virtual std::shared_ptr <void> create_root () = 0;
    };

    template <typename root_t>
    class module_stub :
      public module_stub_base
    {
      raw_storage <root_t> store;

      struct deleter {
        void operator () (root_t* p) const { p -> ~root_t (); }
      };

      auto create_root ()
        -> std::shared_ptr <void>
      {
        auto ptr = new (store.raw ()) root_t;
        auto root_guard = guard ([ptr] { ptr -> ~root_t (); });
        auto shared = std::shared_ptr <root_t> (ptr, deleter ());
        root_guard.relieve ();
        return shared;
      }
    };

    template <typename root_t>
    struct module_impl {
      void*                    handle;
      std::shared_ptr <root_t> root;

      module_impl () = default;

      ~module_impl () {
        root.reset ();
        detail::FreeLibrary (handle);
      }
    };
  } // detail

  #define RK_MODULE(root_t) \
    static Rk::detail::module_stub <root_t> rk_module_stub_impl; \
    extern "C" __declspec (dllexport) Rk::detail::module_stub_base* rk_module_stub = &rk_module_stub_impl;

  template <typename root_t>
  std::shared_ptr <root_t> load_module (cstring_ref path) {
    using namespace detail;

    auto impl = std::make_shared <module_impl <root_t>> ();

    impl -> handle = load_library (path);
    if (!impl -> handle)
      throw win_error ("load_module: LoadLibraryW failed");

    auto stub_ptr = reinterpret_cast <module_stub_base**> (GetProcAddress (impl -> handle, "rk_module_stub"));
    if (!stub_ptr)
      throw win_error ("load_module: GetProcAddress failed");

    auto root = (*stub_ptr) -> create_root ();
    impl -> root = std::static_pointer_cast <root_t> (root);

    return std::shared_ptr <root_t> (std::move (impl), impl -> root.get ());
  }
}

