/*
 * Copyright (c) 1996 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * This license is for research uses.  For such uses, there is no
 * charge. We define "research use" to mean you may freely use it
 * inside your organization for whatever purposes you see fit. But you
 * may not re-distribute Paradyn or parts of Paradyn, in any form
 * source or binary (including derivatives), electronic or otherwise,
 * to any other organization or entity without our permission.
 * 
 * (for other uses, please contact us at paradyn@cs.wisc.edu)
 * 
 * All warranties, including without limitation, any warranty of
 * merchantability or fitness for a particular purpose, are hereby
 * excluded.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * Even if advised of the possibility of such damages, under no
 * circumstances shall we (or any other person or entity with
 * proprietary rights in the software licensed hereunder) be liable
 * to you or any third party for direct, indirect, or consequential
 * damages of any character regardless of type of action, including,
 * without limitation, loss of profits, loss of use, loss of good
 * will, or computer failure or malfunction.  You agree to indemnify
 * us (and any other person or entity with proprietary rights in the
 * software licensed hereunder) for any and all liability it may
 * incur to third parties resulting from your use of Paradyn.
 */

// tkTools.h
// Ariel Tamches

// Some C++ stuff that I have found to be both useful and generic
// across all the tk4.0 programs I've written...

/*
 * $Log: tkTools.h,v $
 * Revision 1.6  1999/03/03 18:16:13  pcroth
 * Updated to support Windows NT as a front-end platform
 * Changes made to X code, to use Tcl analogues when appropriate
 * Also changed in response to modifications in thread library and igen output.
 *
 * Revision 1.5  1996/08/16 21:07:30  tamches
 * updated copyright for release 1.1
 *
 * Revision 1.4  1996/02/02 18:54:33  tamches
 * added setResultBool
 *
 * Revision 1.3  1995/11/29 00:20:37  tamches
 * added tcl_cmd_installer
 *
 * Revision 1.2  1995/11/06 02:28:02  tamches
 * added tclpanic and resizeScrollbar
 *
 */

#ifndef _TK_TOOLS_H_
#define _TK_TOOLS_H_

#include "tcl.h"
#include "tk.h"
#include "util/h/String.h"

#include <iostream.h>

class tkInstallIdle {
 private:
   bool currentlyInstalled;

   void (*usersRoutine)(ClientData cd);
   ClientData usersClientData;
      // needed since we overwrite clientdata with "this"

 private:
   static void installMe(ClientData cd);
      // "cd" will be "this"

 public:
   tkInstallIdle(void (*iUsersRoutine)(ClientData));
  ~tkInstallIdle();

   void install(ClientData cd);
};

void myTclEval(Tcl_Interp *interp, const string &);
void myTclEval(Tcl_Interp *interp, const char *);
void tclpanic(Tcl_Interp *interp, const string &str);

void getScrollBarValues(Tcl_Interp *, const string &sbName,
                        float &theFirst, float &theLast);

float moveScrollBar(Tcl_Interp *, const string &sbName,
                    float tentativeNewFirst);
   // returns actual new-first, after (possible) pinning

int set_scrollbar(Tcl_Interp *interp, const string &sbname,
                  int total_width, 
                  int global_coord, int &screen_coord);

bool processScrollCallback(Tcl_Interp *interp,
                           int argc, char **argv,
                           const string &sbName,
			   int oldoffsetUnits, int totalWidthUnits,
			   int visibleWidthUnits,
                           float &newFirst);
   // tk4.0 has a complex method of reporting scroll events.
   // It will invoke a callback with a variable number of arguments,
   // depending on whether the sb was unit-moved, page-moved, or dragged.
   // This routine parses those arguments, modifies the scrollbar,
   // and returns true iff anything changed.  If so, the parameter
   // "newFirst" is modified (so you can read it and invoke application-specific
   // stuff.  After all, this routine merely updates the scrollbar)

void resizeScrollbar(Tcl_Interp *interp, const string &sbName,
                     int total_width, int visible_width);

void setResultBool(Tcl_Interp *interp, bool val);

class tcl_cmd_installer {
 private:
   static void dummy_delete_proc(ClientData) {}

 public:
   tcl_cmd_installer(Tcl_Interp *interp, const string &tclCmdName, Tcl_CmdProc proc) {
      Tcl_CreateCommand(interp, (char*)tclCmdName.string_of(), proc, NULL, &dummy_delete_proc);
   }

   tcl_cmd_installer(Tcl_Interp *interp, const string &tclCmdName, Tcl_CmdProc proc, ClientData cd) {
      Tcl_CreateCommand(interp, (char*)tclCmdName.string_of(), proc, cd, &dummy_delete_proc);
   }
  ~tcl_cmd_installer() {}
};

#endif
