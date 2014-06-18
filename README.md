wx-alcatel-metro
================

wxAlcatel is wxWidgets application to help maintenance routers.

wxVideoTerminal is wxWidgets component to emulate video terminal such as VT100.

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

The wxAggShape should be used to define a Lines(links-connetors) and a Nodes(routers).
-> using path_storage, or others


////////////////// AGG Hit-Detection //


Hi George, 

First of all, the problem of that "point-in-polygon" function is that it 
doesn't handle the non-zero fill rule, only even-odd. It can give you the 
wrong result. The following links can help: 

http://www.acm.org/tog/editors/erich/ptinpoly/
http://softsurfer.com/Archive/algorithm_0103/algorithm_0103.htm

In AGG the common way of hit-testing is to use the rasterizer. You do it as 
if you wanted to draw, but instead of calling agg::render_scanlines() you 
call ras.hit_test(x,y); where x,y are integer coordinates on the screen. 
Yes, it looks slow, but remember it isn't slower (actually faster) than 
drawing itself. If rendering of the scene is fairly fast, the hit-test will 
also be fast. 

The advantage is that it works identically with drawing. And it also works 
for strokes, considering line caps and line joins. Well, you may want to 
increase the sensible area with some tolerance. In this case you can simply 
increase the line width accordingly. The line width is set in the world 
coordinates, considering the viewport and the transformer. So that, you have 
to consider it. For example if you want to add exactly one pixel tolerance, 
you set something like this: 
if(hit_test_mode) 
{ 
   double tolerance = 1.0; 
   tolerance /= m_affine.scale(); 
   m_stroke.width(m_width + tolerance); 
} 
Where m_affine contains viewport transformations. Or you can use 
tolerance /= m_affine.scale() * m_viewport.scale(); 
If they are separate. 

It's also possible to use calc_line_point_distance(), but to restrict it to 
the line segments you check the line for the bounding box. Suppose, 
x1,y1,x2,y2 are the coordinates of the line segment, and x,y - the hit-test 
point. You do: 

agg::rect_d bbox(x1,y1,x2,y2); 
bbox.normalize(); // swap coordinates if necessary. 
bbox.x1 -= tolerance; 
bbox.y1 -= tolerance; 
bbox.x2 += tolerance; 
bbox.y2 += tolerance; 
if(x >= bbox.x1 && x <= bbox.x2 && y >= bbox.y1 && y <= bbox.y2) 
{ 
    if(agg::calc_line_to_point_distance(x1,y1,x2,y2, x,y) <= tolerance) 
    { 
        ...the line segment is hit. 
    } 
} 

But this method won't consider line caps and joins. 

McSeem 

Hi, 

one problem I had with hit-testing was that agg::calc_line_point_distance() 
gives you the distance to the infinite line described by the given points, 
not the "segment". So I came up with this horrible beast (I'm sure lots of 
people on this list have something much nicer, please add your insights): 

static float 
distance_to_curve(const Point& p, 
      const Point& a, const Point& aOut, 
      const Point& bIn, const Point& b) 
{ 
   agg::curve4 curve(a.x, a.y, aOut.x, aOut.y, 
                    bIn.x, bIn.y, b.x, b.y); 

   float segDist = FLT_MAX; 
   double x1, y1, x2, y2; 
   unsigned cmd = curve.vertex(&x1, &y1); 
   while (!agg::is_stop(cmd)) { 
      cmd = curve.vertex(&x2, &y2); 
      // first figure out if point is between segment 
      // start and end points 
      double a = agg::calc_distance(p.x, p.y, x2, y2); 
      double b = agg::calc_distance(p.x, p.y, x1, y1); 

      float currentDist = min_c(a, b); 

      if (a > 0.0 && b > 0.0) { 
         double c = agg::calc_distance(x1, y1, x2, y2); 

         double alpha = acos((b*b + c*c - a*a) / (2*b*c)); 
         double beta = acos((a*a + c*c - b*b) / (2*a*c)); 
    
         if (alpha <= PI2 && beta <= PI2) { 
            currentDist = fabs(agg::calc_line_point_distance(x1, y1, 
                                                             x2, y2, 
                                                             p.x, p.y)); 
         } 
      } 

      if (currentDist < segDist) { 
         segDist = currentDist; 
      } 
      x1 = x2; 
      y1 = y2; 
   } 
   return segDist; 
} 

This method is suited for curves, you can extract the part you need for 
lines. And of course you need to first check the bounding box of your entire 
path before using the more elaborate methods. 

Best regards, 
-Stephan 

http://anti-grain-geometry.1086185.n5.nabble.com/Hit-testing-on-a-line-td1027.html