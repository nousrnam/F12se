/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef F2SEAPP_H
#define F2SEAPP_H

#include <wx/app.h>

class F2seApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // F2SEAPP_H
