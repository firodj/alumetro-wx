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

/////////////////////////////////////////////////////////////////////////////
Other notes:

/////////////////////////////////////////////////////////////////////////////

http://trac.wxwidgets.org/ticket/10931:
The only hint I may give is to set wxUSE_IPV6 to 1 in your wx/msw/setup.h as then wx/wx.h itself will include winsock2.h instead of winsock.h.

