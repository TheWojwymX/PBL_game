    //
    // Created by Jacek on 20.04.2024.
    //

    #include "EnemiesManager.h"

    EnemiesManager &EnemiesManager::getInstance() {
        static EnemiesManager instance;
        return instance;
    }

    void EnemiesManager::Update() {
        ChooseModelBasedOnDistance(); //LOD (safe to comment out)
        if(Input::Instance().IsKeyPressed(GLFW_KEY_KP_1)) {

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(0.5f, 1.0f);
            float scale = dis(gen);

            SpawnEnemy(2, glm::vec3(scale), 0);
        }

        for(int i = 0; i < _enemies.size(); i++) {
            if(_enemies[i] == nullptr) continue;
            _enemies[i]->EnemyAI();
            ReturnToComingForNormalDestination(_enemies[i]);
            CheckIfAtWalls(_enemies[i]);
            AvoidEnemy(_enemies[i]);
        }

    }

    void EnemiesManager::Init() {
        for(int i = 0; i < COMPONENTSMANAGER.GetComponents().size(); i++){
            if(COMPONENTSMANAGER.GetComponents()[i] != nullptr && COMPONENTSMANAGER.GetComponents()[i]->_type == ComponentType::ENEMYAI){
                _enemies.push_back(std::dynamic_pointer_cast<Enemy>(COMPONENTSMANAGER.GetComponents()[i]));
            }
        }

        for(int i = 0; i < _enemies.size(); i++){
            _enemies[i]->_destinationVector = CalcClosestDomePosition(_enemies[i]);
        }

        //Setup wave "!" symbols
        for (int i = 0; i < 4; i++)
        {
            NODESMANAGER.getNodeByName("waveSymbol" + to_string(i + 1))->GetTransform()->SetPosition(
                    glm::vec3(_spawnersPositions[i][0], 0, _spawnersPositions[i][1]));
        }

        //Here you can modify rounds
        _roundsInfo[0] = {{0, 3}}; // Spawner 0 - spawn 3 enemies
        _roundsInfo[1] = {{2, 3}}; // Spawner 1 - spawn 3 enemies
        _roundsInfo[2] = {{2, 2}, {1, 2}}; //Spawner 2 - spawn 2 enemies, Spawner 1 - spawn 2 enemies
        _roundsInfo[3] = {{3, 3}, {2, 2}}; //Spawner 3 - spawn 3 enemies, Spawner 2 - spawn 2 enemies

        //TEMPORARY
        _roundsInfo[4] = {{0, 3}}; // Spawner 0 - spawn 3 enemies
        _roundsInfo[5] = {{1, 3}}; // Spawner 1 - spawn 3 enemies
        _roundsInfo[6] = {{2, 2}, {1, 2}}; //Spawner 2 - spawn 2 enemies, Spawner 1 - spawn 2 enemies
        _roundsInfo[7] = {{3, 3}, {2, 2}}; //Spawner 3 - spawn 3 enemies, Spawner 2 - spawn 2 enemies
        //

    }

    void EnemiesManager::SpawnEnemiesForRound(int roundNumber) {
        if (_roundsInfo.find(roundNumber) != _roundsInfo.end()) {
            const auto &spawns = _roundsInfo[roundNumber];
            for (const auto &spawn : spawns) {
                for (int i = 0; i < spawn.second; ++i) {

                    const auto &spawns = _roundsInfo[roundNumber];

                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<float> dis(0.5f, 1.0f);
                    float scale = dis(gen);

                    SpawnEnemy(2, glm::vec3(scale), spawn.first); // Example: spawn.first is spawnerIndex, spawn.second is enemyCount
                }
            }
        }
    }

    void EnemiesManager::ReturnToComingForNormalDestination(shared_ptr<Enemy> enemy){
        if(enemy->_destinationVector != CalcClosestDomePosition(enemy) && glm::distance(enemy->_destinationVector, enemy->GetOwnerPosition()) <= 1){
            enemy->_destinationVector = CalcClosestDomePosition(enemy);
            enemy->_isAvoiding = false;
        }
    }

    void EnemiesManager::AvoidEnemy(shared_ptr<Enemy> thisEnemy){
        if(thisEnemy->_isAtWalls){
            return;
        }

        glm::vec3 avoidanceVector(0.0f);
        bool needsToAvoid = false;

        for(int j = 0; j < _enemies.size(); j++) {
            if(_enemies[j] == nullptr) continue;
            if (thisEnemy == _enemies[j]) continue;

            glm::vec3 toOther = _enemies[j]->GetOwnerPosition() - thisEnemy->GetOwnerPosition();
            float distanceToOther = glm::length(toOther);

            if (distanceToOther < (_enemies[j]->_size + thisEnemy->_size)/2) {
                needsToAvoid = true;
                toOther = glm::normalize(toOther);
                avoidanceVector -= toOther / distanceToOther;
            }
        }

        if (needsToAvoid) {
            avoidanceVector = glm::normalize(avoidanceVector);
            thisEnemy->_isAvoiding = true;
            avoidanceVector.y = 0;
            thisEnemy->_destinationVector = thisEnemy->GetOwnerPosition() + avoidanceVector * 3.0f;
            thisEnemy->_distanceToStop = 1;
        } else {
            thisEnemy->_isAvoiding = false;
            thisEnemy->_distanceToStop = 0.1;
        }
    }

    void EnemiesManager::CheckIfAtWalls(shared_ptr<Enemy> enemy){
        if(!enemy->_isAtWalls){
            if(glm::distance(enemy->GetOwnerPosition(), CalcClosestDomePosition(enemy)) < 0.1f){
                enemy->_isAtWalls = true;
                if(enemy->GetOwnerNode()->GetComponent<Animation>() != nullptr)
                {
                    enemy->GetOwnerNode()->GetComponent<Animation>()->_enemyState=state::ATTACK;
                }
            }
        }
    }

    void EnemiesManager::SpawnEnemy(int distanceToAvoid, glm::vec3 scale, int spawnerIndex) {

        std::cout << "Ilosc mrowek to: " << _enemies.size() << std::endl;

        if (spawnerIndex >= 0 && spawnerIndex < _spawnersPositions.size()) {
            std::string nameOfEnemy = "Enemy" + std::to_string(_enemies.size() + 1);
            std::string particleGeneratorNode = "Particle" +  to_string(_enemies.size() + 1);

            NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfEnemy);
            NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), particleGeneratorNode);

            glm::vec3 enemyPosition = CalcRandomSpawnPosition(_spawnersPositions[spawnerIndex]);
            enemyPosition.y = 0.62 * scale.y + 299.49;
            NODESMANAGER.getNodeByName(nameOfEnemy)->GetTransform()->SetPosition(enemyPosition);
            NODESMANAGER.getNodeByName(nameOfEnemy)->GetTransform()->SetScale(scale);

            auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
            newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("antModel");
            newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
            newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
            newMeshRenderer->Initiate();
            NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newMeshRenderer);

            auto newAnimation = COMPONENTSMANAGER.CreateComponent<Animation>();
            newAnimation->setMeshRenderer(NODESMANAGER.getNodeByName(nameOfEnemy)->GetComponent<MeshRenderer>());
            newAnimation->setMeshRendererId(COMPONENTSMANAGER._nextComponentID-1);
            newAnimation->InitComponent(1);
            NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newAnimation);

            auto antShot = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antShot");
            antShot->SetOffset(glm::vec3(0.0f,0.0f,2.0f));
            antShot->object = NODESMANAGER.getNodeByName(nameOfEnemy);
            antShot->enemyScale = scale;
            antShot->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
            antShot->Init();
            NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antShot);

            auto antDie = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antDie");
            antDie->SetOffset(glm::vec3(0.0f,0.0f,2.0f));
            antDie->object = NODESMANAGER.getNodeByName(nameOfEnemy);
            antDie->enemyScale = scale;
            antDie->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
            antDie->Init();
            NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antDie);

            auto newEnemyComponent = COMPONENTSMANAGER.CreateComponent<Enemy>();
            NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newEnemyComponent);
            newEnemyComponent->_destinationVector = CalcClosestDomePosition(newEnemyComponent);
            newEnemyComponent->_size = distanceToAvoid;
            _enemies.push_back(newEnemyComponent);
        }
    }


    glm::vec3 EnemiesManager::CalcRandomSpawnPosition(glm::vec2 spawnerPos){
        float spawnerRadius = 15.0f;
        float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * static_cast<float>(3.141592);
        float randomRadius = static_cast<float>(std::rand()) / RAND_MAX;
        randomRadius = std::sqrt(randomRadius);
        randomRadius *= spawnerRadius;
        float x = spawnerPos.x + randomRadius * std::cos(angle);
        float z = spawnerPos.y + randomRadius * std::sin(angle);
        return glm::vec3(x, 99.8f, z);
    }

    glm::vec3 EnemiesManager::CalcClosestDomePosition(shared_ptr<Enemy> enemy){
        glm::vec2 vector = glm::vec2(enemy->GetOwnerPosition().x - GAMEMANAGER._domePosition.x, enemy->GetOwnerPosition().z - GAMEMANAGER._domePosition.y);
        vector = glm::normalize(vector);
        vector = glm::vec2(GAMEMANAGER._domePosition.x + (GAMEMANAGER._domeRadius + 4.5) * vector.x, GAMEMANAGER._domePosition.y + (GAMEMANAGER._domeRadius + 4.5) * vector.y);

        return glm::vec3(vector.x, enemy->GetOwnerPosition().y, vector.y);
    }

    void EnemiesManager::ChooseModelBasedOnDistance() {
        glm::vec3 playerCamera = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition();
        shared_ptr<Model> normalModel = RESOURCEMANAGER.GetModelByName("antModel");
        shared_ptr<Model> farModel = RESOURCEMANAGER.GetModelByName("sandModel");     //Change Model
        float lodDistance = 40.0f;                                                          //Change Distance

        for(int i = 0; i < _enemies.size(); i++){
            if(_enemies[i] == nullptr) continue;
            float distance = glm::length(_enemies[i]->GetOwnerPosition() - playerCamera);
            if(distance > lodDistance){
                if(_enemies[i]->_ownerNode->GetComponent<MeshRenderer>()->_model != farModel){
                    _enemies[i]->_ownerNode->GetComponent<MeshRenderer>()->_model = farModel;
                }
            }
            else{
                if(_enemies[i]->_ownerNode->GetComponent<MeshRenderer>()->_model != normalModel){
                    _enemies[i]->_ownerNode->GetComponent<MeshRenderer>()->_model = normalModel;
                }
            }
        }
    }

    void EnemiesManager::SetSymbolsForWave() {
        //First hide all the symbols
        for (int i = 0; i < 4; i++) {
            NODESMANAGER.getNodeByName("waveSymbol" + to_string(i + 1))->GetTransform()->SetPosition(
                    glm::vec3(_spawnersPositions[i][0], 0, _spawnersPositions[i][1]));
        }

        if (_roundsInfo.find(GAMEMANAGER.roundNumber) != _roundsInfo.end()) {
            auto &roundInfo = _roundsInfo[GAMEMANAGER.roundNumber];
            for (const auto &spawnerInfo: roundInfo) {
                int spawnerIndex = spawnerInfo.first;
                NODESMANAGER.getNodeByName(
                        "waveSymbol" + std::to_string(spawnerIndex + 1))->GetTransform()->SetPosition(
                        glm::vec3(_spawnersPositions[spawnerIndex][0], 320, _spawnersPositions[spawnerIndex][1]));
            }
        }
    }