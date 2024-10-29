#pragma once

#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>

class GustModelBase {
private:
    std::default_random_engine random_generator_;
    std::uniform_real_distribution<double> gust_dist_;

    bool initialized_;
    double dt_;

    double alpha_;
    double beta_;
    double delta_;
    double gamma_;

    double u_km1;
    double u_km2;
    double y_km1;
    double y_km2;

    double run(const double &dt);

public:
    GustModelBase();
    void initializeParameters(const double &V, const double &L, const double &sigma);
    double integrate(const double &dt);
};

class DrydenWind {
public:
    DrydenWind();
    void initialize(const float &wx_nominal, const float &wy_nominal, const float &wz_nominal,
                    const double &wx_sigma, const double &wy_sigma, const double &wz_sigma,
                    const double &altitude = 2.0);
    glm::vec3 getWind(const double &dt);

    float get_wx_nominal() {
        return wx_nominal_;
    }
    void set_wx_nominal(float x){
        wx_nominal_ = x;
    }

    void set_wy_nominal(float x){
        wy_nominal_ = x;
    }

    void set_wz_nominal(float x){
        wz_nominal_ = x;
    }

    float wx_nominal_;
    float wy_nominal_;
    float wz_nominal_;
private:
    GustModelBase wx_gust_;
    GustModelBase wy_gust_;
    GustModelBase wz_gust_;

    bool initialized_;
};