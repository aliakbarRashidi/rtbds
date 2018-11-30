/*
* RTBDS keeps files synchronized in real-time between several machines.
* Copyright (C) 2018  Guillaume TRUCHOT - guillaume.truchot@outlook.com
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
* 
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#define RTBDSCLIENTBACKEND_VERSION ("0.1.0")

#if defined(_MSC_VER)
#if defined(RTBDSCLIENTBACKEND_EXPORTS)
#define RTBDSCLIENTBACKEND_DLL __declspec(dllexport)
#else
#define RTBDSCLIENTBACKEND_DLL __declspec(dllimport)
#endif
#else
#define RTBDSCLIENTBACKEND_DLL
#endif
