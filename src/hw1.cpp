#include "hw1.h"
#include "hw1_scenes.h"
#include <limits>

using namespace hw1;

// Forward declarations for helper functions
Real point_to_segment_distance(Real px, Real py, Real x0, Real y0, Real x1, Real y1, bool flat_cap_start, bool flat_cap_end);
int calculate_winding_number(Real px, Real py, const std::vector<Vector2>& polyline);
bool point_in_circle(Real px, Real py, Real cx, Real cy, Real radius);
Vector2 evaluate_bezier(Vector2 p0, Vector2 p1, Vector2 p2, Real t);
Real point_to_bezier_distance(Real px, Real py, Vector2 p0, Vector2 p1, Vector2 p2);
bool point_near_bezier(Real px, Real py, Vector2 p0, Vector2 p1, Vector2 p2, Real stroke_width);

Image3 hw_1_1(const std::vector<std::string> &params) {
    // Homework 1.1: render a circle at the specified
    // position, with the specified radius and color.

    Image3 img(640 /* width */, 480 /* height */);

    Vector2 center = Vector2{img.width / 2 + Real(0.5), img.height / 2 + Real(0.5)};
    Real radius = 100.0;
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-center") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]);
            //fliped y
            y = img.height - y;
            center = Vector2{x, y};
        } else if (params[i] == "-radius") {
            radius = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        }
    }

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Real distanceX = (x + 0.5) - center.x;
            Real distanceY = (y + 0.5) - center.y;
            Real distance = sqrt(distanceX*distanceX + distanceY*distanceY);

            if(distance <= radius){
                img(x, y) = color;
            }else{
                img(x, y) = Vector3{0.5, 0.5, 0.5};
            }
        }
    }
    return img;
}
//-----------------------------------------------------------------------------------------------------------//

//helper functions 1-2 to 1-8
Real point_to_segment_distance(Real px, Real py, Real x0, Real y0, Real x1, Real y1, bool flat_cap_start, bool flat_cap_end) {
    Real dx = x1 - x0;
    Real dy = y1 - y0;
    
    Real length_squared = dx * dx + dy * dy;
    
    if (length_squared == 0) {
        Real dpx = px - x0;
        Real dpy = py - y0;
        return sqrt(dpx * dpx + dpy * dpy);
    }
    
    Real t = ((px - x0) * dx + (py - y0) * dy) / length_squared;
    
    if (flat_cap_start && t < 0) {
        return std::numeric_limits<Real>::infinity();
    }
    if (flat_cap_end && t > 1) {
        return std::numeric_limits<Real>::infinity();
    }
    
    t = std::max(Real(0), std::min(Real(1), t));
    
    Real closest_x = x0 + t * dx;
    Real closest_y = y0 + t * dy;
    
    Real dist_x = px - closest_x;
    Real dist_y = py - closest_y;
    return sqrt(dist_x * dist_x + dist_y * dist_y);
}


int calculate_winding_number(Real px, Real py, const std::vector<Vector2>& polyline) {
    int winding_number = 0;
    int n = polyline.size();
    
    for (int i = 0; i < n; i++) {
        Vector2 p0 = polyline[i];
        Vector2 p1 = polyline[(i + 1) % n];
        
        if (p0.y <= py) {
            if (p1.y > py) {
                Real cross = (p1.x - p0.x) * (py - p0.y) - (px - p0.x) * (p1.y - p0.y);
                if (cross > 0) {
                    winding_number++;
                }
            }
        } else {
            if (p1.y <= py) {
                Real cross = (p1.x - p0.x) * (py - p0.y) - (px - p0.x) * (p1.y - p0.y);
                if (cross < 0) {
                    winding_number--;
                }
            }
        }
    }
    
    return winding_number;
}

bool point_in_circle(Real px, Real py, Real cx, Real cy, Real radius) {
    Real dx = px - cx;
    Real dy = py - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}

// Bézier curve helpers
Vector2 evaluate_bezier(Vector2 p0, Vector2 p1, Vector2 p2, Real t) {
    Real s = 1.0 - t;
    return s * s * p0 + 2.0 * s * t * p1 + t * t * p2;
}

// Better Bézier distance calculation
Real point_to_bezier_distance(Real px, Real py, Vector2 p0, Vector2 p1, Vector2 p2) {
    Real min_dist = std::numeric_limits<Real>::infinity();
    int num_samples = 30;  // Increase samples for smoother curves
    
    // Check distance to line segments connecting sample points
    Vector2 prev_point = p0;
    
    for (int i = 1; i <= num_samples; i++) {
        Real t = Real(i) / num_samples;
        Vector2 curr_point = evaluate_bezier(p0, p1, p2, t);
        
        // Distance to line segment between prev_point and curr_point
        Real dist = point_to_segment_distance(px, py, 
                                              prev_point.x, prev_point.y,
                                              curr_point.x, curr_point.y,
                                              false, false);
        
        min_dist = std::min(min_dist, dist);
        prev_point = curr_point;
    }
    
    return min_dist;
}

bool point_near_bezier(Real px, Real py, Vector2 p0, Vector2 p1, Vector2 p2, Real stroke_width) {
    Real dist = point_to_bezier_distance(px, py, p0, p1, p2);
    return dist <= stroke_width / 2.0;
}

//Bézier support
bool check_polyline_stroke(Real px, Real py, const Polyline* polyline) {
    Real half_width = polyline->stroke_width / 2.0;
    
    for (int i = 0; i < (int)polyline->points.size() - 1; i++) {
        Vector2 p0 = polyline->points[i];
        Vector2 p1 = polyline->points[i + 1];
    
        if (i < (int)polyline->segments.size() && polyline->segments[i].has_value()) {
            Vector2 control = polyline->segments[i]->control;
            if (point_near_bezier(px, py, p0, control, p1, polyline->stroke_width)) {
                return true;
            }

        } else {
            // Straight line segment
            bool flat_start = (i == 0) && !polyline->is_closed;
            bool flat_end = (i == (int)polyline->points.size() - 2) && !polyline->is_closed;
            
            Real dist = point_to_segment_distance(px, py, p0.x, p0.y, p1.x, p1.y,
                                                  flat_start, flat_end);
            
            if (dist <= half_width) {
                return true;
            }
        }
    }
    
    // Closing segment for closed polylines
    if (polyline->is_closed && polyline->points.size() >= 2) {
        Vector2 p0 = polyline->points.back();
        Vector2 p1 = polyline->points.front();
        
        int last_idx = polyline->points.size() - 1;
        // Check if closing segment has a Bézier curve
        if (last_idx < (int)polyline->segments.size() && polyline->segments[last_idx].has_value()) {
            Vector2 control = polyline->segments[last_idx]->control;
            if (point_near_bezier(px, py, p0, control, p1, polyline->stroke_width)) {
                return true;
            }
        } else {
            Real dist = point_to_segment_distance(px, py, p0.x, p0.y, p1.x, p1.y, false, false);
            if (dist <= half_width) {
                return true;
            }
        }
    }
    

    // Round joins
    int start_idx = polyline->is_closed ? 0 : 1;
    int end_idx = polyline->is_closed ? polyline->points.size() : polyline->points.size() - 1;
    
    for (int i = start_idx; i < end_idx; i++) {
        Vector2 vertex = polyline->points[i % polyline->points.size()];
        if (point_in_circle(px, py, vertex.x, vertex.y, half_width)) {

            return true;
        }
    }


    
    return false;
}

//-----------------------------------------------------------------------------------------------------------//
Image3 hw_1_2(const std::vector<std::string> &params) {
    // Note: hw_1_2 doesn't support Bézier curves from command line
    // Keep original implementation
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Image3 img(640, 480);
    std::vector<Vector2> polyline;
    bool is_closed = false;
    std::optional<Vector3> fill_color;
    std::optional<Vector3> stroke_color;
    Real stroke_width = 1;
    
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-points") {
            while (params.size() > i+1 &&
                    params[i+1].length() > 0 &&
                    params[i+1][0] != '-') {
                Real x = std::stof(params[++i]);
                Real y = std::stof(params[++i]);
                y = img.height - y;
                polyline.push_back(Vector2{x, y});
            }
        } else if (params[i] == "--closed") {
            is_closed = true;
        } else if (params[i] == "-fill_color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            fill_color = Vector3{r, g, b};
        } else if (params[i] == "-stroke_color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            stroke_color = Vector3{r, g, b};
        } else if (params[i] == "-stroke_width") {
            stroke_width = std::stof(params[++i]);
        }
    }

    if (fill_color && !is_closed) {
        std::cout << "Error: can't have a non-closed shape with fill color." << std::endl;
        return Image3(0, 0);
    }

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 pixel_color = Vector3{0.5, 0.5, 0.5};

            if (fill_color && is_closed) {
                int winding = calculate_winding_number(x + 0.5, y + 0.5, polyline);
                if (winding != 0) {
                    pixel_color = *fill_color;
                }
            }
            
            if (stroke_color && polyline.size() >= 2) {
                bool on_stroke = false;
                Real half_width = stroke_width / 2.0;
                
                for (int i = 0; i < (int)polyline.size() - 1; i++) {
                    Vector2 p0 = polyline[i];
                    Vector2 p1 = polyline[i + 1];
                    
                    bool flat_start = (i == 0) && !is_closed;
                    bool flat_end = (i == (int)polyline.size() - 2) && !is_closed;
                    
                    Real dist = point_to_segment_distance(x + 0.5, y + 0.5, 
                                                          p0.x, p0.y, p1.x, p1.y,
                                                          flat_start, flat_end);
                    
                    if (dist <= half_width) {
                        on_stroke = true;
                        break;
                    }
                }
                
                if (is_closed && polyline.size() >= 2) {
                    Vector2 p0 = polyline.back();
                    Vector2 p1 = polyline.front();
                    Real dist = point_to_segment_distance(x + 0.5, y + 0.5,
                                                          p0.x, p0.y, p1.x, p1.y,
                                                          false, false);
                    if (dist <= half_width) {
                        on_stroke = true;
                    }
                }
                
                if (!on_stroke) {
                    int start_idx = is_closed ? 0 : 1;
                    int end_idx = is_closed ? polyline.size() : polyline.size() - 1;
                    
                    for (int i = start_idx; i < end_idx; i++) {
                        Vector2 vertex = polyline[i % polyline.size()];
                        if (point_in_circle(x + 0.5, y + 0.5, vertex.x, vertex.y, half_width)) {
                            on_stroke = true;
                            break;
                        }
                    }
                }
                
                if (on_stroke) {
                    pixel_color = *stroke_color;
                }
            }
            
            img(x, y) = pixel_color;
        }
    }
    return img;
}

Image3 hw_1_3(const std::vector<std::string> &params) {
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 pixel_color = scene.background;

            for (int shape_idx = scene.shapes.size() - 1; shape_idx >= 0; shape_idx--) {
                const Shape& shape = scene.shapes[shape_idx];
                //circle
                if (auto *circle = std::get_if<Circle>(&shape)) {
                    Real center_x = circle->center.x;
                    Real center_y = img.height - circle->center.y;
                    
                    Real distanceX = (x + 0.5) - center_x;
                    Real distanceY = (y + 0.5) - center_y;
                    Real distance = sqrt(distanceX * distanceX + distanceY * distanceY);

                    if (circle->fill_color && distance <= circle->radius) {
                        pixel_color = *(circle->fill_color);
                    }

                    if (circle->stroke_color) {
                        Real stroke_dist = abs(distance - circle->radius);
                        if (stroke_dist <= circle->stroke_width / 2.0) {
                            pixel_color = *(circle->stroke_color);
                        }
                    }
                //polyline 
                } else if (auto *polyline = std::get_if<Polyline>(&shape)) {
                    std::vector<Vector2> flipped_points;
                    flipped_points.reserve(polyline->points.size());
                    for (const Vector2& pt : polyline->points) {
                        flipped_points.push_back(Vector2{pt.x, img.height - pt.y});
                    }
                    
                    if (polyline->fill_color && polyline->is_closed && flipped_points.size() >= 3) {
                        int winding = calculate_winding_number(x + 0.5, y + 0.5, flipped_points);
                        if (winding != 0) {
                            pixel_color = *(polyline->fill_color);
                        }
                    }

                    if (polyline->stroke_color && flipped_points.size() >= 2) {
                        // Create temporary polyline with flipped points for stroke check
                        Polyline temp_polyline = *polyline;
                        temp_polyline.points = flipped_points;
                        
                        if (check_polyline_stroke(x + 0.5, y + 0.5, &temp_polyline)) {
                            pixel_color = *(polyline->stroke_color);
                        }
                    }
                }
            }
            
            img(x, y) = pixel_color;
        }
    }
    return img;
}

Image3 hw_1_4(const std::vector<std::string> &params) {
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 pixel_color = scene.background;

            Real canvas_x = x + 0.5;
            Real canvas_y = img.height - (y + 0.5);

            for (int shape_idx = scene.shapes.size() - 1; shape_idx >= 0; shape_idx--) {
                const Shape& shape = scene.shapes[shape_idx];
                //circle
                if (auto *circle = std::get_if<Circle>(&shape)) {
                    Matrix3x3 inv_transform = inverse(circle->transform);
                    Vector3 canvas_point = Vector3{canvas_x, canvas_y, 1.0};
                    Vector3 object_point = inv_transform * canvas_point;
                    
                    Real distance = sqrt(object_point.x * object_point.x + object_point.y * object_point.y);

                    if (circle->fill_color && distance <= 1.0) {
                        pixel_color = *(circle->fill_color);
                    }

                    if (circle->stroke_color) {
                        Real stroke_dist = abs(distance - 1.0);
                        if (stroke_dist <= circle->stroke_width / 2.0) {
                            pixel_color = *(circle->stroke_color);
                        }
                    }
                //polyline
                } else if (auto *polyline = std::get_if<Polyline>(&shape)) {
                    Matrix3x3 inv_transform = inverse(polyline->transform);
                    Vector3 canvas_point = Vector3{canvas_x, canvas_y, 1.0};
                    Vector3 object_point = inv_transform * canvas_point;
                    
                    Real px = object_point.x;
                    Real py = object_point.y;
                    
                    if (polyline->fill_color && polyline->is_closed && polyline->points.size() >= 3) {
                        int winding = calculate_winding_number(px, py, polyline->points);
                        if (winding != 0) {
                            pixel_color = *(polyline->fill_color);
                        }
                    }

                    if (polyline->stroke_color && polyline->points.size() >= 2) {
                        if (check_polyline_stroke(px, py, polyline)) {
                            pixel_color = *(polyline->stroke_color);
                        }
                    }
                }
            }
            
            img(x, y) = pixel_color;
        }
    }
    return img;
}

Image3 hw_1_5(const std::vector<std::string> &params) {
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 accumulated_color = Vector3{0, 0, 0};
            
            for (int sub_y = 0; sub_y < 4; sub_y++) {
                for (int sub_x = 0; sub_x < 4; sub_x++) {
                    Real offset_x = (sub_x + 0.5) / 4.0;
                    Real offset_y = (sub_y + 0.5) / 4.0;
                    
                    Real canvas_x = x + offset_x;
                    Real canvas_y = img.height - (y + offset_y);
                    
                    Vector3 subpixel_color = scene.background;
                    
                    for (int shape_idx = scene.shapes.size() - 1; shape_idx >= 0; shape_idx--) {
                        const Shape& shape = scene.shapes[shape_idx];
                        //circle
                        if (auto *circle = std::get_if<Circle>(&shape)) {
                            Matrix3x3 inv_transform = inverse(circle->transform);
                            Vector3 canvas_point = Vector3{canvas_x, canvas_y, 1.0};
                            Vector3 object_point = inv_transform * canvas_point;
                            
                            Real distance = sqrt(object_point.x * object_point.x + object_point.y * object_point.y);

                            if (circle->fill_color && distance <= 1.0) {
                                subpixel_color = *(circle->fill_color);
                            }

                            if (circle->stroke_color) {
                                Real stroke_dist = abs(distance - 1.0);
                                if (stroke_dist <= circle->stroke_width / 2.0) {
                                    subpixel_color = *(circle->stroke_color);
                                }
                            }
                            
                        } else if (auto *polyline = std::get_if<Polyline>(&shape)) {
                            Matrix3x3 inv_transform = inverse(polyline->transform);
                            Vector3 canvas_point = Vector3{canvas_x, canvas_y, 1.0};
                            Vector3 object_point = inv_transform * canvas_point;
                            
                            Real px = object_point.x;
                            Real py = object_point.y;
                            //polyline
                            if (polyline->fill_color && polyline->is_closed && polyline->points.size() >= 3) {
                                int winding = calculate_winding_number(px, py, polyline->points);
                                if (winding != 0) {
                                    subpixel_color = *(polyline->fill_color);
                                }
                            }

                            if (polyline->stroke_color && polyline->points.size() >= 2) {
                                if (check_polyline_stroke(px, py, polyline)) {
                                    subpixel_color = *(polyline->stroke_color);
                                }
                            }
                        }
                    }
                    
                    accumulated_color = accumulated_color + subpixel_color;
                }
            }
            
            img(x, y) = accumulated_color / 16.0;
        }
    }
    return img;
}

Image3 hw_1_6(const std::vector<std::string> &params) {
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector3 accumulated_color = Vector3{0, 0, 0};
            
            for (int sub_y = 0; sub_y < 4; sub_y++) {
                for (int sub_x = 0; sub_x < 4; sub_x++) {
                    Real offset_x = (sub_x + 0.5) / 4.0;
                    Real offset_y = (sub_y + 0.5) / 4.0;
                    
                    Real canvas_x = x + offset_x;
                    Real canvas_y = img.height - (y + offset_y);
                    
                    std::vector<Vector3> layer_colors;
                    std::vector<Real> layer_alphas;
                    
                    for (int shape_idx = scene.shapes.size() - 1; shape_idx >= 0; shape_idx--) {
                        const Shape& shape = scene.shapes[shape_idx];
                        //circle
                        if (auto *circle = std::get_if<Circle>(&shape)) {
  
                            Matrix3x3 inv_transform = inverse(circle->transform);
                            Vector3 canvas_point = Vector3{canvas_x, canvas_y, 1.0};
                            Vector3 object_point = inv_transform * canvas_point;
                            
                            Real distance = sqrt(object_point.x * object_point.x + object_point.y * object_point.y);

                            if (circle->fill_color && distance <= circle->radius) {
                                layer_colors.push_back(*(circle->fill_color));
                                layer_alphas.push_back(circle->fill_alpha);
                            }

                            if (circle->stroke_color) {
                                Real stroke_dist = abs(distance - 1.0);
                                if (stroke_dist <= circle->stroke_width / 2.0) {
                                    layer_colors.push_back(*(circle->stroke_color));
                                    layer_alphas.push_back(circle->stroke_alpha);
                                }
                            }
                        //polyline
                        } else if (auto *polyline = std::get_if<Polyline>(&shape)) {
                            Matrix3x3 inv_transform = inverse(polyline->transform);
                            Vector3 canvas_point = Vector3{canvas_x, canvas_y, 1.0};
                            Vector3 object_point = inv_transform * canvas_point;
                            
                            Real px = object_point.x;
                            Real py = object_point.y;
                            
                            if (polyline->fill_color && polyline->is_closed && polyline->points.size() >= 3) {
                                int winding = calculate_winding_number(px, py, polyline->points);
                                if (winding != 0) {
                                    layer_colors.push_back(*(polyline->fill_color));
                                    layer_alphas.push_back(polyline->fill_alpha);
                                }
                            }

                            if (polyline->stroke_color && polyline->points.size() >= 2) {
                                if (check_polyline_stroke(px, py, polyline)) {
                                    layer_colors.push_back(*(polyline->stroke_color));
                                    layer_alphas.push_back(polyline->stroke_alpha);
                                }
                            }
                        }
                    }
                    
                    Vector3 final_color = Vector3{0, 0, 0};
                    Real transparency = 1.0;
                    
                    for (int i = layer_colors.size() - 1; i >= 0; i--) {
                        Vector3 layer_color = layer_colors[i];
                        Real alpha = layer_alphas[i];
                        
                        final_color = final_color + transparency * alpha * layer_color;
                        transparency *= (1.0 - alpha);
                    }
                    
                    final_color = final_color + transparency * scene.background;
                    
                    accumulated_color = accumulated_color + final_color;
                }
            }
            
            img(x, y) = accumulated_color / 16.0;
        }
    }
    return img;
}