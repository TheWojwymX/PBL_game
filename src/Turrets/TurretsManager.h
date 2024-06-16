#pragma once

#define TURRETSMANAGER TurretsManager::getInstance()

#include "Managers/ComponentsManager.h"
#include "Turret.h"
#include "Managers/NodesManager.h"
#include "Enemies/EnemiesManager.h"

class TurretsManager {
public:
    static TurretsManager &getInstance();

    ~TurretsManager() = default;

    TurretsManager() = default;

    TurretsManager(const TurretsManager &) = delete;

    TurretsManager &operator=(const TurretsManager &) = delete;

    void Init();

    void Update();

    std::vector<std::shared_ptr<Turret>> _turrets;

    void PrepareBlueprintTurret();

    void UpdateBlueprintTurret();

    bool _shouldEnableBlueprintTurret = false;

    void SpawnTurret(TurretType type);

    bool IsSelectedTurretInRange();

    bool IsTooCloseToTurret(glm::vec3 pos);

    bool _isInBlueprintMode = false;

    float _distanceToAnotherTurret = 1.5f;

    float _sideRange = 50.0f;
    float _forwardRange = 50.0f;
    float _backRange = 2.0f;
    int selectedIndex = -1;

    int RaycastTurrets();

    bool RayIntersectsBoundingBox(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                                  const glm::vec3& minBoundingBox, const glm::vec3& maxBoundingBox);

    void CheckEnemiesInRange();


    void AttackEnemy(const shared_ptr<Turret>& turret, const shared_ptr<Enemy>& enemy);

    void Reload(const shared_ptr<Turret> &turret);

    void MoveTurret();

    bool _isPlayerInMovingMode = false;
    int _indexOfMovingTurret;

    void PlaceMovingTurret();

    void CalculateRangePositions(shared_ptr<Turret> turret);

    shared_ptr<Node> _player = NODESMANAGER.getNodeByName("player");
    shared_ptr<Node> _blueprintTurret = make_shared<Node>();

    bool IsInForbiddenArea();

    glm::vec3 _additionalColor;

    shared_ptr<PDAController> _PDAController = make_shared<PDAController>();

    bool _isInTurretChoiceMenu = false;

    void PlayerActions();

    void ShowBlueprintTurret();

    void HideBlueprintTurret();

    void ChangeToSpawningMode();

private:
    std::vector<glm::ivec2> _turretCosts;
    TurretType _turretType;

    bool IsPointInTrapezoid(glm::vec3 point, std::vector<glm::vec3> trapPoints);
};
