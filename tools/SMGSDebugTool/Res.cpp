#include "Res.h"

#include "logo.xpm"

#include "SMGSDebugToolApp.h"

#include <wx/fs_mem.h>

/*
内存文件系统
*/
static class file_load
{
public:
    file_load()
    {
        wxFileSystem::AddHandler(new wxMemoryFSHandler);
        wxInitAllImageHandlers();
        wxMemoryFSHandler::AddFile("logo_xpm",wxImage(logo_xpm),wxBITMAP_TYPE_PNG );
        wxMemoryFSHandler::AddFile("about.htm",
                                   _T("<html><body>")
                                   _T("<center><img  height=\"100\" width=\"100\" src=\"memory:logo_xpm\"> </center>")
                                   _T("</body></html>"));
    }
} load_file;
