/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file ghost/intern/GHOST_SystemPathsWii.h
 *  \ingroup GHOST
 */


#ifndef __GHOST_SYSTEMPATHSWII_H__
#define __GHOST_SYSTEMPATHSWII_H__

#include "GHOST_SystemPaths.h"


/**
 * WII Implementation of GHOST_SystemPaths class.
 * \see GHOST_SystemPaths.
 * \author	Andrea Weikert
 * \date	August 1, 2010
 */
class GHOST_SystemPathsWii : public GHOST_SystemPaths {
public:
	/**
	 * Constructor.
	 */
	GHOST_SystemPathsWii();

	/**
	 * Destructor.
	 */
	~GHOST_SystemPathsWii();

	/**
	 * Determine the base dir in which shared resources are located. It will first try to use
	 * "unpack and run" path, then look for properly installed path, including versioning.
	 * \return Unsigned char string pointing to system dir (eg /usr/share/).
	 */
	const GHOST_TUns8 *getSystemDir(int version, const char *versionstr) const;

	/**
	 * Determine the base dir in which user configuration is stored, including versioning.
	 * If needed, it will create the base directory.
	 * \return Unsigned char string pointing to user dir (eg ~/).
	 */
	const GHOST_TUns8 *getUserDir(int version, const char *versionstr) const;

	/**
	 * Determine the directory of the current binary
	 * \return Unsigned char string pointing to the binary dir
	 */
	const GHOST_TUns8 *getBinaryDir() const;

	/**
	 * Add the file to the operating system most recently used files
	 */
	void addToSystemRecentFiles(const char *filename) const;
};

#endif // __GHOST_SYSTEMPATHSWII_H__

