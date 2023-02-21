/********************************************************************************
*  This file is part of CinoLib                                                 *
*  Copyright(C) 2022: Marco Livesu                                              *
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
#ifndef CINO_VOXEL_GRID_TO_HEXMESH_H
#define CINO_VOXEL_GRID_TO_HEXMESH_H

#include <cinolib/voxel_grid.h>
#include <cinolib/meshes/hexmesh.h>

namespace cinolib
{

// Converts a voxel grid into a hexahedral mesh. Users can select what voxel types
// can be retained in the output mesh. Legal choices are combinations of the following
// types:
//         - VOXEL_INSIDE   => fully inside the voxelized object
//         - VOXEL_OUTSIDE  => fully outside the voxelized object
//         - VOXEL_BOUNDARY => traversed by the boundary of the voxelized object
//
template<class M, class V, class E, class F, class P>
CINO_INLINE
void voxel_grid_to_hexmesh(const VoxelGrid                         & g,
                                 AbstractPolyhedralMesh<M,V,E,F,P> & m,
                           const int voxel_types = VOXEL_INSIDE | VOXEL_BOUNDARY);
}

#ifndef  CINO_STATIC_LIB
#include "voxel_grid_to_hexmesh.cpp"
#endif

#endif // CINO_VOXEL_GRID_TO_HEXMESH_H
