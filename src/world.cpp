/* File      : world.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <fstream>

#include "world.hpp"


namespace mrtp {

//Local functions

static bool file_exists(const char *path) {
    std::fstream check(path);
    return check.good();
}

static bool read_vector(std::shared_ptr<cpptoml::table> items, const char *id, 
                        Eigen::Vector3f *vector) {
    auto raw = items->get_array_of<double>(id);
    if (!raw) { return false; }
    Eigen::Vector3d tmp(raw->data());
    *vector = tmp.cast<float>();
    return true;
}

static bool read_texture(std::shared_ptr<cpptoml::table> items, std::string *output) {
    auto raw = items->get_as<std::string>("texture");
    if (!raw) { return false; }
    const char *texture = raw->data();
    if (!file_exists(texture)) { return false; }
    *output = texture;
    return true;
}

//Member functions

World::World(const char *path): path_(path) {}

World::~World() {}

WorldStatus_t World::initialize() {
    if (!file_exists(path_)) { return ws_no_file; }

    std::shared_ptr<cpptoml::table> config;
    try { config = cpptoml::parse_file(path_); } catch (...) { return ws_parse_error; }

    auto tab_camera = config->get_table("camera");
    if (!tab_camera) { return ws_no_camera; }
    
    auto raw_eye = tab_camera->get_array_of<double>("center");
    if (!raw_eye) { return ws_camera_param; }
    
    auto raw_lookat = tab_camera->get_array_of<double>("target");
    if (!raw_lookat) { return ws_camera_param; }

    float camera_roll = (float)tab_camera->get_as<double>("roll").value_or(0.0f);

    Eigen::Vector3d temp_eye(raw_eye->data());
    Eigen::Vector3f camera_eye = temp_eye.cast<float>();
    Eigen::Vector3d temp_lookat(raw_lookat->data());
    Eigen::Vector3f camera_lookat = temp_lookat.cast<float>();

    Camera camera(&camera_eye, &camera_lookat, camera_roll);
    cameras_.push_back(camera);
    ptr_camera_ = &cameras_.back();

    auto tab_light = config->get_table("light");
    if (!tab_light) { return ws_no_light; }

    auto raw_center = tab_light->get_array_of<double>("center");
    if (!raw_center) { return ws_light_param; }

    Eigen::Vector3d temp_center(raw_center->data());
    Eigen::Vector3f light_center = temp_center.cast<float>();

    Light light(&light_center);
    lights_.push_back(light);
    ptr_light_ = &lights_.back();

    WorldStatus_t check;

    auto planes = config->get_table_array("planes");
    if ((check = load_planes(planes)) != ws_ok) { return check; }

    auto spheres = config->get_table_array("spheres");
    if ((check = load_spheres(spheres)) != ws_ok) { return check; }

    auto cylinders = config->get_table_array("cylinders");
    if ((check = load_cylinders(cylinders)) != ws_ok) { return check; }

    if (ptr_actors_.empty()) { return ws_no_actors; }

    return ws_ok;
}

WorldStatus_t World::load_planes(std::shared_ptr<cpptoml::table_array> array) {
    if (!array) { return ws_ok; }
    for (const auto& items : *array) {
        WorldStatus_t check;
        if ((check = load_plane(items)) != ws_ok) { return check; }
    }
    return ws_ok;
}

WorldStatus_t World::load_spheres(std::shared_ptr<cpptoml::table_array> array) {
    if (!array) { return ws_ok; }
    for (const auto& items : *array) {
        WorldStatus_t check;
        if ((check = load_sphere(items)) != ws_ok) { return check; }
    }
    return ws_ok;
}

WorldStatus_t World::load_cylinders(std::shared_ptr<cpptoml::table_array> array) {
    if (!array) { return ws_ok; }
    for (const auto& items : *array) {
        WorldStatus_t check;
        if ((check = load_cylinder(items)) != ws_ok) { return check; }
    }
    return ws_ok;
}

WorldStatus_t World::load_plane(std::shared_ptr<cpptoml::table> items) {
    Eigen::Vector3f center;
    if (!read_vector(items, "center", &center)) { return ws_plane_param; }

    Eigen::Vector3f normal;
    if (!read_vector(items, "normal", &normal)) { return ws_plane_param; }

    std::string texture;
    if (!read_texture(items, &texture)) { return ws_plane_texture; }

    float scale = (float)items->get_as<double>("scale").value_or(0.15f);
    float reflect = (float)items->get_as<double>("reflect").value_or(0.0f);

    Plane plane(&center, &normal, scale, reflect, texture.c_str());
    planes_.push_back(plane);
    ptr_actors_.push_back(&planes_.back());

    return ws_ok;
}

WorldStatus_t World::load_sphere(std::shared_ptr<cpptoml::table> items) {
    Eigen::Vector3f center;
    if (!read_vector(items, "center", &center)) { return ws_sphere_param; }

    Eigen::Vector3f axis(0.0f, 0.0f, 1.0f);
    read_vector(items, "axis", &axis);

    std::string texture;
    if (!read_texture(items, &texture)) { return ws_sphere_texture; }

    float radius = (float)items->get_as<double>("radius").value_or(1.0f);
    float reflect = (float)items->get_as<double>("reflect").value_or(0.0f);

    Sphere sphere(&center, radius, &axis, reflect, texture.c_str());
    spheres_.push_back(sphere);
    ptr_actors_.push_back(&spheres_.back());

    return ws_ok;
}

WorldStatus_t World::load_cylinder(std::shared_ptr<cpptoml::table> items) {
    Eigen::Vector3f center;
    if (!read_vector(items, "center", &center)) { return ws_cylinder_param; }

    Eigen::Vector3f direction;
    if (!read_vector(items, "direction", &direction)) { return ws_cylinder_param; }

    std::string texture;
    if (!read_texture(items, &texture)) { return ws_cylinder_texture; }

    float span = (float)items->get_as<double>("span").value_or(-1.0f);
    float radius = (float)items->get_as<double>("radius").value_or(1.0f);
    float reflect = (float)items->get_as<double>("reflect").value_or(0.0f);

    Cylinder cylinder(&center, &direction, radius, span, reflect, texture.c_str());
    cylinders_.push_back(cylinder);
    ptr_actors_.push_back(&cylinders_.back());

    return ws_ok;
}

} //end namespace mrtp
