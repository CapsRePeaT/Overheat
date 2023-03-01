/********************************************************************************
*  This file is part of CinoLib                                                 *
*  Copyright(C) 2016: Marco Livesu                                              *
*                                                                               *
*  The MIT License                                                              *
*                                                                               *
*  Permission is hereby granted, free of charge, to any person obtaining a      *
*  copy of this software and associated documentation files (the "Software"),   *
*  to deal in the Software without restriction, including without limitation    *
*  the rights to use, copy, modify, merge, publish, distribute, sublicense,     *
*  and/or sell copies of the Software, and to permit persons to whom the        *
*  Software is furnished to do so, subject to the following conditions:         *
*                                                                               *
*  The above copyright notice and this permission notice shall be included in   *
*  all copies or substantial portions of the Software.                          *
*                                                                               *
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
*  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE *
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      *
*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS *
*  IN THE SOFTWARE.                                                             *
*                                                                               *
*  Author(s):                                                                   *
*                                                                               *
*     Marco Livesu (marco.livesu@gmail.com)                                     *
*     http://pers.ge.imati.cnr.it/livesu/                                       *
*                                                                               *
*     Italian National Research Council (CNR)                                   *
*     Institute for Applied Mathematics and Information Technologies (IMATI)    *
*     Via de Marini, 6                                                          *
*     16149 Genoa,                                                              *
*     Italy                                                                     *
*********************************************************************************/
#ifndef CINO_READ_HEXEX_H
#define CINO_READ_HEXEX_H

#include <sys/types.h>
#include <vector>
#include <cinolib/cino_inline.h>
#include <cinolib/geometry/vec_mat.h>

namespace cinolib
{

/* Read a tetrahedral mesh and an integer grid map associated to it in the
 * libHexEx format (https://www.graphics.rwth-aachen.de/software/libHexEx).
 * For further deails please refer to the original paper:
 *
 *     HexEx: Robust Hexahedral Mesh Extraction
 *     M.Lyon, D.Bommes and L.Kobbelt
 *     ACM TOG (SIGGRAPH 2016)
*/

CINO_INLINE
void read_HEXEX(const char         * filename,
                std::vector<vec3d> & verts,
                std::vector<uint>  & tets,        // serialized tets (4 vids per tet)
                std::vector<vec3d> & tets_param); // tets param (4 points per tet)

}

#ifndef  CINO_STATIC_LIB
#include "read_HEXEX.cpp"
#endif

#endif // CINO_READ_HEXEX
