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

/** \file audaspace/FX/AUD_RectifyFactory.cpp
 *  \ingroup audfx
 */


#include "AUD_RectifyFactory.h"
#include "AUD_CallbackIIRFilterReader.h"

#include <cmath>

sample_t AUD_RectifyFactory::rectifyFilter(AUD_CallbackIIRFilterReader* reader, void* useless)
{
	return fabs(reader->x(0));
}

AUD_RectifyFactory::AUD_RectifyFactory(std::shared_ptr<AUD_IFactory> factory) :
		AUD_EffectFactory(factory)
{
}

std::shared_ptr<AUD_IReader> AUD_RectifyFactory::createReader()
{
	return std::shared_ptr<AUD_IReader>(new AUD_CallbackIIRFilterReader(getReader(), 1, 1, rectifyFilter));
}
