//
//  Level3.hpp
//  GoldMiner
//
//  Created by Kewei Zhang on 8/3/20.
//  Copyright © 2020 ctg. All rights reserved.
//
#include "Scene.h"

class Level3 : public Scene {
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
