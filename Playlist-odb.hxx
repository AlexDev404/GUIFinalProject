// -*- C++ -*-
//
// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef PLAYLIST_ODB_HXX
#define PLAYLIST_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20475UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "Playlist.hpp"

#include "Albums-odb.hxx"
#include "Artists-odb.hxx"
#include "Genres-odb.hxx"
#include "Track-odb.hxx"

#include <memory>
#include <cstddef>

#include <odb/core.hxx>
#include <odb/traits.hxx>
#include <odb/callback.hxx>
#include <odb/wrapper-traits.hxx>
#include <odb/pointer-traits.hxx>
#include <odb/container-traits.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/result.hxx>
#include <odb/simple-object-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // Playlist
  //
  template <>
  struct class_traits< ::Playlist >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::Playlist >
  {
    public:
    typedef ::Playlist object_type;
    typedef ::Playlist* pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef int id_type;

    static const bool auto_id = true;

    static const bool abstract = false;

    static id_type
    id (const object_type&);

    typedef
    no_op_pointer_cache_traits<pointer_type>
    pointer_cache_traits;

    typedef
    no_op_reference_cache_traits<object_type>
    reference_cache_traits;

    static void
    callback (database&, object_type&, callback_event);

    static void
    callback (database&, const object_type&, callback_event);
  };
}

#include <odb/details/buffer.hxx>

#include <odb/sqlite/version.hxx>
#include <odb/sqlite/forward.hxx>
#include <odb/sqlite/binding.hxx>
#include <odb/sqlite/sqlite-types.hxx>
#include <odb/sqlite/query.hxx>

namespace odb
{
  // Playlist
  //
  template <typename A>
  struct query_columns< ::Playlist, id_sqlite, A >
  {
    // id
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        int,
        sqlite::id_integer >::query_type,
      sqlite::id_integer >
    id_type_;

    static const id_type_ id;

    // name
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    name_type_;

    static const name_type_ name;

    // year
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    year_type_;

    static const year_type_ year;

    // duration
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        double,
        sqlite::id_real >::query_type,
      sqlite::id_real >
    duration_type_;

    static const duration_type_ duration;
  };

  template <typename A>
  const typename query_columns< ::Playlist, id_sqlite, A >::id_type_
  query_columns< ::Playlist, id_sqlite, A >::
  id (A::table_name, "\"id\"", 0);

  template <typename A>
  const typename query_columns< ::Playlist, id_sqlite, A >::name_type_
  query_columns< ::Playlist, id_sqlite, A >::
  name (A::table_name, "\"name\"", 0);

  template <typename A>
  const typename query_columns< ::Playlist, id_sqlite, A >::year_type_
  query_columns< ::Playlist, id_sqlite, A >::
  year (A::table_name, "\"year\"", 0);

  template <typename A>
  const typename query_columns< ::Playlist, id_sqlite, A >::duration_type_
  query_columns< ::Playlist, id_sqlite, A >::
  duration (A::table_name, "\"duration\"", 0);

  template <typename A>
  struct pointer_query_columns< ::Playlist, id_sqlite, A >:
    query_columns< ::Playlist, id_sqlite, A >
  {
  };

  template <>
  class access::object_traits_impl< ::Playlist, id_sqlite >:
    public access::object_traits< ::Playlist >
  {
    public:
    struct id_image_type
    {
      long long id_value;
      bool id_null;

      std::size_t version;
    };

    struct image_type
    {
      // id_
      //
      long long id_value;
      bool id_null;

      // name_
      //
      details::buffer name_value;
      std::size_t name_size;
      bool name_null;

      // year_
      //
      details::buffer year_value;
      std::size_t year_size;
      bool year_null;

      // duration_
      //
      double duration_value;
      bool duration_null;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    using object_traits<object_type>::id;

    static id_type
    id (const id_image_type&);

    static id_type
    id (const image_type&);

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (sqlite::bind*,
          image_type&,
          sqlite::statement_kind);

    static void
    bind (sqlite::bind*, id_image_type&);

    static bool
    init (image_type&,
          const object_type&,
          sqlite::statement_kind);

    static void
    init (object_type&,
          const image_type&,
          database*);

    static void
    init (id_image_type&, const id_type&);

    typedef sqlite::object_statements<object_type> statements_type;

    typedef sqlite::query_base query_base_type;

    static const std::size_t column_count = 4UL;
    static const std::size_t id_column_count = 1UL;
    static const std::size_t inverse_column_count = 0UL;
    static const std::size_t readonly_column_count = 0UL;
    static const std::size_t managed_optimistic_column_count = 0UL;

    static const std::size_t separate_load_column_count = 0UL;
    static const std::size_t separate_update_column_count = 0UL;

    static const bool versioned = false;

    static const char persist_statement[];
    static const char find_statement[];
    static const char update_statement[];
    static const char erase_statement[];
    static const char query_statement[];
    static const char erase_query_statement[];

    static const char table_name[];

    static void
    persist (database&, object_type&);

    static pointer_type
    find (database&, const id_type&);

    static bool
    find (database&, const id_type&, object_type&);

    static bool
    reload (database&, object_type&);

    static void
    update (database&, const object_type&);

    static void
    erase (database&, const id_type&);

    static void
    erase (database&, const object_type&);

    static result<object_type>
    query (database&, const query_base_type&);

    static unsigned long long
    erase_query (database&, const query_base_type&);

    public:
    static bool
    find_ (statements_type&,
           const id_type*);

    static void
    load_ (statements_type&,
           object_type&,
           bool reload);
  };

  template <>
  class access::object_traits_impl< ::Playlist, id_common >:
    public access::object_traits_impl< ::Playlist, id_sqlite >
  {
  };

  // Playlist
  //
}

#include "Playlist-odb.ixx"

#include <odb/post.hxx>

#endif // PLAYLIST_ODB_HXX
