#pragma once

#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>

namespace dryden_model {

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
        void initialize(const double &wx_nominal, const double &wy_nominal, const double &wz_nominal,
                        const double &wx_sigma, const double &wy_sigma, const double &wz_sigma,
                        const double &altitude = 2.0);
        glm::vec3 getWind(const double &dt);

    private:
        GustModelBase wx_gust_;
        GustModelBase wy_gust_;
        GustModelBase wz_gust_;

        double wx_nominal_;
        double wy_nominal_;
        double wz_nominal_;

        bool initialized_;
    };

}