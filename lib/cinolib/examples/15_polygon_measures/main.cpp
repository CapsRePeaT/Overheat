#include <cinolib/meshes/meshes.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/polygon_kernel.h>
#include <cinolib/profiler.h>
#include <cinolib/polygon_maximum_inscribed_circle.h>
#include <cinolib/smallest_enclosing_disk.h>
#include <cinolib/geometry/n_sided_poygon.h>

int main(int, char **)
{
    using namespace cinolib;

    DrawablePolygonmesh<> m({
        vec3d(0,0,0), // v0
        vec3d(2,0,0), // v1
        vec3d(2,1,0), // v2
        vec3d(1,1,0), // v3
        vec3d(0,1,0), // v4
    });
    m.show_wireframe(true);
    m.show_marked_edge(false);

    std::vector<vec3d> kernel;
    polygon_kernel(m.vector_verts(), kernel);
    DrawablePolygonmesh<> k(kernel);
    k.poly_set_color(Color::PASTEL_RED());

    vec3d  sec_c, mic_c;
    double sec_r, mic_r;
    smallest_enclosing_disk(m.vector_verts(), sec_c, sec_r);
    polygon_maximum_inscribed_circle(m.vector_verts(), mic_c, mic_r);
    DrawablePolygonmesh<> sec(n_sided_polygon(50, CIRCLE));
    DrawablePolygonmesh<> mic(n_sided_polygon(50, CIRCLE));
    sec.translate(sec_c);
    mic.translate(mic_c);
    sec.scale(sec_r);
    mic.scale(mic_r);
    sec.show_mesh_points();
    mic.show_mesh_points();
    sec.show_wireframe(true);
    mic.show_wireframe(true);
    sec.show_wireframe_width(3);
    mic.show_wireframe_width(3);
    sec.show_wireframe_color(Color::RED());
    mic.show_wireframe_color(Color::BLUE());
    sec.show_marked_edge(false);
    mic.show_marked_edge(false);

    GLcanvas gui;
    gui.push(&m);
    gui.push(&k);
    gui.push(&sec);
    gui.push(&mic);

    // position heat sources with CMD + left click
    Profiler profiler;
    gui.callback_app_controls = [&]()
    {
        static float t = 0;
        if(ImGui::SliderFloat("##t", &t, 0.f, 1.f))
        {
            m.vert(2) = vec3d(2,  3,0)*t + vec3d(2,1,0)*(1.0-t);
            m.vert(3) = vec3d(1,0.1,0)*t + vec3d(1,1,0)*(1.0-t);
            m.vert(4) = vec3d(0,  3,0)*t + vec3d(0,1,0)*(1.0-t);
            m.update_p_tessellations();
            m.updateGL();

            profiler.push("Compute polygon kernel");
            double area = polygon_kernel(m.vector_verts(), kernel);
            profiler.pop();
            k = DrawablePolygonmesh<>(kernel);
            k.poly_set_color(Color::PASTEL_RED());
            k.show_marked_edge(false);

            profiler.push("Compute smallest enclosing circle");
            smallest_enclosing_disk(m.vector_verts(), sec_c, sec_r);
            profiler.pop();
            profiler.push("Compute maximum inscribed circle");
            polygon_maximum_inscribed_circle(m.vector_verts(), mic_c, mic_r);
            profiler.pop();
            sec.vector_verts() = n_sided_polygon(50, CIRCLE);
            mic.vector_verts() = n_sided_polygon(50, CIRCLE);
            sec.translate(sec_c);
            mic.translate(mic_c);
            sec.scale(sec_r);
            mic.scale(mic_r);
            sec.updateGL();
            mic.updateGL();

            std::cout << "kernel area: " << area << std::endl;
            std::cout << "radius of smallest enclosing circle: " << sec_r << std::endl;
            std::cout << "radius of maximum inscribed circle: " << mic_r << "\n" << std::endl;
        }
    };

    return gui.launch();
}
