/* File      : world.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include "world.hpp"


namespace mrtp {

WorldStatus_t World::initialize() {
    std::shared_ptr<cpptoml::table> config;
    try {
        config = cpptoml::parse_file(path_);
    } catch (...) {
        return ws_fail;
    }

    auto tab_camera = config->get_table("camera");
    if (!tab_camera) { return ws_err_camera; }
    
    auto raw_eye = tab_camera->get_array_of<double>("center");
    if (!raw_eye) { return ws_err_camera; }
    
    auto raw_lookat = tab_camera->get_array_of<double>("target");
    if (!raw_lookat) { return ws_err_camera; }

    float camera_roll = (float)tab_camera->get_as<double>("roll").value_or(0.0f);

    Eigen::Vector3d temp_eye(raw_eye->data());
    Eigen::Vector3f camera_eye = temp_eye.cast<float>();
    Eigen::Vector3d temp_lookat(raw_lookat->data());
    Eigen::Vector3f camera_lookat = temp_lookat.cast<float>();

    Camera camera(&camera_eye, &camera_lookat, camera_roll);
    cameras_.push_back(camera);
    ptr_camera_ = &cameras_.back();

    auto tab_light = config->get_table("light");
    if (!tab_light) { return ws_err_light; }

    auto raw_center = tab_light->get_array_of<double>("center");
    if (!raw_center) { return ws_err_light; }

    Eigen::Vector3d temp_center(raw_center->data());
    Eigen::Vector3f light_center = temp_center.cast<float>();

    Light light(&light_center);
    lights_.push_back(light);
    ptr_light_ = &lights_.back();

    WorldStatus_t check = load_planes(config->get_table_array("planes"));
    if (check != ws_ok) { return check; }

    check = load_spheres(config->get_table_array("spheres"));
    if (check != ws_ok) { return check; }

    check = load_cylinders(config->get_table_array("cylinders"));
    if (check != ws_ok) { return check; }

    size_t num_actors = planes_.size() + spheres_.size() + cylinders_.size();
    if (num_actors < 1) { return ws_err_no_actors; }

    return check;
}

WorldStatus_t World::load_planes(std::shared_ptr<cpptoml::table_array> table) {
    if (!table) { return ws_ok; }

    for (const auto& items : *table) {
        auto raw_center = items->get_array_of<double>("center");
        Eigen::Vector3d temp_center(raw_center->data());
        Eigen::Vector3f center = temp_center.cast<float>();

        auto raw_normal = items->get_array_of<double>("normal");
        Eigen::Vector3d temp_normal(raw_normal->data());
        Eigen::Vector3f normal = temp_normal.cast<float>();

        auto raw_texture = items->get_as<std::string>("texture");
        const char *texture = raw_texture->data();

        float scale = (float)items->get_as<double>("scale").value_or(0.15f);
        float reflect = (float)items->get_as<double>("reflect").value_or(0.0f);

        Plane plane(&center, &normal, scale, reflect, texture);
        planes_.push_back(plane);
        ptr_actors_.push_back(&planes_.back());
    }
    return ws_ok;
}

WorldStatus_t World::load_spheres(std::shared_ptr<cpptoml::table_array> table) {
    if (!table) { return ws_ok; }

    for (const auto& items : *table) {
        auto raw_center = items->get_array_of<double>("center");
        Eigen::Vector3d temp_center(raw_center->data());
        Eigen::Vector3f center = temp_center.cast<float>();

        //TODO Axis is optional, set to <0, 0, 1>
        auto raw_axis = items->get_array_of<double>("axis");
        Eigen::Vector3d temp_axis(raw_center->data());
        Eigen::Vector3f axis = temp_axis.cast<float>();

        auto raw_texture = items->get_as<std::string>("texture");
        const char *texture = raw_texture->data();

        float radius = (float)items->get_as<double>("radius").value_or(1.0f);
        float reflect = (float)items->get_as<double>("reflect").value_or(0.0f);

        Sphere sphere(&center, radius, &axis, reflect, texture);
        spheres_.push_back(sphere);
        ptr_actors_.push_back(&spheres_.back());
    }
    return ws_ok;
}

WorldStatus_t World::load_cylinders(std::shared_ptr<cpptoml::table_array> table) {
    if (!table) { return ws_ok; }

    for (const auto& items : *table) {
        auto raw_center = items->get_array_of<double>("center");
        Eigen::Vector3d temp_center(raw_center->data());
        Eigen::Vector3f center = temp_center.cast<float>();

        auto raw_direction = items->get_array_of<double>("direction");
        Eigen::Vector3d temp_direction(raw_direction->data());
        Eigen::Vector3f direction = temp_direction.cast<float>();

        auto raw_texture = items->get_as<std::string>("texture");
        const char *texture = raw_texture->data();

        float span = (float)items->get_as<double>("span").value_or(-1.0f);
        float radius = (float)items->get_as<double>("radius").value_or(1.0f);
        float reflect = (float)items->get_as<double>("reflect").value_or(0.0f);

        Cylinder cylinder(&center, &direction, radius, span, reflect, texture);
        cylinders_.push_back(cylinder);
        ptr_actors_.push_back(&cylinders_.back());
    }
    return ws_ok;
}

} //end namespace mrtp
