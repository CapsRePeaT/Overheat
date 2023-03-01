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
#ifndef CINO_DUAL_MESH
#define CINO_DUAL_MESH

#include <cinolib/geometry/vec_mat.h>
#include <cinolib/meshes/polygonmesh.h>
#include <cinolib/meshes/polyhedralmesh.h>

namespace cinolib
{

/* Mesh dualization facilities for surfaces and volumes.
 * Features (e.g. sharp creases) will be preserved in the
 * dual if the CREASE flag is enabled in mesh edges. For
 * geometric features the easiest way t do it is to call
 * the method:
 *
 *      mesh.edge_mark_sharp_creases([angle_thresh]);
 *
 * This works both for surfaces and volumes.
*/

template<class M, class V, class E, class F, class P>
CINO_INLINE
void dual_mesh(const AbstractPolyhedralMesh<M,V,E,F,P> & primal,
                     Polyhedralmesh<M,V,E,F,P>         & dual,
               const bool                                with_clipped_cells); // consider/discard boundary vertices

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class F, class P>
CINO_INLINE
void dual_mesh(const AbstractPolyhedralMesh<M,V,E,F,P> & primal,
                     std::vector<vec3d>                & dual_verts,
                     std::vector<std::vector<uint>>    & dual_faces,
                     std::vector<std::vector<uint>>    & dual_polys,
                     std::vector<std::vector<bool>>    & dual_polys_winding,
               const bool                                with_clipped_cells); // consider/discard boundary vertices

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void dual_mesh(const AbstractPolygonMesh<M,V,E,P>   & primal,
                     std::vector<vec3d>             & dual_verts,
                     std::vector<std::vector<uint>> & dual_faces,
               const bool                             with_clipped_cells); // consider/discard boundary vertices

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<class M, class V, class E, class P>
CINO_INLINE
void dual_mesh(const AbstractPolygonMesh<M,V,E,P> & primal,
                     Polygonmesh<M,V,E,P>         & dual,
               const bool                           with_clipped_cells); // consider/discard boundary vertices

}

#ifndef  CINO_STATIC_LIB
#include "dual_mesh.cpp"
#endif

#endif // CINO_DUAL_MESH
