#include "hw2.h"
#include "hw2_scenes.h"

using namespace hw2;

// Image3 hw_2_1(const std::vector<std::string> &params) {
//     // Homework 2.1: render a single 3D triangle

//     Image3 img(640 /* width */, 480 /* height */);

//     Vector3 p0{0, 0, -1};
//     Vector3 p1{1, 0, -1};
//     Vector3 p2{0, 1, -1};
//     Real s = 1; // scaling factor of the view frustrum
//     Vector3 color = Vector3{1.0, 0.5, 0.5};
//     Real z_near = 1e-6; // distance of the near clipping plane
//     for (int i = 0; i < (int)params.size(); i++) {
//         if (params[i] == "-s") {
//             s = std::stof(params[++i]);
//         } else if (params[i] == "-p0") {
//             p0.x = std::stof(params[++i]);
//             p0.y = std::stof(params[++i]);
//             p0.z = std::stof(params[++i]);
//         } else if (params[i] == "-p1") {
//             p1.x = std::stof(params[++i]);
//             p1.y = std::stof(params[++i]);
//             p1.z = std::stof(params[++i]);
//         } else if (params[i] == "-p2") {
//             p2.x = std::stof(params[++i]);
//             p2.y = std::stof(params[++i]);
//             p2.z = std::stof(params[++i]);
//         } else if (params[i] == "-color") {
//             Real r = std::stof(params[++i]);
//             Real g = std::stof(params[++i]);
//             Real b = std::stof(params[++i]);
//             color = Vector3{r, g, b};
//         } else if (params[i] == "-znear") {
//             z_near = std::stof(params[++i]);
//         }
//     }
 
//     for (int y = 0; y < img.height; y++) {
//         for (int x = 0; x < img.width; x++) {
//             img(x, y) = Vector3{0.5, 0.5, 0.5};
//         }
//     }


//     if(p0.z > -z_near && p1.z > -z_near && p2.z > -z_near){
//         return img;
//     }

//     //camra to projected
//     Vector3 proj0, proj1, proj2;

//     //-x/z, -y/z, -1 
//     proj0.x = p0.x /(-p0.z);
//     proj0.y = p0.y / (-p0.z);
//     proj0.z = p0.z;

//     proj1.x = p1.x /(-p1.z);
//     proj1.y = p1.y / (-p1.z);
//     proj1.z = p1.z;

//     proj2.x = p2.x /(-p2.z);
//     proj2.y = p2.y / (-p2.z);
//     proj2.z = p2.z;

//     Real alpha = Real(img.width) / Real(img.height);

//     auto to_screen = [&](const Vector3& proj) -> Vector3 {
//         Real sx = img.width * (proj.x + s * alpha) / (2.0 * s * alpha);
//         Real sy = img.height * (s - proj.y) / (2.0 * s);
//         return Vector3{sx, sy, 0.0};
//     };
    
//     Vector3 screen0 = to_screen(proj0);
//     Vector3 screen1 = to_screen(proj1);
//     Vector3 screen2 = to_screen(proj2);

//     Real triangle_min_x = std::min({screen0.x, screen1.x, screen2.x});
//     Real triangle_max_x = std::max({screen0.x, screen1.x, screen2.x});
//     Real triangle_min_y = std::min({screen0.y, screen1.y, screen2.y});
//     Real triangle_max_y = std::max({screen0.y, screen1.y, screen2.y}); 
    
//     int min_x = std::max(0, (int)std::floor(triangle_min_x));
//     int max_x = std::min(img.width - 1, (int)std::ceil(triangle_max_x));
//     int min_y = std::max(0, (int)std::floor(triangle_min_y));
//     int max_y = std::min(img.height - 1, (int)std::ceil(triangle_max_y));
     

//     for (int y = min_y; y <= max_y; y++) {
//         for (int x = min_x; x <= max_x; x++) {
//             Real px = x + 0.5; 
//             Real py = y + 0.5;

//             Vector3 v0 = screen1 - screen0;
//             Vector3 v1 = screen2 - screen0;
//             Vector3 v2 = Vector3{px, py, 0.0} - screen0;
            
//             Real dot00 = v0.x * v0.x + v0.y * v0.y;
//             Real dot01 = v0.x * v1.x + v0.y * v1.y;
//             Real dot02 = v0.x * v2.x + v0.y * v2.y;
//             Real dot11 = v1.x * v1.x + v1.y * v1.y;
//             Real dot12 = v1.x * v2.x + v1.y * v2.y;
            
//             Real inv_denom = 1.0 / (dot00 * dot11 - dot01 * dot01);
//             Real u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
//             Real v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

//             if (u >= 0 && v >= 0 && u + v <= 1) {
//                 img(x, y) = color;
//             }
//         }
//     }

//     return img;
// }


//new 2-1 doing triangle clipping
Image3 hw_2_1(const std::vector<std::string> &params) {
    // Homework 2.1: render a single 3D triangle

    Image3 img(640 /* width */, 480 /* height */);

    Vector3 p0{0, 0, -1};
    Vector3 p1{1, 0, -1};
    Vector3 p2{0, 1, -1};
    Real s = 1; // scaling factor of the view frustrum
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    Real z_near = 1e-6; // distance of the near clipping plane
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-p0") {
            p0.x = std::stof(params[++i]);
            p0.y = std::stof(params[++i]);
            p0.z = std::stof(params[++i]);
        } else if (params[i] == "-p1") {
            p1.x = std::stof(params[++i]);
            p1.y = std::stof(params[++i]);
            p1.z = std::stof(params[++i]);
        } else if (params[i] == "-p2") {
            p2.x = std::stof(params[++i]);
            p2.y = std::stof(params[++i]);
            p2.z = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        }
    }
 
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};
        }
    }


    if(p0.z > -z_near && p1.z > -z_near && p2.z > -z_near){
        return img;
    }

    

    Real alpha = Real(img.width) / Real(img.height);

    auto to_screen = [&](const Vector3& proj) -> Vector3 {
        Real sx = img.width * (proj.x + s * alpha) / (2.0 * s * alpha);
        Real sy = img.height * (s - proj.y) / (2.0 * s);
        return Vector3{sx, sy, 0.0};
    };

    //clip
    auto clip_edge = [&](const Vector3& inside, const Vector3& outside, Real clip_z) -> Vector3 {

        Real t = (clip_z - inside.z) / (outside.z - inside.z);
        return Vector3{
            inside.x + t * (outside.x - inside.x),
            inside.y + t * (outside.y - inside.y),
            clip_z
        };
    };
    
//nested inside a function
auto render_triangle = [&](const Vector3& v0, const Vector3& v1, const Vector3& v2) {

    //camra to projected
    Vector3 proj0, proj1, proj2;

    //-x/z, -y/z, -1 
    proj0.x = v0.x / (-v0.z);
    proj0.y = v0.y / (-v0.z);
    proj0.z = v0.z;

    proj1.x = v1.x / (-v1.z);
    proj1.y = v1.y / (-v1.z);
    proj1.z = v1.z;

    proj2.x = v2.x / (-v2.z);
    proj2.y = v2.y / (-v2.z);
    proj2.z = v2.z;

    Vector3 screen0 = to_screen(proj0);
    Vector3 screen1 = to_screen(proj1);
    Vector3 screen2 = to_screen(proj2);

    Real triangle_min_x = std::min({screen0.x, screen1.x, screen2.x});
    Real triangle_max_x = std::max({screen0.x, screen1.x, screen2.x});
    Real triangle_min_y = std::min({screen0.y, screen1.y, screen2.y});
    Real triangle_max_y = std::max({screen0.y, screen1.y, screen2.y}); 
    
    int min_x = std::max(0, (int)std::floor(triangle_min_x));
    int max_x = std::min(img.width - 1, (int)std::ceil(triangle_max_x));
    int min_y = std::max(0, (int)std::floor(triangle_min_y));
    int max_y = std::min(img.height - 1, (int)std::ceil(triangle_max_y));
     
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            Real px = x + 0.5; 
            Real py = y + 0.5;

            Vector3 v0 = screen1 - screen0;
            Vector3 v1 = screen2 - screen0;
            Vector3 v2 = Vector3{px, py, 0.0} - screen0;
            
            Real dot00 = v0.x * v0.x + v0.y * v0.y;
            Real dot01 = v0.x * v1.x + v0.y * v1.y;
            Real dot02 = v0.x * v2.x + v0.y * v2.y;
            Real dot11 = v1.x * v1.x + v1.y * v1.y;
            Real dot12 = v1.x * v2.x + v1.y * v2.y;
            
            

            Real denom = dot00 * dot11 - dot01 * dot01;
            if (std::abs(denom) < 1e-10) {
                continue;
            }

            Real inv_denom = 1.0 / (dot00 * dot11 - dot01 * dot01);
            Real u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
            Real v = (dot00 * dot12 - dot01 * dot02) * inv_denom;
        


            if (u >= 0 && v >= 0 && u + v <= 1) {
                img(x, y) = color;
            }
        }
    }
};

    Real clip_z = -z_near;

    //count in font z cases
    int behind = 0;
    if (p0.z > clip_z) behind++;
    if (p1.z > clip_z) behind++;
    if (p2.z > clip_z) behind++;

    //reject 
    if (behind == 3) {
        return img;
    }

    //normal
    if (behind == 0) {
        render_triangle(p0, p1, p2);
        return img;
    }
    
    // If two vertices are behind the clipping plane (right), the
    //clipped triangle is still a triangle
    if (behind == 2) {
        Vector3 inside, outside1, outside2;
        
        if (p0.z < clip_z) {
            inside = p0; outside1 = p1; outside2 = p2;
        } else if (p1.z < clip_z) {
            inside = p1; outside1 = p0; outside2 = p2;
        } else {
            inside = p2; outside1 = p0; outside2 = p1;
        }
        //new edges
        Vector3 new_v1 = clip_edge(inside, outside1, clip_z);
        Vector3 new_v2 = clip_edge(inside, outside2, clip_z);

        render_triangle(inside, new_v1, new_v2);
    }

    // If only one vertex is behind the clipping plane (left), then we need to clip and further
    //split the resulting quadliteral into two triangles.
    if (behind == 1) {
        Vector3 inside1, inside2, outside;
        
        if (p0.z >= clip_z) {
            outside = p0; inside1 = p1; inside2 = p2;
        } else if (p1.z >= clip_z) {
            outside = p1; inside1 = p0; inside2 = p2;
        } else {
            outside = p2; inside1 = p0; inside2 = p1;
        }

        Vector3 new_v1 = clip_edge(inside1, outside, clip_z);
        Vector3 new_v2 = clip_edge(inside2, outside, clip_z);

        // Render two triangles so its a quadliteral
        render_triangle(inside1, inside2, new_v1);
        render_triangle(inside2, new_v2, new_v1);
    }

    return img;
}



Image3 hw_2_2(const std::vector<std::string> &params) {
    // Homework 2.2: render a triangle mesh

    Image3 img(640 /* width */, 480 /* height */);

    Real s = 1; // scaling factor of the view frustrum
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }

    TriangleMesh mesh = meshes[scene_id];

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};
        }
    }

    //Z-buffer to infinity
    std::vector<std::vector<Real>> z_buffer(img.height, std::vector<Real>(img.width, std::numeric_limits<Real>::infinity()));

    Real aspect = Real(img.width) / Real(img.height);

    //projected space to screen space
    auto to_screen = [&](const Vector3& proj) -> Vector3 {
        Real sx = img.width * (proj.x + s * aspect) / (2.0 * s * aspect);
        Real sy = img.height * (s - proj.y) / (2.0 * s);
        return Vector3{sx, sy, 0.0};
    };

    //screen space back to projected space
    auto to_projected = [&](Real screen_x, Real screen_y) -> Vector3 {
        Real proj_x = (2.0 * s * aspect * screen_x / img.width) - s * aspect;
        Real proj_y = s - (2.0 * s * screen_y / img.height);
        return Vector3{proj_x, proj_y, 0.0};
    };

    //calculate triangle area using cross product
    auto triangle_area = [](const Vector3& a, const Vector3& b, const Vector3& c) -> Real {
        Vector3 ab = b - a;
        Vector3 ac = c - a;

        Real cross_z = ab.x * ac.y - ab.y * ac.x;
        return std::abs(cross_z) * 0.5;
    };

    for (size_t face_idx = 0; face_idx < mesh.faces.size(); face_idx++) {
        Vector3i face = mesh.faces[face_idx];
        Vector3 color = mesh.face_colors[face_idx];

        Vector3 p0 = mesh.vertices[face[0]];
        Vector3 p1 = mesh.vertices[face[1]];
        Vector3 p2 = mesh.vertices[face[2]];

        // Check clipping
        if (p0.z > -z_near && p1.z > -z_near && p2.z > -z_near) {
            continue;
        }

        Vector3 proj0, proj1, proj2;
        proj0.x = p0.x / (-p0.z);
        proj0.y = p0.y / (-p0.z);
        proj0.z = p0.z;
        
        proj1.x = p1.x / (-p1.z);
        proj1.y = p1.y / (-p1.z);
        proj1.z = p1.z;
        
        proj2.x = p2.x / (-p2.z);
        proj2.y = p2.y / (-p2.z);
        proj2.z = p2.z;

        // Convert to screen space
        Vector3 screen0 = to_screen(proj0);
        Vector3 screen1 = to_screen(proj1);
        Vector3 screen2 = to_screen(proj2);

        Real triangle_min_x = std::min({screen0.x, screen1.x, screen2.x});
        Real triangle_max_x = std::max({screen0.x, screen1.x, screen2.x});
        Real triangle_min_y = std::min({screen0.y, screen1.y, screen2.y});
        Real triangle_max_y = std::max({screen0.y, screen1.y, screen2.y});
        
        int min_x = std::max(0, (int)std::floor(triangle_min_x));
        int max_x = std::min(img.width - 1, (int)std::ceil(triangle_max_x));
        int min_y = std::max(0, (int)std::floor(triangle_min_y));
        int max_y = std::min(img.height - 1, (int)std::ceil(triangle_max_y));

        for (int y = min_y; y <= max_y; y++) {
            for (int x = min_x; x <= max_x; x++) {
                Real px = x + 0.5; // pixel center
                Real py = y + 0.5;

                // Check if pixel is inside triangle using barycentric coordinates
                Vector3 v0 = screen1 - screen0;
                Vector3 v1 = screen2 - screen0;
                Vector3 v2 = Vector3{px, py, 0.0} - screen0;
                
                Real dot00 = v0.x * v0.x + v0.y * v0.y;
                Real dot01 = v0.x * v1.x + v0.y * v1.y;
                Real dot02 = v0.x * v2.x + v0.y * v2.y;
                Real dot11 = v1.x * v1.x + v1.y * v1.y;
                Real dot12 = v1.x * v2.x + v1.y * v2.y;
                
                Real denom = dot00 * dot11 - dot01 * dot01;
                if (std::abs(denom) < 1e-10) continue;
                
                Real inv_denom = 1.0 / denom;
                Real u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
                Real v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

                if (u >= 0 && v >= 0 && u + v <= 1) {
                    Real b_bar_1 = u;
                    Real b_bar_2 = v;
                    Real b_bar_0 = 1.0 - u - v;

                    Real z0 = -p0.z;
                    Real z1 = -p1.z;
                    Real z2 = -p2.z;

                    Real inv_z_sum = (b_bar_0 / z0) + (b_bar_1 / z1) + (b_bar_2 / z2);
                    Real b0 = (b_bar_0 / z0) / inv_z_sum;
                    Real b1 = (b_bar_1 / z1) / inv_z_sum;
                    Real b2 = (b_bar_2 / z2) / inv_z_sum;

                    Real depth = b0 * z0 + b1 * z1 + b2 * z2;

                    if (depth < z_buffer[y][x] && depth > z_near) {
                        z_buffer[y][x] = depth;
                        img(x, y) = color;
                    }
                }
            }
        }
    }

    return img;
}

Image3 hw_2_3(const std::vector<std::string> &params) {
    // Homework 2.3: render a triangle mesh with vertex colors

    Image3 img(640 /* width */, 480 /* height */);

    Real s = 1; // scaling factor of the view frustrum
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }

    TriangleMesh mesh = meshes[scene_id];

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};
        }
    }

    // Initialize Z-buffer
    std::vector<std::vector<Real>> z_buffer(img.height, std::vector<Real>(img.width, std::numeric_limits<Real>::infinity()));

    Real aspect = Real(img.width) / Real(img.height);

    //projected space to screen space
    auto to_screen = [&](const Vector3& proj) -> Vector3 {
        Real sx = img.width * (proj.x + s * aspect) / (2.0 * s * aspect);
        Real sy = img.height * (s - proj.y) / (2.0 * s);
        return Vector3{sx, sy, 0.0};
    };

    for (size_t face_idx = 0; face_idx < mesh.faces.size(); face_idx++) {
        Vector3i face = mesh.faces[face_idx];

        Vector3 p0 = mesh.vertices[face[0]];
        Vector3 p1 = mesh.vertices[face[1]];
        Vector3 p2 = mesh.vertices[face[2]];

        Vector3 c0 = mesh.vertex_colors[face[0]];
        Vector3 c1 = mesh.vertex_colors[face[1]];
        Vector3 c2 = mesh.vertex_colors[face[2]];

        //clipping
        if (p0.z > -z_near && p1.z > -z_near && p2.z > -z_near) {
            continue;
        }

        Vector3 proj0, proj1, proj2;
        proj0.x = p0.x / (-p0.z);
        proj0.y = p0.y / (-p0.z);
        proj0.z = p0.z;
        
        proj1.x = p1.x / (-p1.z);
        proj1.y = p1.y / (-p1.z);
        proj1.z = p1.z;
        
        proj2.x = p2.x / (-p2.z);
        proj2.y = p2.y / (-p2.z);
        proj2.z = p2.z;

        Vector3 screen0 = to_screen(proj0);
        Vector3 screen1 = to_screen(proj1);
        Vector3 screen2 = to_screen(proj2);

        Real triangle_min_x = std::min({screen0.x, screen1.x, screen2.x});
        Real triangle_max_x = std::max({screen0.x, screen1.x, screen2.x});
        Real triangle_min_y = std::min({screen0.y, screen1.y, screen2.y});
        Real triangle_max_y = std::max({screen0.y, screen1.y, screen2.y});
        
        int min_x = std::max(0, (int)std::floor(triangle_min_x));
        int max_x = std::min(img.width - 1, (int)std::ceil(triangle_max_x));
        int min_y = std::max(0, (int)std::floor(triangle_min_y));
        int max_y = std::min(img.height - 1, (int)std::ceil(triangle_max_y));

        for (int y = min_y; y <= max_y; y++) {
            for (int x = min_x; x <= max_x; x++) {
                Real px = x + 0.5; 
                Real py = y + 0.5;

                Vector3 v0 = screen1 - screen0;
                Vector3 v1 = screen2 - screen0;
                Vector3 v2 = Vector3{px, py, 0.0} - screen0;
                
                Real dot00 = v0.x * v0.x + v0.y * v0.y;
                Real dot01 = v0.x * v1.x + v0.y * v1.y;
                Real dot02 = v0.x * v2.x + v0.y * v2.y;
                Real dot11 = v1.x * v1.x + v1.y * v1.y;
                Real dot12 = v1.x * v2.x + v1.y * v2.y;
                
                Real denom = dot00 * dot11 - dot01 * dot01;
                if (std::abs(denom) < 1e-10) continue;
                
                Real inv_denom = 1.0 / denom;
                Real u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
                Real v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

                if (u >= 0 && v >= 0 && u + v <= 1) {
                    Real b_bar_1 = u;
                    Real b_bar_2 = v;
                    Real b_bar_0 = 1.0 - u - v;

                    Real z0 = -p0.z;
                    Real z1 = -p1.z;
                    Real z2 = -p2.z;

                    Real inv_z_sum = (b_bar_0 / z0) + (b_bar_1 / z1) + (b_bar_2 / z2);
                    Real b0 = (b_bar_0 / z0) / inv_z_sum;
                    Real b1 = (b_bar_1 / z1) / inv_z_sum;
                    Real b2 = (b_bar_2 / z2) / inv_z_sum;

                    Real depth = b0 * z0 + b1 * z1 + b2 * z2;

                    if (depth < z_buffer[y][x] && depth > z_near) {
                        z_buffer[y][x] = depth;
                        
                        Vector3 color = b0 * c0 + b1 * c1 + b2 * c2;
                        img(x, y) = color;
                    }
                }
            }
        }
    }

    return img;
}

Image3 hw_2_4(const std::vector<std::string> &params) {
    // Homework 2.4: render a scene with transformation
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.camera.resolution.x,
               scene.camera.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }


    std::vector<std::vector<Real>> z_buffer(img.height, 
        std::vector<Real>(img.width, std::numeric_limits<Real>::infinity()));

    Real aspect = Real(img.width) / Real(img.height);
    Real s = scene.camera.s;
    Real z_near = scene.camera.z_near;

    Matrix4x4 view_matrix = inverse(scene.camera.cam_to_world);


    // Helper: Convert projected space to screen space
    auto to_screen = [&](const Vector3& proj) -> Vector3 {
        Real sx = img.width * (proj.x + s * aspect) / (2.0 * s * aspect);
        Real sy = img.height * (s - proj.y) / (2.0 * s);
        return Vector3{sx, sy, 0.0};
    };

    int total_triangles = 0;
    int clipped_triangles = 0;
    int pixels_drawn = 0;

    // Render each mesh
    for (const TriangleMesh& mesh : scene.meshes) {
        
        // Combined transformation: View * Model
        Matrix4x4 model_view = view_matrix * mesh.model_matrix;
        

        // For each triangle
        for (size_t face_idx = 0; face_idx < mesh.faces.size(); face_idx++) {
            total_triangles++;
            Vector3i face = mesh.faces[face_idx];

            // Get the 3D vertices in object space
            Vector3 v0_obj = mesh.vertices[face[0]];
            Vector3 v1_obj = mesh.vertices[face[1]];
            Vector3 v2_obj = mesh.vertices[face[2]];

            //debug
            if (face_idx == 0) {
                std::cout << "First triangle vertices (object space):" << std::endl;
                std::cout << "  v0: " << v0_obj << std::endl;
                std::cout << "  v1: " << v1_obj << std::endl;
                std::cout << "  v2: " << v2_obj << std::endl;
            }

            Vector4 v0_4d = model_view * Vector4{v0_obj.x, v0_obj.y, v0_obj.z, 1.0};
            Vector4 v1_4d = model_view * Vector4{v1_obj.x, v1_obj.y, v1_obj.z, 1.0};
            Vector4 v2_4d = model_view * Vector4{v2_obj.x, v2_obj.y, v2_obj.z, 1.0};

            Vector3 p0{v0_4d.x, v0_4d.y, v0_4d.z};
            Vector3 p1{v1_4d.x, v1_4d.y, v1_4d.z};
            Vector3 p2{v2_4d.x, v2_4d.y, v2_4d.z};

            //debug
            if (face_idx == 0) {
                std::cout << "First triangle vertices (camera space):" << std::endl;
                std::cout << "  p0: " << p0 << " (z > -z_near? " << (p0.z > -z_near) << ")" << std::endl;
                std::cout << "  p1: " << p1 << " (z > -z_near? " << (p1.z > -z_near) << ")" << std::endl;
                std::cout << "  p2: " << p2 << " (z > -z_near? " << (p2.z > -z_near) << ")" << std::endl;
                std::cout << "  z_near: " << z_near << std::endl;
            }

            Vector3 c0 = mesh.vertex_colors[face[0]];
            Vector3 c1 = mesh.vertex_colors[face[1]];
            Vector3 c2 = mesh.vertex_colors[face[2]];

            //clipping
            if (p0.z > -z_near && p1.z > -z_near && p2.z > -z_near) {
                clipped_triangles++;
                continue;
            }

            Vector3 proj0, proj1, proj2;
            proj0.x = p0.x / (-p0.z);
            proj0.y = p0.y / (-p0.z);
            proj0.z = p0.z;
            
            proj1.x = p1.x / (-p1.z);
            proj1.y = p1.y / (-p1.z);
            proj1.z = p1.z;
            
            proj2.x = p2.x / (-p2.z);
            proj2.y = p2.y / (-p2.z);
            proj2.z = p2.z;

            Vector3 screen0 = to_screen(proj0);
            Vector3 screen1 = to_screen(proj1);
            Vector3 screen2 = to_screen(proj2);

            Real triangle_min_x = std::min({screen0.x, screen1.x, screen2.x});
            Real triangle_max_x = std::max({screen0.x, screen1.x, screen2.x});
            Real triangle_min_y = std::min({screen0.y, screen1.y, screen2.y});
            Real triangle_max_y = std::max({screen0.y, screen1.y, screen2.y});
            
            int min_x = std::max(0, (int)std::floor(triangle_min_x));
            int max_x = std::min(img.width - 1, (int)std::ceil(triangle_max_x));
            int min_y = std::max(0, (int)std::floor(triangle_min_y));
            int max_y = std::min(img.height - 1, (int)std::ceil(triangle_max_y));

            for (int y = min_y; y <= max_y; y++) {
                for (int x = min_x; x <= max_x; x++) {
                    Real px = x + 0.5; // pixel center
                    Real py = y + 0.5;

                    Vector3 v0 = screen1 - screen0;
                    Vector3 v1 = screen2 - screen0;
                    Vector3 v2 = Vector3{px, py, 0.0} - screen0;
                    
                    Real dot00 = v0.x * v0.x + v0.y * v0.y;
                    Real dot01 = v0.x * v1.x + v0.y * v1.y;
                    Real dot02 = v0.x * v2.x + v0.y * v2.y;
                    Real dot11 = v1.x * v1.x + v1.y * v1.y;
                    Real dot12 = v1.x * v2.x + v1.y * v2.y;
                    
                    Real denom = dot00 * dot11 - dot01 * dot01;
                    if (std::abs(denom) < 1e-10) continue;
                    
                    Real inv_denom = 1.0 / denom;
                    Real u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
                    Real v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

                    if (u >= 0 && v >= 0 && u + v <= 1) {
                        Real b_bar_1 = u;
                        Real b_bar_2 = v;
                        Real b_bar_0 = 1.0 - u - v;

                        Real z0 = -p0.z;
                        Real z1 = -p1.z;
                        Real z2 = -p2.z;

                        Real inv_z_sum = (b_bar_0 / z0) + (b_bar_1 / z1) + (b_bar_2 / z2);
                        Real b0 = (b_bar_0 / z0) / inv_z_sum;
                        Real b1 = (b_bar_1 / z1) / inv_z_sum;
                        Real b2 = (b_bar_2 / z2) / inv_z_sum;

                        Real depth = b0 * z0 + b1 * z1 + b2 * z2;

                        if (depth < z_buffer[y][x] && depth > z_near) {
                            z_buffer[y][x] = depth;
                            
                            Vector3 color = b0 * c0 + b1 * c1 + b2 * c2;
                            img(x, y) = color;
                            pixels_drawn++;
                        }
                    }
                }
            }
        }
    }

    return img;
}