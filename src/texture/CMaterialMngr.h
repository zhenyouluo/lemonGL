/*
	Copyright 2012 Thomas Le Guerroué-Drévillon
	This file is part of lemonGL.

	lemonGL is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	lemonGL is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with lemonGL.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CMATERIALMNGR_H_
#define CMATERIALMNGR_H_

#include "CMaterial.h"

#include <QFile>

class CMaterialMngr {

public:
	static void add(const QString&);

	static const CMaterial* get(const QString&);

private:
	CMaterialMngr(){}

	static QList<CMaterial> m_materials;
};

#endif /* CMATERIALMNGR_H_ */
