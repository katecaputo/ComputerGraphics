#include "Triangle.hpp"
#include "vec2.hpp"

#include <cmath>

// helper for converting the normal to a triangle
vec3 convertNorm(point3 v1, point3 v2, point3 v3) {
    vec3 AB = v2 - v1;
    vec3 BC = v3 - v2;
    return unit_vector(cross(AB, BC));
}

// default constructor for triangle
Triangle::Triangle() {
    pt1 = point3(0.0, 0.0, -1.0);
    pt2 = point3(1.0, 1.0, -1.0);
    pt3 = point3(-1.0, -1.0, -1.0);
    col = color(0.5, 0.5, 0.5);
    mat_ptr = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    norm = convertNorm(pt1, pt2, pt3);
}

// consturctor for triangle with values input
Triangle::Triangle(point3 v1, point3 v2, point3 v3, vec2 tex1, vec2 tex2,
    vec2 tex3, point3 col_var, std::shared_ptr<Material> m) {

    pt1 = v1;
    pt2 = v2;
    pt3 = v3;
    uv1 = tex1;
    uv2 = tex2;
    uv3 = tex3;
    col = col_var;
    mat_ptr = m;
    norm = convertNorm(pt1, pt2, pt3);
}

// is the triangle hit by the given ray? 
bool Triangle::hit(const Ray& ray1, double t_min, double t_max,
    point3& recP, vec3& recNorm, double& recT,
    std::shared_ptr<Material>& recMtl, double& recU, double& recV) const {

    // edges between points
    vec3 edgeA = pt2 - pt1;
    vec3 edgeB = pt3 - pt1;

    // helper vectors
    vec3 pvec = cross(ray1.direction(), edgeB);
    vec3 tvec = ray1.origin() - pt1;
    vec3 qvec = cross(tvec, edgeA);

    // inverse of determinant
    float invDet = 1 / dot(edgeA, pvec);

    // location along ray, r, where it intersects the plane
    double t = dot(edgeB, qvec) * invDet;
    if (t < t_min || t_max < t) {
        return false;
    }

    // this point is somewhere on the ray r
    vec3 pointOfPlaneIntersection = ray1.at(t); 

    // inside out test
    vec3 edge1 = pt2 - pt1;
    vec3 edge2 = pt3 - pt2;
    vec3 edge3 = pt1 - pt3;
    vec3 c1 = pointOfPlaneIntersection - pt1;
    vec3 c2 = pointOfPlaneIntersection - pt2;
    vec3 c3 = pointOfPlaneIntersection - pt3;

    if (dot(norm, cross(edge1, c1)) > 0 &&
        dot(norm, cross(edge2, c2)) > 0 &&
        dot(norm, cross(edge3, c3)) > 0) {

            // update recorded info
            recMtl = mat_ptr;
            recT = t;
            recP = pointOfPlaneIntersection; 

                // set face normal
                bool front_face = dot(ray1.direction(), norm) < 0;
                if (front_face) {
                    recNorm = norm;
                } else {
                    recNorm = - norm;
                }

            // COMPUTING TEXTURE COORDINATES
            vec3 e1 = vec3(unit_vector(cross(recNorm, vec3(1, 0, 0))));

            // handles case if normal and (1,0,0) are parallel, change e1
            if (e1.x() == 0 && e1.y() == 0 && e1.z() == 0) {
                e1 = vec3(unit_vector(cross(recNorm, vec3(0, 0, 1))));
            }

            vec3 e2 = vec3(unit_vector(cross(recNorm, e1)));

            // adjusts scale from [-1,1] to [0,1]
            recU = (fabs(1+dot(e2, recP))) / 2;
            recV = (fabs(1+dot(e1, recP))) / 2;

            return true;
        } else {
            return false;
        }
};