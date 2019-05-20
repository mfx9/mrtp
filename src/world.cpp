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

    auto tab_planes = config->get_table_array("planes");
    if (tab_planes) {
        WorldStatus_t check = load_planes(tab_planes);
        if (check != ws_ok) { return check; }
    }

    auto tab_spheres = config->get_table_array("spheres");
    if (tab_spheres) {
        WorldStatus_t check = load_spheres(tab_spheres);
        if (check != ws_ok) { return check; }
    }

    auto tab_cylinders = config->get_table_array("cylinders");
    if (tab_cylinders) {
        WorldStatus_t check = load_cylinders(tab_cylinders);
        if (check != ws_ok) { return check; }
    }

    size_t num_actors = planes_.size() + spheres_.size() + cylinders_.size();
    if (num_actors < 1) { return ws_err_no_actors; }

    return ws_ok;
}

WorldStatus_t World::load_planes(std::shared_ptr<cpptoml::table_array> table) {
    for (const auto& tab_plane : *table) {
        auto raw_center = tab_plane->get_array_of<double>("center");
        Eigen::Vector3d temp_center(raw_center->data());
        Eigen::Vector3f plane_center = temp_center.cast<float>();

        auto raw_normal = tab_plane->get_array_of<double>("normal");
        Eigen::Vector3d temp_normal(raw_normal->data());
        Eigen::Vector3f plane_normal = temp_normal.cast<float>();

        auto raw_texture = tab_plane->get_as<std::string>("texture");
        const char *plane_texture = raw_texture->data();

        float plane_scale = (float)tab_plane->get_as<double>("scale").value_or(0.15f);
        float plane_reflect = (float)tab_plane->get_as<double>("reflect").value_or(0.0f);

        Plane plane(&plane_center, &plane_normal, plane_scale, plane_reflect, plane_texture);
        planes_.push_back(plane);
        ptr_actors_.push_back(&planes_.back());
    }
    return ws_ok;
}

WorldStatus_t World::load_spheres(std::shared_ptr<cpptoml::table_array> table) {
    for (const auto& tab_sphere : *table) {
        auto raw_center = tab_sphere->get_array_of<double>("center");
        Eigen::Vector3d temp_center(raw_center->data());
        Eigen::Vector3f sphere_center = temp_center.cast<float>();

        //TODO Axis is optional, set to <0, 0, 1>
        auto raw_axis = tab_sphere->get_array_of<double>("axis");
        Eigen::Vector3d temp_axis(raw_center->data());
        Eigen::Vector3f sphere_axis = temp_axis.cast<float>();

        auto raw_texture = tab_sphere->get_as<std::string>("texture");
        const char *sphere_texture = raw_texture->data();

        float sphere_radius = (float)tab_sphere->get_as<double>("radius").value_or(1.0f);
        float sphere_reflect = (float)tab_sphere->get_as<double>("reflect").value_or(0.0f);

        Sphere sphere(&sphere_center, sphere_radius, &sphere_axis, sphere_reflect, sphere_texture);
        spheres_.push_back(sphere);
        ptr_actors_.push_back(&spheres_.back());
    }
    return ws_ok;
}

WorldStatus_t World::load_cylinders(std::shared_ptr<cpptoml::table_array> table) {
    for (const auto& tab_cylinder : *table) {
        auto raw_center = tab_cylinder->get_array_of<double>("center");
        Eigen::Vector3d temp_center(raw_center->data());
        Eigen::Vector3f cylinder_center = temp_center.cast<float>();

        auto raw_direction = tab_cylinder->get_array_of<double>("direction");
        Eigen::Vector3d temp_direction(raw_direction->data());
        Eigen::Vector3f cylinder_direction = temp_direction.cast<float>();

        auto raw_texture = tab_cylinder->get_as<std::string>("texture");
        const char *cylinder_texture = raw_texture->data();

        float cylinder_span = (float)tab_cylinder->get_as<double>("span").value_or(-1.0f);
        float cylinder_radius = (float)tab_cylinder->get_as<double>("radius").value_or(1.0f);
        float cylinder_reflect = (float)tab_cylinder->get_as<double>("reflect").value_or(0.0f);

        Cylinder cylinder(&cylinder_center, &cylinder_direction, cylinder_radius, cylinder_span, cylinder_reflect, cylinder_texture);
        cylinders_.push_back(cylinder);
        ptr_actors_.push_back(&cylinders_.back());
    }
    return ws_ok;
}

} //end namespace mrtp
