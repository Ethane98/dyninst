/*
 * Copyright (c) 1996-2009 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

// Architecture include. Use this one instead of arch-<platform>

#if !defined(arch_h)
#define arch_h

#include <assert.h>
#include <vector>

#if defined(sparc_sun_sunos4_1_3) \
 || defined(sparc_sun_solaris2_4)
#include "arch-sparc.h"
using namespace NS_sparc;

#elif defined(arch_power)
#include "arch-power.h"
using namespace NS_power;

#elif defined(i386_unknown_solaris2_5) \
   || defined(i386_unknown_nt4_0) \
   || defined(i386_unknown_linux2_0) \
   || defined(x86_64_unknown_linux2_4) \
   || (defined(os_freebsd) \
       && (defined(arch_x86) || defined(arch_x86_64)))
#include "arch-x86.h"
using namespace NS_x86;

#else
#error "unknown architecture"

#endif

// For platforms that require bit-twiddling. These should go away in the future.
#define GET_PTR(insn, gen) codeBuf_t *insn = (codeBuf_t *)(gen).cur_ptr()
#define SET_PTR(insn, gen) (gen).update(insn)
#define REGET_PTR(insn, gen) insn = (codeBuf_t *)(gen).cur_ptr()

#endif