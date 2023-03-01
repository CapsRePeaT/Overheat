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
#ifndef CINO_DRAW_CYLINDER_H
#define CINO_DRAW_CYLINDER_H

#ifdef CINOLIB_USES_OPENGL_GLFW_IMGUI

#include <cinolib/geometry/vec_mat.h>
#include <cinolib/cylinder.h>
#include <cinolib/color.h>
#include <cinolib/gl/gl_glfw.h>

namespace cinolib
{

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// this function does only the rendering. It assumes the cylinder has already
// been constructed and a tessellation is expected in input. Useful to pay the
// cost of tessellation just once, and then render many instances of the same
// cylinder in the scene (e.g. for curve networks)
template<class T>
CINO_INLINE
void draw_cylinder(const mat<3,1,T>         & base,    // base
                   const mat<3,1,T>         & dir,     // axis orientation
                   const std::vector<float> & verts,   // tessellation verts
                   const std::vector<uint>  & tris,    // tessellation triangles
                   const std::vector<float> & normals, // tessellation normals (per vertex)
                   const Color              & color)   // color
{
    mat<3,1,T> Z     = mat<3,1,T>(0,0,1);
    mat<3,1,T> axis  = dir.cross(Z); axis.normalize();
    double     angle = dir.angle_deg(Z,true);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glColor3fv(color.rgba);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(base[0], base[1], base[2]);
    glRotated(-angle, axis[0], axis[1], axis[2]);
    glVertexPointer(3, GL_FLOAT, 0, verts.data());
    glNormalPointer(GL_FLOAT, 0, normals.data());
    glDrawElements(GL_TRIANGLES, (GLsizei)tris.size(), GL_UNSIGNED_INT, tris.data());
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glColor3f(1.f,1.f,1.f);
    glDisable(GL_CULL_FACE);
    glDisable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// tessellate and render a new cylinder
template<class T>
CINO_INLINE
void draw_cylinder(const mat<3,1,T> & bot,
                   const mat<3,1,T> & top,
                   const float        bot_radius,
                   const float        top_radius,
                   const Color      & color,
                   const uint         n_sides = 6)
{
    // tessellation
    std::vector<float> verts, normals;
    std::vector<uint>  tris;
    cylinder((float)bot.dist(top), bot_radius, top_radius, n_sides, verts, tris, normals);
    // rendering
    draw_cylinder(bot, top-bot, verts, tris, normals, color);
}

}

#endif // #ifdef CINOLIB_USES_OPENGL_GLFW_IMGUI

#endif // CINO_DRAW_CYLINDER_H
