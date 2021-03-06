/*
 * Copyright (C) 2009-2010 Ole Andr� Vadla Ravn�s <ole.andre.ravnas@tandberg.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "gumx86relocator.h"

#include "gummemory.h"
#include "testutil.h"

#include <string.h>

#define RELOCATOR_TESTCASE(NAME) \
    void test_relocator_ ## NAME ( \
        TestRelocatorFixture * fixture, gconstpointer data)
#define RELOCATOR_TESTENTRY(NAME) \
    TEST_ENTRY_WITH_FIXTURE ("Core/Relocator", test_relocator, NAME, \
        TestRelocatorFixture)

#define TEST_OUTBUF_SIZE 32

typedef struct _TestRelocatorFixture
{
  guint8 * output;
  GumX86Writer cw;
  GumX86Relocator rl;
} TestRelocatorFixture;

static void
test_relocator_fixture_setup (TestRelocatorFixture * fixture,
                              gconstpointer data)
{
  guint page_size;
  guint8 stack_data[1] = { 42 };
  GumAddressSpec as;

  page_size = gum_query_page_size ();

  as.near_address = (gpointer) stack_data;
  as.max_distance = G_MAXINT32 - page_size;

  fixture->output = (guint8 *) gum_alloc_n_pages_near (1, GUM_PAGE_RWX, &as);
  memset (fixture->output, 0, page_size);

  gum_x86_writer_init (&fixture->cw, fixture->output);
}

static void
test_relocator_fixture_teardown (TestRelocatorFixture * fixture,
                                 gconstpointer data)
{
  gum_x86_relocator_free (&fixture->rl);
  gum_x86_writer_free (&fixture->cw);
  gum_free_pages (fixture->output);
}

#if GLIB_SIZEOF_VOID_P == 8

static void
test_relocator_fixture_assert_output_equals (TestRelocatorFixture * fixture,
                                             const guint8 * expected_code,
                                             guint expected_length)
{
  guint actual_length;
  gboolean same_length, same_content;

  actual_length = gum_x86_writer_offset (&fixture->cw);
  same_length = (actual_length == expected_length);
  if (same_length)
  {
    same_content =
        memcmp (fixture->output, expected_code, expected_length) == 0;
  }
  else
  {
    same_content = FALSE;
  }

  if (!same_length || !same_content)
  {
    gchar * diff;

    if (actual_length != 0)
    {
      diff = test_util_diff_binary (expected_code, expected_length,
          fixture->output, actual_length);
      g_print ("\n\nRelocated code is not equal to expected code:\n\n%s\n",
          diff);
      g_free (diff);
    }
    else
    {
      g_print ("\n\nNo code was relocated!\n\n");
    }
  }

  g_assert (same_length);
  g_assert (same_content);
}

#endif

static const guint8 cleared_outbuf[TEST_OUTBUF_SIZE] = { 0, };

#define SETUP_RELOCATOR_WITH(CODE) \
    gum_x86_relocator_init (&fixture->rl, CODE, &fixture->cw)

#define assert_outbuf_still_zeroed_from_offset(OFF) \
    g_assert_cmpint (memcmp (fixture->output + OFF, cleared_outbuf + OFF, \
        sizeof (cleared_outbuf) - OFF), ==, 0)
#define assert_output_equals(e) test_relocator_fixture_assert_output_equals \
    (fixture, e, sizeof (e))
