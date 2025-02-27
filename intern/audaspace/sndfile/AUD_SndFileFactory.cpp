/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * Copyright 2009-2011 Jörg Hermann Müller
 *
 * This file is part of AudaSpace.
 *
 * Audaspace is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * AudaSpace is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Audaspace; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file audaspace/sndfile/AUD_SndFileFactory.cpp
 *  \ingroup audsndfile
 */


#include "AUD_SndFileFactory.h"
#include "AUD_SndFileReader.h"

#include <cstring>

AUD_SndFileFactory::AUD_SndFileFactory(std::string filename) :
	m_filename(filename)
{
}

AUD_SndFileFactory::AUD_SndFileFactory(const data_t* buffer, int size) :
	m_buffer(new AUD_Buffer(size))
{
	memcpy(m_buffer->getBuffer(), buffer, size);
}

std::shared_ptr<AUD_IReader> AUD_SndFileFactory::createReader()
{
	if(m_buffer.get())
		return std::shared_ptr<AUD_IReader>(new AUD_SndFileReader(m_buffer));
	else
		return std::shared_ptr<AUD_IReader>(new AUD_SndFileReader(m_filename));
}
