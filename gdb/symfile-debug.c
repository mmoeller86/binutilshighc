/* Debug logging for the symbol file functions for the GNU debugger, GDB.

   Copyright (C) 2013-2021 Free Software Foundation, Inc.

   Contributed by Cygnus Support, using pieces from other GDB modules.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Note: Be careful with functions that can throw errors.
   We want to see a logging message regardless of whether an error was thrown.
   This typically means printing a message before calling the real function
   and then if the function returns a result printing a message after it
   returns.  */

#include "defs.h"
#include "gdbcmd.h"
#include "objfiles.h"
#include "observable.h"
#include "source.h"
#include "symtab.h"
#include "symfile.h"

/* We need to save a pointer to the real symbol functions.
   Plus, the debug versions are malloc'd because we have to NULL out the
   ones that are NULL in the real copy.  */

struct debug_sym_fns_data
{
  const struct sym_fns *real_sf = nullptr;
  struct sym_fns debug_sf {};
};

/* We need to record a pointer to the real set of functions for each
   objfile.  */
static const struct objfile_key<debug_sym_fns_data>
  symfile_debug_objfile_data_key;

/* If true all calls to the symfile functions are logged.  */
static bool debug_symfile = false;

/* Return non-zero if symfile debug logging is installed.  */

static int
symfile_debug_installed (struct objfile *objfile)
{
  return (objfile->sf != NULL
	  && symfile_debug_objfile_data_key.get (objfile) != NULL);
}

/* Utility return the name to print for SYMTAB.  */

static const char *
debug_symtab_name (struct symtab *symtab)
{
  return symtab_to_filename_for_display (symtab);
}


/* See objfiles.h.  */

bool
objfile::has_partial_symbols ()
{
  bool retval = false;

  /* If we have not read psymbols, but we have a function capable of reading
     them, then that is an indication that they are in fact available.  Without
     this function the symbols may have been already read in but they also may
     not be present in this objfile.  */
  if ((flags & OBJF_PSYMTABS_READ) == 0
      && qf != nullptr
      && qf->can_lazily_read_symbols ())
    retval = true;
  else if (qf != nullptr)
    retval = qf->has_symbols (this);

  if (debug_symfile)
    fprintf_filtered (gdb_stdlog, "qf->has_symbols (%s) = %d\n",
		      objfile_debug_name (this), retval);

  return retval;
}

struct symtab *
objfile::find_last_source_symtab ()
{
  struct symtab *retval = nullptr;

  if (debug_symfile)
    fprintf_filtered (gdb_stdlog, "qf->find_last_source_symtab (%s)\n",
		      objfile_debug_name (this));

  if (qf != nullptr)
    retval = qf->find_last_source_symtab (this);

  if (debug_symfile)
    fprintf_filtered (gdb_stdlog, "qf->find_last_source_symtab (...) = %s\n",
		      retval ? debug_symtab_name (retval) : "NULL");

  return retval;
}

void
objfile::forget_cached_source_info ()
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog, "qf->forget_cached_source_info (%s)\n",
		      objfile_debug_name (this));

  if (qf != nullptr)
    qf->forget_cached_source_info (this);
}

bool
objfile::map_symtabs_matching_filename
  (const char *name, const char *real_path,
   gdb::function_view<bool (symtab *)> callback)
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->map_symtabs_matching_filename (%s, \"%s\", "
		      "\"%s\", %s)\n",
		      objfile_debug_name (this), name,
		      real_path ? real_path : NULL,
		      host_address_to_string (&callback));

  bool retval = false;
  if (qf != nullptr)
    retval = (qf->map_symtabs_matching_filename
	      (this, name, real_path, callback));

  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->map_symtabs_matching_filename (...) = %d\n",
		      retval);

  return retval;
}

struct compunit_symtab *
objfile::lookup_symbol (block_enum kind, const char *name, domain_enum domain)
{
  struct compunit_symtab *retval = nullptr;

  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->lookup_symbol (%s, %d, \"%s\", %s)\n",
		      objfile_debug_name (this), kind, name,
		      domain_name (domain));

  if (qf != nullptr)
    retval = qf->lookup_symbol (this, kind, name, domain);

  if (debug_symfile)
    fprintf_filtered (gdb_stdlog, "qf->lookup_symbol (...) = %s\n",
		      retval
		      ? debug_symtab_name (compunit_primary_filetab (retval))
		      : "NULL");

  return retval;
}

void
objfile::print_stats (bool print_bcache)
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog, "qf->print_stats (%s, %d)\n",
		      objfile_debug_name (this), print_bcache);

  if (qf != nullptr)
    qf->print_stats (this, print_bcache);
}

void
objfile::dump ()
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog, "qf->dump (%s)\n",
		      objfile_debug_name (this));

  if (qf != nullptr)
    qf->dump (this);
}

void
objfile::expand_symtabs_for_function (const char *func_name)
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->expand_symtabs_for_function (%s, \"%s\")\n",
		      objfile_debug_name (this), func_name);

  if (qf != nullptr)
    qf->expand_symtabs_for_function (this, func_name);
}

void
objfile::expand_all_symtabs ()
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog, "qf->expand_all_symtabs (%s)\n",
		      objfile_debug_name (this));

  if (qf != nullptr)
    qf->expand_all_symtabs (this);
}

void
objfile::expand_symtabs_with_fullname (const char *fullname)
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->expand_symtabs_with_fullname (%s, \"%s\")\n",
		      objfile_debug_name (this), fullname);

  if (qf != nullptr)
    qf->expand_symtabs_with_fullname (this, fullname);
}

void
objfile::map_matching_symbols
  (const lookup_name_info &name, domain_enum domain,
   int global,
   gdb::function_view<symbol_found_callback_ftype> callback,
   symbol_compare_ftype *ordered_compare)
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->map_matching_symbols (%s, %s, %d, %s)\n",
		      objfile_debug_name (this),
		      domain_name (domain), global,
		      host_address_to_string (ordered_compare));

  if (qf != nullptr)
    qf->map_matching_symbols (this, name, domain, global,
			      callback, ordered_compare);
}

void
objfile::expand_symtabs_matching
  (gdb::function_view<expand_symtabs_file_matcher_ftype> file_matcher,
   const lookup_name_info *lookup_name,
   gdb::function_view<expand_symtabs_symbol_matcher_ftype> symbol_matcher,
   gdb::function_view<expand_symtabs_exp_notify_ftype> expansion_notify,
   enum search_domain kind)
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->expand_symtabs_matching (%s, %s, %s, %s, %s)\n",
		      objfile_debug_name (this),
		      host_address_to_string (&file_matcher),
		      host_address_to_string (&symbol_matcher),
		      host_address_to_string (&expansion_notify),
		      search_domain_name (kind));

  if (qf != nullptr)
    qf->expand_symtabs_matching (this, file_matcher, lookup_name,
				 symbol_matcher, expansion_notify, kind);
}

struct compunit_symtab *
objfile::find_pc_sect_compunit_symtab (struct bound_minimal_symbol msymbol,
				       CORE_ADDR pc,
				       struct obj_section *section,
				       int warn_if_readin)
{
  struct compunit_symtab *retval = nullptr;

  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->find_pc_sect_compunit_symtab (%s, %s, %s, %s, %d)\n",
		      objfile_debug_name (this),
		      host_address_to_string (msymbol.minsym),
		      hex_string (pc),
		      host_address_to_string (section),
		      warn_if_readin);

  if (qf != nullptr)
    retval = qf->find_pc_sect_compunit_symtab (this, msymbol, pc, section,
					       warn_if_readin);

  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->find_pc_sect_compunit_symtab (...) = %s\n",
		      retval
		      ? debug_symtab_name (compunit_primary_filetab (retval))
		      : "NULL");

  return retval;
}

void
objfile::map_symbol_filenames (symbol_filename_ftype *fun, void *data,
			       int need_fullname)
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->map_symbol_filenames (%s, %s, %s, %d)\n",
		      objfile_debug_name (this),
		      host_address_to_string (fun),
		      host_address_to_string (data),
		      need_fullname);

  if (qf != nullptr)
    qf->map_symbol_filenames (this, fun, data, need_fullname);
}

struct compunit_symtab *
objfile::find_compunit_symtab_by_address (CORE_ADDR address)
{
  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->find_compunit_symtab_by_address (%s, %s)\n",
		      objfile_debug_name (this),
		      hex_string (address));

  struct compunit_symtab *result = NULL;
  if (qf != nullptr)
    result = qf->find_compunit_symtab_by_address (this, address);

  if (debug_symfile)
    fprintf_filtered (gdb_stdlog,
		      "qf->find_compunit_symtab_by_address (...) = %s\n",
		      result
		      ? debug_symtab_name (compunit_primary_filetab (result))
		      : "NULL");

  return result;
}

enum language
objfile::lookup_global_symbol_language (const char *name,
					domain_enum domain,
					bool *symbol_found_p)
{
  enum language result = language_unknown;

  if (qf != nullptr)
    result = qf->lookup_global_symbol_language (this, name, domain,
						symbol_found_p);
  else
    *symbol_found_p = false;

  return result;
}


/* Debugging version of struct sym_probe_fns.  */

static const std::vector<std::unique_ptr<probe>> &
debug_sym_get_probes (struct objfile *objfile)
{
  const struct debug_sym_fns_data *debug_data
    = symfile_debug_objfile_data_key.get (objfile);

  const std::vector<std::unique_ptr<probe>> &retval
    = debug_data->real_sf->sym_probe_fns->sym_get_probes (objfile);

  fprintf_filtered (gdb_stdlog,
		    "probes->sym_get_probes (%s) = %s\n",
		    objfile_debug_name (objfile),
		    host_address_to_string (retval.data ()));

  return retval;
}

static const struct sym_probe_fns debug_sym_probe_fns =
{
  debug_sym_get_probes,
};

/* Debugging version of struct sym_fns.  */

static void
debug_sym_new_init (struct objfile *objfile)
{
  const struct debug_sym_fns_data *debug_data
    = symfile_debug_objfile_data_key.get (objfile);

  fprintf_filtered (gdb_stdlog, "sf->sym_new_init (%s)\n",
		    objfile_debug_name (objfile));

  debug_data->real_sf->sym_new_init (objfile);
}

static void
debug_sym_init (struct objfile *objfile)
{
  const struct debug_sym_fns_data *debug_data
    = symfile_debug_objfile_data_key.get (objfile);

  fprintf_filtered (gdb_stdlog, "sf->sym_init (%s)\n",
		    objfile_debug_name (objfile));

  debug_data->real_sf->sym_init (objfile);
}

static void
debug_sym_read (struct objfile *objfile, symfile_add_flags symfile_flags)
{
  const struct debug_sym_fns_data *debug_data
    = symfile_debug_objfile_data_key.get (objfile);

  fprintf_filtered (gdb_stdlog, "sf->sym_read (%s, 0x%x)\n",
		    objfile_debug_name (objfile), (unsigned) symfile_flags);

  debug_data->real_sf->sym_read (objfile, symfile_flags);
}

static void
debug_sym_finish (struct objfile *objfile)
{
  const struct debug_sym_fns_data *debug_data
    = symfile_debug_objfile_data_key.get (objfile);

  fprintf_filtered (gdb_stdlog, "sf->sym_finish (%s)\n",
		    objfile_debug_name (objfile));

  debug_data->real_sf->sym_finish (objfile);
}

static void
debug_sym_offsets (struct objfile *objfile,
		   const section_addr_info &info)
{
  const struct debug_sym_fns_data *debug_data
    = symfile_debug_objfile_data_key.get (objfile);

  fprintf_filtered (gdb_stdlog, "sf->sym_offsets (%s, %s)\n",
		    objfile_debug_name (objfile),
		    host_address_to_string (&info));

  debug_data->real_sf->sym_offsets (objfile, info);
}

static symfile_segment_data_up
debug_sym_segments (bfd *abfd)
{
  /* This API function is annoying, it doesn't take a "this" pointer.
     Fortunately it is only used in one place where we (re-)lookup the
     sym_fns table to use.  Thus we will never be called.  */
  gdb_assert_not_reached ("debug_sym_segments called");
}

static void
debug_sym_read_linetable (struct objfile *objfile)
{
  const struct debug_sym_fns_data *debug_data
    = symfile_debug_objfile_data_key.get (objfile);

  fprintf_filtered (gdb_stdlog, "sf->sym_read_linetable (%s)\n",
		    objfile_debug_name (objfile));

  debug_data->real_sf->sym_read_linetable (objfile);
}

static bfd_byte *
debug_sym_relocate (struct objfile *objfile, asection *sectp, bfd_byte *buf)
{
  const struct debug_sym_fns_data *debug_data
    = symfile_debug_objfile_data_key.get (objfile);
  bfd_byte *retval;

  retval = debug_data->real_sf->sym_relocate (objfile, sectp, buf);

  fprintf_filtered (gdb_stdlog,
		    "sf->sym_relocate (%s, %s, %s) = %s\n",
		    objfile_debug_name (objfile),
		    host_address_to_string (sectp),
		    host_address_to_string (buf),
		    host_address_to_string (retval));

  return retval;
}

/* Template of debugging version of struct sym_fns.
   A copy is made, with sym_flavour updated, and a pointer to the real table
   installed in real_sf, and then a pointer to the copy is installed in the
   objfile.  */

static const struct sym_fns debug_sym_fns =
{
  debug_sym_new_init,
  debug_sym_init,
  debug_sym_read,
  debug_sym_finish,
  debug_sym_offsets,
  debug_sym_segments,
  debug_sym_read_linetable,
  debug_sym_relocate,
  &debug_sym_probe_fns,
};

/* Install the debugging versions of the symfile functions for OBJFILE.
   Do not call this if the debug versions are already installed.  */

static void
install_symfile_debug_logging (struct objfile *objfile)
{
  const struct sym_fns *real_sf;
  struct debug_sym_fns_data *debug_data;

  /* The debug versions should not already be installed.  */
  gdb_assert (!symfile_debug_installed (objfile));

  real_sf = objfile->sf;

  /* Alas we have to preserve NULL entries in REAL_SF.  */
  debug_data = new struct debug_sym_fns_data;

#define COPY_SF_PTR(from, to, name, func)	\
  do {						\
    if ((from)->name)				\
      (to)->debug_sf.name = func;		\
  } while (0)

  COPY_SF_PTR (real_sf, debug_data, sym_new_init, debug_sym_new_init);
  COPY_SF_PTR (real_sf, debug_data, sym_init, debug_sym_init);
  COPY_SF_PTR (real_sf, debug_data, sym_read, debug_sym_read);
  COPY_SF_PTR (real_sf, debug_data, sym_finish, debug_sym_finish);
  COPY_SF_PTR (real_sf, debug_data, sym_offsets, debug_sym_offsets);
  COPY_SF_PTR (real_sf, debug_data, sym_segments, debug_sym_segments);
  COPY_SF_PTR (real_sf, debug_data, sym_read_linetable,
	       debug_sym_read_linetable);
  COPY_SF_PTR (real_sf, debug_data, sym_relocate, debug_sym_relocate);
  if (real_sf->sym_probe_fns)
    debug_data->debug_sf.sym_probe_fns = &debug_sym_probe_fns;

#undef COPY_SF_PTR

  debug_data->real_sf = real_sf;
  symfile_debug_objfile_data_key.set (objfile, debug_data);
  objfile->sf = &debug_data->debug_sf;
}

/* Uninstall the debugging versions of the symfile functions for OBJFILE.
   Do not call this if the debug versions are not installed.  */

static void
uninstall_symfile_debug_logging (struct objfile *objfile)
{
  struct debug_sym_fns_data *debug_data;

  /* The debug versions should be currently installed.  */
  gdb_assert (symfile_debug_installed (objfile));

  debug_data = symfile_debug_objfile_data_key.get (objfile);

  objfile->sf = debug_data->real_sf;
  symfile_debug_objfile_data_key.clear (objfile);
}

/* Call this function to set OBJFILE->SF.
   Do not set OBJFILE->SF directly.  */

void
objfile_set_sym_fns (struct objfile *objfile, const struct sym_fns *sf)
{
  if (symfile_debug_installed (objfile))
    {
      gdb_assert (debug_symfile);
      /* Remove the current one, and reinstall a new one later.  */
      uninstall_symfile_debug_logging (objfile);
    }

  /* Assume debug logging is disabled.  */
  objfile->sf = sf;

  /* Turn debug logging on if enabled.  */
  if (debug_symfile)
    install_symfile_debug_logging (objfile);
}

static void
set_debug_symfile (const char *args, int from_tty, struct cmd_list_element *c)
{
  for (struct program_space *pspace : program_spaces)
    for (objfile *objfile : pspace->objfiles ())
      {
	if (debug_symfile)
	  {
	    if (!symfile_debug_installed (objfile))
	      install_symfile_debug_logging (objfile);
	  }
	else
	  {
	    if (symfile_debug_installed (objfile))
	      uninstall_symfile_debug_logging (objfile);
	  }
      }
}

static void
show_debug_symfile (struct ui_file *file, int from_tty,
			struct cmd_list_element *c, const char *value)
{
  fprintf_filtered (file, _("Symfile debugging is %s.\n"), value);
}

void _initialize_symfile_debug ();
void
_initialize_symfile_debug ()
{
  add_setshow_boolean_cmd ("symfile", no_class, &debug_symfile, _("\
Set debugging of the symfile functions."), _("\
Show debugging of the symfile functions."), _("\
When enabled, all calls to the symfile functions are logged."),
			   set_debug_symfile, show_debug_symfile,
			   &setdebuglist, &showdebuglist);

  /* Note: We don't need a new-objfile observer because debug logging
     will be installed when objfile init'n calls objfile_set_sym_fns.  */
}
