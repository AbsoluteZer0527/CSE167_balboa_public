#include "hw2.h"
#include "hw2_scenes.h"

using namespace hw2;

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

    //camra to projected
    Vector3 proj0, proj1, proj2;

    //-x/z, -y/z, -1 
    proj0.x = p0.x /(-p0.z);
    proj0.y = p0.y / (-p0.z);
    proj0.z = p0.z;

    proj1.x = p1.x /(-p1.z);
    proj1.y = p1.y / (-p1.z);
    proj1.z = p1.z;

    proj2.x = p2.x /(-p2.z);
    proj2.y = p2.y / (-p2.z);
    proj2.z = p2.z;

    Real alpha = Real(img.width) / Real(img.height);

    auto to_screen = [&](const Vector3& proj) -> Vector3 {
        Real sx = img.width * (proj.x + s * alpha) / (2.0 * s * alpha);
        Real sy = img.height * (s - proj.y) / (2.0 * s);
        return Vector3{sx, sy, 0.0};
    };
    
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
            
            Real inv_denom = 1.0 / (dot00 * dot11 - dot01 * dot01);
            Real u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
            Real v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

            if (u >= 0 && v >= 0 && u + v <= 1) {
                img(x, y) = color;
            }
        }
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
    // silence warnings, feel free to remove these
    UNUSED(s); 
    UNUSED(z_near);
    UNUSED(scene_id);

    TriangleMesh mesh = meshes[scene_id];
    UNUSED(mesh); // silence warning, feel free to remove this

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
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
    // silence warnings, feel free to remove these
    UNUSED(s); 
    UNUSED(z_near);
    UNUSED(scene_id);

    TriangleMesh mesh = meshes[scene_id];
    UNUSED(mesh); // silence warning, feel free to remove this

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
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
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

