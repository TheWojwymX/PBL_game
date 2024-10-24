#pragma once

#define TURRETSMANAGER TurretsManager::getInstance()

#include "Managers/ComponentsManager.h"
#include "Turret.h"
#include "Managers/NodesManager.h"
#include "Enemies/EnemiesManager.h"
#include "BatchRandomGenerator.h"

class TurretsManager {
public:
    static TurretsManager &getInstance();

    ~TurretsManager() = default;

    TurretsManager();

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

    int _selectedIndex = -1;

    void RaycastTurrets();

    void CheckEnemiesInRange();


    void AttackEnemy(const shared_ptr<Turret>& turret, const shared_ptr<Enemy>& enemy);

    void Reload(const shared_ptr<Turret> &turret);

    void MoveTurret();

    bool _isPlayerInMovingMode = false;
    int _indexOfMovingTurret;

    void PlaceMovingTurret();

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

    int GetTurretCost(TurretType type) { return _turretCosts[type]; }

    void Reset();
    int _turretIndexAtRestart = 0;
    int _newTurretIndex = 0;
    std::vector<glm::vec4> _turretStats;
    TurretType _turretType;

private:
    std::vector<int> _turretCosts;
    int _turretCostIncrease;
    std::shared_ptr<Turret> _heldTurret = nullptr;
    std::shared_ptr<Camera> _cameraRef = nullptr;
    BatchRandomGenerator _randFlareRot;

    bool IsPointInTrapezoid(glm::vec3 point, std::vector<glm::vec3> trapPoints);
    void InitTurretStats();

};
