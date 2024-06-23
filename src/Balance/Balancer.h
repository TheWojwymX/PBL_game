//
// Created by TheWojwymX on 23.06.2024.
//

#ifndef SANDBOX_BALANCER_H
#define SANDBOX_BALANCER_H

#define BALANCER Balancer::GetInstance()

class Balancer {
public:
    static Balancer& GetInstance();
    ~Balancer() = default;
    Balancer();
    Balancer(const Balancer&) = delete;
    Balancer& operator=(const Balancer&) = delete;

    void addToImgui();

    float _antSpeed = 7.0f;
    float _antHp = 3.0f;
    float _antDamage = 0.5f;
    float _antAttackFrequency = 0.6f;

    float _beetleSpeed = 3.0f;
    float _beetleHp = 30.0f;
    float _beetleDamage = 5.0f;
    float _beetleAttackFrequency = 1.5f;

    float _waspSpeed = 10.0f;
    float _waspHp = 2.0f;
    float _waspDamage = 1.0f;
    float _waspAttackFrequency = 0.45f;

    float _minigunFireRate = 0.5f;
    float _minigunDamage = 1.0f;
    float _minigunSideRange = 80.0f;
    float _minigunForwardRange = 60.0f;

    float _sniperFireRate = 2.0f;
    float _sniperDamage = 5.0f;
    float _sniperSideRange = 50.0f;
    float _sniperForwardRange = 100.0f;

    float _rifleFireRate = 1.0f;
    float _rifleDamage = 1.0f;
    float _rifleSideRange = 80.0f;
    float _rifleForwardRange = 80.0f;

};


#endif //SANDBOX_BALANCER_H
