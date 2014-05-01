========================================================================
    wxWindows : "wxAlcatel" Project Overview
========================================================================

wxWindows has created this "wxAlcatel" project for you as a starting point.

This file contains a summary of what you will find in each of the files that make up your project.

wxWindows.vcproj
    This is the main project file for projects generated using an Application Wizard. 
    It contains information about the version of the product that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

IMPORTANT NOTE:
    Please set WXWIN Environment Variables to wxWidgets directory.

///////////////////////////
Backup by Git:
further read --> http://thelucid.com/2008/12/02/git-setting-up-a-remote-repository-and-doing-an-initial-push/

Setup remote repo (on flashdisk):
$ git remote -v

$ ls /cygdrive/
$ ls /cygdrive/f/repo/

if not exsits:
$ mkdir wxalcatel.git
$ cd wxalcatel.git/
$ git init --bare

$ git remote add usb_f /cygdrive/f/repo/wxalcatel.git

Commands:
$ git status
$ git add *.cpp *.h
$ git commit -m "Log message"
$ git push usb_f master


/////////////////////////////////////////////////////////////////////////////
Other notes:

/////////////////////////////////////////////////////////////////////////////

* wxWidgets *
define environment variable:
WXWIN = C:\wxWidgets-3.0.0
usable inside msvc path/dir with $(WXWIN)


http://trac.wxwidgets.org/ticket/10931:
The only hint I may give is to set wxUSE_IPV6 to 1 in your wx/msw/setup.h as then wx/wx.h itself will include winsock2.h instead of winsock.h.
So, be sure to enable IPv6 support (on setup.h) to be able using WinSock2.

/////////////////////////////////////////////////////////////////////////////

* libssh-0.6.3 *
Put on D:\LIB\libssh-0.6.3.
Replace cmake\Modules\FindZLIB.cmake - for availability to use zlib from WXWIN.
Install OpenSSL-Win32 (cmake will detect via registry/path)
Install Doxygen (--''--)
Install GraphViz (--''--)
after CMake:
Edit config.h to Disable HAVE_NTOHLL and HAVE_HTONLL since it's notsupported under Windows 8 WinSocket2.
this library support issue-banner (for non logged user)
Only import ssh_shared VCproject. optionally u can run doxygen project to build document.
--> it seems libssh2 will be rmoved.

Add following on CMakeFiles src/thread

if (WIN32)
    set(THREADS_USE_PTHREADS_WIN32 true)
    set(THREADS_PTHREADS_WIN32_EXCEPTION_SCHEME SE)
    find_package(Threads REQUIRED)
    include_directories(${THREADS_PTHREADS_INCLUDE_DIR})
endif (WIN32)

After configure, then failed, set the PTHREAD include and library yo VSE2.lib, Generate then.

///////////////////////////////////////////////////////////////////////////////////

* agg-2.4 **
This is the lates antigrain (BSD license, 2.5 same source diff license) before the creator passed
away (R.I.P.). 
To be easy, just include agg-2.4 src that the fuction will be used and compile
without problem (see examples, .dsp or .vcproc -- .dsp must be Win32 EOL). 
Use CMakeFiles to make agg as library, get it from wxArt2D, or:

// older is: git clone https://github.com/springmeyer/antigrain-geometry.git

newer is:
svn checkout svn://svn.code.sf.net/p/agg/svn/ agg-svn

read `cmake_install.html`

mirrored: http://agg.sourceforge.net/antigrain.com/index.html
milist:   http://blog.gmane.org/gmane.comp.graphics.agg

////////////////////////////////////////////////////////////////////////

Graph stucture data

References:
http://en.wikipedia.org/wiki/Graph_(abstract_data_type)
http://networkx.github.io/
http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/adjacency_list.html
d3js.org
graphviz


