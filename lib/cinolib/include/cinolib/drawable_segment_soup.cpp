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
#include <cinolib/drawable_segment_soup.h>
#include <cinolib/cino_inline.h>
#include <cinolib/gl/draw_sphere.h>
#include <cinolib/gl/draw_cylinder.h>

namespace cinolib
{

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
DrawableSegmentSoup::DrawableSegmentSoup() {}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void DrawableSegmentSoup::draw(const float scene_size) const
{
    if(no_depth_test) glDisable(GL_DEPTH_TEST);

    if(!use_gl_lines)
    {
        float cylind_rad = scene_size*0.002f*thickness;

        for(uint i=0; i<size()/2; ++i)
        {
            const Color & c = (colors.size()>i) ? colors.at(i) : default_color;
            draw_cylinder(at(2*i+0), at(2*i+1), cylind_rad, cylind_rad, c, segment_n_sides);

            if(draw_joint_spheres)
            {
                draw_sphere(at(2*i+0), cylind_rad, c, joint_sphere_subd);
            }
        }
    }
    else
    {
        glLineWidth(thickness);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);
        glDisable(GL_LIGHTING);
        for(uint i=0; i<size()/2; ++i)
        {
            const Color & c = (colors.size()>i) ? colors.at(i) : default_color;
            glColor3fv(c.rgba);
            vec3d a = at(2*i+0);
            vec3d b = at(2*i+1);
            glBegin(GL_LINES);
                glVertex3d(a.x(), a.y(), a.z());
                glVertex3d(b.x(), b.y(), b.z());
            glEnd();
        }
        glColor3f(0,0,0);
        glEnable(GL_LIGHTING);
        glDisable(GL_LINE_SMOOTH);
    }

    if(no_depth_test) glEnable(GL_DEPTH_TEST);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void DrawableSegmentSoup::push_seg(const vec3d v0, const vec3d v1)
{
    push_back(v0);
    push_back(v1);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void DrawableSegmentSoup::push_seg(const vec3d v0, const vec3d v1, const Color & color)
{
    // if it is the first non default color, fill
    uint n_segs = uint(this->size()/2);
    if(colors.size()<n_segs) colors.resize(n_segs,default_color);
    colors.push_back(color);
    push_back(v0);
    push_back(v1);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void DrawableSegmentSoup::pop_seg()
{
    assert(size()>1);
    pop_back();
    pop_back();
}

}
