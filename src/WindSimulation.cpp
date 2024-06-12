#include "WindSimulation.h"


GustModelBase::GustModelBase() : dt_(0.05) {
    random_generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    gust_dist_ = std::uniform_real_distribution<double>(-1.0, 1.0);
    initialized_ = false;
}

void GustModelBase::initializeParameters(const double &V, const double &L, const double &sigma) {
    double b = 2 * sqrt(3) * L / V;
    double c = 2 * L / V;

    alpha_ = sigma * sqrt(2 * L / 3.14159265 / V);
    beta_ = alpha_ * b;
    delta_ = 2 * c;
    gamma_ = c * c;

    u_km1 = 0;
    u_km2 = 0;
    y_km1 = 0;
    y_km2 = 0;

    initialized_ = true;
}

double GustModelBase::run(const double &dt) {
    if (initialized_) {
        double C1 = 1.0 + 2 * delta_ / dt + 4 * gamma_ / dt / dt;
        double C2 = 2.0 - 8 * gamma_ / dt / dt;
        double C3 = 1.0 - 2 * delta_ / dt + 4 * gamma_ / dt / dt;
        double C4 = alpha_ + 2 * beta_ / dt;
        double C5 = 2 * alpha_;
        double C6 = alpha_ - 2 * beta_ / dt;

        double u_k = gust_dist_(random_generator_);
        double y_k = (C4 * u_k + C5 * u_km1 + C6 * u_km2 - C2 * y_km1 - C3 * y_km2) / C1;

        u_km2 = u_km1;
        u_km1 = u_k;
        y_km2 = y_km1;
        y_km1 = y_k;

        return y_k;
    } else {
        return 0.0;
    }
}

double GustModelBase::integrate(const double &dt) {
    if (dt > dt_) {
        double t = 0;
        double y = 0;
        while (t < dt) {
            double t_inc = std::min(dt_, dt - t);
            y = run(t_inc);
            t += t_inc;
        }
        return y;
    } else {
        return run(dt);
    }
}

DrydenWind::DrydenWind() : initialized_(false) {}

void DrydenWind::initialize(const double &wx_nominal, const double &wy_nominal, const double &wz_nominal,
                            const double &wx_sigma, const double &wy_sigma, const double &wz_sigma,
                            const double &altitude) {
    wx_nominal_ = wx_nominal;
    wy_nominal_ = wy_nominal;
    wz_nominal_ = wz_nominal;
    double Lz_ft = 3.281 * altitude;
    double Lx_ft = Lz_ft / pow(0.177 + 0.000823 * Lz_ft, 1.2);
    double Ly_ft = Lx_ft;

    wx_gust_.initializeParameters(1.0, Lx_ft / 3.281, wx_sigma);
    wy_gust_.initializeParameters(1.0, Ly_ft / 3.281, wy_sigma);
    wz_gust_.initializeParameters(1.0, Lz_ft / 3.281, wz_sigma);

    initialized_ = true;
}

glm::vec3 DrydenWind::getWind(const double &dt) {
    if (initialized_) {
        return glm::vec3(wx_nominal_, wy_nominal_, wz_nominal_) +
               glm::vec3(wx_gust_.integrate(dt), wy_gust_.integrate(dt), wz_gust_.integrate(dt));
    } else {
        return glm::vec3(0.f, 0.f, 0.f);
    }
}