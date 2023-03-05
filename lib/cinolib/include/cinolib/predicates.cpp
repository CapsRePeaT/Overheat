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
#include <cinolib/predicates.h>
#include <algorithm>
#include <bitset>

namespace cinolib
{

#ifndef CINOLIB_USES_SHEWCHUK_PREDICATES
/*********************************************************
 * BEGIN OF IMPlEMENTATION OF INEXACT GEOMETRIC PREDICATES
 *********************************************************/

// basically the Shewchuk's orient2dfast()
CINO_INLINE
double orient2d(const double * pa,
                const double * pb,
                const double * pc)
{
    double acx = pa[0] - pc[0];
    double bcx = pb[0] - pc[0];
    double acy = pa[1] - pc[1];
    double bcy = pb[1] - pc[1];

    return acx * bcy - acy * bcx;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// basically the Shewchuk's orient3dfast()
CINO_INLINE
double orient3d(const double * pa,
                const double * pb,
                const double * pc,
                const double * pd)
{
    double adx = pa[0] - pd[0];
    double bdx = pb[0] - pd[0];
    double cdx = pc[0] - pd[0];
    double ady = pa[1] - pd[1];
    double bdy = pb[1] - pd[1];
    double cdy = pc[1] - pd[1];
    double adz = pa[2] - pd[2];
    double bdz = pb[2] - pd[2];
    double cdz = pc[2] - pd[2];

    return adx * (bdy * cdz - bdz * cdy)
         + bdx * (cdy * adz - cdz * ady)
         + cdx * (ady * bdz - adz * bdy);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// basically the Shewchuk's incirclefast()
CINO_INLINE
double incircle(const double * pa,
                const double * pb,
                const double * pc,
                const double * pd)
{
    double adx = pa[0] - pd[0];
    double ady = pa[1] - pd[1];
    double bdx = pb[0] - pd[0];
    double bdy = pb[1] - pd[1];
    double cdx = pc[0] - pd[0];
    double cdy = pc[1] - pd[1];

    double abdet = adx * bdy - bdx * ady;
    double bcdet = bdx * cdy - cdx * bdy;
    double cadet = cdx * ady - adx * cdy;
    double alift = adx * adx + ady * ady;
    double blift = bdx * bdx + bdy * bdy;
    double clift = cdx * cdx + cdy * cdy;

    return alift * bcdet + blift * cadet + clift * abdet;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// basically the Shewchuk's inspherefast()
CINO_INLINE
double insphere(const double * pa,
                const double * pb,
                const double * pc,
                const double * pd,
                const double * pe)
{
    double aex = pa[0] - pe[0];
    double bex = pb[0] - pe[0];
    double cex = pc[0] - pe[0];
    double dex = pd[0] - pe[0];
    double aey = pa[1] - pe[1];
    double bey = pb[1] - pe[1];
    double cey = pc[1] - pe[1];
    double dey = pd[1] - pe[1];
    double aez = pa[2] - pe[2];
    double bez = pb[2] - pe[2];
    double cez = pc[2] - pe[2];
    double dez = pd[2] - pe[2];

    double ab = aex * bey - bex * aey;
    double bc = bex * cey - cex * bey;
    double cd = cex * dey - dex * cey;
    double da = dex * aey - aex * dey;

    double ac = aex * cey - cex * aey;
    double bd = bex * dey - dex * bey;

    double abc = aez * bc - bez * ac + cez * ab;
    double bcd = bez * cd - cez * bd + dez * bc;
    double cda = cez * da + dez * ac + aez * cd;
    double dab = dez * ab + aez * bd + bez * da;

    double alift = aex * aex + aey * aey + aez * aez;
    double blift = bex * bex + bey * bey + bez * bez;
    double clift = cex * cex + cey * cey + cez * cez;
    double dlift = dex * dex + dey * dey + dez * dez;

    return (dlift * abc - clift * dab) + (blift * cda - alift * bcd);
}

/*******************************************************
 * END OF IMPlEMENTATION OF INEXACT GEOMETRIC PREDICATES
 *******************************************************/
#endif

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
double orient2d(const vec2d & pa,
                const vec2d & pb,
                const vec2d & pc)
{
    return orient2d(pa.ptr(), pb.ptr(), pc.ptr());
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
double orient3d(const vec3d & pa,
                const vec3d & pb,
                const vec3d & pc,
                const vec3d & pd)
{
    return orient3d(pa.ptr(), pb.ptr(), pc.ptr(), pd.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
double incircle(const vec2d & pa,
                const vec2d & pb,
                const vec2d & pc,
                const vec2d & pd)
{
    return incircle(pa.ptr(), pb.ptr(), pc.ptr(), pd.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
double insphere(const vec3d & pa,
                const vec3d & pb,
                const vec3d & pc,
                const vec3d & pd,
                const vec3d & pe)
{
    return insphere(pa.ptr(), pb.ptr(), pc.ptr(), pd.ptr(), pe.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// true if the area of the triangle p0-p1-p2 is zero
CINO_INLINE
bool points_are_colinear_2d(const vec2d & p0,
                            const vec2d & p1,
                            const vec2d & p2)
{
    return points_are_colinear_2d(p0.ptr(), p1.ptr(), p2.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// true if the area of the triangle p0-p1-p2 is zero
CINO_INLINE
bool points_are_colinear_2d(const double * p0,
                            const double * p1,
                            const double * p2)
{
    return (orient2d(p0,p1,p2)==0);
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// true if the area of all the orthogonal 2d projections of the triangle p0-p1-p2 is zero
CINO_INLINE
bool points_are_colinear_3d(const vec3d & p0,
                            const vec3d & p1,
                            const vec3d & p2)
{
    return points_are_colinear_3d(p0.ptr(), p1.ptr(), p2.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// true if the area of all the orthogonal 2d projections of the triangle p0-p1-p2 is zero
CINO_INLINE
bool points_are_colinear_3d(const double * p0,
                            const double * p1,
                            const double * p2)
{
    double p0_dropX[2] = { p0[1], p0[2] };
    double p0_dropY[2] = { p0[0], p0[2] };
    double p0_dropZ[2] = { p0[0], p0[1] };
    double p1_dropX[2] = { p1[1], p1[2] };
    double p1_dropY[2] = { p1[0], p1[2] };
    double p1_dropZ[2] = { p1[0], p1[1] };
    double p2_dropX[2] = { p2[1], p2[2] };
    double p2_dropY[2] = { p2[0], p2[2] };
    double p2_dropZ[2] = { p2[0], p2[1] };

    // check if all the 2d orthogonal projections of p are colinear
    if(points_are_colinear_2d(p0_dropX, p1_dropX, p2_dropX) &&
       points_are_colinear_2d(p0_dropY, p1_dropY, p2_dropY) &&
       points_are_colinear_2d(p0_dropZ, p1_dropZ, p2_dropZ))
    {
        return true;
    }
    return false;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// true if the volume of the tetrahedron p0-p1-p2-p3 is zero
CINO_INLINE
bool points_are_coplanar_3d(const vec3d & p0,
                            const vec3d & p1,
                            const vec3d & p2,
                            const vec3d & p3)
{
    return points_are_coplanar_3d(p0.ptr(), p1.ptr(), p2.ptr(), p3.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// true if the volume of the tetrahedron p0-p1-p2-p3 is zero
CINO_INLINE
bool points_are_coplanar_3d(const double * p0,
                            const double * p1,
                            const double * p2,
                            const double * p3)
{
    return (orient3d(p0,p1,p2,p3)==0);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of s
// STRICTLY_INSIDE  if p lies inside segment s (endpoints excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_segment_2d(const vec2d & p,
                                   const vec2d & s0,
                                   const vec2d & s1)
{
    return point_in_segment_2d(p.ptr(), s0.ptr(), s1.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of s
// STRICTLY_INSIDE  if p lies inside segment s (endpoints excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_segment_2d(const double * p,
                                   const double * s0,
                                   const double * s1)
{
    if(vec_equals_2d(p, s0)) return ON_VERT0;
    if(vec_equals_2d(p, s1)) return ON_VERT1;

    if(!points_are_colinear_2d(s0, s1, p)) return STRICTLY_OUTSIDE;

    if((p[0] > std::min(s0[0], s1[0]) && p[0] < std::max(s0[0], s1[0])) ||
       (p[1] > std::min(s0[1], s1[1]) && p[1] < std::max(s0[1], s1[1])))
    {
        return STRICTLY_INSIDE;
    }

    return STRICTLY_OUTSIDE;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of s
// STRICTLY_INSIDE  if p lies inside segment s (endpoints excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_segment_3d(const vec3d & p,
                                   const vec3d & s0,
                                   const vec3d & s1)
{
    return point_in_segment_3d(p.ptr(), s0.ptr(), s1.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of s
// STRICTLY_INSIDE  if p lies inside segment s (endpoints excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_segment_3d(const double * p,
                                   const double * s0,
                                   const double * s1)
{
    if(vec_equals_3d(p, s0)) return ON_VERT0;
    if(vec_equals_3d(p, s1)) return ON_VERT1;

    if(!points_are_colinear_3d(s0, s1, p)) return STRICTLY_OUTSIDE;

    if((p[0] > std::min(s0[0], s1[0]) && p[0] < std::max(s0[0], s1[0])) ||
       (p[1] > std::min(s0[1], s1[1]) && p[1] < std::max(s0[1], s1[1])) ||
       (p[2] > std::min(s0[2], s1[2]) && p[2] < std::max(s0[2], s1[2])))
    {
        return STRICTLY_INSIDE;
    }

    return STRICTLY_OUTSIDE;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of t
// ON_EDGEj         if p lies inside the j-th edge of t (endpoints excluded)
// STRICTLY_INSIDE  if p lies inside triangle t (borders excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_triangle_2d(const vec2d & p,
                                    const vec2d & t0,
                                    const vec2d & t1,
                                    const vec2d & t2)
{
    return point_in_triangle_2d(p.ptr(), t0.ptr(), t1.ptr(), t2.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of t
// ON_EDGEj         if p lies inside the j-th edge of t (endpoints excluded)
// STRICTLY_INSIDE  if p lies inside triangle t (borders excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_triangle_2d(const double * p,
                                    const double * t0,
                                    const double * t1,
                                    const double * t2)
{
    if(vec_equals_2d(p, t0)) return ON_VERT0;
    if(vec_equals_2d(p, t1)) return ON_VERT1;
    if(vec_equals_2d(p, t2)) return ON_VERT2;

    double e0p_area = orient2d(t0, t1, p);
    double e1p_area = orient2d(t1, t2, p);
    double e2p_area = orient2d(t2, t0, p);

    bool hit = (e0p_area >= 0 && e1p_area >= 0 && e2p_area >= 0) ||
               (e0p_area <= 0 && e1p_area <= 0 && e2p_area <= 0);

    if(hit)
    {
        if(e0p_area == 0) return ON_EDGE0;
        if(e1p_area == 0) return ON_EDGE1;
        if(e2p_area == 0) return ON_EDGE2;

        return STRICTLY_INSIDE;
    }

    return STRICTLY_OUTSIDE;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of t
// ON_EDGEj         if p lies inside the j-th edge of t (endpoints excluded)
// STRICTLY_INSIDE  if p lies inside triangle t (borders excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_triangle_3d(const vec3d & p,
                                    const vec3d & t0,
                                    const vec3d & t1,
                                    const vec3d & t2)
{
    return point_in_triangle_3d(p.ptr(), t0.ptr(), t1.ptr(), t2.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of t
// ON_EDGEj         if p lies inside the j-th edge of t (endpoints excluded)
// STRICTLY_INSIDE  if p lies inside triangle t (borders excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_triangle_3d(const double * p,
                                    const double * t0,
                                    const double * t1,
                                    const double * t2)
{
    // test for point in vert
    if(vec_equals_3d(p, t0)) return ON_VERT0;
    if(vec_equals_3d(p, t1)) return ON_VERT1;
    if(vec_equals_3d(p, t2)) return ON_VERT2;

    // test for point in edge in 3D
    if(point_in_segment_3d(p, t0, t1) == STRICTLY_INSIDE) return ON_EDGE0;
    if(point_in_segment_3d(p, t1, t2) == STRICTLY_INSIDE) return ON_EDGE1;
    if(point_in_segment_3d(p, t2, t0) == STRICTLY_INSIDE) return ON_EDGE2;

    // test for the interior: project t on XYZ and, if the check is never false in
    // any of the projections, then p must be inside t

    double p_dropX[2]  = { p[1],  p[2]};
    double t0_dropX[2] = {t0[1], t0[2]}, t1_dropX[2] = {t1[1], t1[2]}, t2_dropX[2] = {t2[1], t2[2]};

    if(point_in_triangle_2d(p_dropX, t0_dropX, t1_dropX, t2_dropX) == STRICTLY_OUTSIDE) return STRICTLY_OUTSIDE;

    double p_dropY[2]  = { p[0],  p[2]};
    double t0_dropY[2] = {t0[0], t0[2]}, t1_dropY[2] = {t1[0], t1[2]}, t2_dropY[2] = {t2[0], t2[2]};

    if(point_in_triangle_2d(p_dropY, t0_dropY, t1_dropY, t2_dropY) == STRICTLY_OUTSIDE) return STRICTLY_OUTSIDE;

    double p_dropZ[2]  = { p[0],  p[1]};
    double t0_dropZ[2] = {t0[0], t0[1]}, t1_dropZ[2] = {t1[0], t1[1]}, t2_dropZ[2] = {t2[0], t2[1]};

    if(point_in_triangle_2d(p_dropZ, t0_dropZ, t1_dropZ, t2_dropZ) == STRICTLY_OUTSIDE) return STRICTLY_OUTSIDE;

    return STRICTLY_INSIDE;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of t
// ON_EDGEj         if p lies inside the j-th edge of t (endpoints excluded)
// ON_FACEk         if p lies inside the k-th face of t (borders excluded)
// STRICTLY_INSIDE  if p lies inside tetrahedron t (borders excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_tet(const vec3d & p,
                            const vec3d & t0,
                            const vec3d & t1,
                            const vec3d & t2,
                            const vec3d & t3)
{
    return point_in_tet(p.ptr(), t0.ptr(), t1.ptr(), t2.ptr(), t3.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// ON_VERTi         if p coincides with the i-th vertex of t
// ON_EDGEj         if p lies inside the j-th edge of t (endpoints excluded)
// ON_FACEk         if p lies inside the k-th face of t (borders excluded)
// STRICTLY_INSIDE  if p lies inside tetrahedron t (borders excluded)
// STRICTLY_OUTSIDE otherwise
CINO_INLINE
PointInSimplex point_in_tet(const double * p,
                            const double * t0,
                            const double * t1,
                            const double * t2,
                            const double * t3)
{
    // test for point in vert
    if(vec_equals_3d(p, t0)) return ON_VERT0;
    if(vec_equals_3d(p, t1)) return ON_VERT1;
    if(vec_equals_3d(p, t2)) return ON_VERT2;
    if(vec_equals_3d(p, t3)) return ON_VERT2;

    // according to refrence tet as in cinolib/standard_elements_tables.h
    double f0p_vol = orient3d(t0,t2,t1,p);
    double f1p_vol = orient3d(t0,t1,t3,p);
    double f2p_vol = orient3d(t0,t3,t2,p);
    double f3p_vol = orient3d(t1,t2,t3,p);

    bool hit = (f0p_vol >= 0 && f1p_vol >= 0 && f2p_vol >= 0 && f3p_vol >= 0) ||
               (f0p_vol <= 0 && f1p_vol <= 0 && f2p_vol <= 0 && f3p_vol <= 0);

    if(hit)
    {
        bool on_f0 = (f0p_vol == 0);
        bool on_f1 = (f1p_vol == 0);
        bool on_f2 = (f2p_vol == 0);
        bool on_f3 = (f3p_vol == 0);

        // according to refrence tet as in cinolib/standard_elements_tables.h
        if(on_f0 && on_f2) return ON_EDGE0;
        if(on_f0 && on_f3) return ON_EDGE1;
        if(on_f0 && on_f1) return ON_EDGE2;
        if(on_f1 && on_f3) return ON_EDGE3;
        if(on_f1 && on_f2) return ON_EDGE4;
        if(on_f2 && on_f3) return ON_EDGE5;

        if(on_f0) return ON_FACE0;
        if(on_f1) return ON_FACE1;
        if(on_f2) return ON_FACE2;
        if(on_f3) return ON_FACE3;

        return STRICTLY_INSIDE;
    }

    return STRICTLY_OUTSIDE;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if segments are fully disjoint
// SIMPLICIAL_COMPLEX   if segments coincide or intersect at a shared vertex
// INTERSECT            if segments intersect at an inner point (for s0, s1, or both)
// OVERLAP              if segments are colinear and partially overlapped
CINO_INLINE
SimplexIntersection segment_segment_intersect_2d(const vec2d & s00,
                                                 const vec2d & s01,
                                                 const vec2d & s10,
                                                 const vec2d & s11)
{
    return segment_segment_intersect_2d(s00.ptr(), s01.ptr(), s10.ptr(), s11.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if segments are fully disjoint
// SIMPLICIAL_COMPLEX   if segments coincide or intersect at a shared vertex
// INTERSECT            if segments intersect at an inner point (for s0, s1, or both)
// OVERLAP              if segments are colinear and partially overlapped
CINO_INLINE
SimplexIntersection segment_segment_intersect_2d(const double * s00,
                                                 const double * s01,
                                                 const double * s10,
                                                 const double * s11)
{
    // https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
    double det_s00 = orient2d(s10, s11, s00);
    double det_s01 = orient2d(s10, s11, s01);
    double det_s10 = orient2d(s00, s01, s10);
    double det_s11 = orient2d(s00, s01, s11);

    // Shewchuk's orient predicates return a rough approximation of the determinant.
    // I am converting values to { -1, 0, 1 } for a simpler check of intersection cases
    int s00_wrt_s1 = (det_s00 > 0) ? 1 : ((det_s00 < 0) ? -1 : 0);
    int s01_wrt_s1 = (det_s01 > 0) ? 1 : ((det_s01 < 0) ? -1 : 0);
    int s10_wrt_s0 = (det_s10 > 0) ? 1 : ((det_s10 < 0) ? -1 : 0);
    int s11_wrt_s0 = (det_s11 > 0) ? 1 : ((det_s11 < 0) ? -1 : 0);

    // segments intersect at a single point
    if(s00_wrt_s1 != s01_wrt_s1 && s10_wrt_s0 != s11_wrt_s0)
    {
        // edges share an endpoint
        if(vec_equals_2d(s00, s10) || vec_equals_2d(s00, s11) || vec_equals_2d(s01, s10) || vec_equals_2d(s01, s11))
            return SIMPLICIAL_COMPLEX;

        // at least one segment endpoint is involved in the intersection
        return INTERSECT;
    }

    // degenerate case: colinear segments
    if(s00_wrt_s1 == 0 && s01_wrt_s1 == 0 && s10_wrt_s0 == 0 && s11_wrt_s0 == 0)
    {
        // coincident segments
        if((vec_equals_2d(s00, s10) && vec_equals_2d(s01, s11)) || (vec_equals_2d(s00, s11) && vec_equals_2d(s01, s10)))
            return SIMPLICIAL_COMPLEX;

        double Xmin_s1 = std::min(s10[0], s11[0]);
        double Xmax_s1 = std::max(s10[0], s11[0]);
        double Ymin_s1 = std::min(s10[1], s11[1]);
        double Ymax_s1 = std::max(s10[1], s11[1]);
        double Xmin_s0 = std::min(s00[0], s01[0]);
        double Xmax_s0 = std::max(s00[0], s01[0]);
        double Ymin_s0 = std::min(s00[1], s01[1]);
        double Ymax_s0 = std::max(s00[1], s01[1]);

        if(// test s0 endpoints against s1 range
           (s00[0] > Xmin_s1 && s00[0] < Xmax_s1) ||
           (s00[1] > Ymin_s1 && s00[1] < Ymax_s1) ||
           (s01[0] > Xmin_s1 && s01[0] < Xmax_s1) ||
           (s01[1] > Ymin_s1 && s01[1] < Ymax_s1) ||
           // test s1 endpoints against s0 range
           (s10[0] > Xmin_s0 && s10[0] < Xmax_s0) ||
           (s10[1] > Ymin_s0 && s10[1] < Ymax_s0) ||
           (s11[0] > Xmin_s0 && s11[0] < Xmax_s0) ||
           (s11[1] > Ymin_s0 && s11[1] < Ymax_s0))
        {
            return INTERSECT;
        }
    }
    return DO_NOT_INTERSECT;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if segments are fully disjoint
// SIMPLICIAL_COMPLEX   if segments coincide or intersect at a shared vertex
// INTERSECT            if segments intersect at an inner point (for s0, s1, or both)
// OVERLAP              if segments are colinear and partially overlapped
CINO_INLINE
SimplexIntersection segment_segment_intersect_3d(const vec3d & s00,
                                                 const vec3d & s01,
                                                 const vec3d & s10,
                                                 const vec3d & s11)
{
    return segment_segment_intersect_3d(s00.ptr(), s01.ptr(), s10.ptr(), s11.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if segments are fully disjoint
// SIMPLICIAL_COMPLEX   if segments coincide or intersect at a shared vertex
// INTERSECT            if segments intersect at an inner point (for s0, s1, or both)
// OVERLAP              if segments are colinear and partially overlapped
CINO_INLINE
SimplexIntersection segment_segment_intersect_3d(const double * s00,
                                                 const double * s01,
                                                 const double * s10,
                                                 const double * s11)
{
    assert(!segment_is_degenerate_3d(s00, s01) && !segment_is_degenerate_3d(s10, s11));

    if(!points_are_coplanar_3d(s00, s01, s10, s11)) return DO_NOT_INTERSECT;

    // check for coincident segments
    bool s00_is_shared = (vec_equals_3d(s00, s10) || vec_equals_3d(s00, s11));
    bool s01_is_shared = (vec_equals_3d(s01, s10) || vec_equals_3d(s01, s11));
    bool s10_is_shared = (vec_equals_3d(s10, s00) || vec_equals_3d(s10, s01));
    bool s11_is_shared = (vec_equals_3d(s11, s00) || vec_equals_3d(s11, s01));

    // s0 and s1 are coincident or one edge is degenerate and coincides with one vertex of the other
    if(s00_is_shared && s01_is_shared && s10_is_shared && s11_is_shared) return SIMPLICIAL_COMPLEX;

    // check 2D projections of the segments

    double s00_dropX[2] = {s00[1], s00[2]}, s01_dropX[2] = {s01[1], s01[2]};
    double s10_dropX[2] = {s10[1], s10[2]}, s11_dropX[2] = {s11[1], s11[2]};
    int x_res = segment_segment_intersect_2d(s00_dropX, s01_dropX, s10_dropX, s11_dropX);
    if (x_res == DO_NOT_INTERSECT) return DO_NOT_INTERSECT;

    double s00_dropY[2] = {s00[0], s00[2]}, s01_dropY[2] = {s01[0], s01[2]};
    double s10_dropY[2] = {s10[0], s10[2]}, s11_dropY[2] = {s11[0], s11[2]};
    int y_res = segment_segment_intersect_2d(s00_dropY, s01_dropY, s10_dropY, s11_dropY);
    if (y_res == DO_NOT_INTERSECT) return DO_NOT_INTERSECT;

    double s00_dropZ[2] = {s00[0], s00[1]}, s01_dropZ[2] = {s01[0], s01[1]};
    double s10_dropZ[2] = {s10[0], s10[1]}, s11_dropZ[2] = {s11[0], s11[1]};
    int z_res = segment_segment_intersect_2d(s00_dropZ, s01_dropZ, s10_dropZ, s11_dropZ);
    if (z_res == DO_NOT_INTERSECT) return DO_NOT_INTERSECT;

    // segments are deemed overlapping if they are so in at least two projections our of three
    // (overlapping axis aligned segments will look like a valid simplcial complex in one projection)
    if((x_res == OVERLAP && y_res == OVERLAP) ||
       (x_res == OVERLAP && z_res == OVERLAP) ||
       (y_res == OVERLAP && z_res == OVERLAP))
    {
        return OVERLAP;
    }

    return INTERSECT;

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if s and t are fully disjoint
// SIMPLICIAL_COMPLEX   if s is an edge of t, or s is degenerate and coincides with a vertex of t
// INTERSECT            if s and t intersect and do not forma a valid simplex
CINO_INLINE
SimplexIntersection segment_triangle_intersect_2d(const vec2d & s0,
                                                  const vec2d & s1,
                                                  const vec2d & t0,
                                                  const vec2d & t1,
                                                  const vec2d & t2)
{
    return segment_triangle_intersect_2d(s0.ptr(), s1.ptr(), t0.ptr(), t1.ptr(), t2.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if s and t are fully disjoint
// SIMPLICIAL_COMPLEX   if s is an edge of t, or s is degenerate and coincides with a vertex of t
// INTERSECT            if s and t intersect and do not forma a valid simplex
CINO_INLINE
SimplexIntersection segment_triangle_intersect_2d(const double * s0,
                                                  const double * s1,
                                                  const double * t0,
                                                  const double * t1,
                                                  const double * t2)
{
    assert(!segment_is_degenerate_2d(s0, s1) && !triangle_is_degenerate_2d(t0, t1, t2));

    if(point_in_triangle_2d(s0, t0, t1, t2) >= STRICTLY_INSIDE ||
       point_in_triangle_2d(s1, t0, t1, t2) >= STRICTLY_INSIDE)
    {
        return INTERSECT;
    }

    bool simpl_complex = false;

    switch(segment_segment_intersect_2d(s0, s1, t0, t1))
    {
        case SIMPLICIAL_COMPLEX : simpl_complex = true; break;
        case INTERSECT          : return INTERSECT;
        case OVERLAP            : return INTERSECT;
        case DO_NOT_INTERSECT   : break;
    }

    switch(segment_segment_intersect_2d(s0, s1, t1, t2))
    {
        case SIMPLICIAL_COMPLEX : simpl_complex = true; break;
        case INTERSECT          : return INTERSECT;
        case OVERLAP            : return INTERSECT;
        case DO_NOT_INTERSECT   : break;
    }

    switch(segment_segment_intersect_2d(s0, s1, t2, t0))
    {
        case SIMPLICIAL_COMPLEX : simpl_complex = true; break;
        case INTERSECT          : return INTERSECT;
        case OVERLAP            : return INTERSECT;
        case DO_NOT_INTERSECT   : break;
    }

    if(simpl_complex) return SIMPLICIAL_COMPLEX;
    return DO_NOT_INTERSECT;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if s and t are fully disjoint
// SIMPLICIAL_COMPLEX   if s is an edge of t, or s is degenerate and coincides with a vertex of t
// INTERSECT            if s and t intersect and do not form a valid simplex
CINO_INLINE
SimplexIntersection segment_triangle_intersect_3d(const vec3d & s0,
                                                  const vec3d & s1,
                                                  const vec3d & t0,
                                                  const vec3d & t1,
                                                  const vec3d & t2)
{
    return segment_triangle_intersect_3d(s0.ptr(), s1.ptr(), t0.ptr(), t1.ptr(), t2.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if s and t are fully disjoint
// SIMPLICIAL_COMPLEX   if s is an edge of t, or s is degenerate and coincides with a vertex of t
// INTERSECT            if s and t intersect and do not form a valid simplex
CINO_INLINE
SimplexIntersection segment_triangle_intersect_3d(const double * s0,
                                                  const double * s1,
                                                  const double * t0,
                                                  const double * t1,
                                                  const double * t2)
{
    assert(!segment_is_degenerate_3d(s0, s1) && !triangle_is_degenerate_3d(t0, t1, t2));

    if((vec_equals_3d(s0, t0) || vec_equals_3d(s0, t1) || vec_equals_3d(s0, t2)) &&
       (vec_equals_3d(s1, t0) || vec_equals_3d(s1, t1) || vec_equals_3d(s1, t2)))
    {
        return SIMPLICIAL_COMPLEX;
    }

    auto vol_s0_t = orient3d(s0, t0, t1, t2);
    auto vol_s1_t = orient3d(s1, t0, t1, t2);

    if(vol_s0_t > 0 && vol_s1_t > 0) return DO_NOT_INTERSECT; // s is above t
    if(vol_s0_t < 0 && vol_s1_t < 0) return DO_NOT_INTERSECT; // s is below t
    if(vol_s0_t == 0 && vol_s1_t == 0)                        // s and t are coplanar
    {
        // same code as the 2D version, I just copied it here....

        if(point_in_triangle_3d(s0, t0, t1, t2) || point_in_triangle_3d(s1, t0, t1, t2)) return INTERSECT;

        uint simpl_complex = 0;

        switch(segment_segment_intersect_3d(s0, s1, t0, t1))
        {
            case SIMPLICIAL_COMPLEX : ++simpl_complex; break;
            case INTERSECT          : return INTERSECT;
            case OVERLAP            : return INTERSECT;
            case DO_NOT_INTERSECT   : break;
        }

        switch(segment_segment_intersect_3d(s0, s1, t1, t2))
        {
            case SIMPLICIAL_COMPLEX : ++simpl_complex; break;
            case INTERSECT          : return INTERSECT;
            case OVERLAP            : return INTERSECT;
            case DO_NOT_INTERSECT   : break;
        }

        switch(segment_segment_intersect_3d(s0, s1, t2, t0))
        {
            case SIMPLICIAL_COMPLEX : ++simpl_complex; break;
            case INTERSECT          : return INTERSECT;
            case OVERLAP            : return INTERSECT;
            case DO_NOT_INTERSECT   : break;
        }

        // if it is a simplicial complex from any view, then it really is...
        if(simpl_complex == 3) return SIMPLICIAL_COMPLEX;
        return DO_NOT_INTERSECT;
    }

    // s intersects t (borders included), if the signs of the three tetrahedra
    // obtained combining s with the three edges of t are all equal

    // if one point is coplanar and coincides with a triangle vertex, then they form a valid complex
    if(vec_equals_3d(s0, t0) || vec_equals_3d(s0, t1) || vec_equals_3d(s0, t2) ||
       vec_equals_3d(s1, t0) || vec_equals_3d(s1, t1) || vec_equals_3d(s1, t2))
    {
        return SIMPLICIAL_COMPLEX;
    }

    double vol_s_t01 = orient3d(s0, s1, t0, t1);
    double vol_s_t12 = orient3d(s0, s1, t1, t2);
    double vol_s_t20 = orient3d(s0, s1, t2, t0);

    if((vol_s_t01 > 0 && vol_s_t12 < 0) || (vol_s_t01 < 0 && vol_s_t12 > 0)) return DO_NOT_INTERSECT;
    if((vol_s_t12 > 0 && vol_s_t20 < 0) || (vol_s_t12 < 0 && vol_s_t20 > 0)) return DO_NOT_INTERSECT;
    if((vol_s_t20 > 0 && vol_s_t01 < 0) || (vol_s_t20 < 0 && vol_s_t01 > 0)) return DO_NOT_INTERSECT;

    return INTERSECT;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if s and t are fully disjoint
// SIMPLICIAL_COMPLEX   if s is an edge of t, or s is degenerate and coincides with a vertex of t
// INTERSECT            if s and t intersect and do not forma a valid simplex
CINO_INLINE
SimplexIntersection segment_tet_intersect_3d(const vec3d & s0,
                                             const vec3d & s1,
                                             const vec3d & t0,
                                             const vec3d & t1,
                                             const vec3d & t2,
                                             const vec3d & t3)
{
    return segment_tet_intersect_3d(s0.ptr(), s1.ptr(), t0.ptr(), t1.ptr(), t2.ptr(), t3.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if s and t are fully disjoint
// SIMPLICIAL_COMPLEX   if s is an edge of t, or s is degenerate and coincides with a vertex of t
// INTERSECT            if s and t intersect and do not forma a valid simplex
CINO_INLINE
SimplexIntersection segment_tet_intersect_3d(const double * s0,
                                             const double * s1,
                                             const double * t0,
                                             const double * t1,
                                             const double * t2,
                                             const double * t3)
{
    assert(!segment_is_degenerate_3d(s0, s1) && !tet_is_degenerate(t0, t1, t2, t3));

    // check if s is an edge of t
    if((vec_equals_3d(s0, t0) && vec_equals_3d(s1, t2)) || (vec_equals_3d(s1, t0) && vec_equals_3d(s0, t2))) return SIMPLICIAL_COMPLEX;
    if((vec_equals_3d(s0, t2) && vec_equals_3d(s1, t1)) || (vec_equals_3d(s1, t2) && vec_equals_3d(s0, t1))) return SIMPLICIAL_COMPLEX;
    if((vec_equals_3d(s0, t1) && vec_equals_3d(s1, t0)) || (vec_equals_3d(s1, t1) && vec_equals_3d(s0, t0))) return SIMPLICIAL_COMPLEX;
    if((vec_equals_3d(s0, t1) && vec_equals_3d(s1, t3)) || (vec_equals_3d(s1, t1) && vec_equals_3d(s0, t3))) return SIMPLICIAL_COMPLEX;
    if((vec_equals_3d(s0, t3) && vec_equals_3d(s1, t0)) || (vec_equals_3d(s1, t3) && vec_equals_3d(s0, t0))) return SIMPLICIAL_COMPLEX;
    if((vec_equals_3d(s0, t3) && vec_equals_3d(s1, t2)) || (vec_equals_3d(s1, t3) && vec_equals_3d(s0, t2))) return SIMPLICIAL_COMPLEX;

    // check if s intersects any of the faces of t
    if(segment_triangle_intersect_3d(s0, s1, t0, t2, t1) >= INTERSECT ||
       segment_triangle_intersect_3d(s0, s1, t0, t1, t3) >= INTERSECT ||
       segment_triangle_intersect_3d(s0, s1, t0, t3, t2) >= INTERSECT ||
       segment_triangle_intersect_3d(s0, s1, t1, t2, t3) >= INTERSECT)
    {
        return INTERSECT;
    }

    // locate s endpoints wrt t
    auto s0_wrt_t = point_in_tet(s0, t0, t1, t2, t3);
    auto s1_wrt_t = point_in_tet(s1, t0, t1, t2, t3);

    // if one veertex of s is a vertex of t, and the other is outside then s touches t only at that vertex
    // (otherwise there would be an intersection between s and any of the faces of t, which I already tested)
    if(s0_wrt_t >= ON_VERT0 && s0_wrt_t <= ON_VERT3 && s1_wrt_t == STRICTLY_OUTSIDE) return SIMPLICIAL_COMPLEX;
    if(s1_wrt_t >= ON_VERT0 && s1_wrt_t <= ON_VERT3 && s0_wrt_t == STRICTLY_OUTSIDE) return SIMPLICIAL_COMPLEX;

    // if a point of s is either inside, or on some edge or on some face of t, then they intersect
    if(s0_wrt_t == STRICTLY_INSIDE || s0_wrt_t >= ON_EDGE0) return INTERSECT;
    if(s1_wrt_t == STRICTLY_INSIDE || s1_wrt_t >= ON_EDGE0) return INTERSECT;

    return DO_NOT_INTERSECT;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if triangles are fully disjoint
// SIMPLICIAL_COMPLEX   if triangles coincide or intersect at a shared sub-simplex
// INTERSECT            if triangles intersect without making a valid simplcial complex
CINO_INLINE
SimplexIntersection triangle_triangle_intersect_2d(const vec2d & t00,
                                                   const vec2d & t01,
                                                   const vec2d & t02,
                                                   const vec2d & t10,
                                                   const vec2d & t11,
                                                   const vec2d & t12)
{
    return triangle_triangle_intersect_2d(t00.ptr(), t01.ptr(), t02.ptr(),
                                          t10.ptr(), t11.ptr(), t12.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if triangles are fully disjoint
// SIMPLICIAL_COMPLEX   if triangles coincide or intersect at a shared sub-simplex
// INTERSECT            if triangles intersect without making a valid simplcial complex
CINO_INLINE
SimplexIntersection triangle_triangle_intersect_2d(const double * t00,
                                                   const double * t01,
                                                   const double * t02,
                                                   const double * t10,
                                                   const double * t11,
                                                   const double * t12)
{
    assert(!triangle_is_degenerate_2d(t00, t01, t02) &&
           !triangle_is_degenerate_2d(t10, t11, t12));

    // binary flags to mark coincident vertices in t0 and t1
    std::bitset<3> t0_shared = { 0b000 };
    std::bitset<3> t1_shared = { 0b000 };

    // find vert correspondences
    if(vec_equals_2d(t00, t10)) { t0_shared[0] = true; t1_shared[0] = true; }
    if(vec_equals_2d(t00, t11)) { t0_shared[0] = true; t1_shared[1] = true; }
    if(vec_equals_2d(t00, t12)) { t0_shared[0] = true; t1_shared[2] = true; }
    if(vec_equals_2d(t01, t10)) { t0_shared[1] = true; t1_shared[0] = true; }
    if(vec_equals_2d(t01, t11)) { t0_shared[1] = true; t1_shared[1] = true; }
    if(vec_equals_2d(t01, t12)) { t0_shared[1] = true; t1_shared[2] = true; }
    if(vec_equals_2d(t02, t10)) { t0_shared[2] = true; t1_shared[0] = true; }
    if(vec_equals_2d(t02, t11)) { t0_shared[2] = true; t1_shared[1] = true; }
    if(vec_equals_2d(t02, t12)) { t0_shared[2] = true; t1_shared[2] = true; }

    // count number of coincident vertices in t0 and t1
    uint t0_count = uint(t0_shared.count());
    uint t1_count = uint(t1_shared.count());

    // either t0 and t1 are coincident or one of the two triangles
    // is degenerate and is an edge/vertex of the other
    if(t0_count == 3 || t1_count == 3) return SIMPLICIAL_COMPLEX;

    // t0 and t1 share an edge. Let e be the shared edge and { opp0, opp1 } be the two vertices opposite to
    // e in t0 and t1, respectively. If opp0 and opp1 lie at the same side of e, the two triangles overlap.
    // Otherwise they are edge-adjacent and form a valid simplicial complex
    if(t0_count == 2 && t1_count == 2)
    {
        uint e[2];      // indices of the shared vertices (in t0)
        uint count = 0; // index for e (to fill it)
        uint opp0  = 0; // index of the vertex opposite to e in t0
        uint opp1  = 0; // index of the vertex opposite to e in t1
        for(uint i=0; i<3; ++i)
        {
            if(!t0_shared[i]) opp0 = i; else e[count++] = i;
            if(!t1_shared[i]) opp1 = i;
        }

        const double* t0[3] = {t00, t01, t02};
        const double* t1[3] = {t10, t11, t12};

        double opp0_wrt_e = orient2d(t0[e[0]], t0[e[1]], t0[opp0]);
        double opp1_wrt_e = orient2d(t0[e[0]], t0[e[1]], t1[opp1]);

        if((opp0_wrt_e>0 && opp1_wrt_e<0) || (opp0_wrt_e<0 && opp1_wrt_e>0)) return SIMPLICIAL_COMPLEX;
        return INTERSECT;
    }

    // t0 and t1 share a vertex. Let v be the shared vertex and { opp0 , opp1 } be the two edges opposite to
    // v in t0 and t1, respectively. If v-opp0 intersects t1, or v-opp1 interects t0, the two triangles overlap.
    // Otherwise they are verte-adjacent and form a valid simplicial complex
    if(t0_count == 1 && t1_count == 1)
    {
        uint v0; // index of the shared vertex in t0
        uint v1; // index of the shared vertex in t1
        for(uint i = 0; i < 3; ++i)
        {
            if(t0_shared[i]) v0 = i;
            if(t1_shared[i]) v1 = i;
        }

        const double* t0[3] = {t00, t01, t02};
        const double* t1[3] = {t10, t11, t12};

        // check for intersection with t0 and t1 edges emanating from v1 and v0, respectively
        const double* e_v0_0[] = { t0[v0], t0[(v0 +1) %3] };
        const double* e_v0_1[] = { t0[v0], t0[(v0 +2) %3] };
        const double* e_v1_0[] = { t1[v1], t1[(v1 +1) %3] };
        const double* e_v1_1[] = { t1[v1], t1[(v1 +2) %3] };
        if(segment_triangle_intersect_2d(e_v0_0[0], e_v0_0[1], t10, t11, t12) == INTERSECT ||
           segment_triangle_intersect_2d(e_v0_1[0], e_v0_1[1], t10, t11, t12) == INTERSECT ||
           segment_triangle_intersect_2d(e_v1_0[0], e_v1_0[1], t00, t01, t02) == INTERSECT ||
           segment_triangle_intersect_2d(e_v1_1[0], e_v1_1[1], t00, t01, t02) == INTERSECT)
        {
            return INTERSECT;
        }
        return SIMPLICIAL_COMPLEX;
    }

    // t0 and t1 do not share sub-simplices. They can be fully disjoint, intersecting at a single point, or overlapping

    if(segment_segment_intersect_2d(t00, t01, t10, t11) >= INTERSECT ||
       segment_segment_intersect_2d(t00, t01, t11, t12) >= INTERSECT ||
       segment_segment_intersect_2d(t00, t01, t12, t10) >= INTERSECT ||
       segment_segment_intersect_2d(t01, t02, t10, t11) >= INTERSECT ||
       segment_segment_intersect_2d(t01, t02, t11, t12) >= INTERSECT ||
       segment_segment_intersect_2d(t01, t02, t12, t10) >= INTERSECT ||
       segment_segment_intersect_2d(t02, t00, t10, t11) >= INTERSECT ||
       segment_segment_intersect_2d(t02, t00, t11, t12) >= INTERSECT ||
       segment_segment_intersect_2d(t02, t00, t12, t10) >= INTERSECT )
    {
        return INTERSECT;
    }

    if(point_in_triangle_2d(t00, t10, t11, t12) >= STRICTLY_INSIDE ||
       point_in_triangle_2d(t01, t10, t11, t12) >= STRICTLY_INSIDE ||
       point_in_triangle_2d(t02, t10, t11, t12) >= STRICTLY_INSIDE ||
       point_in_triangle_2d(t10, t00, t01, t02) >= STRICTLY_INSIDE ||
       point_in_triangle_2d(t11, t00, t01, t02) >= STRICTLY_INSIDE ||
       point_in_triangle_2d(t12, t00, t01, t02) >= STRICTLY_INSIDE )
    {
        return INTERSECT;
    }

    return DO_NOT_INTERSECT;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if triangles are fully disjoint
// SIMPLICIAL_COMPLEX   if triangles coincide or intersect at a shared sub-simplex
// INTERSECT            if triangles intersect without making a valid simplcial complex
CINO_INLINE
SimplexIntersection triangle_triangle_intersect_3d(const vec3d & t00,
                                                   const vec3d & t01,
                                                   const vec3d & t02,
                                                   const vec3d & t10,
                                                   const vec3d & t11,
                                                   const vec3d & t12)
{
    return triangle_triangle_intersect_3d(t00.ptr(), t01.ptr(), t02.ptr(),
                                          t10.ptr(), t11.ptr(), t12.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns:
// DO_NOT_INTERSECT     if triangles are fully disjoint
// SIMPLICIAL_COMPLEX   if triangles coincide or intersect at a shared sub-simplex
// INTERSECT            if triangles intersect without making a valid simplcial complex
CINO_INLINE
SimplexIntersection triangle_triangle_intersect_3d(const double * t00,
                                                   const double * t01,
                                                   const double * t02,
                                                   const double * t10,
                                                   const double * t11,
                                                   const double * t12)
{
    assert(!triangle_is_degenerate_3d(t00, t01, t02) &&
           !triangle_is_degenerate_3d(t10, t11, t12));

    // binary flags to mark coincident vertices in t0 and t1
    std::bitset<3> t0_shared = { 0b000 };
    std::bitset<3> t1_shared = { 0b000 };

    // find vert correspondences
    if(vec_equals_3d(t00, t10)) { t0_shared[0] = true; t1_shared[0] = true; }
    if(vec_equals_3d(t00, t11)) { t0_shared[0] = true; t1_shared[1] = true; }
    if(vec_equals_3d(t00, t12)) { t0_shared[0] = true; t1_shared[2] = true; }
    if(vec_equals_3d(t01, t10)) { t0_shared[1] = true; t1_shared[0] = true; }
    if(vec_equals_3d(t01, t11)) { t0_shared[1] = true; t1_shared[1] = true; }
    if(vec_equals_3d(t01, t12)) { t0_shared[1] = true; t1_shared[2] = true; }
    if(vec_equals_3d(t02, t10)) { t0_shared[2] = true; t1_shared[0] = true; }
    if(vec_equals_3d(t02, t11)) { t0_shared[2] = true; t1_shared[1] = true; }
    if(vec_equals_3d(t02, t12)) { t0_shared[2] = true; t1_shared[2] = true; }

    // count number of coincident vertices in t0 and t1
    uint t0_count = uint(t0_shared.count());

    // either t0 and t1 are coincident
    if(t0_count == 3) return SIMPLICIAL_COMPLEX;

    // t0 and t1 share an edge. Let e be the shared edge and { opp0, opp1 } be the two vertices opposite to
    // e in t0 and t1, respectively. If opp0 and opp1 lie at the same side of e, the two triangles overlap.
    // Otherwise they are edge-adjacent and form a valid simplicial complex
    if(t0_count == 2)
    {
        uint e[2];      // indices of the shared vertices (in t0)
        uint count = 0; // index for e (to fill it)
        uint opp0  = 0; // index of the vertex opposite to e in t0
        uint opp1  = 0; // index of the vertex opposite to e in t1
        for(uint i = 0; i < 3; ++i)
        {
            if(!t0_shared[i]) opp0 = i; else e[count++] = i;
            if(!t1_shared[i]) opp1 = i;
        }

        const double* t0[3] = {t00, t01, t02};
        const double* t1[3] = {t10, t11, t12};

        // if they are not coplanar, then they form a valid complex
        if(orient3d(t00, t01, t02, t1[opp1]) != 0) return SIMPLICIAL_COMPLEX;

        double e0_dropX[2]   = {t0[e[0]][1], t0[e[0]][2]};
        double e1_dropX[2]   = {t0[e[1]][1], t0[e[1]][2]};
        double opp0_dropX[2] = {t0[opp0][1], t0[opp0][2]};
        double opp1_dropX[2] = {t1[opp1][1], t1[opp1][2]};
        double opp0_wrt_e = orient2d(e0_dropX, e1_dropX, opp0_dropX);
        double opp1_wrt_e = orient2d(e0_dropX, e1_dropX, opp1_dropX);
        if((opp0_wrt_e > 0 && opp1_wrt_e < 0) || (opp0_wrt_e < 0 && opp1_wrt_e > 0)) return SIMPLICIAL_COMPLEX;

        double e0_dropY[2]   = {t0[e[0]][0], t0[e[0]][2]};
        double e1_dropY[2]   = {t0[e[1]][0], t0[e[1]][2]};
        double opp0_dropY[2] = {t0[opp0][0], t0[opp0][2]};
        double opp1_dropY[2] = {t1[opp1][0], t1[opp1][2]};
        opp0_wrt_e = orient2d(e0_dropY, e1_dropY, opp0_dropY);
        opp1_wrt_e = orient2d(e0_dropY, e1_dropY, opp1_dropY);
        if((opp0_wrt_e > 0 && opp1_wrt_e < 0) || (opp0_wrt_e < 0 && opp1_wrt_e > 0)) return SIMPLICIAL_COMPLEX;

        double e0_dropZ[2]   = {t0[e[0]][0], t0[e[0]][1]};
        double e1_dropZ[2]   = {t0[e[1]][0], t0[e[1]][1]};
        double opp0_dropZ[2] = {t0[opp0][0], t0[opp0][1]};
        double opp1_dropZ[2] = {t1[opp1][0], t1[opp1][1]};
        opp0_wrt_e = orient2d(e0_dropZ, e1_dropZ, opp0_dropZ);
        opp1_wrt_e = orient2d(e0_dropZ, e1_dropZ, opp1_dropZ);
        if((opp0_wrt_e > 0 && opp1_wrt_e < 0) || (opp0_wrt_e < 0 && opp1_wrt_e > 0)) return SIMPLICIAL_COMPLEX;

        return INTERSECT;
    }

    // t0 and t1 share a vertex. Let v be the shared vertex and { opp0 , opp1 } be the two edges opposite to
    // v in t0 and t1, respectively. If opp0 intersects t1, or opp1 interects t0, the two triangles overlap.
    // Otherwise they are vertex-adjacent and form a valid simplicial complex
    if(t0_count == 1)
    {
        uint v0; // index of the shared vertex in t0
        uint v1; // index of the shared vertex in t1
        for(uint i = 0; i < 3; ++i)
        {
            if(t0_shared[i]) v0 = i;
            if(t1_shared[i]) v1 = i;
        }

        const double* t0[3] = {t00, t01, t02};
        const double* t1[3] = {t10, t11, t12};

        const double* opp0[2] = { t0[(v0 +1) %3], t0[(v0 +2) %3] };
        const double* opp1[2] = { t1[(v1 +1) %3], t1[(v1 +2) %3] };

        if(segment_triangle_intersect_3d(opp0[0], opp0[1], t10, t11, t12) >= INTERSECT ||
           segment_triangle_intersect_3d(opp1[0], opp1[1], t00, t01, t02) >= INTERSECT)
        {
            return INTERSECT;
        }
        return SIMPLICIAL_COMPLEX;
    }

    // t0 and t1 do not share sub-simplices. They can be fully disjoint, intersecting at a single point, or overlapping

    if(segment_triangle_intersect_3d(t00, t01, t10, t11, t12) >= INTERSECT ||
       segment_triangle_intersect_3d(t01, t02, t10, t11, t12) >= INTERSECT ||
       segment_triangle_intersect_3d(t02, t00, t10, t11, t12) >= INTERSECT ||
       segment_triangle_intersect_3d(t10, t11, t00, t01, t02) >= INTERSECT ||
       segment_triangle_intersect_3d(t11, t12, t00, t01, t02) >= INTERSECT ||
       segment_triangle_intersect_3d(t12, t10, t00, t01, t02) >= INTERSECT)
    {
        return INTERSECT;
    }

    return DO_NOT_INTERSECT;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if s0==s1
CINO_INLINE
bool segment_is_degenerate_2d(const vec2d & s0,
                              const vec2d & s1)
{
    return segment_is_degenerate_2d(s0.ptr(), s1.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if s0==s1
CINO_INLINE
bool segment_is_degenerate_3d(const vec3d & s0,
                              const vec3d & s1)
{
    return segment_is_degenerate_3d(s0.ptr(), s1.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if s0==s1
CINO_INLINE
bool segment_is_degenerate_2d(const double * s0,
                              const double * s1)
{
    return vec_equals_2d(s0, s1);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if s0==s1
CINO_INLINE
bool segment_is_degenerate_3d(const double * s0,
                              const double * s1)
{
    return vec_equals_3d(s0, s1);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if t0, t1 and t2 are colinear
CINO_INLINE
bool triangle_is_degenerate_2d(const vec2d & t0,
                               const vec2d & t1,
                               const vec2d & t2)
{
    return triangle_is_degenerate_2d(t0.ptr(), t1.ptr(), t2.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if t0, t1 and t2 are colinear
CINO_INLINE
bool triangle_is_degenerate_3d(const vec3d & t0,
                               const vec3d & t1,
                               const vec3d & t2)
{
    return triangle_is_degenerate_3d(t0.ptr(), t1.ptr(), t2.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if t0, t1 and t2 are colinear
CINO_INLINE
bool triangle_is_degenerate_2d(const double * t0,
                               const double * t1,
                               const double * t2)
{
    return points_are_colinear_2d(t0, t1, t2);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if t0, t1 and t2 are colinear
CINO_INLINE
bool triangle_is_degenerate_3d(const double * t0,
                               const double * t1,
                               const double * t2)
{
    return points_are_colinear_3d(t0, t1, t2);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if t0, t1, t2 and t3 are coplanar
CINO_INLINE
bool tet_is_degenerate(const vec3d & t0,
                       const vec3d & t1,
                       const vec3d & t2,
                       const vec3d & t3)
{
    return tet_is_degenerate(t0.ptr(), t1.ptr(), t2.ptr(), t3.ptr());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if t0, t1, t2 and t3 are coplanar
CINO_INLINE
bool tet_is_degenerate(const double * t0,
                       const double * t1,
                       const double * t2,
                       const double * t3)
{
    return points_are_coplanar_3d(t0, t1, t2, t3);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if v0 and v1 are equal
CINO_INLINE
bool vec_equals_2d(const double * v0,
                   const double * v1)
{
    return ((v0[0] == v1[0]) &&
            (v0[1] == v1[1]));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// returns true if v0 and v1 are equal
CINO_INLINE
bool vec_equals_3d(const double * v0,
                   const double * v1)
{
    return ((v0[0] == v1[0]) &&
            (v0[1] == v1[1]) &&
            (v0[2] == v1[2]));
}

}
