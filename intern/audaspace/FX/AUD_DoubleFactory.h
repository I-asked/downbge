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

/** \file audaspace/FX/AUD_DoubleFactory.h
 *  \ingroup audfx
 */


#ifndef __AUD_DOUBLEFACTORY_H__
#define __AUD_DOUBLEFACTORY_H__

#include "AUD_IFactory.h"

/**
 * This factory plays two other factories behind each other.
 */
class AUD_DoubleFactory : public AUD_IFactory
{
private:
	/**
	 * First played factory.
	 */
	std::shared_ptr<AUD_IFactory> m_factory1;

	/**
	 * Second played factory.
	 */
	std::shared_ptr<AUD_IFactory> m_factory2;

	// hide copy constructor and operator=
	AUD_DoubleFactory(const AUD_DoubleFactory&);
	AUD_DoubleFactory& operator=(const AUD_DoubleFactory&);

public:
	/**
	 * Creates a new double factory.
	 * \param factory1 The first input factory.
	 * \param factory2 The second input factory.
	 */
	AUD_DoubleFactory(std::shared_ptr<AUD_IFactory> factory1, std::shared_ptr<AUD_IFactory> factory2);

	virtual std::shared_ptr<AUD_IReader> createReader();
};

#endif //__AUD_DOUBLEFACTORY_H__
