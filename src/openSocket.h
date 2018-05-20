/*
 Part of Libnodave, a free communication libray for Siemens S7 300/400.
 
 (C) Thomas Hergenhahn (thomas.hergenhahn@web.de) 2002, 2003.2004

 Libnodave is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 Libnodave is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Libnodave; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  
*/


#ifndef OPEN_SOCKET_H
#define OPEN_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

EXPORTSPEC connection_t CALL_SPEC openSocket(const int port, const char * peer);

EXPORTSPEC int CALL_SPEC closeSocket(connection_t h);

#ifdef __cplusplus
 }
#endif


#endif //OPEN_SOCKET_H


/*
    Changes: 
    07/12/03  moved openSocket to it's own file, because it can be reused in other TCP clients
    04/07/04  ported C++ version to C
    12/17/04  additonal defines for WIN32
    04/09/05  removed CYGWIN defines. As there were no more differences against LINUX, it should 
	      work with LINUX defines.
Version 0.8.4.5    
    07/10/09  	Added closeSocket()
	2018-02-14	API detection moved into config.h
*/
